/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2004 Robert Osfield 
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

#ifndef OBJ_H
#define OBJ_H

#include <string>
#include <vector>
#include <map>
#include <istream>

#include <osg/ref_ptr>
#include <osg/Referenced>
#include <osg/Vec2>
#include <osg/Vec3>
#include <osg/Vec4>


namespace obj
{

class Material
{
public:
    Material():
        ambient(0.2f,0.2f,0.2f,1.0f),
        diffuse(0.8f,0.8f,0.8f,1.0f),
        specular(0.0f,0.0f,0.0f,1.0f),
        emissive(0.0f,0.0f,0.0f,1.0f),
        shininess(0.0f),
        sharpness(0.0f),
        illum(0),
        Tf(0.0f,0.0f,0.0f,1.0f),
        textureReflection(false),
        alpha(1.0f),
        uScale(1.0f),
        vScale(1.0f),
        uOffset(1.0f),
        vOffset(1.0f) {}
    
    std::string name;
    
    osg::Vec4   ambient;
    osg::Vec4   diffuse;
    osg::Vec4   specular;
    osg::Vec4   emissive;
    float       shininess;
    float       sharpness;
    int         illum;
    
    osg::Vec4   Tf;
    int         Ni;
    int         Ns;

    std::string map_Ka;
    std::string map_Kd;
    std::string map_Ks;
    bool        textureReflection;
    float       alpha;
    float       uScale;
    float       vScale;
    float       uOffset;
    float       vOffset;
    
protected:

};

class Element : public osg::Referenced
{
public:

    typedef std::vector<int> IndexList;

    enum DataType
    {
        POINTS,
        POLYLINE,
        POLYGON
    };
    
    Element(DataType type):
        dataType(type) {}
        
    enum CoordinateCombination
    {
        VERTICES,
        VERTICES_NORMALS,
        VERTICES_TEXCOORDS,
        VERTICES_NORMALS_TEXCOORDS
    };
    
    CoordinateCombination getCoordinateCombination() const
    {
        if (vertexIndices.size()==normalIndices.size())
            return (vertexIndices.size()==texCoordIndices.size()) ? VERTICES_NORMALS_TEXCOORDS : VERTICES_NORMALS;
        else 
            return (vertexIndices.size()==texCoordIndices.size()) ?  VERTICES_TEXCOORDS : VERTICES;
    }
    
    DataType  dataType;  
    IndexList vertexIndices;
    IndexList normalIndices;
    IndexList texCoordIndices;
};

class ElementState
{
public:

    ElementState():
        coordinateCombination(Element::VERTICES),
        smoothingGroup(0) {}
        
    bool operator < (const ElementState& rhs) const
    {
        if (materialName<rhs.materialName) return true;
        else if (rhs.materialName<materialName) return false;

        if (objectName<rhs.objectName) return true;
        else if (rhs.objectName<objectName) return false;

        if (groupName<rhs.groupName) return true;
        else if (rhs.groupName<groupName) return false;

        if (coordinateCombination<rhs.coordinateCombination) return true;
        else if (rhs.coordinateCombination<coordinateCombination) return false;

        return (smoothingGroup<rhs.smoothingGroup);
    }


    std::string                     objectName;
    std::string                     groupName;
    std::string                     materialName;
    Element::CoordinateCombination  coordinateCombination;
    int                             smoothingGroup;
};

class Model
{
public:
    Model():
        currentElementList(0) {}
    
    void setDatabasePath(const std::string& path) { databasePath = path; }
    const std::string& getDatabasePath() const { return databasePath; }

    bool readMTL(std::istream& fin);
    bool readOBJ(std::istream& fin);
    
    bool readline(std::istream& fin, char* line, const int LINE_SIZE);
    void addElement(Element* element);
    
    osg::Vec3 averageNormal(const Element& element) const;
    osg::Vec3 computeNormal(const Element& element) const;
    bool needReverse(const Element& element) const;
    
    int remapVertexIndex(int vi) { return (vi<0) ? vertices.size()+vi : vi-1; }
    int remapNormalIndex(int vi) { return (vi<0) ? normals.size()+vi : vi-1; }
    int remapTexCoordIndex(int vi) { return (vi<0) ? texcoords.size()+vi : vi-1; }
    
    typedef std::map<std::string,Material>          MaterialMap;
    typedef std::vector< osg::Vec2 >                Vec2Array;
    typedef std::vector< osg::Vec3 >                Vec3Array;
    typedef std::vector< osg::ref_ptr<Element> >    ElementList;
    typedef std::map< ElementState,ElementList >    ElementStateMap;
    

    std::string     databasePath;
    MaterialMap     materialMap;
       
    Vec3Array       vertices;
    Vec3Array       normals;
    Vec2Array       texcoords;
    
    ElementState    currentElementState;
    
    ElementStateMap elementStateMap;
    ElementList*    currentElementList;

};

}





#endif
