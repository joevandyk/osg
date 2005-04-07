// ***************************************************************************
//
//   Generated automatically by genwrapper.
//   Please DO NOT EDIT this file!
//
// ***************************************************************************

#include <osgIntrospection/ReflectionMacros>
#include <osgIntrospection/TypedMethodInfo>
#include <osgIntrospection/Attributes>

#include <osg/BoundingBox>
#include <osg/BoundingSphere>
#include <osg/Matrix>
#include <osg/Plane>
#include <osg/Polytope>
#include <osg/Vec3>

TYPE_NAME_ALIAS(unsigned int, osg::Polytope::ClippingMask);

TYPE_NAME_ALIAS(std::vector< osg::Plane >, osg::Polytope::PlaneList);

TYPE_NAME_ALIAS(std::vector< osg::Vec3 >, osg::Polytope::VertexList);

TYPE_NAME_ALIAS(osg::fast_back_stack< osg::Polytope::ClippingMask >, osg::Polytope::MaskStack);

BEGIN_VALUE_REFLECTOR(osg::Polytope)
	Constructor0();
	Constructor1(IN, const osg::Polytope &, cv);
	Constructor1(IN, const osg::Polytope::PlaneList &, pl);
	Method0(void, clear);
	MethodWithDefaults2(void, setToUnitFrustum, IN, bool, withNear, true, IN, bool, withFar, true);
	Method2(void, setAndTransformProvidingInverse, IN, const osg::Polytope &, pt, IN, const osg::Matrix &, matrix);
	Method1(void, set, IN, const osg::Polytope::PlaneList &, pl);
	Method1(void, add, IN, const osg::Plane &, pl);
	Method0(void, flip);
	Method0(osg::Polytope::PlaneList &, getPlaneList);
	Method0(const osg::Polytope::PlaneList &, getPlaneList);
	Method1(void, setReferenceVertexList, IN, osg::Polytope::VertexList &, vertices);
	Method0(osg::Polytope::VertexList &, getReferenceVertexList);
	Method0(const osg::Polytope::VertexList &, getReferenceVertexList);
	Method0(void, setupMask);
	Method0(osg::Polytope::ClippingMask &, getCurrentMask);
	Method0(osg::Polytope::ClippingMask, getCurrentMask);
	Method1(void, setResultMask, IN, osg::Polytope::ClippingMask, mask);
	Method0(osg::Polytope::ClippingMask, getResultMask);
	Method0(osg::Polytope::MaskStack &, getMaskStack);
	Method0(const osg::Polytope::MaskStack &, getMaskStack);
	Method0(void, pushCurrentMask);
	Method0(void, popCurrentMask);
	Method1(bool, contains, IN, const osg::Vec3 &, v);
	Method1(bool, contains, IN, const std::vector< osg::Vec3 > &, vertices);
	Method1(bool, contains, IN, const osg::BoundingSphere &, bs);
	Method1(bool, contains, IN, const osg::BoundingBox &, bb);
	Method1(bool, containsAllOf, IN, const std::vector< osg::Vec3 > &, vertices);
	Method1(bool, containsAllOf, IN, const osg::BoundingSphere &, bs);
	Method1(bool, containsAllOf, IN, const osg::BoundingBox &, bb);
	Method1(void, transform, IN, const osg::Matrix &, matrix);
	Method1(void, transformProvidingInverse, IN, const osg::Matrix &, matrix);
	WriteOnlyProperty(const osg::Polytope::PlaneList &, );
	ReadOnlyProperty(osg::Polytope::ClippingMask, CurrentMask);
	ReadOnlyProperty(osg::Polytope::MaskStack &, MaskStack);
	ReadOnlyProperty(osg::Polytope::PlaneList &, PlaneList);
	Property(osg::Polytope::VertexList &, ReferenceVertexList);
	Property(osg::Polytope::ClippingMask, ResultMask);
END_REFLECTOR

BEGIN_VALUE_REFLECTOR(osg::fast_back_stack< osg::Polytope::ClippingMask >)
	Constructor0();
	Constructor1(IN, const osg::fast_back_stack< osg::Polytope::ClippingMask > &, fbs);
	Constructor1(IN, const osg::Polytope::ClippingMask &, value);
	Method0(void, clear);
	Method0(bool, empty);
	Method0(unsigned int, size);
	Method0(osg::Polytope::ClippingMask &, back);
	Method0(const osg::Polytope::ClippingMask &, back);
	Method0(void, push_back);
	Method1(void, push_back, IN, const osg::Polytope::ClippingMask &, value);
	Method0(void, pop_back);
END_REFLECTOR

STD_VECTOR_REFLECTOR(std::vector< osg::Plane >);

