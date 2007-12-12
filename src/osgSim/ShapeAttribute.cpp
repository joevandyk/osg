/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2007 Robert Osfield
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

#include <memory.h>

#include <osgSim/ShapeAttribute>

namespace osgSim
{
ShapeAttribute::ShapeAttribute() :
    _type(UNKNOW),
    _integer(0)
{}

ShapeAttribute::ShapeAttribute(const char * name) :
    _name(name),
    _type(UNKNOW),
    _integer(0)
{}

ShapeAttribute::ShapeAttribute(const char * name, int value) :
    _name(name),
    _type(INTEGER),
    _integer(value)
{}
        
ShapeAttribute::ShapeAttribute(const char * name, double value) :
    _name(name),
    _type(DOUBLE),
    _double(value)
{}

ShapeAttribute::ShapeAttribute(const char * name, const char * value) :
    _name(name),
    _type(STRING)
{
    std::string str(value);
    _string = new char[str.size() + 1];
    memcpy(_string, str.c_str(), str.size());
    _string[str.size()] = 0;
}

ShapeAttribute::ShapeAttribute(const ShapeAttribute & sa) :
    _name(sa._name),
    _type(sa._type)
{
    switch (_type)
    {
        case INTEGER:
        {
            _integer = sa._integer;
            break;
        }
        case STRING:
        {
            std::string str(sa._string);
            _string = new char[str.size() + 1];
            memcpy(_string, str.c_str(), str.size());
            _string[str.size()] = 0;
            break;
        }
        case DOUBLE:
        {
            _double = sa._double;
            break;
        }
        case UNKNOW:
        default:
        {
            _integer = 0;
            break;
        }
    }
}
    
    
ShapeAttribute::~ShapeAttribute()
{
    if ((_type == STRING) && (_string)) delete [] _string; 
}



int ShapeAttribute::compare(const osgSim::ShapeAttribute& sa) const
{
    if (_name<sa._name) return -1;
    if (sa._name<_name) return 1;
    
    if (_type<sa._type) return -1;
    if (sa._type<_type) return 1;
    
    if (_name<sa._name) return -1;
    if (sa._name<_name) return 1;
 
    switch (_type)
    {
        case STRING:
        {
            if (_string<sa._string) return -1;
            if (sa._string<_string) return 1;
        }
        case DOUBLE:
        {
            if (_double<sa._double) return -1;
            if (sa._double<_double) return 1;
        }
        case INTEGER:
        case UNKNOW:
        default:
        {
            if (_integer<sa._integer) return -1;
            if (sa._integer<_integer) return 1;
        }
    }
    
    return 0;
}
    



/** return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs.*/
int ShapeAttributeList::compare(const osgSim::ShapeAttributeList& sal) const
{
    const_iterator salIt, thisIt, thisEnd = end();
    
    int ret;
    for (thisIt = begin(), salIt = sal.begin(); thisIt!= thisEnd; ++thisIt, ++salIt)
        if ((ret = thisIt->compare(*salIt)) != 0) return ret;        
    
    return 0;
}

}
