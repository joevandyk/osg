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

#include <osg/Notify>
#include <osg/Object>
#include <osg/Image>
#include <osg/Node>
#include <osg/Group>
#include <osg/Geode>

#include <osgDB/Registry>
#include <osgDB/WriteFile>

using namespace osg;
using namespace osgDB;

bool osgDB::writeObjectFile(const Object& object,const std::string& filename)
{
    ReaderWriter::WriteResult wr = Registry::instance()->writeObject(object,filename);
    if (wr.error()) notify(WARN) << wr.message() << std::endl;
    return wr.success();
}


bool osgDB::writeImageFile(const Image& image,const std::string& filename)
{
    ReaderWriter::WriteResult wr = Registry::instance()->writeImage(image,filename);
    if (wr.error()) notify(WARN) << wr.message() << std::endl;
    return wr.success();
}


bool osgDB::writeNodeFile(const Node& node,const std::string& filename)
{
    ReaderWriter::WriteResult wr = Registry::instance()->writeNode(node,filename);
    if (wr.error()) notify(WARN) << wr.message() << std::endl;
    return wr.success();
}
