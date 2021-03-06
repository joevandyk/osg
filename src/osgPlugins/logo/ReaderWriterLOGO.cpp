#include <stdio.h>
#include <osg/Geode>
#include <osg/Drawable>
#include <osg/BlendFunc>
#include <osg/StateSet>
#include <osg/Notify>
#include <osg/Viewport>

#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>
#include <osgDB/Registry>
#include <osgDB/Input>
#include <osgDB/Output>

#include <osgUtil/CullVisitor>

using namespace osg;
using namespace osgDB;


class Logos: public osg::Drawable
{
    public:
        enum RelativePosition{
            Center,
            UpperLeft,
            UpperRight,
            LowerLeft,
            LowerRight,
            UpperCenter,
            LowerCenter,
            last_position
        };

        struct logosCullCallback : public osg::Drawable::CullCallback
        {
            virtual bool cull(osg::NodeVisitor *visitor, osg::Drawable* drawable, osg::State*) const
            {
                Logos *logos = dynamic_cast <Logos *>(drawable);
                osgUtil::CullVisitor *cv = dynamic_cast<osgUtil::CullVisitor *>(visitor);
                if( logos != NULL && cv != NULL )
                {
                    osg::Viewport *vp = cv->getViewport();
                    if( vp != NULL )
                    {
                        if( vp->width() != logos->getViewport()->width() ||
                            vp->height() != logos->getViewport()->height() )
                        {
                            logos->getViewport()->setViewport( vp->x(), vp->y(), vp->width(), vp->height() );
                            logos->dirtyDisplayList(); 
                        }
                    }
                }
                return false;
            }
        };

        Logos() 
        {
            osg::StateSet *sset = new osg::StateSet;
            osg::BlendFunc *transp = new osg::BlendFunc;
            transp->setFunction(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
            sset->setAttribute( transp );
            sset->setMode( GL_BLEND, osg::StateAttribute::ON );
            sset->setMode( GL_DEPTH_TEST, osg::StateAttribute::OFF );
            sset->setTextureMode( 0, GL_TEXTURE_2D, osg::StateAttribute::OFF );
#if 1            
            // for now we'll crudely set the bin number to 100 to force it to draw later and ontop of the scene
            sset->setRenderBinDetails( 100 , "RenderBin" );
#else
            sset->setRenderBinDetails( StateSet::TRANSPARENT_BIN + 1 , "RenderBin" );
#endif
            setStateSet( sset );
            viewport = new osg::Viewport;
            setCullCallback( new logosCullCallback );
            _contextID = 0;
        }

        Logos(const Logos& logo, const CopyOp& copyop=CopyOp::SHALLOW_COPY) :Drawable( logo, copyop ) {}

        virtual Object* cloneType() const { return new Logos(); }
        virtual Object* clone( const CopyOp& copyop) const { return new Logos(*this, copyop ); }
        virtual bool isSameKindAs(const Object* obj) const { return dynamic_cast<const Logos*>(obj)!=NULL; }
        virtual const char* className() const { return "Logos"; }

        virtual void drawImplementation(osg::RenderInfo& renderInfo) const
        {
            if( renderInfo.getContextID() != _contextID ) 
                return;


            float vx = 0.0;
            float vy = 0.0;
            float vw = 1.0;
            float vh = 1.0;
            if (viewport)
            {
                vx = viewport->x();
                vy = viewport->y();
                vw = viewport->width();
                vh = viewport->height();
            }

            glMatrixMode( GL_PROJECTION );
            glPushMatrix();
            glLoadIdentity();
            glOrtho( 0.0, vw, 0.0, vh, -1.0, 1.0 );

            glMatrixMode( GL_MODELVIEW );
            glPushMatrix();
            glLoadIdentity();

            glColor4f( 1, 1, 1, 1 );

            std::vector <osg::Image *>::const_iterator p;
            float th = 0.0;
            for( p = logos[Center].begin(); p != logos[Center].end(); p++ )
                th += (*p)->t();

            float place[][4] = {
                { vw*0.5, ((vh*0.5) + th*0.5), -0.5, -1.0 },
                { vx, vh, 0.0, -1.0 },
                { vw, vh, -1.0, -1.0 },
                { vx, vy, 0.0, 1.0 },
                { vw, vy, -1.0, 1.0 },
                { vw*0.5, vh , -0.5, -1.0 },
                { vw*0.5, 0.0 , -0.5, 1.0 },
            };

            for( int i = Center; i < last_position; i++ )
            {
                if( logos[i].size() != 0 )
                {
                    float x = place[i][0];
                    float y = place[i][1];
                    float xi = place[i][2];
                    float yi = place[i][3];
                    for( p = logos[i].begin(); p != logos[i].end(); p++ )
                    {
                        osg::Image *img = *p;
                        x = place[i][0] + xi * img->s();
                        if( i == Center || i == UpperLeft || i == UpperRight || i == UpperCenter)
                            y += yi * img->t();
                        glRasterPos2f( x, y );
                        glDrawPixels( img->s(), img->t(), img->getPixelFormat(), img->getDataType(), img->data() );
                        if( i == LowerLeft || i == LowerRight || i == LowerCenter)
                            y += yi * img->t();
                    }
                }
            }

            glPopMatrix();
            glMatrixMode( GL_PROJECTION );
            glPopMatrix();
            glMatrixMode( GL_MODELVIEW );
        }

        void addLogo( RelativePosition pos, std::string name )
        {
            osg::Image *image = osgDB::readImageFile( name.c_str() );
            if( image != NULL )
                logos[pos].push_back( image ); 
            else
                osg::notify(osg::WARN)<< "Logos::addLogo image file not found : " << name << ".\n";
        }

        osg::Viewport *getViewport() { return viewport; }
        void setContextID( unsigned int id ) { _contextID = id; }

        bool hasLogos()
        {
            int n = 0;
            for( int i = Center; i <= last_position; i++ )
                n += logos[i].size();
            return (n != 0);
        }

        virtual osg::BoundingBox computeBound() const 
        {
            return osg::BoundingBox( -1, -1, -1, 1, 1, 1);
        }

    protected:
        Logos& operator = (const Logos&) { return *this;}

        virtual ~Logos() {}
    private :
        std::vector <osg::Image *> logos[last_position];
        osg::Viewport *viewport;
        unsigned int _contextID;
};


class LOGOReaderWriter : public osgDB::ReaderWriter
{
    public:
        LOGOReaderWriter()
        {
            supportsExtension("logo","Ascii logo placement format");
        }
        
        virtual const char* className() const { return "Logo Database Reader/Writer"; }

        virtual ReadResult readNode(const std::string& file, const osgDB::ReaderWriter::Options* options) const
        {
            std::string ext = osgDB::getLowerCaseFileExtension(file);
            if (!acceptsExtension(ext)) return ReadResult::FILE_NOT_HANDLED;

            std::string fileName = osgDB::findDataFile( file, options );
            if (fileName.empty()) 
                return ReadResult::FILE_NOT_FOUND;

            osg::notify(osg::INFO)<< "ReaderWriterLOGO::readNode( "<<fileName.c_str()<<" )\n";

            std::string filePath = osgDB::getFilePath(fileName);
            if (!filePath.empty()) {
                osg::notify(osg::DEBUG_INFO)<< "Adding : " << filePath << " to the file data path\n";
                osgDB::getDataFilePathList().push_back(filePath);
            }


            osg::Geode *geode = new osg::Geode;

            unsigned int screen = 0;

            Logos* ld = new Logos;
            ld->setContextID( screen );

            Logos::RelativePosition pos = Logos::LowerRight;

            FILE *fp;
            if( (fp = fopen( fileName.c_str(), "r")) == NULL )
                return NULL;
            while( !feof(fp))
            {
                char buff[128];

                if( fscanf( fp, "%s", buff ) != 1 )
                    break;

                std::string str(buff);

                if( str == "Center" )
                    pos = Logos::Center;
                else if( str == "UpperLeft" )
                    pos = Logos::UpperLeft;
                else if( str == "UpperRight" )
                    pos = Logos::UpperRight;
                else if( str == "LowerLeft" )
                    pos = Logos::LowerLeft;
                else if( str == "LowerRight" )
                    pos = Logos::LowerRight;
                else if( str == "UpperCenter" )
                    pos = Logos::UpperCenter;
                else if( str == "LowerCenter" )
                    pos = Logos::LowerCenter;
                else if( str == "Camera" )
                {
                    int tn;
                    if( (fscanf( fp, "%d", &tn )) != 1 )
                    {
                        osg::notify(osg::WARN) << "Error... Camera requires an integer argument\n";
                        break;
                    }
                    
                    if (tn < 0)
                    {
                        osg::notify(osg::WARN) << "Error... Camera requires an positive or null value argument\n";
                        break;
                    }
                    
                    unsigned int n = static_cast<unsigned int>(tn);
                    if( screen != n )
                    {
                        screen = n;
                        if( ld->hasLogos() )
                        {
                        geode->addDrawable( ld );
                        ld = new Logos;
                        ld->setContextID( screen );
                    }
                    else
                        ld->setContextID( screen );
                    }
                }
                else
                {
                    if( str.length() )
                    ld->addLogo( pos, str );
                }
            }
            fclose( fp );
       
            if( ld->hasLogos() )
                geode->addDrawable( ld );
        
            geode->setCullingActive(false);
            return geode;
        }
};


// now register with Registry to instantiate the above
// reader/writer.
REGISTER_OSGPLUGIN(logo, LOGOReaderWriter)
