#ifndef __FLT_2_OSG_H
#define __FLT_2_OSG_H

#include <osg/ref_ptr>
#include <osg/Vec4>

#include <map>
#include <vector>
#include <string>

#include "Record.h"
#include "GeoSetBuilder.h"

namespace osg {
class Object;
class Group;
class LOD;
class Geode;
class GeoSet;
class Material;
class Texture;
class Vec4;
};


namespace flt {

class Record;
class HeaderRecord;
class ColorPaletteRecord;
class MaterialPaletteRecord;
class OldMaterialPaletteRecord;
class TexturePaletteRecord;
class VertexPaletteRecord;
class VertexRecord;
class NormalVertexRecord;
class TextureVertexRecord;
class NormalTextureVertexRecord;
class GroupRecord;
class LodRecord;
class OldLodRecord;
class DofRecord;
class SwitchRecord;
class ObjectRecord;
class FaceRecord;
class MatrixRecord;
class ExternalRecord;
class LightPointRecord;
class VertexListRecord;
class LongIDRecord;
class InstanceDefinitionRecord;
class InstanceReferenceRecord;

//class GeoSetBuilder;

struct SMaterial;

#define ADD_NORMAL(DGSET,VERTEX)                                \
        (DGSET)->addNormal(osg::Vec3(                           \
            (float)(VERTEX)->Normal.x(),                        \
            (float)(VERTEX)->Normal.y(),                        \
            (float)(VERTEX)->Normal.z()));

#define ADD_VERTEX_COLOR(DGSET,VERTEX,COLOR_POOL)               \
    {                                                           \
        if ((VERTEX)->swFlags & V_NO_COLOR_BIT)                 \
            (DGSET)->addColor(_faceColor);                      \
        else                                                    \
        {                                                       \
            if ((VERTEX)->swFlags & V_PACKED_COLOR_BIT)         \
                (DGSET)->addColor(pVert->PackedColor.get());    \
            else                                                \
                (DGSET)->addColor((COLOR_POOL)->getColor((VERTEX)->dwVertexColorIndex)); \
        }                                                       \
    }

#define ADD_TCOORD(DGSET,VERTEX)                                \
        (DGSET)->addTCoord(osg::Vec2(                           \
            (float)(VERTEX)->Texture.x(),                       \
            (float)(VERTEX)->Texture.y()));

#define ADD_OLD_COLOR(DGSET,VERTEX,COLOR_POOL)                  \
    {                                                           \
        if (COLOR_POOL)                                         \
            (DGSET)->addColor((COLOR_POOL)->getColor((VERTEX)->color_index)); \
        else                                                    \
            (DGSET)->addColor(osg::Vec4(1,1,1,1));              \
    }

#define ADD_OLD_TCOORD(DGSET,VERTEX)                            \
        (DGSET)->addTCoord(osg::Vec2(                           \
            (float)(VERTEX)->t[0],                              \
            (float)(VERTEX)->t[1]));

#define ADD_OLD_NORMAL(DGSET,VERTEX)                            \
        (DGSET)->addNormal(osg::Vec3(                           \
            (float)pVert->n[0] / (1<<30),                       \
            (float)pVert->n[1] / (1<<30),                       \
            (float)pVert->n[2] / (1<<30)));


class ConvertFromFLT
{
    public:

        ConvertFromFLT();
        virtual ~ConvertFromFLT();

        osg::Group* convert(HeaderRecord* rec);

        osg::Group* visitAncillary(osg::Group& osgParent, osg::Group& osgPrimary, PrimNodeRecord* rec);
        osg::Group* visitPrimaryNode(osg::Group& osgParent, PrimNodeRecord* rec);

        // Ancillary records
        osg::Group* visitMatrix(osg::Group& osgParent, const osg::Group& osgPrimary, MatrixRecord* rec);
        void visitLongID(osg::Group& osgParent, LongIDRecord* rec);

        // Palette records
        void visitColorPalette(osg::Group& osgParent, ColorPaletteRecord* rec);
        void visitMaterialPalette(osg::Group& osgParent, MaterialPaletteRecord* rec);
        void visitOldMaterialPalette(osg::Group& osgParent, OldMaterialPaletteRecord* rec);
        void visitTexturePalette(osg::Group& osgParent, TexturePaletteRecord* rec);
        void visitVertexPalette(osg::Group& osgParent, VertexPaletteRecord* rec);
        void visitVertex(osg::Group& osgParent, VertexRecord* rec);
        void visitNormalVertex(osg::Group& osgParent, NormalVertexRecord* rec);
        void visitTextureVertex(osg::Group& osgParent, TextureVertexRecord* rec);
        void visitNormalTextureVertex(osg::Group& osgParent, NormalTextureVertexRecord* rec);

        // Primary records
        osg::Group* visitHeader(HeaderRecord* rec);
        osg::Group* visitGroup(osg::Group& osgParent, GroupRecord* rec);
        osg::Group* visitRoadConstruction(osg::Group& osgParent, GroupRecord* rec);
        osg::Group* visitLOD(osg::Group& osgParent, LodRecord* rec);
        osg::Group* visitOldLOD(osg::Group& osgParent, OldLodRecord* rec);
        osg::Group* visitDOF(osg::Group& osgParent, DofRecord* rec);
        osg::Group* visitSwitch(osg::Group& osgParent, SwitchRecord* rec);
        osg::Group* visitObject(osg::Group& osgParent, ObjectRecord* rec);
        osg::Group* visitExternal(osg::Group& osgParent, ExternalRecord* rec);
        osg::Group* visitInstanceDefinition(osg::Group& osgParent,InstanceDefinitionRecord* rec);
        osg::Group* visitInstanceReference(osg::Group& osgParent,InstanceReferenceRecord* rec);

        void visitFace(GeoSetBuilder* pParent, FaceRecord* rec);
        void visitLightPoint(GeoSetBuilder* pBuilder, LightPointRecord* rec);
        int  visitVertexList(GeoSetBuilder* pParent, VertexListRecord* rec);

    private:

        int addVertices(GeoSetBuilder* pBuilder, PrimNodeRecord* primRec);
        int addVertex(DynGeoSet* dgset, Record* rec);
        int addVertex(GeoSetBuilder* pBuilder, Record* rec) {return addVertex( pBuilder->getDynGeoSet(), rec);} ;
        Record* getVertexFromPool(int nOffset);
        void regisiterVertex(int nOffset, Record* pRec);

        typedef std::map<int,Record*> VertexPaletteOffsetMap;
        VertexPaletteOffsetMap _VertexPaletteOffsetMap;

        int                 _diOpenFlightVersion;
        int                 _diCurrentOffset;
        unsigned short      _wObjTransparency;
        int                 _nSubfaceLevel;
        double              _unitScale;
        bool                _bHdrRgbMode;
        osg::Vec4           _faceColor;

        osg::Group*         _osgParent;
};

    
}; // end namespace flt

#endif // __FLT_2_OSG_H

