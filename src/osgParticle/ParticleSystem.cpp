#include <osgParticle/ParticleSystem>

#include <vector>

#include <osg/Drawable>
#include <osg/CopyOp>
#include <osg/State>
#include <osg/Matrix>
#include <osg/GL>
#include <osg/StateSet>
#include <osg/Texture2D>
#include <osg/BlendFunc>
#include <osg/TexEnv>
#include <osg/Material>
#include <osg/Notify>

#include <osgDB/ReadFile>

osgParticle::ParticleSystem::ParticleSystem()
:    osg::Drawable(), 
    _def_bbox(osg::Vec3(-10, -10, -10), osg::Vec3(10, 10, 10)),
    _alignment(BILLBOARD),
    _align_X_axis(1, 0, 0),
    _align_Y_axis(0, 1, 0),
    _doublepass(false),
    _frozen(false),
    _bmin(0, 0, 0), 
    _bmax(0, 0, 0), 
    _reset_bounds_flag(false),
    _bounds_computed(false),
    _def_ptemp(Particle()),
    _last_frame(0),
    _freeze_on_cull(false),
    _detail(1),
    _draw_count(0)
{
    // we don't support display lists because particle systems
    // are dynamic, and they always changes between frames
    setSupportsDisplayList(false);
}

osgParticle::ParticleSystem::ParticleSystem(const ParticleSystem& copy, const osg::CopyOp& copyop)
:    osg::Drawable(copy, copyop), 
    _def_bbox(copy._def_bbox),
    _alignment(copy._alignment),
    _align_X_axis(copy._align_X_axis),
    _align_Y_axis(copy._align_Y_axis),
    _doublepass(copy._doublepass),
    _frozen(copy._frozen),
    _bmin(copy._bmin), 
    _bmax(copy._bmax), 
    _reset_bounds_flag(copy._reset_bounds_flag),
    _bounds_computed(copy._bounds_computed),
    _def_ptemp(copy._def_ptemp),
    _last_frame(copy._last_frame),
    _freeze_on_cull(copy._freeze_on_cull),
    _detail(copy._detail),
    _draw_count(0)
{
}

osgParticle::ParticleSystem::~ParticleSystem()
{
}

void osgParticle::ParticleSystem::update(double dt)
{
    // reset bounds
    _reset_bounds_flag = true;

    // set up iterators for particles
    Particle_vector::iterator i;
    Particle_vector::iterator end = _particles.end();

    // update particles
    for (i=_particles.begin(); i!=end; ++i) {
        if (i->isAlive()) {
            if (i->update(dt)) {
                update_bounds(i->getPosition(), i->getCurrentSize());
            } else {
                _deadparts.push(&(*i));
            }
        }
    }

    // force recomputing of bounding box on next frame
    dirtyBound();
}

void osgParticle::ParticleSystem::drawImplementation(osg::State& state) const
{
    // update the frame count, so other objects can detect when
    // this particle system is culled
    _last_frame = state.getFrameStamp()->getFrameNumber();

    // get the current modelview matrix
    osg::Matrix modelview = state.getModelViewMatrix();

    if (_alignment == BILLBOARD)
        state.applyModelViewMatrix(0);

    // set up depth mask for first rendering pass
    glPushAttrib(GL_DEPTH_BUFFER_BIT); 
    glDepthMask(GL_FALSE);

    // render, first pass
    single_pass_render(state, modelview);

    // restore depth mask settings
    glPopAttrib();

    // render, second pass
    if (_doublepass) {    
        // set up color mask for second rendering pass
        glPushAttrib(GL_COLOR_BUFFER_BIT);
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

        // render the particles onto the depth buffer
        single_pass_render(state, modelview);

        // restore color mask settings
        glPopAttrib();
    }
}

void osgParticle::ParticleSystem::setDefaultAttributes(const std::string& texturefile, bool emissive_particles, bool lighting, int texture_unit)
{
    osg::StateSet *stateset = new osg::StateSet;

    stateset->setMode(GL_LIGHTING, lighting? osg::StateAttribute::ON: osg::StateAttribute::OFF);
    stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

    osg::Material *material = new osg::Material;
    material->setSpecular(osg::Material::FRONT, osg::Vec4(0, 0, 0, 1));
    material->setEmission(osg::Material::FRONT, osg::Vec4(0, 0, 0, 1));
    material->setColorMode(lighting? osg::Material::AMBIENT_AND_DIFFUSE : osg::Material::OFF);
    stateset->setAttributeAndModes(material, osg::StateAttribute::ON);

    if (!texturefile.empty()) {
        osg::Texture2D *texture = new osg::Texture2D;
        texture->setImage(osgDB::readImageFile(texturefile));
        texture->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
        texture->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);
        stateset->setTextureAttributeAndModes(texture_unit, texture, osg::StateAttribute::ON);

        osg::TexEnv *texenv = new osg::TexEnv;
        texenv->setMode(osg::TexEnv::MODULATE);
        stateset->setTextureAttribute(texture_unit, texenv);
    }

    osg::BlendFunc *blend = new osg::BlendFunc;
    if (emissive_particles) {    
        blend->setFunction(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE);
    } else {
        blend->setFunction(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE_MINUS_SRC_ALPHA);
    }
    stateset->setAttributeAndModes(blend, osg::StateAttribute::ON);

    setStateSet(stateset);
}


void osgParticle::ParticleSystem::single_pass_render(osg::State&  /*state*/, const osg::Matrix& modelview) const
{
    _draw_count = 0;
    if (_particles.size() <= 0) return;

    Particle_vector::const_iterator i;
    Particle_vector::const_iterator i0 = _particles.begin();
    Particle_vector::const_iterator end = _particles.end();
    
    i0->beginRender();

    float scale = sqrtf(static_cast<float>(_detail));
    for (i=i0; i<end; i+=_detail) {
        if (i->isAlive()) {
            if (i->getShape() != i0->getShape()) {
                i0->endRender();
                i->beginRender();
                i0 = i;
            }
            ++_draw_count;

            switch (_alignment) {
                case BILLBOARD:
                    i->render(modelview.preMult(i->getPosition()), osg::Vec3(1, 0, 0), osg::Vec3(0, 1, 0), scale);
                    break;
                case FIXED:
                    i->render(i->getPosition(), _align_X_axis, _align_Y_axis, scale);
                    break;
                default: ;
            }            
            
        }        
    }

    i0->endRender();
    
}
