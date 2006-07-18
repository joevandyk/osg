// ***************************************************************************
//
//   Generated automatically by genwrapper.
//   Please DO NOT EDIT this file!
//
// ***************************************************************************

#include <osgIntrospection/ReflectionMacros>
#include <osgIntrospection/TypedMethodInfo>
#include <osgIntrospection/Attributes>

#include <osg/Array>
#include <osg/Matrixd>
#include <osg/Node>
#include <osg/NodeVisitor>
#include <osg/Object>
#include <osg/test/CopyOp>
#include <osg/test/Drawable>
#include <osg/test/Vec3>
#include <osg/test/Vec4>
#include <osgSim/SphereSegment>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

TYPE_NAME_ALIAS(std::vector< osg::ref_ptr< osg::Vec3Array > >, osgSim::SphereSegment::LineList);

BEGIN_ENUM_REFLECTOR(osgSim::SphereSegment::DrawMask)
	I_EnumLabel(osgSim::SphereSegment::SURFACE);
	I_EnumLabel(osgSim::SphereSegment::SPOKES);
	I_EnumLabel(osgSim::SphereSegment::EDGELINE);
	I_EnumLabel(osgSim::SphereSegment::SIDES);
	I_EnumLabel(osgSim::SphereSegment::ALL);
END_REFLECTOR

BEGIN_OBJECT_REFLECTOR(osgSim::SphereSegment)
	I_BaseType(osg::Geode);
	I_Constructor0();
	I_Constructor7(IN, const osg::Vec3 &, centre, IN, float, radius, IN, float, azMin, IN, float, azMax, IN, float, elevMin, IN, float, elevMax, IN, int, density);
	I_Constructor6(IN, const osg::Vec3 &, centre, IN, float, radius, IN, const osg::Vec3 &, vec, IN, float, azRange, IN, float, elevRange, IN, int, density);
	I_Constructor2(IN, const osgSim::SphereSegment &, rhs, IN, const osg::CopyOp &, co);
	I_Method1(void, setCentre, IN, const osg::Vec3 &, c);
	I_Method0(const osg::Vec3 &, getCentre);
	I_Method1(void, setRadius, IN, float, r);
	I_Method0(float, getRadius);
	I_Method3(void, setArea, IN, const osg::Vec3 &, vec, IN, float, azRange, IN, float, elevRange);
	I_Method3(void, getArea, IN, osg::Vec3 &, vec, IN, float &, azRange, IN, float &, elevRange);
	I_Method4(void, setArea, IN, float, azMin, IN, float, azMax, IN, float, elevMin, IN, float, elevMax);
	I_Method4(void, getArea, IN, float &, azMin, IN, float &, azMax, IN, float &, elevMin, IN, float &, elevMax);
	I_Method1(void, setDensity, IN, int, d);
	I_Method0(int, getDensity);
	I_Method1(void, setDrawMask, IN, osgSim::SphereSegment::DrawMask, dm);
	I_Method0(osgSim::SphereSegment::DrawMask, getDrawMask);
	I_Method1(void, setSurfaceColor, IN, const osg::Vec4 &, c);
	I_Method0(osg::Vec4, getSurfaceColor);
	I_Method1(void, setSpokeColor, IN, const osg::Vec4 &, c);
	I_Method0(osg::Vec4, getSpokeColor);
	I_Method1(void, setEdgeLineColor, IN, const osg::Vec4 &, c);
	I_Method0(osg::Vec4, getEdgeLineColor);
	I_Method1(void, setSideColor, IN, const osg::Vec4 &, c);
	I_Method0(osg::Vec4, getSideColor);
	I_Method1(void, setAllColors, IN, const osg::Vec4 &, c);
	I_Method0(osg::Object *, cloneType);
	I_Method1(osg::Object *, clone, IN, const osg::CopyOp &, copyop);
	I_Method1(bool, isSameKindAs, IN, const osg::Object *, obj);
	I_Method0(const char *, className);
	I_Method0(const char *, libraryName);
	I_Method1(void, accept, IN, osg::NodeVisitor &, nv);
	I_Method2(osgSim::SphereSegment::LineList, computeIntersection, IN, const osg::Matrixd &, matrix, IN, osg::Node *, subgraph);
	I_Method2(osgSim::SphereSegment::LineList, computeIntersection, IN, const osg::Matrixd &, matrix, IN, osg::Drawable *, drawable);
	I_Method2(osg::Node *, computeIntersectionSubgraph, IN, const osg::Matrixd &, matrix, IN, osg::Node *, subgraph);
	I_Method2(osg::Node *, computeIntersectionSubgraph, IN, const osg::Matrixd &, matrix, IN, osg::Drawable *, drawable);
	I_WriteOnlyProperty(const osg::Vec4 &, AllColors);
	I_Property(const osg::Vec3 &, Centre);
	I_Property(int, Density);
	I_Property(osgSim::SphereSegment::DrawMask, DrawMask);
	I_ReadOnlyProperty(osg::Vec4, EdgeLineColor);
	I_Property(float, Radius);
	I_ReadOnlyProperty(osg::Vec4, SideColor);
	I_ReadOnlyProperty(osg::Vec4, SpokeColor);
	I_ReadOnlyProperty(osg::Vec4, SurfaceColor);
END_REFLECTOR

BEGIN_VALUE_REFLECTOR(osg::ref_ptr< osg::Vec3Array >)
	I_Constructor0();
	I_Constructor1(IN, osg::Vec3Array *, ptr);
	I_Constructor1(IN, const osg::ref_ptr< osg::Vec3Array > &, rp);
	I_Method0(osg::Vec3Array *, get);
	I_Method0(bool, valid);
	I_Method0(osg::Vec3Array *, release);
	I_Method1(void, swap, IN, osg::ref_ptr< osg::Vec3Array > &, rp);
	I_Constructor0();
	I_Constructor1(IN, osg::Vec3Array *, ptr);
	I_Constructor1(IN, const osg::ref_ptr< osg::Vec3Array > &, rp);
	I_Method0(osg::Vec3Array *, get);
	I_Method0(bool, valid);
	I_Method0(osg::Vec3Array *, release);
	I_Method1(void, swap, IN, osg::ref_ptr< osg::Vec3Array > &, rp);
	I_ReadOnlyProperty(osg::Vec3Array *, );
END_REFLECTOR

STD_VECTOR_REFLECTOR(std::vector< osg::ref_ptr< osg::Vec3Array > >);

