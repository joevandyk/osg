// ***************************************************************************
//
//   Generated automatically by genwrapper.
//   Please DO NOT EDIT this file!
//
// ***************************************************************************

#include <osgIntrospection/ReflectionMacros>
#include <osgIntrospection/TypedMethodInfo>
#include <osgIntrospection/Attributes>

#include <osg/Vec3f>
#include <osg/Vec4f>

TYPE_NAME_ALIAS(float, osg::Vec4f::value_type);

BEGIN_VALUE_REFLECTOR(osg::Vec4f)
	ReaderWriter(osgIntrospection::StdReaderWriter<reflected_type>);	// user-defined
	Comparator(osgIntrospection::PartialOrderComparator<reflected_type>);	// user-defined
	Constructor0();
	Constructor4(IN, osg::Vec4f::value_type, x, IN, osg::Vec4f::value_type, y, IN, osg::Vec4f::value_type, z, IN, osg::Vec4f::value_type, w);
	Constructor2(IN, const osg::Vec3f &, v3, IN, osg::Vec4f::value_type, w);
	Method0(osg::Vec4f::value_type *, ptr);
	Method0(const osg::Vec4f::value_type *, ptr);
	Method4(void, set, IN, osg::Vec4f::value_type, x, IN, osg::Vec4f::value_type, y, IN, osg::Vec4f::value_type, z, IN, osg::Vec4f::value_type, w);
	Method0(osg::Vec4f::value_type &, x);
	Method0(osg::Vec4f::value_type &, y);
	Method0(osg::Vec4f::value_type &, z);
	Method0(osg::Vec4f::value_type &, w);
	Method0(osg::Vec4f::value_type, x);
	Method0(osg::Vec4f::value_type, y);
	Method0(osg::Vec4f::value_type, z);
	Method0(osg::Vec4f::value_type, w);
	Method0(osg::Vec4f::value_type &, red);
	Method0(osg::Vec4f::value_type &, green);
	Method0(osg::Vec4f::value_type &, blue);
	Method0(osg::Vec4f::value_type &, alpha);
	Method0(osg::Vec4f::value_type, red);
	Method0(osg::Vec4f::value_type, green);
	Method0(osg::Vec4f::value_type, blue);
	Method0(osg::Vec4f::value_type, alpha);
	Method0(unsigned long, asABGR);
	Method0(unsigned long, asRGBA);
	Method0(bool, valid);
	Method0(bool, isNaN);
	Method0(osg::Vec4f::value_type, length);
	Method0(osg::Vec4f::value_type, length2);
	Method0(osg::Vec4f::value_type, normalize);
END_REFLECTOR

