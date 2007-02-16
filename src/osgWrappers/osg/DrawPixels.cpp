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

#include <osg/.svn/text-base/BoundingBox.svn-base>
#include <osg/.svn/text-base/CopyOp.svn-base>
#include <osg/.svn/text-base/Image.svn-base>
#include <osg/.svn/text-base/Object.svn-base>
#include <osg/.svn/text-base/Vec3.svn-base>
#include <osg/DrawPixels>
#include <osg/State>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

BEGIN_OBJECT_REFLECTOR(osg::DrawPixels)
	I_BaseType(osg::Drawable);
	I_BaseType(osg::Drawable);
	I_Constructor0(____DrawPixels,
	               "",
	               "");
	I_ConstructorWithDefaults2(IN, const osg::DrawPixels &, drawimage, , IN, const osg::CopyOp &, copyop, osg::CopyOp::SHALLOW_COPY,
	                           ____DrawPixels__C5_DrawPixels_R1__C5_CopyOp_R1,
	                           "Copy constructor using CopyOp to manage deep vs shallow copy. ",
	                           "");
	I_Method0(osg::Object *, cloneType,
	          Properties::VIRTUAL,
	          __Object_P1__cloneType,
	          "",
	          "");
	I_Method1(osg::Object *, clone, IN, const osg::CopyOp &, copyop,
	          Properties::VIRTUAL,
	          __Object_P1__clone__C5_CopyOp_R1,
	          "",
	          "");
	I_Method1(bool, isSameKindAs, IN, const osg::Object *, obj,
	          Properties::VIRTUAL,
	          __bool__isSameKindAs__C5_Object_P1,
	          "",
	          "");
	I_Method0(const char *, libraryName,
	          Properties::VIRTUAL,
	          __C5_char_P1__libraryName,
	          "",
	          "");
	I_Method0(const char *, className,
	          Properties::VIRTUAL,
	          __C5_char_P1__className,
	          "",
	          "");
	I_Method1(void, setPosition, IN, const osg::Vec3 &, position,
	          Properties::NON_VIRTUAL,
	          __void__setPosition__C5_osg_Vec3_R1,
	          "",
	          "");
	I_Method0(osg::Vec3 &, getPosition,
	          Properties::NON_VIRTUAL,
	          __osg_Vec3_R1__getPosition,
	          "",
	          "");
	I_Method0(const osg::Vec3 &, getPosition,
	          Properties::NON_VIRTUAL,
	          __C5_osg_Vec3_R1__getPosition,
	          "",
	          "");
	I_Method1(void, setImage, IN, osg::Image *, image,
	          Properties::NON_VIRTUAL,
	          __void__setImage__osg_Image_P1,
	          "",
	          "");
	I_Method0(osg::Image *, getImage,
	          Properties::NON_VIRTUAL,
	          __osg_Image_P1__getImage,
	          "",
	          "");
	I_Method0(const osg::Image *, getImage,
	          Properties::NON_VIRTUAL,
	          __C5_osg_Image_P1__getImage,
	          "",
	          "");
	I_Method1(void, setUseSubImage, IN, bool, useSubImage,
	          Properties::NON_VIRTUAL,
	          __void__setUseSubImage__bool,
	          "",
	          "");
	I_Method0(bool, getUseSubImage,
	          Properties::NON_VIRTUAL,
	          __bool__getUseSubImage,
	          "",
	          "");
	I_Method4(void, setSubImageDimensions, IN, unsigned int, offsetX, IN, unsigned int, offsetY, IN, unsigned int, width, IN, unsigned int, height,
	          Properties::NON_VIRTUAL,
	          __void__setSubImageDimensions__unsigned_int__unsigned_int__unsigned_int__unsigned_int,
	          "",
	          "");
	I_Method4(void, getSubImageDimensions, IN, unsigned int &, offsetX, IN, unsigned int &, offsetY, IN, unsigned int &, width, IN, unsigned int &, height,
	          Properties::NON_VIRTUAL,
	          __void__getSubImageDimensions__unsigned_int_R1__unsigned_int_R1__unsigned_int_R1__unsigned_int_R1,
	          "",
	          "");
	I_Method1(void, drawImplementation, IN, osg::State &, state,
	          Properties::VIRTUAL,
	          __void__drawImplementation__State_R1,
	          "",
	          "");
	I_Method0(osg::BoundingBox, computeBound,
	          Properties::VIRTUAL,
	          __BoundingBox__computeBound,
	          "",
	          "");
	I_Constructor0(____DrawPixels,
	               "",
	               "");
	I_ConstructorWithDefaults2(IN, const osg::DrawPixels &, drawimage, , IN, const osg::CopyOp &, copyop, osg::CopyOp::SHALLOW_COPY,
	                           ____DrawPixels__C5_DrawPixels_R1__C5_CopyOp_R1,
	                           "Copy constructor using CopyOp to manage deep vs shallow copy. ",
	                           "");
	I_Method0(osg::Object *, cloneType,
	          Properties::VIRTUAL,
	          __Object_P1__cloneType,
	          "",
	          "");
	I_Method1(osg::Object *, clone, IN, const osg::CopyOp &, copyop,
	          Properties::VIRTUAL,
	          __Object_P1__clone__C5_CopyOp_R1,
	          "",
	          "");
	I_Method1(bool, isSameKindAs, IN, const osg::Object *, obj,
	          Properties::VIRTUAL,
	          __bool__isSameKindAs__C5_Object_P1,
	          "",
	          "");
	I_Method0(const char *, libraryName,
	          Properties::VIRTUAL,
	          __C5_char_P1__libraryName,
	          "",
	          "");
	I_Method0(const char *, className,
	          Properties::VIRTUAL,
	          __C5_char_P1__className,
	          "",
	          "");
	I_Method1(void, setPosition, IN, const osg::Vec3 &, position,
	          Properties::NON_VIRTUAL,
	          __void__setPosition__C5_osg_Vec3_R1,
	          "",
	          "");
	I_Method0(osg::Vec3 &, getPosition,
	          Properties::NON_VIRTUAL,
	          __osg_Vec3_R1__getPosition,
	          "",
	          "");
	I_Method0(const osg::Vec3 &, getPosition,
	          Properties::NON_VIRTUAL,
	          __C5_osg_Vec3_R1__getPosition,
	          "",
	          "");
	I_Method1(void, setImage, IN, osg::Image *, image,
	          Properties::NON_VIRTUAL,
	          __void__setImage__osg_Image_P1,
	          "",
	          "");
	I_Method0(osg::Image *, getImage,
	          Properties::NON_VIRTUAL,
	          __osg_Image_P1__getImage,
	          "",
	          "");
	I_Method0(const osg::Image *, getImage,
	          Properties::NON_VIRTUAL,
	          __C5_osg_Image_P1__getImage,
	          "",
	          "");
	I_Method1(void, setUseSubImage, IN, bool, useSubImage,
	          Properties::NON_VIRTUAL,
	          __void__setUseSubImage__bool,
	          "",
	          "");
	I_Method0(bool, getUseSubImage,
	          Properties::NON_VIRTUAL,
	          __bool__getUseSubImage,
	          "",
	          "");
	I_Method4(void, setSubImageDimensions, IN, unsigned int, offsetX, IN, unsigned int, offsetY, IN, unsigned int, width, IN, unsigned int, height,
	          Properties::NON_VIRTUAL,
	          __void__setSubImageDimensions__unsigned_int__unsigned_int__unsigned_int__unsigned_int,
	          "",
	          "");
	I_Method4(void, getSubImageDimensions, IN, unsigned int &, offsetX, IN, unsigned int &, offsetY, IN, unsigned int &, width, IN, unsigned int &, height,
	          Properties::NON_VIRTUAL,
	          __void__getSubImageDimensions__unsigned_int_R1__unsigned_int_R1__unsigned_int_R1__unsigned_int_R1,
	          "",
	          "");
	I_Method1(void, drawImplementation, IN, osg::State &, state,
	          Properties::VIRTUAL,
	          __void__drawImplementation__State_R1,
	          "",
	          "");
	I_Method0(osg::BoundingBox, computeBound,
	          Properties::VIRTUAL,
	          __BoundingBox__computeBound,
	          "",
	          "");
	I_SimpleProperty(osg::Image *, Image, 
	                 __osg_Image_P1__getImage, 
	                 __void__setImage__osg_Image_P1);
	I_SimpleProperty(const osg::Vec3 &, Position, 
	                 __C5_osg_Vec3_R1__getPosition, 
	                 __void__setPosition__C5_osg_Vec3_R1);
	I_SimpleProperty(bool, UseSubImage, 
	                 __bool__getUseSubImage, 
	                 __void__setUseSubImage__bool);
END_REFLECTOR

BEGIN_OBJECT_REFLECTOR(osg::DrawPixels)
	I_BaseType(osg::Drawable);
	I_BaseType(osg::Drawable);
	I_Constructor0(____DrawPixels,
	               "",
	               "");
	I_ConstructorWithDefaults2(IN, const osg::DrawPixels &, drawimage, , IN, const osg::CopyOp &, copyop, osg::CopyOp::SHALLOW_COPY,
	                           ____DrawPixels__C5_DrawPixels_R1__C5_CopyOp_R1,
	                           "Copy constructor using CopyOp to manage deep vs shallow copy. ",
	                           "");
	I_Method0(osg::Object *, cloneType,
	          Properties::VIRTUAL,
	          __Object_P1__cloneType,
	          "",
	          "");
	I_Method1(osg::Object *, clone, IN, const osg::CopyOp &, copyop,
	          Properties::VIRTUAL,
	          __Object_P1__clone__C5_CopyOp_R1,
	          "",
	          "");
	I_Method1(bool, isSameKindAs, IN, const osg::Object *, obj,
	          Properties::VIRTUAL,
	          __bool__isSameKindAs__C5_Object_P1,
	          "",
	          "");
	I_Method0(const char *, libraryName,
	          Properties::VIRTUAL,
	          __C5_char_P1__libraryName,
	          "",
	          "");
	I_Method0(const char *, className,
	          Properties::VIRTUAL,
	          __C5_char_P1__className,
	          "",
	          "");
	I_Method1(void, setPosition, IN, const osg::Vec3 &, position,
	          Properties::NON_VIRTUAL,
	          __void__setPosition__C5_osg_Vec3_R1,
	          "",
	          "");
	I_Method0(osg::Vec3 &, getPosition,
	          Properties::NON_VIRTUAL,
	          __osg_Vec3_R1__getPosition,
	          "",
	          "");
	I_Method0(const osg::Vec3 &, getPosition,
	          Properties::NON_VIRTUAL,
	          __C5_osg_Vec3_R1__getPosition,
	          "",
	          "");
	I_Method1(void, setImage, IN, osg::Image *, image,
	          Properties::NON_VIRTUAL,
	          __void__setImage__osg_Image_P1,
	          "",
	          "");
	I_Method0(osg::Image *, getImage,
	          Properties::NON_VIRTUAL,
	          __osg_Image_P1__getImage,
	          "",
	          "");
	I_Method0(const osg::Image *, getImage,
	          Properties::NON_VIRTUAL,
	          __C5_osg_Image_P1__getImage,
	          "",
	          "");
	I_Method1(void, setUseSubImage, IN, bool, useSubImage,
	          Properties::NON_VIRTUAL,
	          __void__setUseSubImage__bool,
	          "",
	          "");
	I_Method0(bool, getUseSubImage,
	          Properties::NON_VIRTUAL,
	          __bool__getUseSubImage,
	          "",
	          "");
	I_Method4(void, setSubImageDimensions, IN, unsigned int, offsetX, IN, unsigned int, offsetY, IN, unsigned int, width, IN, unsigned int, height,
	          Properties::NON_VIRTUAL,
	          __void__setSubImageDimensions__unsigned_int__unsigned_int__unsigned_int__unsigned_int,
	          "",
	          "");
	I_Method4(void, getSubImageDimensions, IN, unsigned int &, offsetX, IN, unsigned int &, offsetY, IN, unsigned int &, width, IN, unsigned int &, height,
	          Properties::NON_VIRTUAL,
	          __void__getSubImageDimensions__unsigned_int_R1__unsigned_int_R1__unsigned_int_R1__unsigned_int_R1,
	          "",
	          "");
	I_Method1(void, drawImplementation, IN, osg::State &, state,
	          Properties::VIRTUAL,
	          __void__drawImplementation__State_R1,
	          "",
	          "");
	I_Method0(osg::BoundingBox, computeBound,
	          Properties::VIRTUAL,
	          __BoundingBox__computeBound,
	          "",
	          "");
	I_Constructor0(____DrawPixels,
	               "",
	               "");
	I_ConstructorWithDefaults2(IN, const osg::DrawPixels &, drawimage, , IN, const osg::CopyOp &, copyop, osg::CopyOp::SHALLOW_COPY,
	                           ____DrawPixels__C5_DrawPixels_R1__C5_CopyOp_R1,
	                           "Copy constructor using CopyOp to manage deep vs shallow copy. ",
	                           "");
	I_Method0(osg::Object *, cloneType,
	          Properties::VIRTUAL,
	          __Object_P1__cloneType,
	          "",
	          "");
	I_Method1(osg::Object *, clone, IN, const osg::CopyOp &, copyop,
	          Properties::VIRTUAL,
	          __Object_P1__clone__C5_CopyOp_R1,
	          "",
	          "");
	I_Method1(bool, isSameKindAs, IN, const osg::Object *, obj,
	          Properties::VIRTUAL,
	          __bool__isSameKindAs__C5_Object_P1,
	          "",
	          "");
	I_Method0(const char *, libraryName,
	          Properties::VIRTUAL,
	          __C5_char_P1__libraryName,
	          "",
	          "");
	I_Method0(const char *, className,
	          Properties::VIRTUAL,
	          __C5_char_P1__className,
	          "",
	          "");
	I_Method1(void, setPosition, IN, const osg::Vec3 &, position,
	          Properties::NON_VIRTUAL,
	          __void__setPosition__C5_osg_Vec3_R1,
	          "",
	          "");
	I_Method0(osg::Vec3 &, getPosition,
	          Properties::NON_VIRTUAL,
	          __osg_Vec3_R1__getPosition,
	          "",
	          "");
	I_Method0(const osg::Vec3 &, getPosition,
	          Properties::NON_VIRTUAL,
	          __C5_osg_Vec3_R1__getPosition,
	          "",
	          "");
	I_Method1(void, setImage, IN, osg::Image *, image,
	          Properties::NON_VIRTUAL,
	          __void__setImage__osg_Image_P1,
	          "",
	          "");
	I_Method0(osg::Image *, getImage,
	          Properties::NON_VIRTUAL,
	          __osg_Image_P1__getImage,
	          "",
	          "");
	I_Method0(const osg::Image *, getImage,
	          Properties::NON_VIRTUAL,
	          __C5_osg_Image_P1__getImage,
	          "",
	          "");
	I_Method1(void, setUseSubImage, IN, bool, useSubImage,
	          Properties::NON_VIRTUAL,
	          __void__setUseSubImage__bool,
	          "",
	          "");
	I_Method0(bool, getUseSubImage,
	          Properties::NON_VIRTUAL,
	          __bool__getUseSubImage,
	          "",
	          "");
	I_Method4(void, setSubImageDimensions, IN, unsigned int, offsetX, IN, unsigned int, offsetY, IN, unsigned int, width, IN, unsigned int, height,
	          Properties::NON_VIRTUAL,
	          __void__setSubImageDimensions__unsigned_int__unsigned_int__unsigned_int__unsigned_int,
	          "",
	          "");
	I_Method4(void, getSubImageDimensions, IN, unsigned int &, offsetX, IN, unsigned int &, offsetY, IN, unsigned int &, width, IN, unsigned int &, height,
	          Properties::NON_VIRTUAL,
	          __void__getSubImageDimensions__unsigned_int_R1__unsigned_int_R1__unsigned_int_R1__unsigned_int_R1,
	          "",
	          "");
	I_Method1(void, drawImplementation, IN, osg::State &, state,
	          Properties::VIRTUAL,
	          __void__drawImplementation__State_R1,
	          "",
	          "");
	I_Method0(osg::BoundingBox, computeBound,
	          Properties::VIRTUAL,
	          __BoundingBox__computeBound,
	          "",
	          "");
	I_SimpleProperty(osg::Image *, Image, 
	                 __osg_Image_P1__getImage, 
	                 __void__setImage__osg_Image_P1);
	I_SimpleProperty(const osg::Vec3 &, Position, 
	                 __C5_osg_Vec3_R1__getPosition, 
	                 __void__setPosition__C5_osg_Vec3_R1);
	I_SimpleProperty(bool, UseSubImage, 
	                 __bool__getUseSubImage, 
	                 __void__setUseSubImage__bool);
END_REFLECTOR

