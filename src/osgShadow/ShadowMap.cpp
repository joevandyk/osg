/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield 
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * OpenSceneGraph Public License for more details.
*/

#include <osgShadow/ShadowMap>
#include <osgShadow/ShadowedScene>
#include <osg/Notify>
#include <osg/ComputeBoundsVisitor>
#include <osg/PolygonOffset>
#include <osg/CullFace>
#include <osg/io_utils>

using namespace osgShadow;

#include <iostream>
//for debug
#include <osg/LightSource>
#include <osg/PolygonMode>
#include <osg/Geometry>
#include <osgDB/ReadFile>
#include <osgText/Text>

    //////////////////////////////////////////////////////////////////
    // fragment shader
    //
    static const char fragmentShaderSource_noBaseTexture[] = 
        "uniform sampler2DShadow osgShadow_shadowTexture; \n"
        "uniform vec2 osgShadow_ambientBias; \n"
        "\n"
        "void main(void) \n"
        "{ \n"
        "    gl_FragColor = gl_Color * (osgShadow_ambientBias.x + shadow2DProj( osgShadow_shadowTexture, gl_TexCoord[0] ) * osgShadow_ambientBias.y); \n"
        "}\n";

    //////////////////////////////////////////////////////////////////
    // fragment shader
    //
    static const char fragmentShaderSource_withBaseTexture[] = 
        "uniform sampler2D osgShadow_baseTexture; \n"
        "uniform sampler2DShadow osgShadow_shadowTexture; \n"
        "uniform vec2 osgShadow_ambientBias; \n"
        "\n"
        "void main(void) \n"
        "{ \n"
        "    vec4 color = gl_Color * texture2D( osgShadow_baseTexture, gl_TexCoord[0].xy ); \n"
        "    gl_FragColor = color * (osgShadow_ambientBias.x + shadow2DProj( osgShadow_shadowTexture, gl_TexCoord[1] ) * osgShadow_ambientBias.y); \n"
        "}\n";

    ShadowMap::ShadowMap():
    _baseTextureUnit(0),
        _shadowTextureUnit(1),
        _ambientBias(0.3f,1.2f),
        _textureSize(1024,1024)
    {
    }

    ShadowMap::ShadowMap(const ShadowMap& copy, const osg::CopyOp& copyop):
    ShadowTechnique(copy,copyop),
        _baseTextureUnit(copy._baseTextureUnit),
        _shadowTextureUnit(copy._shadowTextureUnit),
        _ambientBias(copy._ambientBias),
        _textureSize(copy._textureSize)
    {
    }

    void ShadowMap::setTextureUnit(unsigned int unit)
    {
        _shadowTextureUnit = unit;
    }

    void ShadowMap::setAmbientBias(const osg::Vec2& ambientBias)
    {
        _ambientBias = ambientBias;
    }

    void ShadowMap::setTextureSize(const osg::Vec2s& textureSize)
    {
        _textureSize = textureSize;
        dirty();
    }

    void ShadowMap::setLight(osg::Light* light)
    {
        _light = light;
    }


    void ShadowMap::setLight(osg::LightSource* ls)
    {
        _ls = ls;
        _light = _ls->getLight();
    }

    void ShadowMap::createUniforms()
    {
        _uniformList.clear();

        osg::Uniform* baseTextureSampler = new osg::Uniform("osgShadow_baseTexture",(int)_baseTextureUnit);
        _uniformList.push_back(baseTextureSampler);

        osg::Uniform* shadowTextureSampler = new osg::Uniform("osgShadow_shadowTexture",(int)_shadowTextureUnit);
        _uniformList.push_back(shadowTextureSampler);

        osg::Uniform* ambientBias = new osg::Uniform("osgShadow_ambientBias",_ambientBias);
        _uniformList.push_back(ambientBias);

    }

    void ShadowMap::createShaders()
    {
        // if we are not given shaders, use the default
        if( _shaderList.empty() )
        {
            if (_shadowTextureUnit==0)
            {
                osg::Shader* fragment_shader = new osg::Shader(osg::Shader::FRAGMENT, fragmentShaderSource_noBaseTexture);
                _shaderList.push_back(fragment_shader);
            }
            else
            {
                osg::Shader* fragment_shader = new osg::Shader(osg::Shader::FRAGMENT, fragmentShaderSource_withBaseTexture);
                _shaderList.push_back(fragment_shader);

            }
        }
    }

    void ShadowMap::init()
    {
        if (!_shadowedScene) return;

        _texture = new osg::Texture2D;
        _texture->setTextureSize(_textureSize.x(), _textureSize.y());
        _texture->setInternalFormat(GL_DEPTH_COMPONENT);
        _texture->setShadowComparison(true);
        _texture->setShadowTextureMode(osg::Texture2D::LUMINANCE);
        _texture->setFilter(osg::Texture2D::MIN_FILTER,osg::Texture2D::LINEAR);
        _texture->setFilter(osg::Texture2D::MAG_FILTER,osg::Texture2D::LINEAR);
        _texture->setWrap(osg::Texture2D::WRAP_S,osg::Texture2D::CLAMP_TO_EDGE);
        _texture->setWrap(osg::Texture2D::WRAP_T,osg::Texture2D::CLAMP_TO_EDGE);

        // set up the render to texture camera.
        {
            // create the camera
            _camera = new osg::Camera;

            _camera->setCullCallback(new CameraCullCallback(this));

            _camera->setClearMask(GL_DEPTH_BUFFER_BIT);
            //_camera->setClearMask(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
            _camera->setClearColor(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
            _camera->setComputeNearFarMode(osg::Camera::DO_NOT_COMPUTE_NEAR_FAR);

            // set viewport
            _camera->setViewport(0,0,_textureSize.x(),_textureSize.y());

            // set the camera to render before the main camera.
            _camera->setRenderOrder(osg::Camera::PRE_RENDER);

            // tell the camera to use OpenGL frame buffer object where supported.
            _camera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
            //_camera->setRenderTargetImplementation(osg::Camera::SEPERATE_WINDOW);

            // attach the texture and use it as the color buffer.
            _camera->attach(osg::Camera::DEPTH_BUFFER, _texture.get());

            osg::StateSet* stateset = _camera->getOrCreateStateSet();

            float factor = 0.0f;
            float units = 1.0f;

            osg::ref_ptr<osg::PolygonOffset> polygon_offset = new osg::PolygonOffset;
            polygon_offset->setFactor(factor);
            polygon_offset->setUnits(units);
            stateset->setAttribute(polygon_offset.get(), osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
            stateset->setMode(GL_POLYGON_OFFSET_FILL, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

            osg::ref_ptr<osg::CullFace> cull_face = new osg::CullFace;
            cull_face->setMode(osg::CullFace::FRONT);
            stateset->setAttribute(cull_face.get(), osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
            stateset->setMode(GL_CULL_FACE, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

        }

        {
            _stateset = new osg::StateSet;        
            _stateset->setTextureAttributeAndModes(_shadowTextureUnit,_texture.get(),osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
            _stateset->setTextureMode(_shadowTextureUnit,GL_TEXTURE_GEN_S,osg::StateAttribute::ON);
            _stateset->setTextureMode(_shadowTextureUnit,GL_TEXTURE_GEN_T,osg::StateAttribute::ON);
            _stateset->setTextureMode(_shadowTextureUnit,GL_TEXTURE_GEN_R,osg::StateAttribute::ON);
            _stateset->setTextureMode(_shadowTextureUnit,GL_TEXTURE_GEN_Q,osg::StateAttribute::ON);

            _texgen = new osg::TexGen;

            // add Program, when empty of Shaders then we are using fixed functionality
            _program = new osg::Program;
            _stateset->setAttribute(_program.get());

            // create default shaders if needed
            createShaders();

            // add the shader list to the program
            for(ShaderList::const_iterator itr=_shaderList.begin();
                itr!=_shaderList.end();
                ++itr)
            {
                _program->addShader(itr->get());
            }

            // create own uniforms
            createUniforms();

            // add the uniform list to the stateset
            for(UniformList::const_iterator itr=_uniformList.begin();
                itr!=_uniformList.end();
                ++itr)
            {
                _stateset->addUniform(itr->get());
            }

        }

        _dirty = false;
    }


    void ShadowMap::update(osg::NodeVisitor& nv)
    {
        _shadowedScene->osg::Group::traverse(nv);
    }

    void ShadowMap::cull(osgUtil::CullVisitor& cv)
    {
        // record the traversal mask on entry so we can reapply it later.
        unsigned int traversalMask = cv.getTraversalMask();

        osgUtil::RenderStage* orig_rs = cv.getRenderStage();

        // do traversal of shadow recieving scene which does need to be decorated by the shadow map
        {
            cv.pushStateSet(_stateset.get());

            _shadowedScene->osg::Group::traverse(cv);

            cv.popStateSet();

        }

        // need to compute view frustum for RTT camera.
        // 1) get the light position
        // 2) get the center and extents of the view frustum

        const osg::Light* selectLight = 0;
        osg::Vec4 lightpos;
        osg::Vec3 lightDir;

        //MR testing giving a specific light
        osgUtil::PositionalStateContainer::AttrMatrixList& aml = orig_rs->getPositionalStateContainer()->getAttrMatrixList();
        for(osgUtil::PositionalStateContainer::AttrMatrixList::iterator itr = aml.begin();
            itr != aml.end();
            ++itr)
        {
            const osg::Light* light = dynamic_cast<const osg::Light*>(itr->first.get());
            if (light)
            {
                if( _light.valid()) {
                    if( _light.get() == light )
                        selectLight = light;
                    else
                        continue;
                }
                else
                    selectLight = light;

                osg::RefMatrix* matrix = itr->second.get();
                if (matrix) 
                {
                    lightpos = light->getPosition() * (*matrix);
                    lightDir = light->getDirection() * (*matrix);
                }
                else 
                {
                    lightpos = light->getPosition();
                    lightDir = light->getDirection();
                }

            }
        }

        osg::Matrix eyeToWorld;
        eyeToWorld.invert(*cv.getModelViewMatrix());

        lightpos = lightpos * eyeToWorld;
        //MR do we do this for the direction also ? preliminary Yes, but still no good result
        lightDir = lightDir * eyeToWorld;
        lightDir.normalize();

        if (selectLight)
        {

            //std::cout<<"----- VxOSG::ShadowMap selectLight spot cutoff "<<selectLight->getSpotCutoff()<<std::endl;

            if(selectLight->getSpotCutoff() < 180.0f)   // spotlight, then we don't need the bounding box
            {
                osg::Vec3 position(lightpos.x(), lightpos.y(), lightpos.z());
                float spotAngle = selectLight->getSpotCutoff();
                _camera->setReferenceFrame(osg::Camera::ABSOLUTE_RF);
                _camera->setProjectionMatrixAsPerspective(spotAngle, 1.0, 0.1, 1000.0);
                _camera->setViewMatrixAsLookAt(position,position+lightDir,osg::Vec3(0.0f,1.0f,0.0f));
            }
            else
            {
                // get the bounds of the model.    
                osg::ComputeBoundsVisitor cbbv(osg::NodeVisitor::TRAVERSE_ACTIVE_CHILDREN);
                cbbv.setTraversalMask(getShadowedScene()->getCastsShadowTraversalMask());

                _shadowedScene->osg::Group::traverse(cbbv);

                osg::BoundingBox bb = cbbv.getBoundingBox();

                if (lightpos[3]!=0.0)   // point light
                {
                    osg::Vec3 position(lightpos.x(), lightpos.y(), lightpos.z());

                    float centerDistance = (position-bb.center()).length();

                    float znear = centerDistance-bb.radius();
                    float zfar  = centerDistance+bb.radius();
                    float zNearRatio = 0.001f;
                    if (znear<zfar*zNearRatio) znear = zfar*zNearRatio;

                    float top   = (bb.radius()/centerDistance)*znear;
                    float right = top;

                    _camera->setReferenceFrame(osg::Camera::ABSOLUTE_RF);
                    _camera->setProjectionMatrixAsFrustum(-right,right,-top,top,znear,zfar);
                    _camera->setViewMatrixAsLookAt(position,bb.center(),osg::Vec3(0.0f,1.0f,0.0f));
                }
                else    // directional light
                {
                    // make an orthographic projection
                    osg::Vec3 lightDir(lightpos.x(), lightpos.y(), lightpos.z());
                    lightDir.normalize();

                    // set the position far away along the light direction
                    osg::Vec3 position = lightDir * bb.radius()  * 20;

                    float centerDistance = (position-bb.center()).length();

                    float znear = centerDistance-bb.radius();
                    float zfar  = centerDistance+bb.radius();
                    float zNearRatio = 0.001f;
                    if (znear<zfar*zNearRatio) znear = zfar*zNearRatio;

                    float top   = bb.radius();
                    float right = top;

                    _camera->setReferenceFrame(osg::Camera::ABSOLUTE_RF);
                    _camera->setProjectionMatrixAsOrtho(-right, right, -top, top, znear, zfar);
                    _camera->setViewMatrixAsLookAt(position,bb.center(),osg::Vec3(0.0f,1.0f,0.0f));
                }

                // compute the matrix which takes a vertex from local coords into tex coords
                // will use this later to specify osg::TexGen..
                osg::Matrix MVPT = _camera->getViewMatrix() * 
                    _camera->getProjectionMatrix() *
                    osg::Matrix::translate(1.0,1.0,1.0) *
                    osg::Matrix::scale(0.5f,0.5f,0.5f);

                _texgen->setMode(osg::TexGen::EYE_LINEAR);
                _texgen->setPlanesFromMatrix(MVPT);
            }


            cv.setTraversalMask( traversalMask & 
                getShadowedScene()->getCastsShadowTraversalMask() );

            // do RTT camera traversal
            _camera->accept(cv);

            orig_rs->getPositionalStateContainer()->addPositionedTextureAttribute(_shadowTextureUnit, cv.getModelViewMatrix(), _texgen.get());
        } // if(selectLight)


        // reapply the original traversal mask
        cv.setTraversalMask( traversalMask );
    }

    void ShadowMap::cleanSceneGraph()
    {
    }

    ///////////////////// Debug Methods

    osg::ref_ptr<osg::Camera> ShadowMap::makeDebugHUD()
    {
        osg::ref_ptr<osg::Camera> camera = new osg::Camera;

        osg::Vec2 size(1280, 1024);
        // set the projection matrix
        camera->setProjectionMatrix(osg::Matrix::ortho2D(0,size.x(),0,size.y()));

        // set the view matrix    
        camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
        camera->setViewMatrix(osg::Matrix::identity());

        // only clear the depth buffer
        camera->setClearMask(GL_DEPTH_BUFFER_BIT);
        camera->setClearColor(osg::Vec4(0.2f, 0.3f, 0.5f, 0.2f));
        //camera->setClearMask(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        // draw subgraph after main camera view.
        camera->setRenderOrder(osg::Camera::POST_RENDER);

        // we don't want the camera to grab event focus from the viewers main camera(s).
        camera->setAllowEventFocus(false);

        osg::Geode* geode = new osg::Geode;

        osg::Vec3 position(10.0f,size.y()-100.0f,0.0f);
        osg::Vec3 delta(0.0f,-120.0f,0.0f); 
        float lenght = 300.0f;

        // turn the text off to avoid linking with osgText
#if 0
        std::string timesFont("fonts/arial.ttf");

        {
            osgText::Text* text = new  osgText::Text;
            geode->addDrawable( text );

            text->setFont(timesFont);
            text->setPosition(position);
            text->setText("Shadow Map HUD");

            position += delta;
        }
#endif
        
        osg::Vec3 widthVec(lenght, 0.0f, 0.0f);
        osg::Vec3 depthVec(0.0f,lenght, 0.0f);
        osg::Vec3 centerBase( 10.0f + lenght/2, size.y()-lenght/2, 0.0f);
        centerBase += delta;
        
        geode->addDrawable( osg::createTexturedQuadGeometry( centerBase-widthVec*0.5f-depthVec*0.5f, 
                                                             widthVec, depthVec) );

        osg::StateSet* stateset = geode->getOrCreateStateSet();

        stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
        stateset->setMode(GL_BLEND,osg::StateAttribute::ON);
        //stateset->setAttribute(new osg::PolygonOffset(1.0f,1.0f),osg::StateAttribute::ON);
        stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

        //stateset->setTextureAttributeAndModes(_baseTextureUnit, new osg::Texture2D(osgDB::readImageFile("Images/lz.rgb")));
        stateset->setTextureAttributeAndModes(_baseTextureUnit,_texture.get(),osg::StateAttribute::ON);

        //TODO might need to have a shader for correct display

        camera->addChild(geode);

        return camera;
    }

    //////////////////////// End Debug Section

