#include <osg/Image>
#include <osg/Notify>
#include <osg/Geode>
#include <osg/GL>

#include <osgDB/Registry>
#include <osgDB/FileNameUtils>
#include <osgDB/FileUtils>

#include "MpegImageStream.h"

class ReaderWriterMPEG : public osgDB::ReaderWriter
{
    public:

        virtual const char* className() const { return "MPEG ImageStream Reader"; }

        virtual bool acceptsExtension(const std::string& extension) const
        {
            return osgDB::equalCaseInsensitive(extension,"mpg") ||
                   osgDB::equalCaseInsensitive(extension,"mpv");
        }

        virtual ReadResult readImage(const std::string& file, const osgDB::ReaderWriter::Options* options) const
        {
            std::string ext = osgDB::getLowerCaseFileExtension(file);
            if (!acceptsExtension(ext)) return ReadResult::FILE_NOT_HANDLED;

            std::string fileName = osgDB::findDataFile( file, options );
            if (fileName.empty()) return ReadResult::FILE_NOT_FOUND;

            osg::MpegImageStream* mpeg = new osg::MpegImageStream(fileName.c_str());
            mpeg->play();
            
            return mpeg;
        }
};

// now register with Registry to instantiate the above
// reader/writer.
osgDB::RegisterReaderWriterProxy<ReaderWriterMPEG> g_readerWriter_MPEG_Proxy;
