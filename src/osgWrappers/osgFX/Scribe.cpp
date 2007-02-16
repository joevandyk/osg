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

#include <osg/.svn/text-base/CopyOp.svn-base>
#include <osg/.svn/text-base/NodeVisitor.svn-base>
#include <osg/.svn/text-base/Object.svn-base>
#include <osg/.svn/text-base/Vec4.svn-base>
#include <osgFX/Scribe>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

BEGIN_OBJECT_REFLECTOR(osgFX::Scribe)
	I_BaseType(osgFX::Effect);
	I_BaseType(osgFX::Effect);
	I_Constructor0(____Scribe,
	               "",
	               "");
	I_ConstructorWithDefaults2(IN, const osgFX::Scribe &, copy, , IN, const osg::CopyOp &, copyop, osg::CopyOp::SHALLOW_COPY,
	                           ____Scribe__C5_Scribe_R1__C5_osg_CopyOp_R1,
	                           "",
	                           "");
	I_Method0(osg::Object *, cloneType,
	          Properties::VIRTUAL,
	          __osg_Object_P1__cloneType,
	          "",
	          "");
	I_Method1(osg::Object *, clone, IN, const osg::CopyOp &, copyop,
	          Properties::VIRTUAL,
	          __osg_Object_P1__clone__C5_osg_CopyOp_R1,
	          "",
	          "");
	I_Method1(bool, isSameKindAs, IN, const osg::Object *, obj,
	          Properties::VIRTUAL,
	          __bool__isSameKindAs__C5_osg_Object_P1,
	          "",
	          "");
	I_Method0(const char *, className,
	          Properties::VIRTUAL,
	          __C5_char_P1__className,
	          "",
	          "");
	I_Method0(const char *, libraryName,
	          Properties::VIRTUAL,
	          __C5_char_P1__libraryName,
	          "",
	          "");
	I_Method1(void, accept, IN, osg::NodeVisitor &, nv,
	          Properties::VIRTUAL,
	          __void__accept__osg_NodeVisitor_R1,
	          "",
	          "");
	I_Method0(const char *, effectName,
	          Properties::VIRTUAL,
	          __C5_char_P1__effectName,
	          "",
	          "");
	I_Method0(const char *, effectDescription,
	          Properties::VIRTUAL,
	          __C5_char_P1__effectDescription,
	          "",
	          "");
	I_Method0(const char *, effectAuthor,
	          Properties::VIRTUAL,
	          __C5_char_P1__effectAuthor,
	          "",
	          "");
	I_Method0(const osg::Vec4 &, getWireframeColor,
	          Properties::NON_VIRTUAL,
	          __C5_osg_Vec4_R1__getWireframeColor,
	          "get the wireframe color ",
	          "");
	I_Method1(void, setWireframeColor, IN, const osg::Vec4 &, color,
	          Properties::NON_VIRTUAL,
	          __void__setWireframeColor__C5_osg_Vec4_R1,
	          "set the wireframe color ",
	          "");
	I_Method0(float, getWireframeLineWidth,
	          Properties::NON_VIRTUAL,
	          __float__getWireframeLineWidth,
	          "get the wireframe line width ",
	          "");
	I_Method1(void, setWireframeLineWidth, IN, float, w,
	          Properties::NON_VIRTUAL,
	          __void__setWireframeLineWidth__float,
	          "set the wireframe line width ",
	          "");
	I_Constructor0(____Scribe,
	               "",
	               "");
	I_ConstructorWithDefaults2(IN, const osgFX::Scribe &, copy, , IN, const osg::CopyOp &, copyop, osg::CopyOp::SHALLOW_COPY,
	                           ____Scribe__C5_Scribe_R1__C5_osg_CopyOp_R1,
	                           "",
	                           "");
	I_Method0(osg::Object *, cloneType,
	          Properties::VIRTUAL,
	          __osg_Object_P1__cloneType,
	          "",
	          "");
	I_Method1(osg::Object *, clone, IN, const osg::CopyOp &, copyop,
	          Properties::VIRTUAL,
	          __osg_Object_P1__clone__C5_osg_CopyOp_R1,
	          "",
	          "");
	I_Method1(bool, isSameKindAs, IN, const osg::Object *, obj,
	          Properties::VIRTUAL,
	          __bool__isSameKindAs__C5_osg_Object_P1,
	          "",
	          "");
	I_Method0(const char *, className,
	          Properties::VIRTUAL,
	          __C5_char_P1__className,
	          "",
	          "");
	I_Method0(const char *, libraryName,
	          Properties::VIRTUAL,
	          __C5_char_P1__libraryName,
	          "",
	          "");
	I_Method1(void, accept, IN, osg::NodeVisitor &, nv,
	          Properties::VIRTUAL,
	          __void__accept__osg_NodeVisitor_R1,
	          "",
	          "");
	I_Method0(const char *, effectName,
	          Properties::VIRTUAL,
	          __C5_char_P1__effectName,
	          "",
	          "");
	I_Method0(const char *, effectDescription,
	          Properties::VIRTUAL,
	          __C5_char_P1__effectDescription,
	          "",
	          "");
	I_Method0(const char *, effectAuthor,
	          Properties::VIRTUAL,
	          __C5_char_P1__effectAuthor,
	          "",
	          "");
	I_Method0(const osg::Vec4 &, getWireframeColor,
	          Properties::NON_VIRTUAL,
	          __C5_osg_Vec4_R1__getWireframeColor,
	          "get the wireframe color ",
	          "");
	I_Method1(void, setWireframeColor, IN, const osg::Vec4 &, color,
	          Properties::NON_VIRTUAL,
	          __void__setWireframeColor__C5_osg_Vec4_R1,
	          "set the wireframe color ",
	          "");
	I_Method0(float, getWireframeLineWidth,
	          Properties::NON_VIRTUAL,
	          __float__getWireframeLineWidth,
	          "get the wireframe line width ",
	          "");
	I_Method1(void, setWireframeLineWidth, IN, float, w,
	          Properties::NON_VIRTUAL,
	          __void__setWireframeLineWidth__float,
	          "set the wireframe line width ",
	          "");
	I_SimpleProperty(const osg::Vec4 &, WireframeColor, 
	                 __C5_osg_Vec4_R1__getWireframeColor, 
	                 __void__setWireframeColor__C5_osg_Vec4_R1);
	I_SimpleProperty(float, WireframeLineWidth, 
	                 __float__getWireframeLineWidth, 
	                 __void__setWireframeLineWidth__float);
END_REFLECTOR

BEGIN_OBJECT_REFLECTOR(osgFX::Scribe)
	I_BaseType(osgFX::Effect);
	I_BaseType(osgFX::Effect);
	I_Constructor0(____Scribe,
	               "",
	               "");
	I_ConstructorWithDefaults2(IN, const osgFX::Scribe &, copy, , IN, const osg::CopyOp &, copyop, osg::CopyOp::SHALLOW_COPY,
	                           ____Scribe__C5_Scribe_R1__C5_osg_CopyOp_R1,
	                           "",
	                           "");
	I_Method0(osg::Object *, cloneType,
	          Properties::VIRTUAL,
	          __osg_Object_P1__cloneType,
	          "",
	          "");
	I_Method1(osg::Object *, clone, IN, const osg::CopyOp &, copyop,
	          Properties::VIRTUAL,
	          __osg_Object_P1__clone__C5_osg_CopyOp_R1,
	          "",
	          "");
	I_Method1(bool, isSameKindAs, IN, const osg::Object *, obj,
	          Properties::VIRTUAL,
	          __bool__isSameKindAs__C5_osg_Object_P1,
	          "",
	          "");
	I_Method0(const char *, className,
	          Properties::VIRTUAL,
	          __C5_char_P1__className,
	          "",
	          "");
	I_Method0(const char *, libraryName,
	          Properties::VIRTUAL,
	          __C5_char_P1__libraryName,
	          "",
	          "");
	I_Method1(void, accept, IN, osg::NodeVisitor &, nv,
	          Properties::VIRTUAL,
	          __void__accept__osg_NodeVisitor_R1,
	          "",
	          "");
	I_Method0(const char *, effectName,
	          Properties::VIRTUAL,
	          __C5_char_P1__effectName,
	          "",
	          "");
	I_Method0(const char *, effectDescription,
	          Properties::VIRTUAL,
	          __C5_char_P1__effectDescription,
	          "",
	          "");
	I_Method0(const char *, effectAuthor,
	          Properties::VIRTUAL,
	          __C5_char_P1__effectAuthor,
	          "",
	          "");
	I_Method0(const osg::Vec4 &, getWireframeColor,
	          Properties::NON_VIRTUAL,
	          __C5_osg_Vec4_R1__getWireframeColor,
	          "get the wireframe color ",
	          "");
	I_Method1(void, setWireframeColor, IN, const osg::Vec4 &, color,
	          Properties::NON_VIRTUAL,
	          __void__setWireframeColor__C5_osg_Vec4_R1,
	          "set the wireframe color ",
	          "");
	I_Method0(float, getWireframeLineWidth,
	          Properties::NON_VIRTUAL,
	          __float__getWireframeLineWidth,
	          "get the wireframe line width ",
	          "");
	I_Method1(void, setWireframeLineWidth, IN, float, w,
	          Properties::NON_VIRTUAL,
	          __void__setWireframeLineWidth__float,
	          "set the wireframe line width ",
	          "");
	I_Constructor0(____Scribe,
	               "",
	               "");
	I_ConstructorWithDefaults2(IN, const osgFX::Scribe &, copy, , IN, const osg::CopyOp &, copyop, osg::CopyOp::SHALLOW_COPY,
	                           ____Scribe__C5_Scribe_R1__C5_osg_CopyOp_R1,
	                           "",
	                           "");
	I_Method0(osg::Object *, cloneType,
	          Properties::VIRTUAL,
	          __osg_Object_P1__cloneType,
	          "",
	          "");
	I_Method1(osg::Object *, clone, IN, const osg::CopyOp &, copyop,
	          Properties::VIRTUAL,
	          __osg_Object_P1__clone__C5_osg_CopyOp_R1,
	          "",
	          "");
	I_Method1(bool, isSameKindAs, IN, const osg::Object *, obj,
	          Properties::VIRTUAL,
	          __bool__isSameKindAs__C5_osg_Object_P1,
	          "",
	          "");
	I_Method0(const char *, className,
	          Properties::VIRTUAL,
	          __C5_char_P1__className,
	          "",
	          "");
	I_Method0(const char *, libraryName,
	          Properties::VIRTUAL,
	          __C5_char_P1__libraryName,
	          "",
	          "");
	I_Method1(void, accept, IN, osg::NodeVisitor &, nv,
	          Properties::VIRTUAL,
	          __void__accept__osg_NodeVisitor_R1,
	          "",
	          "");
	I_Method0(const char *, effectName,
	          Properties::VIRTUAL,
	          __C5_char_P1__effectName,
	          "",
	          "");
	I_Method0(const char *, effectDescription,
	          Properties::VIRTUAL,
	          __C5_char_P1__effectDescription,
	          "",
	          "");
	I_Method0(const char *, effectAuthor,
	          Properties::VIRTUAL,
	          __C5_char_P1__effectAuthor,
	          "",
	          "");
	I_Method0(const osg::Vec4 &, getWireframeColor,
	          Properties::NON_VIRTUAL,
	          __C5_osg_Vec4_R1__getWireframeColor,
	          "get the wireframe color ",
	          "");
	I_Method1(void, setWireframeColor, IN, const osg::Vec4 &, color,
	          Properties::NON_VIRTUAL,
	          __void__setWireframeColor__C5_osg_Vec4_R1,
	          "set the wireframe color ",
	          "");
	I_Method0(float, getWireframeLineWidth,
	          Properties::NON_VIRTUAL,
	          __float__getWireframeLineWidth,
	          "get the wireframe line width ",
	          "");
	I_Method1(void, setWireframeLineWidth, IN, float, w,
	          Properties::NON_VIRTUAL,
	          __void__setWireframeLineWidth__float,
	          "set the wireframe line width ",
	          "");
	I_SimpleProperty(const osg::Vec4 &, WireframeColor, 
	                 __C5_osg_Vec4_R1__getWireframeColor, 
	                 __void__setWireframeColor__C5_osg_Vec4_R1);
	I_SimpleProperty(float, WireframeLineWidth, 
	                 __float__getWireframeLineWidth, 
	                 __void__setWireframeLineWidth__float);
END_REFLECTOR

