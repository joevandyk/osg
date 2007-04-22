// ***************************************************************************
//
//   Generated automatically by genwrapper.
//   Please DO NOT EDIT this file!
//
// ***************************************************************************

#include <osgIntrospection/ReflectionMacros>
#include <osgIntrospection/TypedMethodInfo>
#include <osgIntrospection/StaticMethodInfo>
#include <osgIntrospection/Attributes>

#include <osg/BoundingBox>
#include <osg/BoundingSphere>
#include <osg/LineSegment>
#include <osg/Matrix>
#include <osg/Vec3>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

BEGIN_OBJECT_REFLECTOR(osg::LineSegment)
	I_BaseType(osg::Referenced);
	I_Constructor0(____LineSegment,
	               "",
	               "");
	I_Constructor1(IN, const osg::LineSegment &, seg,
	               Properties::NON_EXPLICIT,
	               ____LineSegment__C5_LineSegment_R1,
	               "",
	               "");
	I_Constructor2(IN, const osg::Vec3 &, s, IN, const osg::Vec3 &, e,
	               ____LineSegment__C5_Vec3_R1__C5_Vec3_R1,
	               "",
	               "");
	I_Method2(void, set, IN, const osg::Vec3 &, s, IN, const osg::Vec3 &, e,
	          Properties::NON_VIRTUAL,
	          __void__set__C5_Vec3_R1__C5_Vec3_R1,
	          "",
	          "");
	I_Method0(osg::Vec3 &, start,
	          Properties::NON_VIRTUAL,
	          __Vec3_R1__start,
	          "",
	          "");
	I_Method0(const osg::Vec3 &, start,
	          Properties::NON_VIRTUAL,
	          __C5_Vec3_R1__start,
	          "",
	          "");
	I_Method0(osg::Vec3 &, end,
	          Properties::NON_VIRTUAL,
	          __Vec3_R1__end,
	          "",
	          "");
	I_Method0(const osg::Vec3 &, end,
	          Properties::NON_VIRTUAL,
	          __C5_Vec3_R1__end,
	          "",
	          "");
	I_Method0(bool, valid,
	          Properties::NON_VIRTUAL,
	          __bool__valid,
	          "",
	          "");
	I_Method1(bool, intersect, IN, const osg::BoundingBox &, bb,
	          Properties::NON_VIRTUAL,
	          __bool__intersect__C5_BoundingBox_R1,
	          "return true if segment intersects BoundingBox. ",
	          "");
	I_Method3(bool, intersect, IN, const osg::BoundingBox &, bb, IN, float &, r1, IN, float &, r2,
	          Properties::NON_VIRTUAL,
	          __bool__intersect__C5_BoundingBox_R1__float_R1__float_R1,
	          "return true if segment intersects BoundingBox and return the intersection ratios. ",
	          "");
	I_Method1(bool, intersect, IN, const osg::BoundingSphere &, bs,
	          Properties::NON_VIRTUAL,
	          __bool__intersect__C5_BoundingSphere_R1,
	          "return true if segment intersects BoundingSphere. ",
	          "");
	I_Method3(bool, intersect, IN, const osg::BoundingSphere &, bs, IN, float &, r1, IN, float &, r2,
	          Properties::NON_VIRTUAL,
	          __bool__intersect__C5_BoundingSphere_R1__float_R1__float_R1,
	          "return true if segment intersects BoundingSphere and return the intersection ratio. ",
	          "");
	I_Method4(bool, intersect, IN, const osg::Vec3 &, v1, IN, const osg::Vec3 &, v2, IN, const osg::Vec3 &, v3, IN, float &, r,
	          Properties::NON_VIRTUAL,
	          __bool__intersect__C5_Vec3_R1__C5_Vec3_R1__C5_Vec3_R1__float_R1,
	          "return true if segment intersects triangle and set ratio long segment. ",
	          "");
	I_Method2(void, mult, IN, const osg::LineSegment &, seg, IN, const osg::Matrix &, m,
	          Properties::NON_VIRTUAL,
	          __void__mult__C5_LineSegment_R1__C5_Matrix_R1,
	          "post multiply a segment by matrix. ",
	          "");
	I_Method2(void, mult, IN, const osg::Matrix &, m, IN, const osg::LineSegment &, seg,
	          Properties::NON_VIRTUAL,
	          __void__mult__C5_Matrix_R1__C5_LineSegment_R1,
	          "pre multiply a segment by matrix. ",
	          "");
END_REFLECTOR

