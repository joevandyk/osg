#ifdef USE_MEM_CHECK
#include <mcheck.h>
#endif

#include <osg/Group>
#include <osg/Notify>

#include <osgDB/Registry>
#include <osgDB/ReadFile>

#include <osgUtil/TrackballManipulator>
#include <osgUtil/FlightManipulator>
#include <osgUtil/DriveManipulator>

#include <GL/glut.h>
#include <osgGLUT/Viewer>

#include <osg/Quat>

#include "receiver.h"
#include "broadcaster.h"


class CameraPacket {
    public:
    
        CameraPacket():_masterKilled(false) {}
        
        void setPacket(const osg::Camera& camera,const osg::FrameStamp* frameStamp)
        {
            _eye    = camera.getEyePoint();
            _center = camera.getCenterPoint();
            _up     = camera.getUpVector();
            if (frameStamp)
            {
                _frameStamp    = *frameStamp;
            }
        }
        
        void getCamera(osg::Camera& camera,float angle_offset=0.0f)
        {
        
            osg::Vec3 lv = _center-_eye;
            osg::Matrix matrix;
            matrix.makeIdent();
            matrix.makeRot(angle_offset,_up.x(),_up.y(),_up.z());
            lv = lv*matrix;
        
            camera.setLookAt(_eye,_eye+lv,_up);
                        
        }
        
        void getSceneViewUpdate(osgUtil::SceneView& sv)
        {
            // note pass a seperate reference counted FrameStamp
            // rather than this frame stamp as it can get overwritten.
            sv.setFrameStamp(new osg::FrameStamp(_frameStamp));
        }
        
        void setMasterKilled(const bool flag) { _masterKilled = flag; }
        const bool getMasterKilled() const { return _masterKilled; }
        
        bool            _masterKilled;
        osg::Vec3       _eye;
        osg::Vec3       _center;
        osg::Vec3       _up;
        bool            _attachMatrix;
        osg::Matrix     _matrix;

        // note don't use a ref_ptr as used elsewhere for FrameStamp
        // since we don't want to copy the pointer - but the memory.
        // FrameStamp doesn't have a private destructor to allow
        // us to do this, even though its a reference counted object.    
        osg::FrameStamp  _frameStamp;
        
};


class MySceneView : public osgUtil::SceneView {

    public:
    
        enum ViewerMode
        {
            STAND_ALONE,
            SLAVE,
            MASTER
        };
    
        MySceneView(ViewerMode viewerMode,int socketNumber,float camera_fov, float camera_offset):
            _viewerMode(viewerMode),_socketNumber(socketNumber),
            _camera_fov(camera_fov), _camera_offset(camera_offset)
        {
            setDefaults();
            getCamera()->setAdjustAspectRatioMode(osg::Camera::ADJUST_VERTICAL);
            getCamera()->setFOV(camera_fov,camera_fov*(600.0f/800.0f),1.0f,1000.0f);
            
            _bc.setPort(socketNumber);
            _rc.setPort(socketNumber);
        };
        
        ~MySceneView()
        {
            if (_viewerMode==MASTER)
            {
                // need to broadcast my death.
                CameraPacket cp;
                cp.setPacket(*getCamera(),getFrameStamp());
                cp.setMasterKilled(true);
                
                _bc.setBuffer(&cp, sizeof( CameraPacket ));
	        _bc.sync();
                
                cout << "broadcasting death"<<endl;
                
            }
        }
        
        // override the basic SceneView::app traversal.
        virtual void app()
        {
            osgUtil::SceneView::app();
            switch (_viewerMode)
            {
            case(MASTER):
                {
                    CameraPacket cp;
                    cp.setPacket(*getCamera(),getFrameStamp());

                    _bc.setBuffer(&cp, sizeof( CameraPacket ));
	            _bc.sync();

                }
                break;
            case(SLAVE):
                {
                    CameraPacket cp;

                    _rc.setBuffer(&cp, sizeof( CameraPacket ));
	            _rc.sync();

                    cp.getCamera(*getCamera(),_camera_offset);
                    cp.getSceneViewUpdate(*this);
                    
                    if (cp.getMasterKilled()) 
                    {
                        cout << "recieved master killed"<<endl;
                        _viewerMode = STAND_ALONE;
                    }
                }
                break;
            default:
                // no need to anything here, just a normal interactive viewer.
                break;
            }
        }
        
    protected:
    
        ViewerMode      _viewerMode;
        int             _socketNumber;
        float           _camera_fov;
        float           _camera_offset;
        

        Broadcaster     _bc;
        Receiver        _rc;


};

/*
 * Function to read several files (typically one) as specified on the command
 * line, and return them in an osg::Node
 */
osg::Node* getNodeFromFiles(int argc,char **argv, 
                            MySceneView::ViewerMode& viewerMode, int& socketNumber,
                            float& camera_fov, float& camera_offset)
{
    osg::Node *rootnode = new osg::Node;

    int i;

    typedef std::vector<osg::Node*> NodeList;
    NodeList nodeList;
    for( i = 1; i < argc; i++ )
    {

        if (argv[i][0]=='-')
        {
            switch(argv[i][1])
            {

                case('m'):
                    viewerMode = MySceneView::MASTER;
                    break;
                case('s'):
                    viewerMode = MySceneView::SLAVE;
                    break;
                case('n'):
                    ++i;
                    if (i<argc)
                    {
                        socketNumber = atoi(argv[i]);
                    }
                    break;
                case('f'):
                    ++i;
                    if (i<argc)
                    {
                        camera_fov = atoi(argv[i]);
                    }
                    break;
                case('o'):
                    ++i;
                    if (i<argc)
                    {
                        camera_offset = atoi(argv[i]);
                    }
                    break;
                    
                case('l'):
                    ++i;
                    if (i<argc)
                    {
                        osgDB::Registry::instance()->loadLibrary(argv[i]);
                    }
                    break;
                case('e'):
                    ++i;
                    if (i<argc)
                    {
                        std::string libName = osgDB::Registry::instance()->createLibraryNameForExt(argv[i]);
                        osgDB::Registry::instance()->loadLibrary(libName);
                    }
                    break;
            }
        } else
        {
            osg::Node *node = osgDB::readNodeFile( argv[i] );

            if( node != (osg::Node *)0L )
            {
                if (node->getName().empty()) node->setName( argv[i] );
                nodeList.push_back(node);
            }
        }

    }

    if (nodeList.size()==0)
    {
        osg::notify(osg::WARN) << "No data loaded."<<endl;
        exit(0);
    }
    
    
/*
    if (master) osg::notify(osg::NOTICE)<<"set to MASTER, broadcasting on socketNumber "<<socketNumber<<endl;
    else osg::notify(osg::NOTICE)<<"set to SLAVE, reciving on socketNumber "<<socketNumber<<endl;
    
*/
    

    if (nodeList.size()==1)
    {
        rootnode = nodeList.front();
    }
    else                         // size >1
    {
        osg::Group* group = new osg::Group();
        for(NodeList::iterator itr=nodeList.begin();
            itr!=nodeList.end();
            ++itr)
        {
            group->addChild(*itr);
        }

        rootnode = group;
    }

    return rootnode;
}


int main( int argc, char **argv )
{

#ifdef USE_MEM_CHECK
    mtrace();
#endif

    // initialize the GLUT
    glutInit( &argc, argv );

    if (argc<2)
    {
        osg::notify(osg::NOTICE)<<"usage:"<<endl;
        osg::notify(osg::NOTICE)<<"    osgcluster [options] infile1 [infile2 ...]"<<endl;
        osg::notify(osg::NOTICE)<<endl;
        osg::notify(osg::NOTICE)<<"options:"<<endl;
        osg::notify(osg::NOTICE)<<"    -m                 - set this viewer to be master"<<endl;
        osg::notify(osg::NOTICE)<<"    -s                 - set this viewer to be a slave"<<endl;
        osg::notify(osg::NOTICE)<<"    -o                 - offset the slave camera from the master position"<<endl;
        osg::notify(osg::NOTICE)<<"                         by specified number of degress. A positive offset "<<endl;
        osg::notify(osg::NOTICE)<<"                         turns camera towards right."<<endl;
        osg::notify(osg::NOTICE)<<"    -f                 - set the horizontal field of view of the camera."<<endl;
        osg::notify(osg::NOTICE)<<"    -n SocketNumber    - set the socket number, defaults to 8100."<<endl;
        osg::notify(osg::NOTICE)<<"                         to broadcast on if a master"<<endl;
        osg::notify(osg::NOTICE)<<"                         to reciever on if a slave"<<endl;
        osg::notify(osg::NOTICE)<<endl;
        osg::notify(osg::NOTICE)<<"    -l libraryName     - load plugin of name libraryName"<<endl;
        osg::notify(osg::NOTICE)<<"                         i.e. -l osgdb_pfb"<<endl;
        osg::notify(osg::NOTICE)<<"                         Useful for loading reader/writers which can load"<<endl;
        osg::notify(osg::NOTICE)<<"                         other file formats in addition to its extension."<<endl;
        osg::notify(osg::NOTICE)<<"    -e extensionName   - load reader/wrter plugin for file extension"<<endl;
        osg::notify(osg::NOTICE)<<"                         i.e. -e pfb"<<endl;
        osg::notify(osg::NOTICE)<<"                         Useful short hand for specifying full library name as"<<endl;
        osg::notify(osg::NOTICE)<<"                         done with -l above, as it automatically expands to the"<<endl;
        osg::notify(osg::NOTICE)<<"                         full library name appropriate for each platform."<<endl;
        osg::notify(osg::NOTICE)<<endl;

        return 0;
    }

    osg::Timer timer;
    osg::Timer_t before_load = timer.tick();
    
    MySceneView::ViewerMode viewerMode = MySceneView::STAND_ALONE;
    int socketNumber=8100;
    float camera_fov=45.0f;
    float camera_offset=45.0f;

    osg::Node* rootnode = getNodeFromFiles( argc, argv, viewerMode, socketNumber,camera_fov,camera_offset);
    
    osg::Timer_t after_load = timer.tick();
    cout << "Time for load = "<<timer.delta_s(before_load,after_load)<<" seconds"<<endl;

    osg::ref_ptr<MySceneView> mySceneView = new MySceneView(viewerMode,socketNumber,camera_fov,osg::inDegrees(camera_offset));
    mySceneView->setSceneData(rootnode);

    // initialize the viewer.
    osgGLUT::Viewer viewer;
    viewer.addViewport( mySceneView.get() );
    
    // register trackball, flight and drive.
    viewer.registerCameraManipulator(new osgUtil::TrackballManipulator);
    viewer.registerCameraManipulator(new osgUtil::FlightManipulator);
    viewer.registerCameraManipulator(new osgUtil::DriveManipulator);

    viewer.open();
    viewer.run();

    return 0;
}
