#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/Texture2D>
#include <osg/Geometry>
#include <osg/MatrixTransform>
#include <osg/BlendFunc>

#include <osgUtil/Tesselator>
#include <osgUtil/TransformCallback>

#include <osgText/Text>

#include <osgGA/TrackballManipulator>

#include <osgGLUT/Viewer>
#include <osgGLUT/glut>

#include <osgDB/ReadFile>

//#include "CreateShadowedScene.h"

osg::Geometry* createWing(const osg::Vec3& left, const osg::Vec3& nose, const osg::Vec3& right,float chordRatio,const osg::Vec4& color)
{
    osg::Geometry* geom = new osg::Geometry;

    osg::Vec3 normal = (nose-right)^(left-nose);
    normal.normalize();

    osg::Vec3 left_to_right = right-left;
    osg::Vec3 mid = (right+left)*0.5f;
    osg::Vec3 mid_to_nose = (nose-mid)*chordRatio*0.5f;
    
    osg::Vec3Array* vertices = new osg::Vec3Array;
    vertices->push_back(left);
    //vertices->push_back(mid+mid_to_nose);
    
    unsigned int noSteps = 40;
    for(unsigned int i=1;i<noSteps;++i)
    {
        float ratio = (float)i/(float)noSteps;
        vertices->push_back(left + left_to_right*ratio + mid_to_nose* (cosf((ratio-0.5f)*osg::PI*2.0f)+1.0f));
    }
    
    vertices->push_back(right);
    vertices->push_back(nose);

    geom->setVertexArray(vertices);


    osg::Vec3Array* normals = new osg::Vec3Array;
    normals->push_back(normal);
    geom->setNormalArray(normals);
    geom->setNormalBinding(osg::Geometry::BIND_OVERALL);
    
    
    osg::Vec4Array* colors = new osg::Vec4Array;
    colors->push_back(color);
    geom->setColorArray(colors);
    geom->setColorBinding(osg::Geometry::BIND_OVERALL);
    

    geom->addPrimitiveSet(new osg::DrawArrays(GL_POLYGON,0,vertices->getNumElements()));
    
    osgUtil::Tesselator tesselator;
    tesselator.retesselatePolygons(*geom);

    return geom;
    
}

osg:: Node* createTextBelow(const osg::BoundingBox& bb)
{
    osg::Geode* geode = osgNew osg::Geode();

    osgText::PolygonFont*   polygonFont= osgNew  osgText::PolygonFont("fonts/times.ttf",20, 3);
    osgText::Text*          text = osgNew  osgText::Text(polygonFont);
 
    text->setText("OpenSceneGraph");
    text->setAlignment(osgText::Text::CENTER_CENTER);
    text->setAxisAlignment(osgText::Text::XZ_PLANE);
    text->setPosition(bb.center()-osg::Vec3(0.0f,0.0f,(bb.zMax()-bb.zMin())));
    text->setColor(osg::Vec4(0.37f,0.48f,0.67f,1.0f));
    osg::StateSet* stateset = text->getOrCreateStateSet();
    stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
 
    geode->addDrawable( text );

    return geode;
}

osg:: Node* createTextLeft(const osg::BoundingBox& bb)
{
    osg::Geode* geode = osgNew osg::Geode();

    //std::string font("fonts/times.ttf");
    std::string font("fonts/arial.ttf");

    //osgText::Text* text = osgNew  osgText::Text(osgNew osgText::PolygonFont(font,80, 3));
    osgText::Text* text = osgNew  osgText::Text(osgNew osgText::TextureFont(font,80));
 
    text->setText("OpenSceneGraph");
    text->setAlignment(osgText::Text::RIGHT_CENTER);
    text->setAxisAlignment(osgText::Text::XZ_PLANE);
    text->setPosition(bb.center()-osg::Vec3((bb.xMax()-bb.xMin()),-(bb.yMax()-bb.yMin())*0.5f,(bb.zMax()-bb.zMin())*0.2f));
    //text->setColor(osg::Vec4(0.37f,0.48f,0.67f,1.0f)); // Neil's orignal OSG colour
    text->setColor(osg::Vec4(0.20f,0.45f,0.60f,1.0f)); // OGL logo colour


    osg::StateSet* stateset = text->getOrCreateStateSet();


    osg::BlendFunc *transp= osgNew osg::BlendFunc();
    transp->setFunction(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
    stateset->setAttributeAndModes(transp,osg::StateAttribute::ON);
    stateset->setTextureMode(0,GL_TEXTURE_2D,osg::StateAttribute::ON);
    stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);


    geode->addDrawable( text );
    return geode;
}

osg:: Node* createGlobe(const osg::BoundingBox& bb,float ratio)
{
    osg::Geode* geode = osgNew osg::Geode();

    osg::StateSet* stateset = osgNew osg::StateSet();

    osg::Image* image = osgDB::readImageFile("land_shallow_topo_2048.jpg");
    if (image)
    {
	osg::Texture2D* texture = osgNew osg::Texture2D;
	texture->setImage(image);
	texture->setMaxAnisotropy(8);
	stateset->setTextureAttributeAndModes(0,texture,osg::StateAttribute::ON);
    }
    
    geode->setStateSet( stateset );
    
    // the globe
    geode->addDrawable(new osg::ShapeDrawable(osgNew osg::Sphere(bb.center(),bb.radius()*ratio)));
    
    
    osg::MatrixTransform* xform = new osg::MatrixTransform;
    xform->setAppCallback(new osgUtil::TransformCallback(bb.center(),osg::Vec3(0.0f,0.0f,1.0f),osg::inDegrees(30.0f)));
    xform->addChild(geode);
        
    return xform;
}

osg:: Node* createBox(const osg::BoundingBox& bb,float chordRatio)
{
    osg::Geode* geode = osgNew osg::Geode();

    osg::Vec4 white(1.0f,1.0f,1.0f,1.0f);

    // front faces.
    geode->addDrawable(createWing(bb.corner(4),bb.corner(6),bb.corner(7),chordRatio,white));
    geode->addDrawable(createWing(bb.corner(7),bb.corner(5),bb.corner(4),chordRatio,white));

    geode->addDrawable(createWing(bb.corner(4),bb.corner(5),bb.corner(1),chordRatio,white));
    geode->addDrawable(createWing(bb.corner(1),bb.corner(0),bb.corner(4),chordRatio,white));
    
    geode->addDrawable(createWing(bb.corner(1),bb.corner(5),bb.corner(7),chordRatio,white));
    geode->addDrawable(createWing(bb.corner(7),bb.corner(3),bb.corner(1),chordRatio,white));

    // back faces
    geode->addDrawable(createWing(bb.corner(2),bb.corner(0),bb.corner(1),chordRatio,white));
    geode->addDrawable(createWing(bb.corner(1),bb.corner(3),bb.corner(2),chordRatio,white));

    geode->addDrawable(createWing(bb.corner(2),bb.corner(3),bb.corner(7),chordRatio,white));
    geode->addDrawable(createWing(bb.corner(7),bb.corner(6),bb.corner(2),chordRatio,white));

    geode->addDrawable(createWing(bb.corner(2),bb.corner(6),bb.corner(4),chordRatio,white));
    geode->addDrawable(createWing(bb.corner(4),bb.corner(0),bb.corner(2),chordRatio,white));

    return geode;
}

osg:: Node* createBoxNo5(const osg::BoundingBox& bb,float chordRatio)
{
    osg::Geode* geode = osgNew osg::Geode();

    osg::Vec4 white(1.0f,1.0f,1.0f,1.0f);

    // front faces.
    geode->addDrawable(createWing(bb.corner(4),bb.corner(6),bb.corner(7),chordRatio,white));

    geode->addDrawable(createWing(bb.corner(1),bb.corner(0),bb.corner(4),chordRatio,white));
    
    geode->addDrawable(createWing(bb.corner(7),bb.corner(3),bb.corner(1),chordRatio,white));

    // back faces
    geode->addDrawable(createWing(bb.corner(2),bb.corner(0),bb.corner(1),chordRatio,white));
    geode->addDrawable(createWing(bb.corner(1),bb.corner(3),bb.corner(2),chordRatio,white));

    geode->addDrawable(createWing(bb.corner(2),bb.corner(3),bb.corner(7),chordRatio,white));
    geode->addDrawable(createWing(bb.corner(7),bb.corner(6),bb.corner(2),chordRatio,white));

    geode->addDrawable(createWing(bb.corner(2),bb.corner(6),bb.corner(4),chordRatio,white));
    geode->addDrawable(createWing(bb.corner(4),bb.corner(0),bb.corner(2),chordRatio,white));

    return geode;
}

osg:: Node* createBoxNo5No2(const osg::BoundingBox& bb,float chordRatio)
{
    osg::Geode* geode = osgNew osg::Geode();

    osg::Vec4 red(1.0f,0.0f,0.0f,1.0f);
    osg::Vec4 green(0.0f,1.0f,0.0f,1.0f);
    osg::Vec4 blue(0.0f,0.0f,1.0f,1.0f);

    // front faces.
    geode->addDrawable(createWing(bb.corner(4),bb.corner(6),bb.corner(7),chordRatio,red));

    geode->addDrawable(createWing(bb.corner(1),bb.corner(0),bb.corner(4),chordRatio,green));
    
    geode->addDrawable(createWing(bb.corner(7),bb.corner(3),bb.corner(1),chordRatio,blue));

    return geode;
}

osg:: Node* createBackdrop(const osg::Vec3& corner,const osg::Vec3& top,const osg::Vec3& right)
{



    osg::Geometry* geom = new osg::Geometry;

    osg::Vec3 normal = (corner-top)^(right-corner);
    normal.normalize();

    osg::Vec3Array* vertices = new osg::Vec3Array;
    vertices->push_back(top);
    vertices->push_back(corner);
    vertices->push_back(right);
    vertices->push_back(right+(top-corner));

    geom->setVertexArray(vertices);

    osg::Vec3Array* normals = new osg::Vec3Array;
    normals->push_back(normal);
    geom->setNormalArray(normals);
    geom->setNormalBinding(osg::Geometry::BIND_OVERALL);

    geom->addPrimitiveSet(new osg::DrawArrays(GL_QUADS,0,vertices->getNumElements()));

    osg::Geode* geode = osgNew osg::Geode();
    geode->addDrawable(geom);
    
    return geode;    
}

osg::Node* createLogo()
{
    osg::BoundingBox bb(osg::Vec3(0.0f,0.0f,0.0f),osg::Vec3(100.0f,100.0f,100.0f));
    float chordRatio = 0.5f; 
    float sphereRatio = 0.6f; 

    // create a group to hold the whole model.
    osg::Group* logo_group = new osg::Group;

    // create a transform to orientate the box and globe.
    osg::MatrixTransform* xform = new osg::MatrixTransform;
    xform->setDataVariance(osg::Object::STATIC);
    xform->setMatrix(osg::Matrix::translate(-bb.center())*
                     osg::Matrix::rotate(-osg::inDegrees(45.0f),0.0f,0.0f,1.0f)*
                     osg::Matrix::rotate(osg::inDegrees(45.0f),1.0f,0.0f,0.0f)*
                     osg::Matrix::translate(bb.center()));

    // add the box and globe to it.
    //xform->addChild(createBox(bb,chordRatio));
    //xform->addChild(createBoxNo5(bb,chordRatio));
    xform->addChild(createBoxNo5No2(bb,chordRatio));
    // add the transform to the group.
    logo_group->addChild(xform);

    logo_group->addChild(createGlobe(bb,sphereRatio));

    // add the text to the group.
    //group->addChild(createTextBelow(bb));
    logo_group->addChild(createTextLeft(bb));
    
    
    // create the backdrop to render the shadow to.
    osg::Vec3 corner(-800.0f,150.0f,-100.0f);
    osg::Vec3 top(0.0f,0.0f,300.0f); top += corner;
    osg::Vec3 right(1000.0f,0.0f,0.0f); right += corner;
    
    
    osg::Group* backdrop = new osg::Group;
    backdrop->addChild(createBackdrop(corner,top,right));

    //osg::Vec3 lightPosition(-500.0f,-2500.0f,500.0f);
    //osg::Node* scene = createShadowedScene(logo_group,backdrop,lightPosition,0.0f,0);

    osg::Group* scene = new osg::Group;
    scene->addChild(logo_group);
    scene->addChild(backdrop);

    return scene;
}

int main( int argc, char **argv )
{

    glutInit( &argc, argv );

    // create the commandline args.
    std::vector<std::string> commandLine;
    for(int i=1;i<argc;++i) commandLine.push_back(argv[i]);

    // create the viewer and the model to it.
    osgGLUT::Viewer viewer;

    viewer.setWindowTitle(argv[0]);

 
    // configure the viewer from the commandline arguments, and eat any
    // parameters that have been matched.
    viewer.readCommandLine(commandLine);
    
    osg::Node* node = createLogo();

    // add model to viewer.
    viewer.addViewport( node );

    // register trackball maniupulators.
    viewer.registerCameraManipulator(osgNew osgGA::TrackballManipulator);
    
    viewer.open();

    viewer.run();

    return 0;
}
