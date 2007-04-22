#ifdef WIN32
/////////////////////////////////////////////////////////////////////////////
// Disable unavoidable warning messages:

//  4103: used #pragma pack to change alignment
//  4114: same type qualifier used more than once
//  4201: nonstandard extension used : nameless struct/union
//  4237: "keyword" reserved for future use
//  4251: class needs to have dll-interface to export class
//  4275: non DLL-interface class used as base for DLL-interface class
//  4290: C++ Exception Specification ignored
//  4503: decorated name length exceeded, name was truncated
//  4786: string too long - truncated to 255 characters

#pragma warning(disable : 4103 4114 4201 4237 4251 4275 4290 4503 4335 4786)

#endif // WIN32

#include <osgViewer/Viewer>

#include <osg/Group>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/Texture2D>
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>
#include <osg/CoordinateSystemNode>

#include <osgDB/FileUtils>
#include <osgDB/ReadFile>

#include <osgText/Text>

#include <osg/CoordinateSystemNode>

#include <osgSim/OverlayNode>
#include <osgSim/SphereSegment>

#include <osgGA/NodeTrackerManipulator>

#include <iostream>

osg::Node* createEarth()
{
    osg::TessellationHints* hints = new osg::TessellationHints;
    hints->setDetailRatio(5.0f);

    
    osg::ShapeDrawable* sd = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0.0,0.0,0.0), osg::WGS_84_RADIUS_POLAR), hints);
    
    osg::Geode* geode = new osg::Geode;
    geode->addDrawable(sd);
    
    std::string filename = osgDB::findDataFile("Images/land_shallow_topo_2048.jpg");
    geode->getOrCreateStateSet()->setTextureAttributeAndModes(0, new osg::Texture2D(osgDB::readImageFile(filename)));
    
    osg::CoordinateSystemNode* csn = new osg::CoordinateSystemNode;
    csn->setEllipsoidModel(new osg::EllipsoidModel());
    csn->addChild(geode);
    
    return csn;
    
}


class ModelPositionCallback : public osg::NodeCallback
{
public:

    ModelPositionCallback():
        _latitude(0.0),
        _longitude(0.0),
        _height(100000.0)
    {
        _rotation.makeRotate(osg::DegreesToRadians(90.0),0.0,0.0,1.0);
    }
    
    void updateParameters()
    {
        _longitude += ((2.0*osg::PI)/360.0)/20.0;
    }


    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
    {
        updateParameters();
        
        osg::NodePath nodePath = nv->getNodePath();

        osg::MatrixTransform* mt = nodePath.empty() ? 0 : dynamic_cast<osg::MatrixTransform*>(nodePath.back());
        if (mt)
        {
            osg::CoordinateSystemNode* csn = 0;

            // find coordinate system node from our parental chain
            unsigned int i;
            for(i=0; i<nodePath.size() && csn==0; ++i)
            {
                csn = dynamic_cast<osg::CoordinateSystemNode*>(nodePath[i]);
            }
            
            if (csn)
            {


                osg::EllipsoidModel* ellipsoid = csn->getEllipsoidModel();
                if (ellipsoid)
                {
                    osg::Matrix inheritedMatrix;
                    for(i+=1; i<nodePath.size()-1; ++i)
                    {
                        osg::Transform* transform = nodePath[i]->asTransform();
                        if (transform) transform->computeLocalToWorldMatrix(inheritedMatrix, nv);
                    }
                    
                    osg::Matrixd matrix(inheritedMatrix);

                    //osg::Matrixd matrix;
                    ellipsoid->computeLocalToWorldTransformFromLatLongHeight(_latitude,_longitude,_height,matrix);
                    matrix.preMult(osg::Matrix::rotate(_rotation));
                    
                    mt->setMatrix(matrix);
                }

            }        
        }
            
        traverse(node,nv);
    }   
    
    double                  _latitude;
    double                  _longitude;
    double                  _height;
    osg::Quat               _rotation;
};


class FindNamedNodeVisitor : public osg::NodeVisitor
{
public:
    FindNamedNodeVisitor(const std::string& name):
        osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN),
        _name(name) {}
    
    virtual void apply(osg::Node& node)
    {
        if (node.getName()==_name)
        {
            _foundNodes.push_back(&node);
        }
        traverse(node);
    }
    
    typedef std::vector< osg::ref_ptr<osg::Node> > NodeList;

    std::string _name;
    NodeList _foundNodes;
};


int main(int argc, char **argv)
{
    // use an ArgumentParser object to manage the program arguments.
    osg::ArgumentParser arguments(&argc,argv);
    
    // set up the usage document, in case we need to print out how to use this program.
    arguments.getApplicationUsage()->setDescription(arguments.getApplicationName()+" is the example which demonstrates use of node tracker.");
    arguments.getApplicationUsage()->setCommandLineUsage(arguments.getApplicationName());
    arguments.getApplicationUsage()->addCommandLineOption("-h or --help","Display this information");
    

    // construct the viewer.
    osgViewer::Viewer viewer;

    viewer.getCamera()->setComputeNearFarMode(osg::CullSettings::COMPUTE_NEAR_FAR_USING_PRIMITIVES);
    viewer.getCamera()->setNearFarRatio(0.00001f);


    osg::Quat rotation;
    osg::Vec4 vec4;
    while (arguments.read("--rotate-model",vec4[0],vec4[1],vec4[2],vec4[3]))
    {
        osg::Quat local_rotate;
        local_rotate.makeRotate(osg::DegreesToRadians(vec4[0]),vec4[1],vec4[2],vec4[3]);
        
        rotation = rotation * local_rotate;
    }

    osg::NodeCallback* nc = 0;
    std::string flightpath_filename;
    while (arguments.read("--flight-path",flightpath_filename))
    {
        std::ifstream fin(flightpath_filename.c_str());
        if (fin)
        {
            osg::AnimationPath* path = new osg::AnimationPath;
            path->read(fin);
            nc = new osg::AnimationPathCallback(path);
        }
    }
    
    osgGA::NodeTrackerManipulator::TrackerMode trackerMode = osgGA::NodeTrackerManipulator::NODE_CENTER_AND_ROTATION;
    std::string mode;
    while (arguments.read("--tracker-mode",mode))
    {
        if (mode=="NODE_CENTER_AND_ROTATION") trackerMode = osgGA::NodeTrackerManipulator::NODE_CENTER_AND_ROTATION;
        else if (mode=="NODE_CENTER_AND_AZIM") trackerMode = osgGA::NodeTrackerManipulator::NODE_CENTER_AND_AZIM;
        else if (mode=="NODE_CENTER") trackerMode = osgGA::NodeTrackerManipulator::NODE_CENTER;
        else
        {
            std::cout<<"Unrecognized --tracker-mode option "<<mode<<", valid options are:"<<std::endl;
            std::cout<<"    NODE_CENTER_AND_ROTATION"<<std::endl;
            std::cout<<"    NODE_CENTER_AND_AZIM"<<std::endl;
            std::cout<<"    NODE_CENTER"<<std::endl;
            return 1;
        }
    }
    
    
    osgGA::NodeTrackerManipulator::RotationMode rotationMode = osgGA::NodeTrackerManipulator::TRACKBALL;
    while (arguments.read("--rotation-mode",mode))
    {
        if (mode=="TRACKBALL") rotationMode = osgGA::NodeTrackerManipulator::TRACKBALL;
        else if (mode=="ELEVATION_AZIM") rotationMode = osgGA::NodeTrackerManipulator::ELEVATION_AZIM;
        else
        {
            std::cout<<"Unrecognized --rotation-mode option "<<mode<<", valid options are:"<<std::endl;
            std::cout<<"    TRACKBALL"<<std::endl;
            std::cout<<"    ELEVATION_AZIM"<<std::endl;
            return 1;
        }
    }

    // if user request help write it out to cout.
    if (arguments.read("-h") || arguments.read("--help"))
    {
        arguments.getApplicationUsage()->write(std::cout);
        return 1;
    }

    // any option left unread are converted into errors to write out later.
    arguments.reportRemainingOptionsAsUnrecognized();

    // report any errors if they have occured when parsing the program aguments.
    if (arguments.errors())
    {
        arguments.writeErrorMessages(std::cout);
        return 1;
    }
    
    // read the scene from the list of file specified commandline args.
    osg::ref_ptr<osg::Node> root = osgDB::readNodeFiles(arguments);

    if (!root) root = createEarth();
    
    if (!root) return 0;

    // add a viewport to the viewer and attach the scene graph.
    viewer.setSceneData(root.get());

    osg::CoordinateSystemNode* csn = dynamic_cast<osg::CoordinateSystemNode*>(root.get());
    if (csn)
    {

        bool insertOverlayNode = true;
        osg::ref_ptr<osgSim::OverlayNode> overlayNode;
        if (insertOverlayNode)
        {
        
            overlayNode = new osgSim::OverlayNode;
            
            // insert the OverlayNode between the coordinate system node and its children.
            for(unsigned int i=0; i<csn->getNumChildren(); ++i)
            {
                overlayNode->addChild( csn->getChild(i) );
            }

            csn->removeChildren(0, csn->getNumChildren());
            csn->addChild(overlayNode.get());
            
            // tell the overlay node to continously update its overlay texture
            // as we know we'll be tracking a moving target.
            overlayNode->setContinuousUpdate(true);
        }
        
        
        osg::Node* cessna = osgDB::readNodeFile("cessna.osg");
        if (cessna)
        {
            double s = 200000.0 / cessna->getBound().radius();
        
            osg::MatrixTransform* scaler = new osg::MatrixTransform;
            scaler->addChild(cessna);
            scaler->setMatrix(osg::Matrixd::scale(s,s,s)*osg::Matrixd::rotate(rotation));
            scaler->getOrCreateStateSet()->setMode(GL_RESCALE_NORMAL,osg::StateAttribute::ON);        
        
            if (false)
            {
                osgSim::SphereSegment* ss = new osgSim::SphereSegment(
                                    osg::Vec3(0.0f,0.0f,0.0f), // center
                                    19.9f, // radius
                                    osg::DegreesToRadians(135.0f),
                                    osg::DegreesToRadians(240.0f),
                                    osg::DegreesToRadians(-10.0f),
                                    osg::DegreesToRadians(30.0f),
                                    60);
                                    
                scaler->addChild(ss);
            }

            osg::MatrixTransform* mt = new osg::MatrixTransform;
            mt->addChild(scaler);


            if (!nc) nc = new ModelPositionCallback;

            mt->setUpdateCallback(nc);

            csn->addChild(mt);
            
            // if we are using an overaly node, use the cessna subgraph as the overlay subgraph
            if (overlayNode.valid())
            {
                overlayNode->setOverlaySubgraph(mt);
            }

            osgGA::NodeTrackerManipulator* tm = new osgGA::NodeTrackerManipulator;
            tm->setTrackerMode(trackerMode);
            tm->setRotationMode(rotationMode);
            tm->setTrackNode(scaler);

            viewer.setCameraManipulator(tm);
        }
        else
        {
             std::cout<<"Failed to read cessna.osg"<<std::endl;
        }
        
    }    

    return viewer.run();
}
