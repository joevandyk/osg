#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <list>
#include <string>
#include <fstream>
#include <sstream>

#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/Texture2D>
#include <osg/Geometry>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/BlendFunc>
#include <osg/ClearNode>
#include <osg/Depth>
#include <osg/Projection>

#include <osgUtil/Tesselator>
#include <osgUtil/TransformCallback>
#include <osgUtil/CullVisitor>
#include <osgUtil/Optimizer>

#include <osgText/Text>

#include <osgGA/TrackballManipulator>

#include <osgProducer/Viewer>

#include <osgDB/ReadFile>
#include <osgDB/FileUtils>

int runApp(std::string xapp);

// class to handle events with a pick
class PickHandler : public osgGA::GUIEventHandler {
public: 

    PickHandler(osgProducer::Viewer* viewer,osgText::Text* updateText):
        _viewer(viewer),
        _updateText(updateText) {}
        
    ~PickHandler() {}
    
    bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& us);

    std::string pick(float x, float y);
    
    void highlight(const std::string& name)
    {
        if (_updateText.get()) _updateText->setText(name);
    }
    
protected:

    osgProducer::Viewer* _viewer;
    osg::ref_ptr<osgText::Text>  _updateText;
};

bool PickHandler::handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter&)
{
    switch(ea.getEventType())
    {
    case(osgGA::GUIEventAdapter::FRAME):
    case(osgGA::GUIEventAdapter::MOVE):
    {
        //osg::notify(osg::NOTICE)<<"MOVE "<<ea.getX()<<ea.getY()<<std::endl;
        std::string picked_name = pick(ea.getX(),ea.getY());
        highlight(picked_name);
        return false;
    }
    case(osgGA::GUIEventAdapter::PUSH):
    {
        //osg::notify(osg::NOTICE)<<"PUSH "<<ea.getX()<<ea.getY()<<std::endl;
        std::string picked_name = pick(ea.getX(),ea.getY());
        if (!picked_name.empty())
        {
            runApp(picked_name);
            return true;
        }
        else
        {
            return false;
        }
    }
    default:
        return false;
    }
}


std::string PickHandler::pick(float x, float y)
{
    osgUtil::IntersectVisitor::HitList hlist;
    if (_viewer->computeIntersections(x, y, hlist))
    {
        for(osgUtil::IntersectVisitor::HitList::iterator hitr=hlist.begin();
            hitr!=hlist.end();
            ++hitr)
        {
            if (hitr->_geode.valid() && !hitr->_geode->getName().empty()) return hitr->_geode->getName();
        }
    }
    return "";
}

osg::Node* createHUD(osgText::Text* updateText)
{    // create the hud. derived from osgHud.cpp
    // adds a set of quads, each in a separate Geode - which can be picked individually
    // eg to be used as a menuing/help system!
    // Can pick texts too!
    osg::MatrixTransform* modelview_abs = new osg::MatrixTransform;
    modelview_abs->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    modelview_abs->setMatrix(osg::Matrix::identity());
    
    osg::Projection* projection = new osg::Projection;
    projection->setMatrix(osg::Matrix::ortho2D(0,1280,0,1024));
    projection->addChild(modelview_abs);
    
    
    std::string timesFont("fonts/times.ttf");
    
    // turn lighting off for the text and disable depth test to ensure its always ontop.
    osg::Vec3 position(50.0f,510.0f,0.0f);
    osg::Vec3 delta(0.0f,-60.0f,0.0f);

    { // this displays what has been selected
        osg::Geode* geode = new osg::Geode();
        osg::StateSet* stateset = geode->getOrCreateStateSet();
        stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
        stateset->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
        geode->setName("The text label");
        geode->addDrawable( updateText );
        modelview_abs->addChild(geode);
        
        updateText->setCharacterSize(20.0f);
        updateText->setFont(timesFont);
        updateText->setColor(osg::Vec4(1.0f,1.0f,0.0f,1.0f));
        updateText->setText("");
        updateText->setPosition(position);
        
        position += delta;
    }    
    
    return projection;

} // end create HUDf




static osg::Vec3 defaultPos( 0.0f, 0.0f, 0.0f );
static osg::Vec3 centerScope(0.0f, 0.0f, 0.0f);

class Xample
{
    std::string texture;
    std::string app;
  public:
      Xample(std::string image, std::string prog)
    { 
        texture    = image;
        app     = prog;
        osg::notify(osg::INFO) << "New Xample!" << std::endl;
    };
    ~Xample() { };
    
    std::string getTexture()
    {
        return texture;
    }
    std::string getApp()
    {
        return app;
    }
}; // end class Xample


typedef std::list<Xample>::iterator OP;
static std::list<Xample> Xamplelist;


void printList()
{
    osg::notify(osg::INFO) << "start printList()" << std::endl;
    for (OP i = Xamplelist.begin() ; i != Xamplelist.end() ; ++i)
    {
        Xample& x = *i;
        osg::notify(osg::INFO) << "current x.texture = " << x.getTexture() << std::endl;
        osg::notify(osg::INFO) << "current x.app = " << x.getApp() << std::endl;
    }
    osg::notify(osg::INFO) << "end printList()" << std::endl;
} // end printList()


int runApp(std::string xapp)
{
    osg::notify(osg::INFO) << "start runApp()" << std::endl;
    for (OP i = Xamplelist.begin() ; i != Xamplelist.end() ; ++i)
    {
        Xample& x = *i;
        if(!xapp.compare(x.getApp()))
        {
            osg::notify(osg::INFO) << "app found!" << std::endl;
            
            const char* cxapp = xapp.c_str();
            
            osg::notify(osg::INFO) << "char* = " << cxapp <<std::endl;
            
            system(cxapp);
            return 1;
        }
    }
    osg::notify(osg::INFO) << "app not found!" << std::endl;
    return 0;
} // end printList()


void readConfFile(char* confFile)                                                                // read confFile            1
{
    osg::notify(osg::INFO) << "Start reading confFile" << std::endl;
    
    std::string fileName = osgDB::findDataFile(confFile);
    if (fileName.empty())
    {
        osg::notify(osg::INFO) << "Config file not found"<<confFile << std::endl;
        return;
    }
    

    std::ifstream in(fileName.c_str());
    if (!in)
    {
        osg::notify(osg::INFO) << "File " << fileName << " can not be opened!" << std::endl;
        exit(1);
    }
    std::string imageBuffer;
    std::string appBuffer;
    
    while (!in.eof())
    {
        getline(in, imageBuffer);
        getline(in, appBuffer);
        if(imageBuffer == "" || appBuffer == "");
        else
        {
            osg::notify(osg::INFO) << "imageBuffer: " << imageBuffer << std::endl;
            osg::notify(osg::INFO) << "appBuffer: " << appBuffer << std::endl;
//            jeweils checken ob image vorhanden ist.
            
            Xample tmp(imageBuffer, appBuffer);                                                    // create Xample objects    2
            
            Xamplelist.push_back(tmp);                                                            // store objects in list    2
            
        }
    }
    
    in.close();
    
    osg::notify(osg::INFO) << "End reading confFile" << std::endl;
    
    printList();
} // end readConfFile


void SetObjectTextureState(osg::Geode *geodeCurrent, std::string texture)
{
    // retrieve or create a StateSet
    osg::StateSet* stateTexture = geodeCurrent->getOrCreateStateSet();

    // load texture.jpg as an image
    osg::Image* imgTexture = osgDB::readImageFile( texture );
    
    // if the image is successfully loaded
    if (imgTexture)
    {
        // create a new two-dimensional texture object
        osg::Texture2D* texCube = new osg::Texture2D;

        // set the texture to the loaded image
        texCube->setImage(imgTexture);

        // set the texture to the state
        stateTexture->setTextureAttributeAndModes(0,texCube,osg::StateAttribute::ON);

        // set the state of the current geode
        geodeCurrent->setStateSet(stateTexture);
    }
} // end SetObjectTextureState


osg::Geode* createTexturedCube(float fRadius,osg::Vec3 vPosition, std::string texture, std::string geodeName)
{
    // create a cube shape
    osg::Box *bCube = new osg::Box(vPosition,fRadius);
    // osg::Box *bCube = new osg::Box(vPosition,fRadius);

    // create a container that makes the cube drawable
    osg::ShapeDrawable *sdCube = new osg::ShapeDrawable(bCube);

    // create a geode object to as a container for our drawable cube object
    osg::Geode* geodeCube = new osg::Geode();
    geodeCube->setName( geodeName );

    // set the object texture state
    SetObjectTextureState(geodeCube, texture);

    // add our drawable cube to the geode container
    geodeCube->addDrawable(sdCube);

    return(geodeCube);
} // end CreateCube


osg::PositionAttitudeTransform* getPATransformation(osg::Node* object, osg::Vec3 position, osg::Vec3 scale, osg::Vec3 pivot)
{
    osg::PositionAttitudeTransform* tmpTrans = new osg::PositionAttitudeTransform();
    tmpTrans->addChild( object );
    
    tmpTrans->setPosition( position );
    tmpTrans->setScale( scale );
    tmpTrans->setPivotPoint( pivot );
    
    return tmpTrans;
}

void printBoundings(osg::Node* current, std::string name)
{
    const osg::BoundingSphere& currentBound = current->getBound();
    osg::notify(osg::INFO) << name << std::endl;
    osg::notify(osg::INFO) << "center = " << currentBound.center() << std::endl;
    osg::notify(osg::INFO) << "radius = " << currentBound.radius() << std::endl;
    
//    return currentBound.radius();
}


osg::Group* setupGraph()                                                                        // create Geodes/Nodes from Xamplelist    3
{
    osg::Group* xGroup = new osg::Group();

    
//    positioning and sizes
    float defaultRadius = 0.8f;

    int itemsInLine    = 4;                                    // name says everything
    float offset    = 0.05f;
    float bs        = (defaultRadius / 4) + offset;
    float xstart    = (3*bs) * (-1);
    float zstart    = xstart * (-1);
    float xnext        = xstart;
    float znext        = zstart;
    float xjump        = (2*bs);
    float zjump        = xjump;
    osg::Vec3 vScale( 0.5f, 0.5f, 0.5f );
    osg::Vec3 vPivot( 0.0f, 0.0f, 0.0f );    

//  run through Xampleliste
    int z = 1;
    for (OP i = Xamplelist.begin() ; i != Xamplelist.end() ; ++i, ++z)
    {
        Xample& x = *i;
        
        osg::Node* tmpCube = createTexturedCube(defaultRadius, defaultPos, x.getTexture(), x.getApp());
        printBoundings(tmpCube, x.getApp());
        osg::Vec3 vPosition( xnext, 0.0f, znext );
        osg::PositionAttitudeTransform*    transX = getPATransformation(tmpCube, vPosition, vScale, vPivot);
        xGroup->addChild( transX );
        
        // line feed
        if(z < itemsInLine)
            xnext += xjump;
        else
        {
            xnext = xstart;
            znext -= zjump;
            z = 0;
        }
    } // end run through list    
    
    return xGroup;
} // end setupGraph


int main( int argc, char **argv )
{
    if (argc<=1)
    {
        readConfFile("osg.conf");                                                                          // read ConfigFile        1
    }
    else
    {
        readConfFile(argv[1]);                                                                          // read ConfigFile        1
    }
    
    // construct the viewer.
    osgProducer::Viewer viewer;

    // set up the value with sensible default event handlers.
    viewer.setUpViewer(osgProducer::Viewer::STANDARD_SETTINGS);
   
    osg::ref_ptr<osgText::Text> updateText = new osgText::Text;

    // add the handler for doing the picking
    viewer.getEventHandlerList().push_front(new PickHandler(&viewer,updateText.get()));

    osg::Group* root = new osg::Group();

    root->addChild( setupGraph() );

    // add the HUD subgraph.    
    root->addChild(createHUD(updateText.get()));
   
    // add model to viewer.
    viewer.setSceneData( root );

    // create the windows and run the threads.
    viewer.realize();
    
    osg::Matrix lookAt;
    lookAt.makeLookAt(osg::Vec3(0.0f, -4.0f, 0.0f), centerScope, osg::Vec3(0.0f, 0.0f, 1.0f));

    //viewer.setView(lookAt);
        
    while( !viewer.done() )
    {
        // wait for all cull and draw threads to complete.
        viewer.sync();

        // update the scene by traversing it with the the update visitor which will
        // call all node update callbacks and animations.
        viewer.update();

        // to be able to turn scene, place next call before viewer.update()
        viewer.setView(lookAt);

    
        // fire off the cull and draw traversals of the scene.
        viewer.frame();
        
    }
    
    // wait for all cull and draw threads to complete before exit.
    viewer.sync();

    return 0;
} // end main
