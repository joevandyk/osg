// -*-c++-*-

/*
 * $Id$
 *
 * STL importer for OpenSceneGraph.
 * Copyright (c)2004 Ulrich Hertlein <u.hertlein@sandbox.de>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <osg/Notify>
#include <osg/Endian>

#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osgDB/FileNameUtils>
#include <osgDB/FileUtils>

#include <osg/Geode>
#include <osg/Geometry>

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>


/**
 * STL importer for OpenSceneGraph.
 */
class ReaderWriterSTL : public osgDB::ReaderWriter
{
public:
    ReaderWriterSTL() {}

    virtual const char* className() const {
        return "STL Reader/Writer";
    }

    virtual bool acceptsExtension(const std::string& extension) const { 
        return
            osgDB::equalCaseInsensitive(extension,"stl") ? true :
            osgDB::equalCaseInsensitive(extension,"sta") ? true : false;
    }

    virtual ReadResult readNode(const std::string& fileName, const osgDB::ReaderWriter::Options*) const;

private:

    struct ReaderObject
    {
        ReaderObject():
            _generateNormal(true),
            _numFacets(0) {}

        bool _generateNormal;
        unsigned int _numFacets;

        osg::ref_ptr<osg::Vec3Array> _vertex;
        osg::ref_ptr<osg::Vec3Array> _normal;
        osg::ref_ptr<osg::Vec4Array> _color;

        bool readStlAscii(FILE* fp);
        bool readStlBinary(FILE* fp);
    };
};


// Register with Registry to instantiate the above reader/writer.
osgDB::RegisterReaderWriterProxy<ReaderWriterSTL> g_readerWriter_STL_Proxy;


/*
 * STL
 */
struct StlHeader {
    char text[80];
    unsigned int numFacets;
};
const unsigned int sizeof_StlHeader = 84;

struct StlVector {
    float x,y,z;
};
struct StlFacet {
    StlVector normal;
    StlVector vertex[3];
    unsigned short color;
};
const unsigned int sizeof_StlFacet = 50;

const unsigned short StlHasColor = 0x8000;
const unsigned short StlColorSize = 0x1f;        // 5 bit
const float StlColorDepth = float(StlColorSize); // 2^5 - 1


// Read node
osgDB::ReaderWriter::ReadResult ReaderWriterSTL::readNode(const std::string& file, const osgDB::ReaderWriter::Options* options) const
{
    std::string ext = osgDB::getLowerCaseFileExtension(file);
    if (!acceptsExtension(ext)) return ReadResult::FILE_NOT_HANDLED;

    std::string fileName = osgDB::findDataFile( file, options );
    if (fileName.empty()) return ReadResult::FILE_NOT_FOUND;

    osg::notify(osg::INFO) << "ReaderWriterSTL::readNode(" << fileName.c_str() << ")\n";

    // determine ASCII vs. binary mode
    FILE* fp = fopen(fileName.c_str(), "r");
    if (!fp) {
        return ReadResult::FILE_NOT_HANDLED;
    }

    ReaderObject readerObject;

    // assumes "unsigned int" is 4 bytes...
    StlHeader header;
    if (fread((void*) &header, sizeof(header), 1, fp) != 1) {
        fclose(fp);
        return ReadResult::FILE_NOT_HANDLED;
    }
    bool isBinary = false;

    // calculate expected file length from number of facets
    unsigned int expectFacets = header.numFacets;
    if (osg::getCpuByteOrder() == osg::BigEndian) {
        osg::swapBytes4((char*) &expectFacets);
    }
    off_t expectLen = sizeof_StlHeader + expectFacets * sizeof_StlFacet;
 
    struct stat stb;
    if (fstat(fileno(fp), &stb) < 0) {
        osg::notify(osg::FATAL) << "ReaderWriterSTL::readNode: Unable to stat '" << fileName << "'" << std::endl;
        fclose(fp);
        return ReadResult::FILE_NOT_HANDLED;
    }
    if (stb.st_size == expectLen) {
        // assume binary
        readerObject._numFacets = expectFacets;
        isBinary = true;
    }
    else if (strstr(header.text, "solid") != 0) {
        // assume ASCII
        isBinary = false;
    }
    else {
        osg::notify(osg::FATAL) << "ReaderWriterSTL::readNode(" << fileName.c_str() << ") unable to determine file format" << std::endl;
        fclose(fp);
        return ReadResult::FILE_NOT_HANDLED;
    }

    // read
    rewind(fp);
    bool ok = (isBinary ? readerObject.readStlBinary(fp) : readerObject.readStlAscii(fp));
    fclose(fp);

    if (!ok) {
        return ReadResult::FILE_NOT_HANDLED;
    }
    osg::notify(osg::NOTICE) << "### found " << readerObject._numFacets << " facets" << std::endl;

    /*
     * setup geometry
     */
    osg::Geometry* geom = new osg::Geometry;
    geom->setVertexArray(readerObject._vertex.get());

    geom->setNormalArray(readerObject._normal.get());
    geom->setNormalBinding(osg::Geometry::BIND_PER_PRIMITIVE);

    if (readerObject._color.valid()) {
        osg::notify(osg::NOTICE) << "### with color" << std::endl;
        geom->setColorArray(readerObject._color.get());
        geom->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE);
    }

    geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, readerObject._numFacets*3));

    osg::Geode* geode = new osg::Geode;
    geode->addDrawable(geom);

    return geode;
}


/**********************************************************************
 *
 * Private
 *
 **********************************************************************/

bool ReaderWriterSTL::ReaderObject::readStlAscii(FILE* fp)
{
    unsigned int vertexCount = 0;
    unsigned int facetIndex[] = { 0,0,0 };
    unsigned int vertexIndex = 0;
    unsigned int normalIndex = 0;

    char buf[256];
    while (fgets(buf, sizeof(buf), fp)) {

        // strip '\n' or '\r\n' and trailing whitespace
        unsigned int len = strlen(buf)-1;
        while (len && (buf[len] == '\n' || buf[len] == '\r' || isspace(buf[len]))) {
            buf[len--] = '\0';
        }
        if (len == 0 || buf[0] == '\0') {
            continue;
        }

        // strip leading whitespace
        char* bp = buf;
        while (isspace(*bp)) {
            ++bp;
        }

        if (strncmp(bp, "vertex", 6) == 0) {
            float vx,vy,vz;
            if (sscanf(bp+6, "%f %f %f", &vx,&vy,&vz) == 3) {
                if (!_vertex.valid())
                    _vertex = new osg::Vec3Array;

                vertexIndex = _vertex->size();
                if (vertexCount < 3) {
                    _vertex->push_back(osg::Vec3(vx,vy,vz));
                    facetIndex[vertexCount++] = vertexIndex;
                }
                else {
                    /*
                     * There are some invalid ASCII files around (at least one ;-)
                     * that have more than three vertices per facet - add an
                     * additional triangle.
                     */
                    _normal->push_back((*_normal)[normalIndex]);
                    _vertex->push_back((*_vertex)[facetIndex[0]]);
                    _vertex->push_back((*_vertex)[facetIndex[2]]);
                    _vertex->push_back(osg::Vec3(vx,vy,vz));
                    facetIndex[1] = facetIndex[2];
                    facetIndex[2] = vertexIndex;
                    _numFacets++;
                }
            }
        }
        else if (strncmp(bp, "facet", 5) == 0) {
            float nx,ny,nz;
            if (sscanf(bp+5, "%*s %f %f %f", &nx,&ny,&nz) == 3) {

                if (!_normal.valid())
                    _normal = new osg::Vec3Array;

                osg::Vec3 normal(nx,ny,nz);
                normal.normalize();

                normalIndex = _normal->size();
                _normal->push_back(normal);

                _numFacets++;
                vertexCount = 0;
            }
        }
        else if (strncmp(bp, "solid", 5) == 0) {
            osg::notify(osg::NOTICE) << "### parsing '" << bp + 6 << "'" << std::endl;
        }
    }

    return true;
}

bool ReaderWriterSTL::ReaderObject::readStlBinary(FILE* fp)
{
    // seek to beginning of facets
    ::fseek(fp, sizeof_StlHeader, SEEK_SET);

    StlFacet facet;
    for (unsigned int i = 0; i < _numFacets; ++i) {

        if (::fread((void*) &facet, sizeof_StlFacet, 1, fp) != 1) {
            osg::notify(osg::FATAL) << "ReaderWriterSTL::readStlBinary: Failed to read facet " << i << std::endl;
            return false;
        }

        // vertices
        if (!_vertex)
            _vertex = new osg::Vec3Array;
        osg::Vec3 v0(facet.vertex[0].x,facet.vertex[0].y,facet.vertex[0].z);
        osg::Vec3 v1(facet.vertex[1].x,facet.vertex[1].y,facet.vertex[1].z);
        osg::Vec3 v2(facet.vertex[2].x,facet.vertex[2].y,facet.vertex[2].z);
        _vertex->push_back(v0);
        _vertex->push_back(v1);
        _vertex->push_back(v2);

        // per-facet normal
        osg::Vec3 normal;
        if (_generateNormal) {
            osg::Vec3 d01 = v1 - v0;
            osg::Vec3 d02 = v2 - v0;
            normal = d01 ^ d02;
            normal.normalize();
        }
        else {
            normal.set(facet.normal.x,facet.normal.y,facet.normal.z);
        }
        if (!_normal.valid())
            _normal = new osg::Vec3Array;
        _normal->push_back(normal);

        /*
         * color extension
         * RGB555 with most-significat bit indicating if color is present
         */
        if (facet.color & StlHasColor) {
            if (!_color) {
                _color = new osg::Vec4Array;
            }
            float r = ((facet.color >> 10) & StlColorSize) / StlColorDepth;
            float g = ((facet.color >> 5) & StlColorSize) / StlColorDepth;
            float b = (facet.color & StlColorSize) / StlColorDepth;
            _color->push_back(osg::Vec4(r,g,b,1.0f));
        }
    }

    return true;
}
