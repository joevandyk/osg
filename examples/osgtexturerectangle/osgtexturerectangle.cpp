// -*-c++-*-

/*
 * demonstrates usage of osg::TextureRectangle.
 *
 * Actually there isn't much difference to the rest of the osg::Texture*
 * bunch only this:
 * - texture coordinates for texture rectangles must be in image
 *   coordinates instead of normalized coordinates (0-1). So for a 500x250
 *   image the coordinates for the entire image would be
 *   0,250 0,0 500,0 500,250 instead of 0,1 0,0 1,0 1,1
 * - only the following wrap modes are supported (but not enforced)
 *   CLAMP, CLAMP_TO_EDGE, CLAMP_TO_BORDER
 * - a border is not supported
 * - mipmap is not supported
 */

#include <osg/Notify>
#include <osg/TextureRectangle>
#include <osg/Geometry>
#include <osg/Geode>

#include <osg/Projection>
#include <osg/MatrixTransform>

#include <osgText/Text>

#include <osgDB/Registry>
#include <osgDB/ReadFile>

#include <osgProducer/Viewer>


/**********************************************************************
 *
 * Texture pan animation callback
 *
 **********************************************************************/

class TexturePanCallback : public osg::NodeCallback
{
public:
    TexturePanCallback(osg::Geometry* geom, osg::Image* img,
                       double delay = 0.05) :
        _geom(geom),
        _img(img),
        _phaseS(35.0f),
        _phaseT(18.0f),
        _phaseScale(5.0f),
        _delay(delay),
        _prevTime(0.0)
    {
    }

    virtual void operator()(osg::Node*, osg::NodeVisitor* nv)
    {
        if (!_geom || !_img)
            return;

        if (nv->getFrameStamp()) {
            double currTime = nv->getFrameStamp()->getReferenceTime();
            if (currTime - _prevTime > _delay) {

                float rad = osg::DegreesToRadians(currTime);

                // zoom scale (0.2 - 1.0)
                float scale = sin(rad * _phaseScale) * 0.4f + 0.6f;
                float scaleR = 1.0f - scale;

                // calculate new texture coordinates
                float s, t;
                s = ((sin(rad * _phaseS) + 1) * 0.5f) * (_img->s() * scaleR);
                t = ((sin(rad * _phaseT) + 1) * 0.5f) * (_img->t() * scaleR);

                // set new texture coordinate array
                osg::Vec2Array* texcoords = (osg::Vec2Array*) _geom->getTexCoordArray(0);
                float w = _img->s() * scale, h = _img->t() * scale;
                (*texcoords)[0].set(s, t+h);
                (*texcoords)[1].set(s, t);
                (*texcoords)[2].set(s+w, t);
                (*texcoords)[3].set(s+w, t+h);

                // record time
                _prevTime = currTime;
            }
        }
    }

private:
    osg::Geometry* _geom;
    osg::Image* _img;

    float _phaseS, _phaseT, _phaseScale;

    double _delay;
    double _prevTime;
};


osg::Node* createRectangle(osg::BoundingBox& bb,
                           const std::string& filename)
{
    osg::Vec3 top_left(bb.xMin(),bb.yMax(),bb.zMax());
    osg::Vec3 bottom_left(bb.xMin(),bb.yMax(),bb.zMin());
    osg::Vec3 bottom_right(bb.xMax(),bb.yMax(),bb.zMin());
    osg::Vec3 top_right(bb.xMax(),bb.yMax(),bb.zMax());
    
    // create geometry
    osg::Geometry* geom = new osg::Geometry;

    osg::Vec3Array* vertices = new osg::Vec3Array(4);
    (*vertices)[0] = top_left;
    (*vertices)[1] = bottom_left;
    (*vertices)[2] = bottom_right;
    (*vertices)[3] = top_right;
    geom->setVertexArray(vertices);
    
    osg::Vec2Array* texcoords = new osg::Vec2Array(4);
    (*texcoords)[0].set(0.0f, 0.0f);
    (*texcoords)[1].set(1.0f, 0.0f);
    (*texcoords)[2].set(1.0f, 1.0f);
    (*texcoords)[3].set(0.0f, 1.0f);
    geom->setTexCoordArray(0,texcoords);

    osg::Vec3Array* normals = new osg::Vec3Array(1);
    (*normals)[0].set(-1.0f,0.0f,0.0f);
    geom->setNormalArray(normals);
    geom->setNormalBinding(osg::Geometry::BIND_OVERALL);

    osg::Vec4Array* colors = new osg::Vec4Array(1);
    (*colors)[0].set(1.0f,1.0f,1.0f,1.0f);
    geom->setColorArray(colors);
    geom->setColorBinding(osg::Geometry::BIND_OVERALL);

    geom->addPrimitiveSet(new osg::DrawArrays(GL_QUADS, 0, 4));

    // disable display list so our modified tex coordinates show up
    geom->setUseDisplayList(false);

    osg::Geode* geode = new osg::Geode;
    geode->addDrawable(geom);

    // setup texture
    osg::TextureRectangle* texture = new osg::TextureRectangle;
    texture->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP);
    texture->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP);

    // load image
    osg::Image* img = osgDB::readImageFile(filename);
    texture->setImage(img);
    
    osg::StateSet* state = geom->getOrCreateStateSet();
    state->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);

    // turn off lighting 
    state->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

    // install 'update' callback
    geode->setUpdateCallback(new TexturePanCallback(geom, img));
    
    return geode;
}


osg::Geode* createText(const std::string& str,
                       const osg::Vec3& pos)
{
    static std::string font("fonts/arial.ttf");

    osg::Geode* geode = new osg::Geode;

    osgText::Text* text = new osgText::Text;
    geode->addDrawable(text);

    text->setFont(font);
    text->setPosition(pos);
    text->setText(str);

    return geode;
}


osg::Node* createHUD()
{
    osg::Group* group = new osg::Group;

    // turn off lighting and depth test
    osg::StateSet* state = group->getOrCreateStateSet();
    state->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    state->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);

    // add text
    osg::Vec3 pos(150.0f, 800.0f, 0.0f);
    const osg::Vec3 delta(0.0f, -80.0f, 0.0f);

    const char* text[] = {
        "TextureRectangle Mini-HOWTO",
        "- essentially behaves like Texture2D, *except* that:",
        "- tex coords must be non-normalized (0..width) instead of (0..1)",
        "- wrap mode must be CLAMP, CLAMP_TO_EDGE, or CLAMP_TO_BORDER",
        "- repeating wrap modes are *not* supported",
        "- texture border is *not* supported",
        "- texture mipmaps are *not* supported",
        NULL
    };
    const char** t = text;
    while (*t) {
        group->addChild(createText(*t++, pos));
        pos += delta;
    }

    // create HUD
    osg::MatrixTransform* modelview_abs = new osg::MatrixTransform;
    modelview_abs->setReferenceFrame(osg::Transform::RELATIVE_TO_ABSOLUTE);
    modelview_abs->setMatrix(osg::Matrix::identity());
    modelview_abs->addChild(group);

    osg::Projection* projection = new osg::Projection;
    projection->setMatrix(osg::Matrix::ortho2D(0,1280,0,1024));
    projection->addChild(modelview_abs);

    return projection;
}


osg::Node* createModel(const std::string& filename)
{
    osg::Group* root = new osg::Group;
    
    osg::BoundingBox bb(0.0f,0.0f,0.0f,1.0f,1.0f,1.0f);
    root->addChild(createRectangle(bb, filename)); // XXX

    root->addChild(createHUD());

    return root;
}


int main(int argc, char** argv)
{
    // use an ArgumentParser object to manage the program arguments.
    osg::ArgumentParser arguments(&argc,argv);

    // set up the usage document, in case we need to print out how to use this program.
    arguments.getApplicationUsage()->setDescription(arguments.getApplicationName()+" is a demo which demonstrates use of osg::TextureRectangle.");
    arguments.getApplicationUsage()->setCommandLineUsage(arguments.getApplicationName()+" [options] <image> ...");
    arguments.getApplicationUsage()->addCommandLineOption("-h or --help","Display this information");
   
    // construct the viewer.
    osgProducer::Viewer viewer(arguments);

    // set up the value with sensible default event handlers.
    viewer.setUpViewer(osgProducer::Viewer::STANDARD_SETTINGS);

    // get details on keyboard and mouse bindings used by the viewer.
    viewer.getUsage(*arguments.getApplicationUsage());

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

    // create a model from the images.
    osg::Node* rootNode = createModel((arguments.argc() > 1 ? arguments[1] : "lz.rgb"));

    // add model to viewer.
    viewer.setSceneData(rootNode);

    // create the windows and run the threads.
    viewer.realize();

    while (!viewer.done())
    {
        // wait for all cull and draw threads to complete.
        viewer.sync();

        // update the scene by traversing it with the the update visitor which will
        // call all node update callbacks and animations.
        viewer.update();

        // fire off the cull and draw traversals of the scene.
        viewer.frame();
    }

    // wait for all cull and draw threads to complete before exit.
    viewer.sync();
    
    return 0;
}
