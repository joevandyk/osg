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

#include <osg/BoundingSphere>
#include <osg/CopyOp>
#include <osg/LOD>
#include <osg/Node>
#include <osg/NodeVisitor>
#include <osg/Object>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

TYPE_NAME_ALIAS(osg::BoundingSphere::vec_type, osg::LOD::vec_type)

TYPE_NAME_ALIAS(osg::BoundingSphere::value_type, osg::LOD::value_type)

TYPE_NAME_ALIAS(std::pair< float COMMA  float >, osg::LOD::MinMaxPair)

TYPE_NAME_ALIAS(std::vector< osg::LOD::MinMaxPair >, osg::LOD::RangeList)

BEGIN_ENUM_REFLECTOR(osg::LOD::CenterMode)
	I_DeclaringFile("osg/LOD");
	I_EnumLabel(osg::LOD::USE_BOUNDING_SPHERE_CENTER);
	I_EnumLabel(osg::LOD::USER_DEFINED_CENTER);
END_REFLECTOR

BEGIN_ENUM_REFLECTOR(osg::LOD::RangeMode)
	I_DeclaringFile("osg/LOD");
	I_EnumLabel(osg::LOD::DISTANCE_FROM_EYE_POINT);
	I_EnumLabel(osg::LOD::PIXEL_SIZE_ON_SCREEN);
END_REFLECTOR

BEGIN_OBJECT_REFLECTOR(osg::LOD)
	I_DeclaringFile("osg/LOD");
	I_BaseType(osg::Group);
	I_Constructor0(____LOD,
	               "",
	               "");
	I_ConstructorWithDefaults2(IN, const osg::LOD &, x, , IN, const osg::CopyOp &, copyop, osg::CopyOp::SHALLOW_COPY,
	                           ____LOD__C5_LOD_R1__C5_CopyOp_R1,
	                           "Copy constructor using CopyOp to manage deep vs shallow copy. ",
	                           "");
	I_Method0(osg::Object *, cloneType,
	          Properties::VIRTUAL,
	          __osg_Object_P1__cloneType,
	          "clone an object of the same type as the node. ",
	          "");
	I_Method1(osg::Object *, clone, IN, const osg::CopyOp &, copyop,
	          Properties::VIRTUAL,
	          __osg_Object_P1__clone__C5_osg_CopyOp_R1,
	          "return a clone of a node, with Object* return type. ",
	          "");
	I_Method1(bool, isSameKindAs, IN, const osg::Object *, obj,
	          Properties::VIRTUAL,
	          __bool__isSameKindAs__C5_osg_Object_P1,
	          "return true if this and obj are of the same kind of object. ",
	          "");
	I_Method0(const char *, className,
	          Properties::VIRTUAL,
	          __C5_char_P1__className,
	          "return the name of the node's class type. ",
	          "");
	I_Method0(const char *, libraryName,
	          Properties::VIRTUAL,
	          __C5_char_P1__libraryName,
	          "return the name of the node's library. ",
	          "");
	I_Method1(void, accept, IN, osg::NodeVisitor &, nv,
	          Properties::VIRTUAL,
	          __void__accept__osg_NodeVisitor_R1,
	          "Visitor Pattern : calls the apply method of a NodeVisitor with this node's type. ",
	          "");
	I_Method1(void, traverse, IN, osg::NodeVisitor &, nv,
	          Properties::VIRTUAL,
	          __void__traverse__NodeVisitor_R1,
	          "Traverse downwards : calls children's accept method with NodeVisitor. ",
	          "");
	I_Method1(bool, addChild, IN, osg::Node *, child,
	          Properties::VIRTUAL,
	          __bool__addChild__Node_P1,
	          "Add Node to Group. ",
	          "If node is not NULL and is not contained in Group then increment its reference count, add it to the child list and dirty the bounding sphere to force it to recompute on next getBound() and return true for success. Otherwise return false. Scene nodes can't be added as child nodes. ");
	I_Method3(bool, addChild, IN, osg::Node *, child, IN, float, min, IN, float, max,
	          Properties::VIRTUAL,
	          __bool__addChild__Node_P1__float__float,
	          "",
	          "");
	I_MethodWithDefaults2(bool, removeChildren, IN, unsigned int, pos, , IN, unsigned int, numChildrenToRemove, 1,
	                      Properties::VIRTUAL,
	                      __bool__removeChildren__unsigned_int__unsigned_int,
	                      "Remove children from Group. ",
	                      "Note, must be override by subclasses of Group which add per child attributes. ");
	I_Method1(void, setCenterMode, IN, osg::LOD::CenterMode, mode,
	          Properties::NON_VIRTUAL,
	          __void__setCenterMode__CenterMode,
	          "Set how the center of object should be determined when computing which child is active. ",
	          "");
	I_Method0(osg::LOD::CenterMode, getCenterMode,
	          Properties::NON_VIRTUAL,
	          __CenterMode__getCenterMode,
	          "Get how the center of object should be determined when computing which child is active. ",
	          "");
	I_Method1(void, setCenter, IN, const osg::LOD::vec_type &, center,
	          Properties::NON_VIRTUAL,
	          __void__setCenter__C5_vec_type_R1,
	          "Sets the object-space point which defines the center of the osg::LOD. ",
	          "center is affected by any transforms in the hierarchy above the osg::LOD. ");
	I_Method0(const osg::LOD::vec_type &, getCenter,
	          Properties::NON_VIRTUAL,
	          __C5_vec_type_R1__getCenter,
	          "return the LOD center point. ",
	          "");
	I_Method1(void, setRadius, IN, osg::LOD::value_type, radius,
	          Properties::NON_VIRTUAL,
	          __void__setRadius__value_type,
	          "Set the object-space reference radius of the volume enclosed by the LOD. ",
	          "Used to determine the bounding sphere of the LOD in the absence of any children. ");
	I_Method0(osg::LOD::value_type, getRadius,
	          Properties::NON_VIRTUAL,
	          __value_type__getRadius,
	          "Get the object-space radius of the volume enclosed by the LOD. ",
	          "");
	I_Method1(void, setRangeMode, IN, osg::LOD::RangeMode, mode,
	          Properties::NON_VIRTUAL,
	          __void__setRangeMode__RangeMode,
	          "Set how the range values should be interpreted when computing which child is active. ",
	          "");
	I_Method0(osg::LOD::RangeMode, getRangeMode,
	          Properties::NON_VIRTUAL,
	          __RangeMode__getRangeMode,
	          "Get how the range values should be interpreted when computing which child is active. ",
	          "");
	I_Method3(void, setRange, IN, unsigned int, childNo, IN, float, min, IN, float, max,
	          Properties::NON_VIRTUAL,
	          __void__setRange__unsigned_int__float__float,
	          "Sets the min and max visible ranges of range of specific child. ",
	          "Values are floating point distance specified in local objects coordinates. ");
	I_Method1(float, getMinRange, IN, unsigned int, childNo,
	          Properties::NON_VIRTUAL,
	          __float__getMinRange__unsigned_int,
	          "returns the min visible range for specified child. ",
	          "");
	I_Method1(float, getMaxRange, IN, unsigned int, childNo,
	          Properties::NON_VIRTUAL,
	          __float__getMaxRange__unsigned_int,
	          "returns the max visible range for specified child. ",
	          "");
	I_Method0(unsigned int, getNumRanges,
	          Properties::NON_VIRTUAL,
	          __unsigned_int__getNumRanges,
	          "returns the number of ranges currently set. ",
	          "An LOD which has been fully set up will have getNumChildren()==getNumRanges(). ");
	I_Method1(void, setRangeList, IN, const osg::LOD::RangeList &, rangeList,
	          Properties::NON_VIRTUAL,
	          __void__setRangeList__C5_RangeList_R1,
	          "set the list of MinMax ranges for each child. ",
	          "");
	I_Method0(const osg::LOD::RangeList &, getRangeList,
	          Properties::NON_VIRTUAL,
	          __C5_RangeList_R1__getRangeList,
	          "return the list of MinMax ranges for each child. ",
	          "");
	I_Method0(osg::BoundingSphere, computeBound,
	          Properties::VIRTUAL,
	          __BoundingSphere__computeBound,
	          "Compute the bounding sphere around Node's geometry or children. ",
	          "This method is automatically called by getBound() when the bounding sphere has been marked dirty via dirtyBound(). ");
	I_SimpleProperty(const osg::LOD::vec_type &, Center, 
	                 __C5_vec_type_R1__getCenter, 
	                 __void__setCenter__C5_vec_type_R1);
	I_SimpleProperty(osg::LOD::CenterMode, CenterMode, 
	                 __CenterMode__getCenterMode, 
	                 __void__setCenterMode__CenterMode);
	I_SimpleProperty(osg::LOD::value_type, Radius, 
	                 __value_type__getRadius, 
	                 __void__setRadius__value_type);
	I_SimpleProperty(const osg::LOD::RangeList &, RangeList, 
	                 __C5_RangeList_R1__getRangeList, 
	                 __void__setRangeList__C5_RangeList_R1);
	I_SimpleProperty(osg::LOD::RangeMode, RangeMode, 
	                 __RangeMode__getRangeMode, 
	                 __void__setRangeMode__RangeMode);
END_REFLECTOR

STD_PAIR_REFLECTOR(std::pair< float COMMA  float >)

STD_VECTOR_REFLECTOR(std::vector< osg::LOD::MinMaxPair >)

