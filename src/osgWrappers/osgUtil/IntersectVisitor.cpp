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

#include <osg/Billboard>
#include <osg/CameraNode>
#include <osg/Drawable>
#include <osg/Geode>
#include <osg/Group>
#include <osg/LOD>
#include <osg/LineSegment>
#include <osg/Matrix>
#include <osg/Matrixd>
#include <osg/Node>
#include <osg/Projection>
#include <osg/Switch>
#include <osg/Transform>
#include <osg/Vec3>
#include <osg/Viewport>
#include <osgUtil/IntersectVisitor>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

TYPE_NAME_ALIAS(std::vector< int >, osgUtil::Hit::VecIndexList);

BEGIN_VALUE_REFLECTOR(osgUtil::Hit)
	I_Constructor0(____Hit,
	               "Describes a point in space produced by an intersection of a line with a scene. ",
	               "A Hit is always on a surface as rendered by the Open Scene Graph scene (usually a triangle or other primitive, but a special hit handler could return a different value perhaps: a sphere shape might return a Hit on the true sphere rather than the approximate tesselated sphere rendered.");
	I_Constructor1(IN, const osgUtil::Hit &, hit,
	               ____Hit__C5_Hit_R1,
	               "",
	               "");
	I_Method0(const osg::Vec3 &, getLocalIntersectPoint,
	          __C5_osg_Vec3_R1__getLocalIntersectPoint,
	          "",
	          "");
	I_Method0(const osg::Vec3 &, getLocalIntersectNormal,
	          __C5_osg_Vec3_R1__getLocalIntersectNormal,
	          "",
	          "");
	I_Method0(const osg::Vec3, getWorldIntersectPoint,
	          __C5_osg_Vec3__getWorldIntersectPoint,
	          "",
	          "");
	I_Method0(const osg::Vec3, getWorldIntersectNormal,
	          __C5_osg_Vec3__getWorldIntersectNormal,
	          "",
	          "");
	I_Method0(float, getRatio,
	          __float__getRatio,
	          "",
	          "");
	I_Method0(const osg::LineSegment *, getOriginalLineSegment,
	          __C5_osg_LineSegment_P1__getOriginalLineSegment,
	          "",
	          "");
	I_Method0(const osg::LineSegment *, getLocalLineSegment,
	          __C5_osg_LineSegment_P1__getLocalLineSegment,
	          "",
	          "");
	I_Method0(osg::NodePath &, getNodePath,
	          __osg_NodePath_R1__getNodePath,
	          "",
	          "");
	I_Method0(const osg::NodePath &, getNodePath,
	          __C5_osg_NodePath_R1__getNodePath,
	          "",
	          "");
	I_Method0(osg::Geode *, getGeode,
	          __osg_Geode_P1__getGeode,
	          "",
	          "");
	I_Method0(const osg::Geode *, getGeode,
	          __C5_osg_Geode_P1__getGeode,
	          "",
	          "");
	I_Method0(osg::Drawable *, getDrawable,
	          __osg_Drawable_P1__getDrawable,
	          "",
	          "");
	I_Method0(const osg::Drawable *, getDrawable,
	          __C5_osg_Drawable_P1__getDrawable,
	          "",
	          "");
	I_Method0(const osg::RefMatrix *, getMatrix,
	          __C5_osg_RefMatrix_P1__getMatrix,
	          "",
	          "");
	I_Method0(const osg::RefMatrix *, getInverseMatrix,
	          __C5_osg_RefMatrix_P1__getInverseMatrix,
	          "",
	          "");
	I_Method0(const osgUtil::Hit::VecIndexList &, getVecIndexList,
	          __C5_VecIndexList_R1__getVecIndexList,
	          "",
	          "");
	I_Method0(int, getPrimitiveIndex,
	          __int__getPrimitiveIndex,
	          "",
	          "");
	I_SimpleProperty(osg::Drawable *, Drawable, 
	                 __osg_Drawable_P1__getDrawable, 
	                 0);
	I_SimpleProperty(osg::Geode *, Geode, 
	                 __osg_Geode_P1__getGeode, 
	                 0);
	I_SimpleProperty(const osg::RefMatrix *, InverseMatrix, 
	                 __C5_osg_RefMatrix_P1__getInverseMatrix, 
	                 0);
	I_SimpleProperty(const osg::Vec3 &, LocalIntersectNormal, 
	                 __C5_osg_Vec3_R1__getLocalIntersectNormal, 
	                 0);
	I_SimpleProperty(const osg::Vec3 &, LocalIntersectPoint, 
	                 __C5_osg_Vec3_R1__getLocalIntersectPoint, 
	                 0);
	I_SimpleProperty(const osg::LineSegment *, LocalLineSegment, 
	                 __C5_osg_LineSegment_P1__getLocalLineSegment, 
	                 0);
	I_SimpleProperty(const osg::RefMatrix *, Matrix, 
	                 __C5_osg_RefMatrix_P1__getMatrix, 
	                 0);
	I_SimpleProperty(osg::NodePath &, NodePath, 
	                 __osg_NodePath_R1__getNodePath, 
	                 0);
	I_SimpleProperty(const osg::LineSegment *, OriginalLineSegment, 
	                 __C5_osg_LineSegment_P1__getOriginalLineSegment, 
	                 0);
	I_SimpleProperty(int, PrimitiveIndex, 
	                 __int__getPrimitiveIndex, 
	                 0);
	I_SimpleProperty(float, Ratio, 
	                 __float__getRatio, 
	                 0);
	I_SimpleProperty(const osgUtil::Hit::VecIndexList &, VecIndexList, 
	                 __C5_VecIndexList_R1__getVecIndexList, 
	                 0);
	I_SimpleProperty(const osg::Vec3, WorldIntersectNormal, 
	                 __C5_osg_Vec3__getWorldIntersectNormal, 
	                 0);
	I_SimpleProperty(const osg::Vec3, WorldIntersectPoint, 
	                 __C5_osg_Vec3__getWorldIntersectPoint, 
	                 0);
	I_PublicMemberProperty(float, _ratio);
	I_PublicMemberProperty(osg::ref_ptr< osg::LineSegment >, _originalLineSegment);
	I_PublicMemberProperty(osg::ref_ptr< osg::LineSegment >, _localLineSegment);
	I_PublicMemberProperty(osg::NodePath, _nodePath);
	I_PublicMemberProperty(osg::ref_ptr< osg::Geode >, _geode);
	I_PublicMemberProperty(osg::ref_ptr< osg::Drawable >, _drawable);
	I_PublicMemberProperty(osg::ref_ptr< osg::RefMatrix >, _matrix);
	I_PublicMemberProperty(osg::ref_ptr< osg::RefMatrix >, _inverse);
	I_PublicMemberProperty(osgUtil::Hit::VecIndexList, _vecIndexList);
	I_PublicMemberProperty(int, _primitiveIndex);
	I_PublicMemberProperty(osg::Vec3, _intersectPoint);
	I_PublicMemberProperty(osg::Vec3, _intersectNormal);
END_REFLECTOR

TYPE_NAME_ALIAS(std::vector< osgUtil::Hit >, osgUtil::IntersectVisitor::HitList);

TYPE_NAME_ALIAS(std::map< const osg::LineSegment * COMMA  osgUtil::IntersectVisitor::HitList >, osgUtil::IntersectVisitor::LineSegmentHitListMap);

BEGIN_ENUM_REFLECTOR(osgUtil::IntersectVisitor::LODSelectionMode)
	I_EnumLabel(osgUtil::IntersectVisitor::USE_HIGHEST_LEVEL_OF_DETAIL);
	I_EnumLabel(osgUtil::IntersectVisitor::USE_SEGMENT_START_POINT_AS_EYE_POINT_FOR_LOD_LEVEL_SELECTION);
END_REFLECTOR

BEGIN_OBJECT_REFLECTOR(osgUtil::IntersectVisitor)
	I_BaseType(osg::NodeVisitor);
	I_Constructor0(____IntersectVisitor,
	               "",
	               "");
	I_Method0(void, reset,
	          __void__reset,
	          "Method to call to reset visitor. ",
	          "Useful if your visitor accumulates state during a traversal, and you plan to reuse the visitor. To flush that state for the next traversal: call reset() prior to each traversal. ");
	I_Method1(void, addLineSegment, IN, osg::LineSegment *, seg,
	          __void__addLineSegment__osg_LineSegment_P1,
	          "Add a line segment to use for intersection testing during scene traversal. ",
	          "Note, a maximum of 32 line segments can be added to a IntersectVistor, adding more than this will result in warning being emitted to the console and the excess segments being ignored. ");
	I_Method1(osgUtil::IntersectVisitor::HitList &, getHitList, IN, const osg::LineSegment *, seg,
	          __HitList_R1__getHitList__C5_osg_LineSegment_P1,
	          "",
	          "");
	I_Method1(int, getNumHits, IN, const osg::LineSegment *, seg,
	          __int__getNumHits__C5_osg_LineSegment_P1,
	          "",
	          "");
	I_Method0(osgUtil::IntersectVisitor::LineSegmentHitListMap &, getSegHitList,
	          __LineSegmentHitListMap_R1__getSegHitList,
	          "",
	          "");
	I_Method0(bool, hits,
	          __bool__hits,
	          "",
	          "");
	I_Method1(void, setLODSelectionMode, IN, osgUtil::IntersectVisitor::LODSelectionMode, mode,
	          __void__setLODSelectionMode__LODSelectionMode,
	          "",
	          "");
	I_Method0(osgUtil::IntersectVisitor::LODSelectionMode, getLODSelectionMode,
	          __LODSelectionMode__getLODSelectionMode,
	          "",
	          "");
	I_Method1(void, setEyePoint, IN, const osg::Vec3 &, eye,
	          __void__setEyePoint__C5_osg_Vec3_R1,
	          "Set the eye point in local coordinates. ",
	          "This is a pseudo-EyePoint for billboarding and LOD purposes. It is copied from the Start point of the most-recently-added segment of the intersection ray set (IntersectState::_segList). ");
	I_Method0(osg::Vec3, getEyePoint,
	          __osg_Vec3__getEyePoint,
	          "Get the eye point in local coordinates. ",
	          "Note, not all NodeVisitor implement this method, it is mainly cull visitors which will implement. ");
	I_Method2(float, getDistanceToEyePoint, IN, const osg::Vec3 &, pos, IN, bool, withLODScale,
	          __float__getDistanceToEyePoint__C5_osg_Vec3_R1__bool,
	          "Get the distance from a point to the eye point, distance value in local coordinate system. ",
	          "This is calculated using the pseudo-EyePoint (above) when doing LOD calculcations. ");
	I_Method1(void, apply, IN, osg::Node &, x,
	          __void__apply__osg_Node_R1,
	          "",
	          "");
	I_Method1(void, apply, IN, osg::Geode &, node,
	          __void__apply__osg_Geode_R1,
	          "",
	          "");
	I_Method1(void, apply, IN, osg::Billboard &, node,
	          __void__apply__osg_Billboard_R1,
	          "",
	          "");
	I_Method1(void, apply, IN, osg::Group &, node,
	          __void__apply__osg_Group_R1,
	          "",
	          "");
	I_Method1(void, apply, IN, osg::Transform &, node,
	          __void__apply__osg_Transform_R1,
	          "",
	          "");
	I_Method1(void, apply, IN, osg::Switch &, node,
	          __void__apply__osg_Switch_R1,
	          "",
	          "");
	I_Method1(void, apply, IN, osg::LOD &, node,
	          __void__apply__osg_LOD_R1,
	          "",
	          "");
	I_SimpleProperty(osg::Vec3, EyePoint, 
	                 __osg_Vec3__getEyePoint, 
	                 __void__setEyePoint__C5_osg_Vec3_R1);
	I_SimpleProperty(osgUtil::IntersectVisitor::LODSelectionMode, LODSelectionMode, 
	                 __LODSelectionMode__getLODSelectionMode, 
	                 __void__setLODSelectionMode__LODSelectionMode);
	I_SimpleProperty(osgUtil::IntersectVisitor::LineSegmentHitListMap &, SegHitList, 
	                 __LineSegmentHitListMap_R1__getSegHitList, 
	                 0);
END_REFLECTOR

BEGIN_OBJECT_REFLECTOR(osgUtil::PickVisitor)
	I_BaseType(osgUtil::IntersectVisitor);
	I_Constructor5(IN, const osg::Viewport *, viewport, IN, const osg::Matrixd &, proj, IN, const osg::Matrixd &, view, IN, float, mx, IN, float, my,
	               ____PickVisitor__C5_osg_Viewport_P1__C5_osg_Matrixd_R1__C5_osg_Matrixd_R1__float__float,
	               "",
	               "");
	I_Method6(void, runNestedPickVisitor, IN, osg::Node &, node, IN, const osg::Viewport *, viewport, IN, const osg::Matrix &, proj, IN, const osg::Matrix &, view, IN, float, mx, IN, float, my,
	          __void__runNestedPickVisitor__osg_Node_R1__C5_osg_Viewport_P1__C5_osg_Matrix_R1__C5_osg_Matrix_R1__float__float,
	          "",
	          "");
	I_Method1(void, apply, IN, osg::Projection &, projection,
	          __void__apply__osg_Projection_R1,
	          "",
	          "");
	I_Method1(void, apply, IN, osg::CameraNode &, camera,
	          __void__apply__osg_CameraNode_R1,
	          "",
	          "");
END_REFLECTOR

STD_MAP_REFLECTOR(std::map< const osg::LineSegment * COMMA  osgUtil::IntersectVisitor::HitList >);

STD_VECTOR_REFLECTOR(std::vector< int >);

STD_VECTOR_REFLECTOR(std::vector< osgUtil::Hit >);

