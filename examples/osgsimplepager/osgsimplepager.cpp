// C++ source file - (C) 2003 Robert Osfield, released under the OSGPL.
//
// Simple example of use of Producer::RenderSurface + KeyboardMouseCallback + SceneView
// example that provides the user with control over view position with support for
// enabling the osgDB::DatabasePager as required by the PagedLOD and TXP databases.

#include <Producer/RenderSurface>
#include <Producer/KeyboardMouse>
#include <Producer/Trackball>

#include <osg/Timer>

#include <osgUtil/SceneView>
#include <osgUtil/IntersectVisitor>

#include <osgDB/ReadFile>

#include <osgFX/Scribe>


class MyKeyboardMouseCallback : public Producer::KeyboardMouseCallback
{
public:

    MyKeyboardMouseCallback(osgUtil::SceneView* sceneView) :
        Producer::KeyboardMouseCallback(),
        _mx(0.0f),_my(0.0f),_mbutton(0),
        _done(false),
        _trackBall(new Producer::Trackball),
        _sceneView(sceneView)
    {
        resetTrackball();
    }

    virtual void specialKeyPress( Producer::KeyCharacter key )
    {
        if (key==Producer::KeyChar_Escape)
            shutdown();
    }

    virtual void shutdown()
    {
        _done = true; 
    }

    virtual void keyPress( Producer::KeyCharacter key)
    {
        if (key==' ') resetTrackball();
    }

    virtual void mouseMotion( float mx, float my ) 
    {
        _mx = mx;
        _my = my;
    }

    virtual void buttonPress( float mx, float my, unsigned int mbutton ) 
    {
         _mx = mx;
         _my = my;
         _mbutton |= (1<<(mbutton-1));
         
         _mx_buttonPress = _mx;
         _my_buttonPress = _my;
    }
    virtual void buttonRelease( float mx, float my, unsigned int mbutton ) 
    {
        _mx = mx;
        _my = my;
        _mbutton &= ~(1<<(mbutton-1));
    }

    bool done() { return _done; }
    float mx()  { return _mx; }
    float my()  { return _my; }
    unsigned int mbutton()  { return _mbutton; }
    
    void resetTrackball()
    {
        osg::Node* scene = _sceneView->getSceneData();
        if (scene)
        {
            const osg::BoundingSphere& bs = scene->getBound();
            _trackBall->reset();
            _trackBall->setOrientation( Producer::Trackball::Z_UP );
            _trackBall->setDistance(bs.radius()*2.0f);
            _trackBall->translate(-bs.center().x(),-bs.center().y(),-bs.center().z());
        }
    }
    
    osg::Matrixd getViewMatrix()
    {
	_trackBall->input( mx(), my(), mbutton() );
        return osg::Matrixd(_trackBall->getMatrix().ptr());
    }

private:

    float                               _mx, _my;
    float                               _mx_buttonPress, _my_buttonPress;
    unsigned int                        _mbutton;
    
    bool                                _done;
    
    osg::ref_ptr<Producer::Trackball>   _trackBall;
    osg::ref_ptr<osgUtil::SceneView>    _sceneView;
};

int main( int argc, char **argv )
{
    if (argc<2) 
    {
        std::cout << argv[0] <<": requires filename argument." << std::endl;
        return 1;
    }

    // load the scene.
    osg::ref_ptr<osg::Node> loadedModel = osgDB::readNodeFile(argv[1]);
    if (!loadedModel) 
    {
        std::cout << argv[0] <<": No data loaded." << std::endl;
        return 1;
    }
    
    // create the window to draw to.
    osg::ref_ptr<Producer::RenderSurface> renderSurface = new Producer::RenderSurface;
    renderSurface->setWindowName("osgsimplepager");
    renderSurface->setWindowRectangle(100,100,800,600);
    renderSurface->useBorder(true);
    renderSurface->realize();
    

    // create the view of the scene.
    osg::ref_ptr<osgUtil::SceneView> sceneView = new osgUtil::SceneView;
    sceneView->setDefaults();
    sceneView->setSceneData(loadedModel.get());
    
    
    // do the set up of the DatabasePager...
    // create the database pager via the osgDB::Registry
    osgDB::DatabasePager* databasePager = osgDB::Registry::instance()->getOrCreateDatabasePager();

    // set wether the DatabasePager thread should be blocked while the scene graph is being used to render a frame
    // setting frame block to true can help achieve constant frame rates on single CPU systems.
    databasePager->setUseFrameBlock(false);

    // register any PagedLOD that need to be tracked in the scene graph
    databasePager->registerPagedLODs(sceneView->getSceneData());

    // need to register the DatabasePager with the SceneView's CullVisitor so it can pass on request
    // for files to be loaded.
    sceneView->getCullVisitor()->setDatabaseRequestHandler(databasePager);        


    // set up a KeyboardMouse to manage the events comming in from the RenderSurface
    osg::ref_ptr<Producer::KeyboardMouse>  kbm = new Producer::KeyboardMouse(renderSurface.get());

    // create a KeyboardMouseCallback to handle the mouse events within this applications
    osg::ref_ptr<MyKeyboardMouseCallback> kbmcb = new MyKeyboardMouseCallback(sceneView.get());


    // record the timer tick at the start of rendering.    
    osg::Timer_t start_tick = osg::Timer::instance()->tick();
    
    unsigned int frameNum = 0;

    // main loop (note, window toolkits which take control over the main loop will require a window redraw callback containing the code below.)
    while( renderSurface->isRealized() && !kbmcb->done())
    {
        // set up the frame stamp for current frame to record the current time and frame number so that animtion code can advance correctly
        osg::FrameStamp* frameStamp = new osg::FrameStamp;
        frameStamp->setReferenceTime(osg::Timer::instance()->delta_s(start_tick,osg::Timer::instance()->tick()));
        frameStamp->setFrameNumber(frameNum++);
        
        // pass frame stamp to the SceneView so that the update, cull and draw traversals all use the same FrameStamp
        sceneView->setFrameStamp(frameStamp);

        // pass any keyboard mouse events onto the local keyboard mouse callback.        
        kbm->update( *kbmcb );
        
        // set the view
        sceneView->setViewMatrix(kbmcb->getViewMatrix());

        // update the viewport dimensions, incase the window has been resized.
        sceneView->setViewport(0,0,renderSurface->getWindowWidth(),renderSurface->getWindowHeight());


        // tell the DatabasePager the frame number of that the scene graph is being actively used to render a frame
        databasePager->signalBeginFrame(frameStamp);

        // syncronize changes required by the DatabasePager thread to the scene graph
        databasePager->updateSceneGraph(frameStamp->getReferenceTime());

        // do the update traversal the scene graph - such as updating animations
        sceneView->update();
        
        // do the cull traversal, collect all objects in the view frustum into a sorted set of rendering bins
        sceneView->cull();
        
        // draw the rendering bins.
        sceneView->draw();
        

        // tell the DatabasePager that the frame is complete and that scene graph is no longer be activity traversed.
        databasePager->signalEndFrame();

        // Swap Buffers
        renderSurface->swapBuffers();
        
 
        // clean up  and compile gl objects with a specified limit       
        double availableTime = 0.0025; // 2.5 ms
        
        // compile any GL objects that are required.
        databasePager->compileGLObjects(*(sceneView->getState()),availableTime);

        // flush deleted GL objects.
        sceneView->flushDeletedGLObjects(availableTime);
        

        
    }

    // switch off the database pager by unreferning it.
    osgDB::Registry::instance()->setDatabasePager(0);

    return 0;
}

