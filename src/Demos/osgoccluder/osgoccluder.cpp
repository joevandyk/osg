#include <osg/GL>
#include <osgGLUT/glut>
#include <osgGLUT/Viewer>

#include <osg/MatrixTransform>
#include <osg/Billboard>
#include <osg/Geode>
#include <osg/Group>
#include <osg/Notify>
#include <osg/LineSegment>

#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>

#include <osgUtil/Optimizer>
#include <osgUtil/IntersectVisitor>

#include <osg/OccluderNode>
#include <osg/Geometry>

void write_usage(std::ostream& out,const std::string& name)
{
    out << std::endl;
    out <<"usage:"<< std::endl;
    out <<"    "<<name<<" [options] infile1 [infile2 ...]"<< std::endl;
    out << std::endl;
    out <<"options:"<< std::endl;
    out <<"    -c                  - set osgoccluder's manual mode for creating occluders."<< std::endl;
    out <<"                          press 'a' to start/add points to an occluder, using "<< std::endl;
    out <<"                                the nearest vertex to the current mouse poistion."<< std::endl;
    out <<"                          press 'e' to end occluder, this occluder is then added"<< std::endl;
    out <<"                                    into the model."<< std::endl;
    out <<"                          press 'O' save just the occluders to disk. "<< std::endl;
    out << std::endl;
    out <<"    -l libraryName      - load plugin of name libraryName"<< std::endl;
    out <<"                          i.e. -l osgdb_pfb"<< std::endl;
    out <<"                          Useful for loading reader/writers which can load"<< std::endl;
    out <<"                          other file formats in addition to its extension."<< std::endl;
    out <<"    -e extensionName    - load reader/wrter plugin for file extension"<< std::endl;
    out <<"                          i.e. -e pfb"<< std::endl;
    out <<"                          Useful short hand for specifying full library name as"<< std::endl;
    out <<"                          done with -l above, as it automatically expands to"<< std::endl;
    out <<"                          the full library name appropriate for each platform."<< std::endl;
    out <<std::endl;
    out <<"    -stereo             - switch on stereo rendering, using the default of,"<< std::endl;
    out <<"                          ANAGLYPHIC or the value set in the OSG_STEREO_MODE "<< std::endl;
    out <<"                          environmental variable. See doc/stereo.html for "<< std::endl;
    out <<"                          further details on setting up accurate stereo "<< std::endl;
    out <<"                          for your system. "<< std::endl;
    out <<"    -stereo ANAGLYPHIC  - switch on anaglyphic(red/cyan) stereo rendering."<< std::endl;
    out <<"    -stereo QUAD_BUFFER - switch on quad buffered stereo rendering."<< std::endl;
    out <<std::endl;
    out <<"    -stencil            - use a visual with stencil buffer enabled, this "<< std::endl;
    out <<"                          also allows the depth complexity statistics mode"<< std::endl;
    out <<"                          to be used (press 'p' three times to cycle to it)."<< std::endl;
    out << std::endl;
}



class OccluderEventHandler : public osgGA::GUIEventHandler
{
    public:
    
        OccluderEventHandler(osgUtil::SceneView* sceneview,osg::Group* rootnode):_sceneview(sceneview),_rootnode(rootnode) {}
    
        virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter&);

        virtual void accept(osgGA::GUIEventHandlerVisitor& v)
        {
            v.visit(*this);
        }
        
        void addPoint(const osg::Vec3& pos);
                
        void endOccluder();
        
        
        osg::ref_ptr<osgUtil::SceneView>        _sceneview;
        osg::ref_ptr<osg::Group>                _rootnode;
        osg::ref_ptr<osg::Group>                _occluders;
        osg::ref_ptr<osg::ConvexPlanarOccluder> _convexPlanarOccluder;
};

bool OccluderEventHandler::handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter&)
{
    switch(ea.getEventType())
    {
        case(osgGA::GUIEventAdapter::KEYBOARD):
        {
            if (ea.getKey()=='a')
            {

                int x = ea.getX();
                int y = ea.getY();
 
                osg::Vec3 near_point,far_point;
                if (!_sceneview->projectWindowXYIntoObject(x,ea.getYmax()-y,near_point,far_point))
                {
                     return true;
                }

                osg::ref_ptr<osg::LineSegment> lineSegment = osgNew osg::LineSegment;
                lineSegment->set(near_point,far_point);

                osgUtil::IntersectVisitor iv;
                iv.addLineSegment(lineSegment.get());

                _rootnode->accept(iv);

                if (iv.hits())
                {
                
                    osgUtil::IntersectVisitor::HitList& hitList = iv.getHitList(lineSegment.get());
                    if (!hitList.empty())
                    {
                    
                        osgUtil::Hit& hit = hitList.front();
                        addPoint(hit.getWorldIntersectPoint());
                    }

                }

                return true;
            }
            else if (ea.getKey()=='e')
            {
                endOccluder();
                return true;
            }
            else if (ea.getKey()=='O')
            {
                if (_occluders.valid())
                {
                    std::cout<<"saving occluders to 'saved_occluders.osg'"<<std::endl;
                    osgDB::writeNodeFile(*_occluders,"saved_occluders.osg");
                }
                else
                {
                    std::cout<<"no occluders to save"<<std::endl;
                }
                return true;
            }
            return false;
        }

        default:
            return false;
    }
}

void OccluderEventHandler::addPoint(const osg::Vec3& pos)
{
    std::cout<<"add point "<<pos<<std::endl;
    
    if (!_convexPlanarOccluder.valid()) _convexPlanarOccluder = new osg::ConvexPlanarOccluder;
    
    osg::ConvexPlanarPolygon& occluder = _convexPlanarOccluder->getOccluder();
    occluder.add(pos);
    
}
                
void OccluderEventHandler::endOccluder()
{
    if (_convexPlanarOccluder.valid()) 
    {
        if (_convexPlanarOccluder->getOccluder().getVertexList().size()>=3)
        {
            osg::OccluderNode* occluderNode = osgNew osg::OccluderNode;
            occluderNode->setOccluder(_convexPlanarOccluder.get());

            if (!_occluders.valid())
            {
                _occluders = new osg::Group;
                _rootnode->addChild(_occluders.get());
            }
            _occluders->addChild(occluderNode);

            std::cout<<"created occluder"<<std::endl;
        }
        else
        {
            std::cout<<"Occluder requires at least 3 points to create occluder."<<std::endl;
        }
    }
    else
    {
        std::cout<<"No occluder points to create occluder with."<<std::endl;
    }
    
    // reset current occluder.
    _convexPlanarOccluder = NULL;
}


osg::Node* createOccluder(const osg::Vec3& v1,const osg::Vec3& v2,const osg::Vec3& v3,const osg::Vec3& v4,float holeRatio=-1.0f)
{
   // create and occluder which will site along side the loadmodel model.
    osg::OccluderNode* occluderNode = osgNew osg::OccluderNode;

    // create the convex planer occluder 
    osg::ConvexPlanarOccluder* cpo = osgNew osg::ConvexPlanarOccluder;

    // attach it to the occluder node.
    occluderNode->setOccluder(cpo);
    occluderNode->setName("occluder");
    
    // set the occluder up for the front face of the bounding box.
    osg::ConvexPlanarPolygon& occluder = cpo->getOccluder();
    occluder.add(v1);
    occluder.add(v2);
    occluder.add(v3);
    occluder.add(v4);

    // create a whole at the center of the occluder if needed.
    if (holeRatio>0.0f)
    {
        // create hole.
        float ratio = holeRatio;
        float one_minus_ratio = 1-ratio;
        osg::Vec3 center = (v1+v2+v3+v4)*0.25f;
        osg::Vec3 v1dash = v1*ratio + center*one_minus_ratio;
        osg::Vec3 v2dash = v2*ratio + center*one_minus_ratio;
        osg::Vec3 v3dash = v3*ratio + center*one_minus_ratio;
        osg::Vec3 v4dash = v4*ratio + center*one_minus_ratio;

        osg::ConvexPlanarPolygon hole;
        hole.add(v1dash);
        hole.add(v2dash);
        hole.add(v3dash);
        hole.add(v4dash);

        cpo->addHole(hole);
    }    
    

   // create a drawable for occluder.
    osg::Geometry* geom = osgNew osg::Geometry;
    
    osg::Vec3Array* coords = osgNew osg::Vec3Array(occluder.getVertexList().begin(),occluder.getVertexList().end());
    geom->setVertexArray(coords);
    
    osg::Vec4Array* colors = osgNew osg::Vec4Array(1);
    (*colors)[0].set(1.0f,1.0f,1.0f,0.5f);
    geom->setColorArray(colors);
    geom->setColorBinding(osg::Geometry::BIND_OVERALL);
    
    geom->addPrimitiveSet(osgNew osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4));
    
    osg::Geode* geode = osgNew osg::Geode;
    geode->addDrawable(geom);
    
    osg::StateSet* stateset = osgNew osg::StateSet;
    stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
    stateset->setMode(GL_BLEND,osg::StateAttribute::ON);
    stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    
    geom->setStateSet(stateset);
    
    // add the occluder geode as a child of the occluder,
    // as the occluder can't self occlude its subgraph the
    // geode will never be occluder by this occluder.
    occluderNode->addChild(geode);    
    
    return occluderNode;
 
 }

osg::Group* createOccludersAroundModel(osg::Node* model)
{
    osg::Group* scene = osgNew osg::Group;
    scene->setName("rootgroup");


    // add the loaded model into a the scene group.
    scene->addChild(model);
    model->setName("model");

    // get the bounding volume of the model.
    const osg::BoundingSphere bs = model->getBound();
    
    // create a bounding box around the sphere.
    osg::BoundingBox bb;
    bb.expandBy(bs);

   // front
   scene->addChild(createOccluder(bb.corner(0),
                                  bb.corner(1),
                                  bb.corner(5),
                                  bb.corner(4)));

   // right side
   scene->addChild(createOccluder(bb.corner(1),
                                  bb.corner(3),
                                  bb.corner(7),
                                  bb.corner(5)));

   // left side
   scene->addChild(createOccluder(bb.corner(2),
                                  bb.corner(0),
                                  bb.corner(4),
                                  bb.corner(6)));

   // back side
   scene->addChild(createOccluder(bb.corner(3),
                                  bb.corner(2),
                                  bb.corner(6),
                                  bb.corner(7),
                                  0.5f)); // create a hole half the size of the occluder.

    return scene;
} 


int main( int argc, char **argv )
{

    // initialize the GLUT
    glutInit( &argc, argv );

    if (argc<2)
    {
        write_usage(std::cout,argv[0]);
        return 0;
    }
    
    // create the commandline args.
    std::vector<std::string> commandLine;
    for(int i=1;i<argc;++i) commandLine.push_back(argv[i]);
    
    bool manuallyCreateImpostors = false;

    std::vector<std::string>::iterator itr=std::find(commandLine.begin(),commandLine.end(),std::string("-c"));
    if (itr!=commandLine.end())    
    {
        manuallyCreateImpostors = true;
        commandLine.erase(itr);
    }

    // initialize the viewer.
    osgGLUT::Viewer viewer;
    viewer.setWindowTitle(argv[0]);

    // configure the viewer from the commandline arguments, and eat any
    // parameters that have been matched.
    viewer.readCommandLine(commandLine);
    
    // configure the plugin registry from the commandline arguments, and 
    // eat any parameters that have been matched.
    osgDB::readCommandLine(commandLine);

    // load the nodes from the commandline arguments.
    osg::Node* loadedmodel = osgDB::readNodeFiles(commandLine);
    if (!loadedmodel)
    {
//        write_usage(osg::notify(osg::NOTICE),argv[0]);
        return 1;
    }
    
    // run optimization over the scene graph
    osgUtil::Optimizer optimzer;
    optimzer.optimize(loadedmodel);

    // add the occluders to the loaded model.
    osg::Group* rootnode = NULL;
    
    if (manuallyCreateImpostors)
    {
        rootnode = new osg::Group;
        rootnode->addChild(loadedmodel);
    }
    else    
    {
        rootnode = createOccludersAroundModel(loadedmodel);
    }
    
     
    // add a viewport to the viewer and attach the scene graph.
    viewer.addViewport( rootnode );
    
    // register trackball, flight and drive.
    viewer.registerCameraManipulator(new osgGA::TrackballManipulator);
    viewer.registerCameraManipulator(new osgGA::FlightManipulator);
    viewer.registerCameraManipulator(new osgGA::DriveManipulator);

    if (manuallyCreateImpostors)
    {
        viewer.addEventHandler(new OccluderEventHandler(viewer.getViewportSceneView(0),rootnode));
    }

    // open the viewer window.
    viewer.open();
    
    // fire up the event loop.
    viewer.run();

    return 0;
}
