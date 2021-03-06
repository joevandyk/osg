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

#include <osg/CopyOp>
#include <osg/Object>
#include <osg/State>
#include <osg/StateAttribute>
#include <osg/StencilTwoSided>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

BEGIN_ENUM_REFLECTOR(osg::StencilTwoSided::Face)
	I_DeclaringFile("osg/StencilTwoSided");
	I_EnumLabel(osg::StencilTwoSided::FRONT);
	I_EnumLabel(osg::StencilTwoSided::BACK);
END_REFLECTOR

BEGIN_ENUM_REFLECTOR(osg::StencilTwoSided::Function)
	I_DeclaringFile("osg/StencilTwoSided");
	I_EnumLabel(osg::StencilTwoSided::NEVER);
	I_EnumLabel(osg::StencilTwoSided::LESS);
	I_EnumLabel(osg::StencilTwoSided::EQUAL);
	I_EnumLabel(osg::StencilTwoSided::LEQUAL);
	I_EnumLabel(osg::StencilTwoSided::GREATER);
	I_EnumLabel(osg::StencilTwoSided::NOTEQUAL);
	I_EnumLabel(osg::StencilTwoSided::GEQUAL);
	I_EnumLabel(osg::StencilTwoSided::ALWAYS);
END_REFLECTOR

BEGIN_ENUM_REFLECTOR(osg::StencilTwoSided::Operation)
	I_DeclaringFile("osg/StencilTwoSided");
	I_EnumLabel(osg::StencilTwoSided::KEEP);
	I_EnumLabel(osg::StencilTwoSided::ZERO);
	I_EnumLabel(osg::StencilTwoSided::REPLACE);
	I_EnumLabel(osg::StencilTwoSided::INCR);
	I_EnumLabel(osg::StencilTwoSided::DECR);
	I_EnumLabel(osg::StencilTwoSided::INVERT);
	I_EnumLabel(osg::StencilTwoSided::INCR_WRAP);
	I_EnumLabel(osg::StencilTwoSided::DECR_WRAP);
END_REFLECTOR

BEGIN_OBJECT_REFLECTOR(osg::StencilTwoSided)
	I_DeclaringFile("osg/StencilTwoSided");
	I_BaseType(osg::StateAttribute);
	I_Constructor0(____StencilTwoSided,
	               "",
	               "");
	I_ConstructorWithDefaults2(IN, const osg::StencilTwoSided &, stencil, , IN, const osg::CopyOp &, copyop, osg::CopyOp::SHALLOW_COPY,
	                           ____StencilTwoSided__C5_StencilTwoSided_R1__C5_CopyOp_R1,
	                           "Copy constructor using CopyOp to manage deep vs shallow copy. ",
	                           "");
	I_Method0(osg::Object *, cloneType,
	          Properties::VIRTUAL,
	          __osg_Object_P1__cloneType,
	          "Clone the type of an attribute, with Object* return type. ",
	          "Must be defined by derived classes. ");
	I_Method1(osg::Object *, clone, IN, const osg::CopyOp &, copyop,
	          Properties::VIRTUAL,
	          __osg_Object_P1__clone__C5_osg_CopyOp_R1,
	          "Clone an attribute, with Object* return type. ",
	          "Must be defined by derived classes. ");
	I_Method1(bool, isSameKindAs, IN, const osg::Object *, obj,
	          Properties::VIRTUAL,
	          __bool__isSameKindAs__C5_osg_Object_P1,
	          "Return true if this and obj are of the same kind of object. ",
	          "");
	I_Method0(const char *, libraryName,
	          Properties::VIRTUAL,
	          __C5_char_P1__libraryName,
	          "Return the name of the attribute's library. ",
	          "");
	I_Method0(const char *, className,
	          Properties::VIRTUAL,
	          __C5_char_P1__className,
	          "Return the name of the attribute's class type. ",
	          "");
	I_Method0(osg::StateAttribute::Type, getType,
	          Properties::VIRTUAL,
	          __Type__getType,
	          "Return the Type identifier of the attribute's class type. ",
	          "");
	I_Method1(int, compare, IN, const osg::StateAttribute &, sa,
	          Properties::VIRTUAL,
	          __int__compare__C5_StateAttribute_R1,
	          "return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs. ",
	          "");
	I_Method1(bool, getModeUsage, IN, osg::StateAttribute::ModeUsage &, usage,
	          Properties::VIRTUAL,
	          __bool__getModeUsage__StateAttribute_ModeUsage_R1,
	          "Return the modes associated with this StateAttribute. ",
	          "");
	I_Method4(void, setFunction, IN, osg::StencilTwoSided::Face, face, IN, osg::StencilTwoSided::Function, func, IN, int, ref, IN, unsigned int, mask,
	          Properties::NON_VIRTUAL,
	          __void__setFunction__Face__Function__int__unsigned_int,
	          "",
	          "");
	I_Method2(void, setFunction, IN, osg::StencilTwoSided::Face, face, IN, osg::StencilTwoSided::Function, func,
	          Properties::NON_VIRTUAL,
	          __void__setFunction__Face__Function,
	          "",
	          "");
	I_Method1(osg::StencilTwoSided::Function, getFunction, IN, osg::StencilTwoSided::Face, face,
	          Properties::NON_VIRTUAL,
	          __Function__getFunction__Face,
	          "",
	          "");
	I_Method2(void, setFunctionRef, IN, osg::StencilTwoSided::Face, face, IN, int, ref,
	          Properties::NON_VIRTUAL,
	          __void__setFunctionRef__Face__int,
	          "",
	          "");
	I_Method1(int, getFunctionRef, IN, osg::StencilTwoSided::Face, face,
	          Properties::NON_VIRTUAL,
	          __int__getFunctionRef__Face,
	          "",
	          "");
	I_Method2(void, setFunctionMask, IN, osg::StencilTwoSided::Face, face, IN, unsigned int, mask,
	          Properties::NON_VIRTUAL,
	          __void__setFunctionMask__Face__unsigned_int,
	          "",
	          "");
	I_Method1(unsigned int, getFunctionMask, IN, osg::StencilTwoSided::Face, face,
	          Properties::NON_VIRTUAL,
	          __unsigned_int__getFunctionMask__Face,
	          "",
	          "");
	I_Method4(void, setOperation, IN, osg::StencilTwoSided::Face, face, IN, osg::StencilTwoSided::Operation, sfail, IN, osg::StencilTwoSided::Operation, zfail, IN, osg::StencilTwoSided::Operation, zpass,
	          Properties::NON_VIRTUAL,
	          __void__setOperation__Face__Operation__Operation__Operation,
	          "set the operations to apply when the various stencil and depth tests fail or pass. ",
	          "First parameter is to control the operation when the stencil test fails. The second parameter is to control the operation when the stencil test passes, but depth test fails. The third parameter controls the operation when both the stencil test and depth pass. Ordering of parameter is the same as if using glStencilOp(,,). ");
	I_Method2(void, setStencilFailOperation, IN, osg::StencilTwoSided::Face, face, IN, osg::StencilTwoSided::Operation, sfail,
	          Properties::NON_VIRTUAL,
	          __void__setStencilFailOperation__Face__Operation,
	          "set the operation when the stencil test fails. ",
	          "");
	I_Method1(osg::StencilTwoSided::Operation, getStencilFailOperation, IN, osg::StencilTwoSided::Face, face,
	          Properties::NON_VIRTUAL,
	          __Operation__getStencilFailOperation__Face,
	          "get the operation when the stencil test fails. ",
	          "");
	I_Method2(void, setStencilPassAndDepthFailOperation, IN, osg::StencilTwoSided::Face, face, IN, osg::StencilTwoSided::Operation, zfail,
	          Properties::NON_VIRTUAL,
	          __void__setStencilPassAndDepthFailOperation__Face__Operation,
	          "set the operation when the stencil test passes but the depth test fails. ",
	          "");
	I_Method1(osg::StencilTwoSided::Operation, getStencilPassAndDepthFailOperation, IN, osg::StencilTwoSided::Face, face,
	          Properties::NON_VIRTUAL,
	          __Operation__getStencilPassAndDepthFailOperation__Face,
	          "get the operation when the stencil test passes but the depth test fails. ",
	          "");
	I_Method2(void, setStencilPassAndDepthPassOperation, IN, osg::StencilTwoSided::Face, face, IN, osg::StencilTwoSided::Operation, zpass,
	          Properties::NON_VIRTUAL,
	          __void__setStencilPassAndDepthPassOperation__Face__Operation,
	          "set the operation when both the stencil test and the depth test pass. ",
	          "");
	I_Method1(osg::StencilTwoSided::Operation, getStencilPassAndDepthPassOperation, IN, osg::StencilTwoSided::Face, face,
	          Properties::NON_VIRTUAL,
	          __Operation__getStencilPassAndDepthPassOperation__Face,
	          "get the operation when both the stencil test and the depth test pass. ",
	          "");
	I_Method2(void, setWriteMask, IN, osg::StencilTwoSided::Face, face, IN, unsigned int, mask,
	          Properties::NON_VIRTUAL,
	          __void__setWriteMask__Face__unsigned_int,
	          "",
	          "");
	I_Method1(unsigned int, getWriteMask, IN, osg::StencilTwoSided::Face, face,
	          Properties::NON_VIRTUAL,
	          __unsigned_int__getWriteMask__Face,
	          "",
	          "");
	I_Method1(void, apply, IN, osg::State &, state,
	          Properties::VIRTUAL,
	          __void__apply__State_R1,
	          "apply the OpenGL state attributes. ",
	          "The render info for the current OpenGL context is passed in to allow the StateAttribute to obtain details on the the current context and state. ");
	I_StaticMethod2(osg::StencilTwoSided::Extensions *, getExtensions, IN, unsigned int, contextID, IN, bool, createIfNotInitalized,
	                __Extensions_P1__getExtensions__unsigned_int__bool_S,
	                "Function to call to get the extension of a specified context. ",
	                "If the Extension object for that context has not yet been created and the 'createIfNotInitalized' flag been set to false then returns NULL. If 'createIfNotInitalized' is true then the Extensions object is automatically created. However, in this case the extension object will only be created with the graphics context associated with ContextID. ");
	I_StaticMethod2(void, setExtensions, IN, unsigned int, contextID, IN, osg::StencilTwoSided::Extensions *, extensions,
	                __void__setExtensions__unsigned_int__Extensions_P1_S,
	                "The setExtensions method allows users to override the extensions across graphics contexts. ",
	                "Typically used when you have different extensions supported across graphics pipes but need to ensure that they all use the same low common denominator extensions. ");
	I_IndexedProperty(osg::StencilTwoSided::Function, Function, 
	                  __Function__getFunction__Face, 
	                  __void__setFunction__Face__Function, 
	                  0);
	I_IndexedProperty(unsigned int, FunctionMask, 
	                  __unsigned_int__getFunctionMask__Face, 
	                  __void__setFunctionMask__Face__unsigned_int, 
	                  0);
	I_IndexedProperty(int, FunctionRef, 
	                  __int__getFunctionRef__Face, 
	                  __void__setFunctionRef__Face__int, 
	                  0);
	I_IndexedProperty(osg::StencilTwoSided::Operation, StencilFailOperation, 
	                  __Operation__getStencilFailOperation__Face, 
	                  __void__setStencilFailOperation__Face__Operation, 
	                  0);
	I_IndexedProperty(osg::StencilTwoSided::Operation, StencilPassAndDepthFailOperation, 
	                  __Operation__getStencilPassAndDepthFailOperation__Face, 
	                  __void__setStencilPassAndDepthFailOperation__Face__Operation, 
	                  0);
	I_IndexedProperty(osg::StencilTwoSided::Operation, StencilPassAndDepthPassOperation, 
	                  __Operation__getStencilPassAndDepthPassOperation__Face, 
	                  __void__setStencilPassAndDepthPassOperation__Face__Operation, 
	                  0);
	I_SimpleProperty(osg::StateAttribute::Type, Type, 
	                 __Type__getType, 
	                 0);
	I_IndexedProperty(unsigned int, WriteMask, 
	                  __unsigned_int__getWriteMask__Face, 
	                  __void__setWriteMask__Face__unsigned_int, 
	                  0);
END_REFLECTOR

BEGIN_OBJECT_REFLECTOR(osg::StencilTwoSided::Extensions)
	I_DeclaringFile("osg/StencilTwoSided");
	I_BaseType(osg::Referenced);
	I_Constructor1(IN, unsigned int, contextID,
	               Properties::NON_EXPLICIT,
	               ____Extensions__unsigned_int,
	               "",
	               "");
	I_Constructor1(IN, const osg::StencilTwoSided::Extensions &, rhs,
	               Properties::NON_EXPLICIT,
	               ____Extensions__C5_Extensions_R1,
	               "",
	               "");
	I_Method1(void, lowestCommonDenominator, IN, const osg::StencilTwoSided::Extensions &, rhs,
	          Properties::NON_VIRTUAL,
	          __void__lowestCommonDenominator__C5_Extensions_R1,
	          "",
	          "");
	I_Method1(void, setupGLExtensions, IN, unsigned int, contextID,
	          Properties::NON_VIRTUAL,
	          __void__setupGLExtensions__unsigned_int,
	          "",
	          "");
	I_Method1(void, setStencilTwoSidedSupported, IN, bool, flag,
	          Properties::NON_VIRTUAL,
	          __void__setStencilTwoSidedSupported__bool,
	          "",
	          "");
	I_Method0(bool, isStencilTwoSidedSupported,
	          Properties::NON_VIRTUAL,
	          __bool__isStencilTwoSidedSupported,
	          "",
	          "");
	I_Method1(void, glActiveStencilFace, IN, GLenum, face,
	          Properties::NON_VIRTUAL,
	          __void__glActiveStencilFace__GLenum,
	          "",
	          "");
	I_SimpleProperty(bool, StencilTwoSidedSupported, 
	                 0, 
	                 __void__setStencilTwoSidedSupported__bool);
END_REFLECTOR

