#include "osg/Image"
#include "osg/Notify"

#include <osg/Geode>

#include <osg/observer_ptr>

#include "osg/GL"

#include "osgDB/FileNameUtils"
#include "osgDB/Registry"
#include "osgDB/FileUtils"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef SEEK_SET
#  define SEEK_SET 0
#endif
#include "QTUtils.h"
#include "QTtexture.h"
#include "QuicktimeImageStream.h"


using namespace osg;

// This class is used as a helper to de-initialize
// properly quicktime, when the last media loaded 
// with the quicktime plugin is released. 
// All loaded media must be added to the observer 
// (see ReaderWriterQT::readImage() function) 
class QuicktimeExitObserver : public osg::Observer
{
public:

   QuicktimeExitObserver () : _instanceCount(0){}
   virtual ~QuicktimeExitObserver(){};

   void addMedia(Image* ptr)
   {
      ptr->addObserver(this);
      ++ _instanceCount;
   }
   
   virtual void objectDeleted(void*) 
   {
      -- _instanceCount;
      if(_instanceCount== 0)
         exitQuicktime();
   }

private:
   unsigned int _instanceCount;
};



class ReaderWriterQT : public osgDB::ReaderWriter
{
public:
   virtual const char* className() const { return "Default Quicktime Image Reader/Writer"; }

   virtual bool acceptsMovieExtension(const std::string& extension) const
   {
      return osgDB::equalCaseInsensitive(extension,"mov") ||
         osgDB::equalCaseInsensitive(extension,"mpg") ||
         osgDB::equalCaseInsensitive(extension,"mpv") ||
         osgDB::equalCaseInsensitive(extension,"mp4") ||
         osgDB::equalCaseInsensitive(extension,"m4v") ||
         osgDB::equalCaseInsensitive(extension,"dv")  ||
         osgDB::equalCaseInsensitive(extension,"avi") ||
         osgDB::equalCaseInsensitive(extension,"swf");
   }

   virtual bool acceptsExtension(const std::string& extension) const
   {
      // this should be the only image importer required on the Mac
      // dont know what else it supports, but these will do
      return

         #ifdef QT_HANDLE_IMAGES_ALSO
         osgDB::equalCaseInsensitive(extension,"rgb") ||
         osgDB::equalCaseInsensitive(extension,"rgba") ||
         osgDB::equalCaseInsensitive(extension,"jpg") || 
         osgDB::equalCaseInsensitive(extension,"jpeg") ||
         osgDB::equalCaseInsensitive(extension,"tif") ||               
         osgDB::equalCaseInsensitive(extension,"tiff") || 
         osgDB::equalCaseInsensitive(extension,"gif") ||
         osgDB::equalCaseInsensitive(extension,"png") ||
         osgDB::equalCaseInsensitive(extension,"pict") ||
         osgDB::equalCaseInsensitive(extension,"pct") ||
         osgDB::equalCaseInsensitive(extension,"tga") ||
         osgDB::equalCaseInsensitive(extension,"psd") ||
         #endif 

         acceptsMovieExtension(extension);
   }

   virtual ReadResult readImage(const std::string& file, const osgDB::ReaderWriter::Options* options) const
   {
      std::string ext = osgDB::getLowerCaseFileExtension(file);
      if (!acceptsExtension(ext)) return ReadResult::FILE_NOT_HANDLED;

      std::string fileName = osgDB::findDataFile( file,  options);
      if (fileName.empty()) return ReadResult::FILE_NOT_FOUND;

      // Note from Riccardo Corsi 
      // Quicktime initialization is done here, when a media is found
      // and before any image or movie is loaded. 
      // After the first call the function does nothing. 
      // The cleaning up is left to the QuicktimeExitObserver (see below)
      initQuicktime();

      // if the file is a movie file then load as an ImageStream.
      if (acceptsMovieExtension(ext))
      {
         // note from Robert Osfield when integrating, we should probably have so
         // error handling mechanism here.  Possibly move the load from
         // the constructor to a seperate load method, and have a valid
         // state on the ImageStream... will integrated as is right now
         // to get things off the ground.
         QuicktimeImageStream* moov = new QuicktimeImageStream(fileName);
         // moov->play();

         // add the media to the observer for proper clean up on exit
         _qtExitObserver.addMedia(moov);

         return moov;
      }
 
      long origWidth, origHeight,buffWidth,buffHeight,buffDepth,origDepth;

      // NOTE - implememntation means that this will always return 32 bits, so it is hard to work out if 
      // an image was monochrome. So it will waste texture memory unless it gets a monochrome hint.

      unsigned char *pixels = LoadBufferFromDarwinPath ( fileName.c_str(), &origWidth,&origHeight,&origDepth,
         &buffWidth,&buffHeight,
         &buffDepth);

      // IMPORTANT - 
      // origDepth in BYTES, buffDepth in BITS 
      if (pixels == 0) 
      {
         osg::notify(osg::WARN) << "LoadBufferFromDarwinPath failed " << fileName.c_str() << QTfailureMessage() << std::endl;
         return 0;
      }                      

      unsigned int pixelFormat;

      switch(origDepth) 
      {
         case 1 :
            pixelFormat = GL_RGB;
            break;
         case 2 :
            pixelFormat = GL_LUMINANCE_ALPHA;
            break;
         case 3 :
            pixelFormat = GL_RGB;
            break;
         case 4 :
            pixelFormat = GL_RGBA;
            break;
         default :
            osg::notify(osg::WARN) << "Unknown file type in " << fileName.c_str() << " with " << origDepth << std::endl;
            pixelFormat = (GLenum)-1;
            return 0;
            break;
      }

      {
         unsigned char *srcp=pixels, *dstp=pixels;

         int i, j;

         // swizzle entire image in-place
         unsigned char r, g, b, a;
         for (i=0; i<buffHeight; i++ ) {

            switch (origDepth) 
            {

               /*
               since 8-bit tgas will get expanded into colour, have to use RGB code for 8-bit images

               case 1 :
               for (j=0; j<buffWidth; j++ ) {
               dstp[0]=srcp[2];
               srcp+=4;
               dstp++;
               }
               break;
               */
               case 2 :
                  for (j=0; j<buffWidth; j++ ) {
                     dstp[1]=srcp[0];
                     dstp[0]=srcp[2];
                     srcp+=4;
                     dstp+=2;
                  }
                  break;

               case 1 :
               case 3 :
                  for (j=0; j<buffWidth; j++ ) {
                     dstp[0]=srcp[1];
                     dstp[1]=srcp[2];
                     dstp[2]=srcp[3];
                     srcp+=4;
                     dstp+=3;
                  }
                  break;
               case 4 :
                  for (j=0; j<buffWidth; j++ ) {
                     r=srcp[1];
                     g=srcp[2];
                     b=srcp[3];
                     a=srcp[0];

                     dstp[0]=r;
                     dstp[1]=g;
                     dstp[2]=b;
                     dstp[3]=a;

                     srcp+=4;
                     dstp+=4;
                  }
                  break;
               default :
                  // osg::notify(osg::WARN) << "ERROR IN RETURNED PIXEL DEPTH, CANNOT COPE" << std::endl;
                  return 0;
                  break;
            }
         }
      }



      Image* image = new Image();
      image->setFileName(fileName.c_str());
      image->setImage(buffWidth,buffHeight,1,
                      buffDepth >> 3,
                      pixelFormat,
                      GL_UNSIGNED_BYTE,
                      pixels,
                      osg::Image::USE_NEW_DELETE );

      notify(DEBUG_INFO) << "image read ok "<<buffWidth<<"  "<<buffHeight<<std::endl;

      // add the media to the observer for proper clean up on exit
      _qtExitObserver.addMedia(image);

      return image;
   }

   virtual WriteResult writeImage(const osg::Image &img,const std::string& fileName, const osgDB::ReaderWriter::Options*) const
   {
      std::string ext = osgDB::getFileExtension(fileName);
      if (!acceptsExtension(ext)) return WriteResult::FILE_NOT_HANDLED;

      initQuicktime();

      //Buidl map  of extension <-> osFileTypes
      std::map<std::string, OSType> extmap;

      extmap.insert(std::pair<std::string, OSType>("jpg",  kQTFileTypeJPEG));
      extmap.insert(std::pair<std::string, OSType>("jpeg", kQTFileTypeJPEG));
      extmap.insert(std::pair<std::string, OSType>("bmp",  kQTFileTypeBMP));
      extmap.insert(std::pair<std::string, OSType>("tif",  kQTFileTypeTIFF));
      extmap.insert(std::pair<std::string, OSType>("tiff", kQTFileTypeTIFF));
      extmap.insert(std::pair<std::string, OSType>("png",  kQTFileTypePNG));
      extmap.insert(std::pair<std::string, OSType>("gif",  kQTFileTypeGIF));
      extmap.insert(std::pair<std::string, OSType>("psd",  kQTFileTypePhotoShop));
      // extmap.insert(std::pair<std::string, OSType>("tga",  kQTFileTypeTargaImage));
      extmap.insert(std::pair<std::string, OSType>("sgi",  kQTFileTypeSGIImage));
      extmap.insert(std::pair<std::string, OSType>("rgb",  kQTFileTypeSGIImage));
      extmap.insert(std::pair<std::string, OSType>("rgba", kQTFileTypeSGIImage));

      std::map<std::string, OSType>::iterator cur = extmap.find(ext);

      // can not handle this type of file, perhaps a movie?
      if (cur == extmap.end())
         return WriteResult::FILE_NOT_HANDLED;

      OSType desiredType = cur->second;
      GraphicsExportComponent     geComp     = NULL;

      OSErr err = OpenADefaultComponent(GraphicsExporterComponentType, desiredType, &geComp);

      if (err != noErr) {
         osg::notify(osg::WARN) << "ReaderWriterQT: could not open Graphics epxorter for type " << ext << ", Err: " << err << std::endl;
         return WriteResult::FILE_NOT_HANDLED;
      }

      GWorldPtr gw = NULL;

      // we are converting the images back to 32bit, it seems, that quicktime can't handle others

      unsigned long desiredPixelFormat = k32ARGBPixelFormat;



      // we need to swizzle the colours again :)
      unsigned int numBytes = img.computeNumComponents(img.getPixelFormat());

      unsigned int buffWidth = img.s();
      unsigned int buffHeight = img.t();
      char * pixels = (char*) malloc(buffHeight * buffWidth * 4);



      const unsigned char *srcp = img.data();
      char *dstp=pixels;
      unsigned int i, j;
      for (i=0; i<buffHeight; i++ ) {

         switch (numBytes) {
            case 1 : 
               dstp[0] = 0;
               dstp[1] = srcp[0];
               dstp[2] = srcp[0];
               dstp[3] = srcp[0];
               srcp+=1;
               dstp+=4;

               break;
            case 3 :
               for (j=0; j<buffWidth; j++ ) {
                  dstp[0]=0;
                  dstp[1]=srcp[0];
                  dstp[2]=srcp[1];
                  dstp[3]=srcp[2];

                  srcp+=3;
                  dstp+=4;
               }
               break;

            case 4 :
               for (j=0; j<buffWidth; j++ ) {
                  // shift from RGBA to ARGB
                  dstp[0]=srcp[3];
                  dstp[1]=srcp[0];
                  dstp[2]=srcp[1];
                  dstp[3]=srcp[2];

                  srcp+=4;
                  dstp+=4;
               }
               break;
               
            default :
               // osg::notify(osg::WARN) << "ERROR IN RETURNED PIXEL DEPTH, CANNOT COPE" << std::endl;
               return WriteResult::ERROR_IN_WRITING_FILE;
               break;
         }
      }

      // Flip the image
      unsigned imageSize = buffWidth*buffHeight*4;
      char *tBuffer = (char*)malloc((size_t)imageSize);
      unsigned int rowBytes = buffWidth * 4;
      for (i = 0, j = imageSize - rowBytes; i < imageSize; i += rowBytes, j -= rowBytes)
         memcpy( &tBuffer[j], &pixels[i], (size_t)rowBytes );

      memcpy(pixels, tBuffer, (size_t)imageSize);
      free(tBuffer);

      FSSpec* fileSpec = NULL;

      try {
         Rect bounds;
         SetRect(&bounds, 0,0, img.s(), img.t());

         err = QTNewGWorldFromPtr(&gw, desiredPixelFormat, &bounds, 0,0,0, pixels, buffWidth*4);
         if (err != noErr) {
            osg::notify(osg::WARN) << "ReaderWriterQT: could not create gworld for type " << ext << ", Err: " << err << std::endl;
            throw err;
         }

         // create a dummy file at location
         FILE *fp = fopen(fileName.c_str(), "wb");
         if (!fp) {
            osg::notify(osg::WARN) << "ReaderWriterQT: could not create file!" << std::endl;
            throw err;
         }

         fclose(fp);

         // get an FSSpec to the file, so quicktime can handle the file.
         fileSpec = darwinPathToFSSpec( const_cast<char*>(fileName.c_str()) );
         if (fileSpec == NULL) {
            osg::notify(osg::WARN) << "ReaderWriterQT: could not get FSSpec" << std::endl;
            throw err;
         }

         err = GraphicsExportSetInputGWorld(geComp, gw);
         if (err != noErr) {
            osg::notify(osg::WARN) << "ReaderWriterQT: could not set input gworld for type " << ext << ", Err: " << err << std::endl;
            throw err;
         }

         err = GraphicsExportSetOutputFile(geComp, fileSpec); 
         if (err != noErr) {
            osg::notify(osg::WARN) << "ReaderWriterQT: could not set output file for type " << ext << ", Err: " << err << std::endl;
            throw err;
         } 

         // Set the compression quality (needed for JPEG, not necessarily for other formats)
         if (desiredType == kQTFileTypeJPEG) {
            err = GraphicsExportSetCompressionQuality(geComp, codecLosslessQuality);
            if (err != noErr) {
               osg::notify(osg::WARN) << "ReaderWriterQT: could not set compression for type " << ext << ", Err: " << err << std::endl;
               throw err;
            }
         }

         if(32 == numBytes)
         {
            err = GraphicsExportSetDepth( geComp,
               k32ARGBPixelFormat );    // depth
         }
         // else k24RGBPixelFormat???

         // do the export
         err = GraphicsExportDoExport(geComp, NULL);
         if (err != noErr) {
            osg::notify(osg::WARN) << "ReaderWriterQT: could not save file for type " << ext << ", Err: " << err << std::endl;
            throw err;
         } 

         if (geComp != NULL)
            CloseComponent(geComp);

         DisposeGWorld (gw);
         if (fileSpec != NULL ) free(fileSpec);   
         if (pixels) free(pixels);

         return WriteResult::FILE_SAVED;
      }


      catch (...) {

         if (geComp != NULL) CloseComponent(geComp);      
         if (gw != NULL) DisposeGWorld (gw);
         if (fileSpec != NULL ) free(fileSpec);
         if (pixels) free(pixels);

         return WriteResult::ERROR_IN_WRITING_FILE;
      }

   }


   mutable QuicktimeExitObserver _qtExitObserver;
};

// now register with Registry to instantiate the above
// reader/writer.
REGISTER_OSGPLUGIN(quicktime, ReaderWriterQT)
