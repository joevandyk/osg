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

#include <osg/Vec2b>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

#include <osg/io_utils>
	
TYPE_NAME_ALIAS(char, osg::Vec2b::value_type)

BEGIN_VALUE_REFLECTOR(osg::Vec2b)
	I_ReaderWriter(osgIntrospection::StdReaderWriter<reflected_type>);	// user-defined
	I_Comparator(osgIntrospection::PartialOrderComparator<reflected_type>);	// user-defined
	I_DeclaringFile("osg/Vec2b");
	I_Constructor0(____Vec2b,
	               "",
	               "");
	I_Constructor2(IN, char, r, IN, char, g,
	               ____Vec2b__char__char,
	               "",
	               "");
	I_Method0(osg::Vec2b::value_type *, ptr,
	          Properties::NON_VIRTUAL,
	          __value_type_P1__ptr,
	          "",
	          "");
	I_Method0(const osg::Vec2b::value_type *, ptr,
	          Properties::NON_VIRTUAL,
	          __C5_value_type_P1__ptr,
	          "",
	          "");
	I_Method2(void, set, IN, osg::Vec2b::value_type, x, IN, osg::Vec2b::value_type, y,
	          Properties::NON_VIRTUAL,
	          __void__set__value_type__value_type,
	          "",
	          "");
	I_Method1(void, set, IN, const osg::Vec2b &, rhs,
	          Properties::NON_VIRTUAL,
	          __void__set__C5_Vec2b_R1,
	          "",
	          "");
	I_Method0(osg::Vec2b::value_type &, x,
	          Properties::NON_VIRTUAL,
	          __value_type_R1__x,
	          "",
	          "");
	I_Method0(osg::Vec2b::value_type &, y,
	          Properties::NON_VIRTUAL,
	          __value_type_R1__y,
	          "",
	          "");
	I_Method0(osg::Vec2b::value_type, x,
	          Properties::NON_VIRTUAL,
	          __value_type__x,
	          "",
	          "");
	I_Method0(osg::Vec2b::value_type, y,
	          Properties::NON_VIRTUAL,
	          __value_type__y,
	          "",
	          "");
	I_Method0(osg::Vec2b::value_type &, r,
	          Properties::NON_VIRTUAL,
	          __value_type_R1__r,
	          "",
	          "");
	I_Method0(osg::Vec2b::value_type &, g,
	          Properties::NON_VIRTUAL,
	          __value_type_R1__g,
	          "",
	          "");
	I_Method0(osg::Vec2b::value_type, r,
	          Properties::NON_VIRTUAL,
	          __value_type__r,
	          "",
	          "");
	I_Method0(osg::Vec2b::value_type, g,
	          Properties::NON_VIRTUAL,
	          __value_type__g,
	          "",
	          "");
END_REFLECTOR

