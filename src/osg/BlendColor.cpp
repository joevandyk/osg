/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2003 Robert Osfield 
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
#include <osg/BlendColor>
#include <osg/GLExtensions>
#include <osg/State>
#include <osg/Notify>

using namespace osg;


BlendColor::BlendColor() :
            _constantColor(0.0f,0.0f,0.0f,0.0f)
{
}

BlendColor::~BlendColor()
{
}

void BlendColor::apply(State& state) const
{
    // get the contextID (user defined ID of 0 upwards) for the 
    // current OpenGL context.
    const unsigned int contextID = state.getContextID();
    
    const Extensions* extensions = getExtensions(contextID,true);
                                        
    if (!extensions->isBlendColorSupported())
    {
        notify(WARN)<<"Warning: BlendColor::apply(..) failed, BlendColor is not support by OpenGL driver."<<std::endl;
        return;
    }

    extensions->glBlendColor(_constantColor[0], _constantColor[1],
                             _constantColor[2], _constantColor[3]);
}





typedef buffered_value< ref_ptr<BlendColor::Extensions> > BufferedExtensions;
static BufferedExtensions s_extensions;

BlendColor::Extensions* BlendColor::getExtensions(unsigned int contextID,bool createIfNotInitalized)
{
    if (!s_extensions[contextID] && createIfNotInitalized) s_extensions[contextID] = new Extensions;
    return s_extensions[contextID].get();
}

void BlendColor::setExtensions(unsigned int contextID,Extensions* extensions)
{
    s_extensions[contextID] = extensions;
}


BlendColor::Extensions::Extensions()
{
    setupGLExtenions();
}

BlendColor::Extensions::Extensions(const Extensions& rhs):
    Referenced()
{
    _isBlendColorSupported = rhs._isBlendColorSupported;
}

void BlendColor::Extensions::lowestCommonDenominator(const Extensions& rhs)
{
    if (!rhs._isBlendColorSupported)  _isBlendColorSupported = false;
    if (!rhs._glBlendColor)           _glBlendColor = 0;
}

void BlendColor::Extensions::setupGLExtenions()
{
    _isBlendColorSupported = isGLExtensionSupported("GL_EXT_blend_color") ||
                             strncmp((const char*)glGetString(GL_VERSION),"1.2",3)>=0;

    _glBlendColor = getGLExtensionFuncPtr("glBlendColor", "glBlendColorEXT");
}

void BlendColor::Extensions::glBlendColor(GLclampf red , GLclampf green , GLclampf blue , GLclampf alpha) const
{
    if (_glBlendColor)
    {
        typedef void (APIENTRY * GLBlendColorProc) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
        ((GLBlendColorProc)_glBlendColor)(red, green, blue, alpha);
    }
    else
    {
        notify(WARN)<<"Error: glBlendColor not supported by OpenGL driver"<<std::endl;
    }
}


