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

#include <osgTerrain/TerrainTechnique>
#include <osgTerrain/Terrain>

using namespace osgTerrain;

TerrainTechnique::TerrainTechnique():
    _terrain(0),
    _dirty(true)
{
    setThreadSafeRefUnref(true);
}

TerrainTechnique::TerrainTechnique(const TerrainTechnique& TerrainTechnique,const osg::CopyOp& copyop):
    osg::Object(TerrainTechnique,copyop),
    _terrain(0),
    _dirty(true)
{
}

TerrainTechnique::~TerrainTechnique()
{
}

void TerrainTechnique::init()
{
    osg::notify(osg::NOTICE)<<className()<<"::initialize(..) not implementated yet"<<std::endl;
    
    _dirty = false;
}

void TerrainTechnique::update(osgUtil::UpdateVisitor* uv)
{
    osg::notify(osg::NOTICE)<<className()<<"::update(..) not implementated yet"<<std::endl;
    if (_terrain) _terrain->osg::Group::traverse(*uv);
}

void TerrainTechnique::cull(osgUtil::CullVisitor* cv)
{
    osg::notify(osg::NOTICE)<<className()<<"::cull(..) not implementated yet"<<std::endl;
    if (_terrain) _terrain->osg::Group::traverse(*cv);
}

void TerrainTechnique::cleanSceneGraph()
{
    osg::notify(osg::NOTICE)<<className()<<"::cleanSceneGraph(..) not implementated yet"<<std::endl;
}

void TerrainTechnique::dirty()
{
    // osg::notify(osg::NOTICE)<<className()<<"::dirty(..) not implementated yet"<<std::endl;    
    _dirty = true;
}

void TerrainTechnique::traverse(osg::NodeVisitor& nv)
{
    if (!_terrain) return;

    // if app traversal update the frame count.
    if (nv.getVisitorType()==osg::NodeVisitor::UPDATE_VISITOR)
    {
        if (_dirty) init();

        osgUtil::UpdateVisitor* uv = dynamic_cast<osgUtil::UpdateVisitor*>(&nv);
        if (uv)
        {
            update(uv);
            return;
        }        
        
    }
    else if (nv.getVisitorType()==osg::NodeVisitor::CULL_VISITOR)
    {
        osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(&nv);
        if (cv)
        {
            cull(cv);
            return;
        }
    }

    if (_dirty) init();

    // otherwise fallback to the Group::traverse()
    _terrain->osg::Group::traverse(nv);
}
