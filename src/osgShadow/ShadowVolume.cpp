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

#include <osgShadow/ShadowVolume>
#include <osg/Notify>

using namespace osgShadow;

ShadowVolume::ShadowVolume()    
{
    osg::notify(osg::NOTICE)<<"Warning: osgShadow::ShadowVolume technique not implemented yet."<<std::endl;
}

ShadowVolume::ShadowVolume(const ShadowVolume& copy, const osg::CopyOp& copyop):
    ShadowTechnique(copy,copyop)
{
}