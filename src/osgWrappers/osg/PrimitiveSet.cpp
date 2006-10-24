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
#include <osg/PrimitiveSet>
#include <osg/State>
#include <osg/Vec2>
#include <osg/Vec3>
#include <osg/Vec4>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

TYPE_NAME_ALIAS(osg::VectorGLsizei, osg::DrawArrayLengths::vector_type);

BEGIN_OBJECT_REFLECTOR(osg::DrawArrayLengths)
	I_BaseType(osg::PrimitiveSet);
	I_BaseType(osg::VectorGLsizei);
	I_ConstructorWithDefaults1(IN, GLenum, mode, 0,
	                           ____DrawArrayLengths__GLenum,
	                           "",
	                           "");
	I_ConstructorWithDefaults2(IN, const osg::DrawArrayLengths &, dal, , IN, const osg::CopyOp &, copyop, osg::CopyOp::SHALLOW_COPY,
	                           ____DrawArrayLengths__C5_DrawArrayLengths_R1__C5_CopyOp_R1,
	                           "",
	                           "");
	I_Constructor4(IN, GLenum, mode, IN, GLint, first, IN, unsigned int, no, IN, GLsizei *, ptr,
	               ____DrawArrayLengths__GLenum__GLint__unsigned_int__GLsizei_P1,
	               "",
	               "");
	I_Constructor3(IN, GLenum, mode, IN, GLint, first, IN, unsigned int, no,
	               ____DrawArrayLengths__GLenum__GLint__unsigned_int,
	               "",
	               "");
	I_Constructor2(IN, GLenum, mode, IN, GLint, first,
	               ____DrawArrayLengths__GLenum__GLint,
	               "",
	               "");
	I_Method0(osg::Object *, cloneType,
	          __Object_P1__cloneType,
	          "Clone the type of an object, with Object* return type. ",
	          "Must be defined by derived classes. ");
	I_Method1(osg::Object *, clone, IN, const osg::CopyOp &, copyop,
	          __Object_P1__clone__C5_CopyOp_R1,
	          "Clone an object, with Object* return type. ",
	          "Must be defined by derived classes. ");
	I_Method1(bool, isSameKindAs, IN, const osg::Object *, obj,
	          __bool__isSameKindAs__C5_Object_P1,
	          "",
	          "");
	I_Method0(const char *, libraryName,
	          __C5_char_P1__libraryName,
	          "return the name of the object's library. ",
	          "Must be defined by derived classes. The OpenSceneGraph convention is that the namespace of a library is the same as the library name. ");
	I_Method0(const char *, className,
	          __C5_char_P1__className,
	          "return the name of the object's class type. ",
	          "Must be defined by derived classes. ");
	I_Method1(void, setFirst, IN, GLint, first,
	          __void__setFirst__GLint,
	          "",
	          "");
	I_Method0(GLint, getFirst,
	          __GLint__getFirst,
	          "",
	          "");
	I_Method2(void, draw, IN, osg::State &, state, IN, bool, useVertexBufferObjects,
	          __void__draw__State_R1__bool,
	          "",
	          "");
	I_Method1(void, accept, IN, osg::PrimitiveFunctor &, functor,
	          __void__accept__PrimitiveFunctor_R1,
	          "",
	          "");
	I_Method1(void, accept, IN, osg::PrimitiveIndexFunctor &, functor,
	          __void__accept__PrimitiveIndexFunctor_R1,
	          "",
	          "");
	I_Method0(unsigned int, getNumIndices,
	          __unsigned_int__getNumIndices,
	          "",
	          "");
	I_Method1(unsigned int, index, IN, unsigned int, pos,
	          __unsigned_int__index__unsigned_int,
	          "",
	          "");
	I_Method1(void, offsetIndices, IN, int, offset,
	          __void__offsetIndices__int,
	          "",
	          "");
	I_Method0(unsigned int, getNumPrimitives,
	          __unsigned_int__getNumPrimitives,
	          "",
	          "");
	I_SimpleProperty(GLint, First, 
	                 __GLint__getFirst, 
	                 __void__setFirst__GLint);
END_REFLECTOR

BEGIN_OBJECT_REFLECTOR(osg::DrawArrays)
	I_BaseType(osg::PrimitiveSet);
	I_ConstructorWithDefaults1(IN, GLenum, mode, 0,
	                           ____DrawArrays__GLenum,
	                           "",
	                           "");
	I_Constructor3(IN, GLenum, mode, IN, GLint, first, IN, GLsizei, count,
	               ____DrawArrays__GLenum__GLint__GLsizei,
	               "",
	               "");
	I_ConstructorWithDefaults2(IN, const osg::DrawArrays &, da, , IN, const osg::CopyOp &, copyop, osg::CopyOp::SHALLOW_COPY,
	                           ____DrawArrays__C5_DrawArrays_R1__C5_CopyOp_R1,
	                           "",
	                           "");
	I_Method0(osg::Object *, cloneType,
	          __Object_P1__cloneType,
	          "Clone the type of an object, with Object* return type. ",
	          "Must be defined by derived classes. ");
	I_Method1(osg::Object *, clone, IN, const osg::CopyOp &, copyop,
	          __Object_P1__clone__C5_CopyOp_R1,
	          "Clone an object, with Object* return type. ",
	          "Must be defined by derived classes. ");
	I_Method1(bool, isSameKindAs, IN, const osg::Object *, obj,
	          __bool__isSameKindAs__C5_Object_P1,
	          "",
	          "");
	I_Method0(const char *, libraryName,
	          __C5_char_P1__libraryName,
	          "return the name of the object's library. ",
	          "Must be defined by derived classes. The OpenSceneGraph convention is that the namespace of a library is the same as the library name. ");
	I_Method0(const char *, className,
	          __C5_char_P1__className,
	          "return the name of the object's class type. ",
	          "Must be defined by derived classes. ");
	I_Method3(void, set, IN, GLenum, mode, IN, GLint, first, IN, GLsizei, count,
	          __void__set__GLenum__GLint__GLsizei,
	          "",
	          "");
	I_Method1(void, setFirst, IN, GLint, first,
	          __void__setFirst__GLint,
	          "",
	          "");
	I_Method0(GLint, getFirst,
	          __GLint__getFirst,
	          "",
	          "");
	I_Method1(void, setCount, IN, GLsizei, count,
	          __void__setCount__GLsizei,
	          "",
	          "");
	I_Method0(GLsizei, getCount,
	          __GLsizei__getCount,
	          "",
	          "");
	I_Method2(void, draw, IN, osg::State &, state, IN, bool, useVertexBufferObjects,
	          __void__draw__State_R1__bool,
	          "",
	          "");
	I_Method1(void, accept, IN, osg::PrimitiveFunctor &, functor,
	          __void__accept__PrimitiveFunctor_R1,
	          "",
	          "");
	I_Method1(void, accept, IN, osg::PrimitiveIndexFunctor &, functor,
	          __void__accept__PrimitiveIndexFunctor_R1,
	          "",
	          "");
	I_Method0(unsigned int, getNumIndices,
	          __unsigned_int__getNumIndices,
	          "",
	          "");
	I_Method1(unsigned int, index, IN, unsigned int, pos,
	          __unsigned_int__index__unsigned_int,
	          "",
	          "");
	I_Method1(void, offsetIndices, IN, int, offset,
	          __void__offsetIndices__int,
	          "",
	          "");
	I_SimpleProperty(GLsizei, Count, 
	                 __GLsizei__getCount, 
	                 __void__setCount__GLsizei);
	I_SimpleProperty(GLint, First, 
	                 __GLint__getFirst, 
	                 __void__setFirst__GLint);
END_REFLECTOR

TYPE_NAME_ALIAS(osg::VectorGLubyte, osg::DrawElementsUByte::vector_type);

BEGIN_OBJECT_REFLECTOR(osg::DrawElementsUByte)
	I_BaseType(osg::PrimitiveSet);
	I_BaseType(osg::VectorGLubyte);
	I_ConstructorWithDefaults1(IN, GLenum, mode, 0,
	                           ____DrawElementsUByte__GLenum,
	                           "",
	                           "");
	I_ConstructorWithDefaults2(IN, const osg::DrawElementsUByte &, array, , IN, const osg::CopyOp &, copyop, osg::CopyOp::SHALLOW_COPY,
	                           ____DrawElementsUByte__C5_DrawElementsUByte_R1__C5_CopyOp_R1,
	                           "",
	                           "");
	I_Constructor3(IN, GLenum, mode, IN, unsigned int, no, IN, GLubyte *, ptr,
	               ____DrawElementsUByte__GLenum__unsigned_int__GLubyte_P1,
	               "",
	               "");
	I_Constructor2(IN, GLenum, mode, IN, unsigned int, no,
	               ____DrawElementsUByte__GLenum__unsigned_int,
	               "",
	               "");
	I_Method0(osg::Object *, cloneType,
	          __Object_P1__cloneType,
	          "Clone the type of an object, with Object* return type. ",
	          "Must be defined by derived classes. ");
	I_Method1(osg::Object *, clone, IN, const osg::CopyOp &, copyop,
	          __Object_P1__clone__C5_CopyOp_R1,
	          "Clone an object, with Object* return type. ",
	          "Must be defined by derived classes. ");
	I_Method1(bool, isSameKindAs, IN, const osg::Object *, obj,
	          __bool__isSameKindAs__C5_Object_P1,
	          "",
	          "");
	I_Method0(const char *, libraryName,
	          __C5_char_P1__libraryName,
	          "return the name of the object's library. ",
	          "Must be defined by derived classes. The OpenSceneGraph convention is that the namespace of a library is the same as the library name. ");
	I_Method0(const char *, className,
	          __C5_char_P1__className,
	          "return the name of the object's class type. ",
	          "Must be defined by derived classes. ");
	I_Method0(const GLvoid *, getDataPointer,
	          __C5_GLvoid_P1__getDataPointer,
	          "",
	          "");
	I_Method0(unsigned int, getTotalDataSize,
	          __unsigned_int__getTotalDataSize,
	          "",
	          "");
	I_Method0(bool, supportsBufferObject,
	          __bool__supportsBufferObject,
	          "",
	          "");
	I_Method2(void, draw, IN, osg::State &, state, IN, bool, useVertexBufferObjects,
	          __void__draw__State_R1__bool,
	          "",
	          "");
	I_Method1(void, accept, IN, osg::PrimitiveFunctor &, functor,
	          __void__accept__PrimitiveFunctor_R1,
	          "",
	          "");
	I_Method1(void, accept, IN, osg::PrimitiveIndexFunctor &, functor,
	          __void__accept__PrimitiveIndexFunctor_R1,
	          "",
	          "");
	I_Method0(unsigned int, getNumIndices,
	          __unsigned_int__getNumIndices,
	          "",
	          "");
	I_Method1(unsigned int, index, IN, unsigned int, pos,
	          __unsigned_int__index__unsigned_int,
	          "",
	          "");
	I_Method1(void, offsetIndices, IN, int, offset,
	          __void__offsetIndices__int,
	          "",
	          "");
	I_MethodWithDefaults1(void, releaseGLObjects, IN, osg::State *, state, 0,
	                      __void__releaseGLObjects__State_P1,
	                      "If State is non-zero, this function releases OpenGL objects for the specified graphics context. ",
	                      "Otherwise, releases OpenGL objexts for all graphics contexts. ");
	I_SimpleProperty(const GLvoid *, DataPointer, 
	                 __C5_GLvoid_P1__getDataPointer, 
	                 0);
	I_SimpleProperty(unsigned int, TotalDataSize, 
	                 __unsigned_int__getTotalDataSize, 
	                 0);
END_REFLECTOR

TYPE_NAME_ALIAS(osg::VectorGLuint, osg::DrawElementsUInt::vector_type);

BEGIN_OBJECT_REFLECTOR(osg::DrawElementsUInt)
	I_BaseType(osg::PrimitiveSet);
	I_BaseType(osg::VectorGLuint);
	I_ConstructorWithDefaults1(IN, GLenum, mode, 0,
	                           ____DrawElementsUInt__GLenum,
	                           "",
	                           "");
	I_ConstructorWithDefaults2(IN, const osg::DrawElementsUInt &, array, , IN, const osg::CopyOp &, copyop, osg::CopyOp::SHALLOW_COPY,
	                           ____DrawElementsUInt__C5_DrawElementsUInt_R1__C5_CopyOp_R1,
	                           "",
	                           "");
	I_Constructor3(IN, GLenum, mode, IN, unsigned int, no, IN, GLuint *, ptr,
	               ____DrawElementsUInt__GLenum__unsigned_int__GLuint_P1,
	               "",
	               "");
	I_Constructor2(IN, GLenum, mode, IN, unsigned int, no,
	               ____DrawElementsUInt__GLenum__unsigned_int,
	               "",
	               "");
	I_Method0(osg::Object *, cloneType,
	          __Object_P1__cloneType,
	          "Clone the type of an object, with Object* return type. ",
	          "Must be defined by derived classes. ");
	I_Method1(osg::Object *, clone, IN, const osg::CopyOp &, copyop,
	          __Object_P1__clone__C5_CopyOp_R1,
	          "Clone an object, with Object* return type. ",
	          "Must be defined by derived classes. ");
	I_Method1(bool, isSameKindAs, IN, const osg::Object *, obj,
	          __bool__isSameKindAs__C5_Object_P1,
	          "",
	          "");
	I_Method0(const char *, libraryName,
	          __C5_char_P1__libraryName,
	          "return the name of the object's library. ",
	          "Must be defined by derived classes. The OpenSceneGraph convention is that the namespace of a library is the same as the library name. ");
	I_Method0(const char *, className,
	          __C5_char_P1__className,
	          "return the name of the object's class type. ",
	          "Must be defined by derived classes. ");
	I_Method0(const GLvoid *, getDataPointer,
	          __C5_GLvoid_P1__getDataPointer,
	          "",
	          "");
	I_Method0(unsigned int, getTotalDataSize,
	          __unsigned_int__getTotalDataSize,
	          "",
	          "");
	I_Method0(bool, supportsBufferObject,
	          __bool__supportsBufferObject,
	          "",
	          "");
	I_Method2(void, draw, IN, osg::State &, state, IN, bool, useVertexBufferObjects,
	          __void__draw__State_R1__bool,
	          "",
	          "");
	I_Method1(void, accept, IN, osg::PrimitiveFunctor &, functor,
	          __void__accept__PrimitiveFunctor_R1,
	          "",
	          "");
	I_Method1(void, accept, IN, osg::PrimitiveIndexFunctor &, functor,
	          __void__accept__PrimitiveIndexFunctor_R1,
	          "",
	          "");
	I_Method0(unsigned int, getNumIndices,
	          __unsigned_int__getNumIndices,
	          "",
	          "");
	I_Method1(unsigned int, index, IN, unsigned int, pos,
	          __unsigned_int__index__unsigned_int,
	          "",
	          "");
	I_Method1(void, offsetIndices, IN, int, offset,
	          __void__offsetIndices__int,
	          "",
	          "");
	I_MethodWithDefaults1(void, releaseGLObjects, IN, osg::State *, state, 0,
	                      __void__releaseGLObjects__State_P1,
	                      "If State is non-zero, this function releases OpenGL objects for the specified graphics context. ",
	                      "Otherwise, releases OpenGL objexts for all graphics contexts. ");
	I_SimpleProperty(const GLvoid *, DataPointer, 
	                 __C5_GLvoid_P1__getDataPointer, 
	                 0);
	I_SimpleProperty(unsigned int, TotalDataSize, 
	                 __unsigned_int__getTotalDataSize, 
	                 0);
END_REFLECTOR

TYPE_NAME_ALIAS(osg::VectorGLushort, osg::DrawElementsUShort::vector_type);

BEGIN_OBJECT_REFLECTOR(osg::DrawElementsUShort)
	I_BaseType(osg::PrimitiveSet);
	I_BaseType(osg::VectorGLushort);
	I_ConstructorWithDefaults1(IN, GLenum, mode, 0,
	                           ____DrawElementsUShort__GLenum,
	                           "",
	                           "");
	I_ConstructorWithDefaults2(IN, const osg::DrawElementsUShort &, array, , IN, const osg::CopyOp &, copyop, osg::CopyOp::SHALLOW_COPY,
	                           ____DrawElementsUShort__C5_DrawElementsUShort_R1__C5_CopyOp_R1,
	                           "",
	                           "");
	I_Constructor3(IN, GLenum, mode, IN, unsigned int, no, IN, GLushort *, ptr,
	               ____DrawElementsUShort__GLenum__unsigned_int__GLushort_P1,
	               "",
	               "");
	I_Constructor2(IN, GLenum, mode, IN, unsigned int, no,
	               ____DrawElementsUShort__GLenum__unsigned_int,
	               "",
	               "");
	I_Method0(osg::Object *, cloneType,
	          __Object_P1__cloneType,
	          "Clone the type of an object, with Object* return type. ",
	          "Must be defined by derived classes. ");
	I_Method1(osg::Object *, clone, IN, const osg::CopyOp &, copyop,
	          __Object_P1__clone__C5_CopyOp_R1,
	          "Clone an object, with Object* return type. ",
	          "Must be defined by derived classes. ");
	I_Method1(bool, isSameKindAs, IN, const osg::Object *, obj,
	          __bool__isSameKindAs__C5_Object_P1,
	          "",
	          "");
	I_Method0(const char *, libraryName,
	          __C5_char_P1__libraryName,
	          "return the name of the object's library. ",
	          "Must be defined by derived classes. The OpenSceneGraph convention is that the namespace of a library is the same as the library name. ");
	I_Method0(const char *, className,
	          __C5_char_P1__className,
	          "return the name of the object's class type. ",
	          "Must be defined by derived classes. ");
	I_Method0(const GLvoid *, getDataPointer,
	          __C5_GLvoid_P1__getDataPointer,
	          "",
	          "");
	I_Method0(unsigned int, getTotalDataSize,
	          __unsigned_int__getTotalDataSize,
	          "",
	          "");
	I_Method0(bool, supportsBufferObject,
	          __bool__supportsBufferObject,
	          "",
	          "");
	I_Method2(void, draw, IN, osg::State &, state, IN, bool, useVertexBufferObjects,
	          __void__draw__State_R1__bool,
	          "",
	          "");
	I_Method1(void, accept, IN, osg::PrimitiveFunctor &, functor,
	          __void__accept__PrimitiveFunctor_R1,
	          "",
	          "");
	I_Method1(void, accept, IN, osg::PrimitiveIndexFunctor &, functor,
	          __void__accept__PrimitiveIndexFunctor_R1,
	          "",
	          "");
	I_Method0(unsigned int, getNumIndices,
	          __unsigned_int__getNumIndices,
	          "",
	          "");
	I_Method1(unsigned int, index, IN, unsigned int, pos,
	          __unsigned_int__index__unsigned_int,
	          "",
	          "");
	I_Method1(void, offsetIndices, IN, int, offset,
	          __void__offsetIndices__int,
	          "",
	          "");
	I_MethodWithDefaults1(void, releaseGLObjects, IN, osg::State *, state, 0,
	                      __void__releaseGLObjects__State_P1,
	                      "If State is non-zero, this function releases OpenGL objects for the specified graphics context. ",
	                      "Otherwise, releases OpenGL objexts for all graphics contexts. ");
	I_SimpleProperty(const GLvoid *, DataPointer, 
	                 __C5_GLvoid_P1__getDataPointer, 
	                 0);
	I_SimpleProperty(unsigned int, TotalDataSize, 
	                 __unsigned_int__getTotalDataSize, 
	                 0);
END_REFLECTOR

BEGIN_ABSTRACT_OBJECT_REFLECTOR(osg::PrimitiveFunctor)
	I_Constructor0(____PrimitiveFunctor,
	               "",
	               "");
	I_Method2(void, setVertexArray, IN, unsigned int, count, IN, const osg::Vec2 *, vertices,
	          __void__setVertexArray__unsigned_int__C5_Vec2_P1,
	          "Sets the array of vertices used to describe the primitives. ",
	          "Somehow mimics the OpenGL glVertexPointer() function.");
	I_Method2(void, setVertexArray, IN, unsigned int, count, IN, const osg::Vec3 *, vertices,
	          __void__setVertexArray__unsigned_int__C5_Vec3_P1,
	          "Sets the array of vertices used to describe the primitives. ",
	          "Somehow mimics the OpenGL glVertexPointer() function.");
	I_Method2(void, setVertexArray, IN, unsigned int, count, IN, const osg::Vec4 *, vertices,
	          __void__setVertexArray__unsigned_int__C5_Vec4_P1,
	          "Sets the array of vertices used to describe the primitives. ",
	          "Somehow mimics the OpenGL glVertexPointer() function.");
	I_Method3(void, drawArrays, IN, GLenum, mode, IN, GLint, first, IN, GLsizei, count,
	          __void__drawArrays__GLenum__GLint__GLsizei,
	          "Mimics the OpenGL glDrawArrays() function. ",
	          "");
	I_Method3(void, drawElements, IN, GLenum, mode, IN, GLsizei, count, IN, const GLubyte *, indices,
	          __void__drawElements__GLenum__GLsizei__C5_GLubyte_P1,
	          "Mimics the OpenGL glDrawElements() function. ",
	          "");
	I_Method3(void, drawElements, IN, GLenum, mode, IN, GLsizei, count, IN, const GLushort *, indices,
	          __void__drawElements__GLenum__GLsizei__C5_GLushort_P1,
	          "Mimics the OpenGL glDrawElements() function. ",
	          "");
	I_Method3(void, drawElements, IN, GLenum, mode, IN, GLsizei, count, IN, const GLuint *, indices,
	          __void__drawElements__GLenum__GLsizei__C5_GLuint_P1,
	          "Mimics the OpenGL glDrawElements() function. ",
	          "");
	I_Method1(void, begin, IN, GLenum, mode,
	          __void__begin__GLenum,
	          "Mimics the OpenGL glBegin() function. ",
	          "");
	I_Method1(void, vertex, IN, const osg::Vec2 &, vert,
	          __void__vertex__C5_Vec2_R1,
	          "Mimics the OpenGL glVertex() \"family of functions\". ",
	          "");
	I_Method1(void, vertex, IN, const osg::Vec3 &, vert,
	          __void__vertex__C5_Vec3_R1,
	          "Mimics the OpenGL glVertex() \"family of functions\". ",
	          "");
	I_Method1(void, vertex, IN, const osg::Vec4 &, vert,
	          __void__vertex__C5_Vec4_R1,
	          "Mimics the OpenGL glVertex() \"family of functions\". ",
	          "");
	I_Method2(void, vertex, IN, float, x, IN, float, y,
	          __void__vertex__float__float,
	          "Mimics the OpenGL glVertex() \"family of functions\". ",
	          "");
	I_Method3(void, vertex, IN, float, x, IN, float, y, IN, float, z,
	          __void__vertex__float__float__float,
	          "Mimics the OpenGL glVertex() \"family of functions\". ",
	          "");
	I_Method4(void, vertex, IN, float, x, IN, float, y, IN, float, z, IN, float, w,
	          __void__vertex__float__float__float__float,
	          "Mimics the OpenGL glVertex() \"family of functions\". ",
	          "");
	I_Method0(void, end,
	          __void__end,
	          "Mimics the OpenGL glEnd() function. ",
	          "");
END_REFLECTOR

BEGIN_ABSTRACT_OBJECT_REFLECTOR(osg::PrimitiveIndexFunctor)
	I_Constructor0(____PrimitiveIndexFunctor,
	               "",
	               "");
	I_Method2(void, setVertexArray, IN, unsigned int, count, IN, const osg::Vec2 *, vertices,
	          __void__setVertexArray__unsigned_int__C5_Vec2_P1,
	          "",
	          "");
	I_Method2(void, setVertexArray, IN, unsigned int, count, IN, const osg::Vec3 *, vertices,
	          __void__setVertexArray__unsigned_int__C5_Vec3_P1,
	          "",
	          "");
	I_Method2(void, setVertexArray, IN, unsigned int, count, IN, const osg::Vec4 *, vertices,
	          __void__setVertexArray__unsigned_int__C5_Vec4_P1,
	          "",
	          "");
	I_Method3(void, drawArrays, IN, GLenum, mode, IN, GLint, first, IN, GLsizei, count,
	          __void__drawArrays__GLenum__GLint__GLsizei,
	          "",
	          "");
	I_Method3(void, drawElements, IN, GLenum, mode, IN, GLsizei, count, IN, const GLubyte *, indices,
	          __void__drawElements__GLenum__GLsizei__C5_GLubyte_P1,
	          "",
	          "");
	I_Method3(void, drawElements, IN, GLenum, mode, IN, GLsizei, count, IN, const GLushort *, indices,
	          __void__drawElements__GLenum__GLsizei__C5_GLushort_P1,
	          "",
	          "");
	I_Method3(void, drawElements, IN, GLenum, mode, IN, GLsizei, count, IN, const GLuint *, indices,
	          __void__drawElements__GLenum__GLsizei__C5_GLuint_P1,
	          "",
	          "");
	I_Method1(void, begin, IN, GLenum, mode,
	          __void__begin__GLenum,
	          "",
	          "");
	I_Method1(void, vertex, IN, unsigned int, pos,
	          __void__vertex__unsigned_int,
	          "",
	          "");
	I_Method0(void, end,
	          __void__end,
	          "",
	          "");
END_REFLECTOR

BEGIN_ENUM_REFLECTOR(osg::PrimitiveSet::Type)
	I_EnumLabel(osg::PrimitiveSet::PrimitiveType);
	I_EnumLabel(osg::PrimitiveSet::DrawArraysPrimitiveType);
	I_EnumLabel(osg::PrimitiveSet::DrawArrayLengthsPrimitiveType);
	I_EnumLabel(osg::PrimitiveSet::DrawElementsUBytePrimitiveType);
	I_EnumLabel(osg::PrimitiveSet::DrawElementsUShortPrimitiveType);
	I_EnumLabel(osg::PrimitiveSet::DrawElementsUIntPrimitiveType);
END_REFLECTOR

BEGIN_ENUM_REFLECTOR(osg::PrimitiveSet::Mode)
	I_EnumLabel(osg::PrimitiveSet::POINTS);
	I_EnumLabel(osg::PrimitiveSet::LINES);
	I_EnumLabel(osg::PrimitiveSet::LINE_STRIP);
	I_EnumLabel(osg::PrimitiveSet::LINE_LOOP);
	I_EnumLabel(osg::PrimitiveSet::TRIANGLES);
	I_EnumLabel(osg::PrimitiveSet::TRIANGLE_STRIP);
	I_EnumLabel(osg::PrimitiveSet::TRIANGLE_FAN);
	I_EnumLabel(osg::PrimitiveSet::QUADS);
	I_EnumLabel(osg::PrimitiveSet::QUAD_STRIP);
	I_EnumLabel(osg::PrimitiveSet::POLYGON);
END_REFLECTOR

BEGIN_ABSTRACT_OBJECT_REFLECTOR(osg::PrimitiveSet)
	I_BaseType(osg::Object);
	I_ConstructorWithDefaults2(IN, osg::PrimitiveSet::Type, primType, osg::PrimitiveSet::PrimitiveType, IN, GLenum, mode, 0,
	                           ____PrimitiveSet__Type__GLenum,
	                           "",
	                           "");
	I_ConstructorWithDefaults2(IN, const osg::PrimitiveSet &, prim, , IN, const osg::CopyOp &, copyop, osg::CopyOp::SHALLOW_COPY,
	                           ____PrimitiveSet__C5_PrimitiveSet_R1__C5_CopyOp_R1,
	                           "",
	                           "");
	I_Method1(bool, isSameKindAs, IN, const osg::Object *, obj,
	          __bool__isSameKindAs__C5_Object_P1,
	          "",
	          "");
	I_Method0(const char *, libraryName,
	          __C5_char_P1__libraryName,
	          "return the name of the object's library. ",
	          "Must be defined by derived classes. The OpenSceneGraph convention is that the namespace of a library is the same as the library name. ");
	I_Method0(const char *, className,
	          __C5_char_P1__className,
	          "return the name of the object's class type. ",
	          "Must be defined by derived classes. ");
	I_Method0(osg::PrimitiveSet::Type, getType,
	          __Type__getType,
	          "",
	          "");
	I_Method0(const GLvoid *, getDataPointer,
	          __C5_GLvoid_P1__getDataPointer,
	          "",
	          "");
	I_Method0(unsigned int, getTotalDataSize,
	          __unsigned_int__getTotalDataSize,
	          "",
	          "");
	I_Method0(bool, supportsBufferObject,
	          __bool__supportsBufferObject,
	          "",
	          "");
	I_Method1(void, setMode, IN, GLenum, mode,
	          __void__setMode__GLenum,
	          "",
	          "");
	I_Method0(GLenum, getMode,
	          __GLenum__getMode,
	          "",
	          "");
	I_Method2(void, draw, IN, osg::State &, state, IN, bool, useVertexBufferObjects,
	          __void__draw__State_R1__bool,
	          "",
	          "");
	I_Method1(void, accept, IN, osg::PrimitiveFunctor &, functor,
	          __void__accept__PrimitiveFunctor_R1,
	          "",
	          "");
	I_Method1(void, accept, IN, osg::PrimitiveIndexFunctor &, functor,
	          __void__accept__PrimitiveIndexFunctor_R1,
	          "",
	          "");
	I_Method1(unsigned int, index, IN, unsigned int, pos,
	          __unsigned_int__index__unsigned_int,
	          "",
	          "");
	I_Method0(unsigned int, getNumIndices,
	          __unsigned_int__getNumIndices,
	          "",
	          "");
	I_Method1(void, offsetIndices, IN, int, offset,
	          __void__offsetIndices__int,
	          "",
	          "");
	I_Method0(unsigned int, getNumPrimitives,
	          __unsigned_int__getNumPrimitives,
	          "",
	          "");
	I_Method0(void, dirty,
	          __void__dirty,
	          "Dirty the primitive, which increments the modified count, to force buffer objects to update. ",
	          "");
	I_Method1(void, setModifiedCount, IN, unsigned int, value,
	          __void__setModifiedCount__unsigned_int,
	          "Set the modified count value. ",
	          "");
	I_Method0(unsigned int, getModifiedCount,
	          __unsigned_int__getModifiedCount,
	          "Get modified count value. ",
	          "");
	I_MethodWithDefaults1(void, releaseGLObjects, IN, osg::State *, x, 0,
	                      __void__releaseGLObjects__State_P1,
	                      "If State is non-zero, this function releases OpenGL objects for the specified graphics context. ",
	                      "Otherwise, releases OpenGL objexts for all graphics contexts. ");
	I_SimpleProperty(const GLvoid *, DataPointer, 
	                 __C5_GLvoid_P1__getDataPointer, 
	                 0);
	I_SimpleProperty(GLenum, Mode, 
	                 __GLenum__getMode, 
	                 __void__setMode__GLenum);
	I_SimpleProperty(unsigned int, ModifiedCount, 
	                 __unsigned_int__getModifiedCount, 
	                 __void__setModifiedCount__unsigned_int);
	I_SimpleProperty(unsigned int, TotalDataSize, 
	                 __unsigned_int__getTotalDataSize, 
	                 0);
	I_SimpleProperty(osg::PrimitiveSet::Type, Type, 
	                 __Type__getType, 
	                 0);
END_REFLECTOR

BEGIN_OBJECT_REFLECTOR(osg::VectorGLsizei)
	I_BaseType(std::vector<GLsizei>);
	I_Constructor0(____VectorGLsizei,
	               "",
	               "");
	I_Constructor1(IN, const osg::VectorGLsizei &, copy,
	               ____VectorGLsizei__C5_VectorGLsizei_R1,
	               "",
	               "");
	I_Constructor2(IN, GLsizei *, beg, IN, GLsizei *, end,
	               ____VectorGLsizei__GLsizei_P1__GLsizei_P1,
	               "",
	               "");
	I_Constructor1(IN, unsigned int, n,
	               ____VectorGLsizei__unsigned_int,
	               "",
	               "");
END_REFLECTOR

BEGIN_OBJECT_REFLECTOR(osg::VectorGLubyte)
	I_BaseType(std::vector<GLubyte>);
	I_Constructor0(____VectorGLubyte,
	               "",
	               "");
	I_Constructor1(IN, const osg::VectorGLubyte &, copy,
	               ____VectorGLubyte__C5_VectorGLubyte_R1,
	               "",
	               "");
	I_Constructor2(IN, GLubyte *, beg, IN, GLubyte *, end,
	               ____VectorGLubyte__GLubyte_P1__GLubyte_P1,
	               "",
	               "");
	I_Constructor1(IN, unsigned int, n,
	               ____VectorGLubyte__unsigned_int,
	               "",
	               "");
END_REFLECTOR

BEGIN_OBJECT_REFLECTOR(osg::VectorGLuint)
	I_BaseType(std::vector<GLuint>);
	I_Constructor0(____VectorGLuint,
	               "",
	               "");
	I_Constructor1(IN, const osg::VectorGLuint &, copy,
	               ____VectorGLuint__C5_VectorGLuint_R1,
	               "",
	               "");
	I_Constructor2(IN, GLuint *, beg, IN, GLuint *, end,
	               ____VectorGLuint__GLuint_P1__GLuint_P1,
	               "",
	               "");
	I_Constructor1(IN, unsigned int, n,
	               ____VectorGLuint__unsigned_int,
	               "",
	               "");
END_REFLECTOR

BEGIN_OBJECT_REFLECTOR(osg::VectorGLushort)
	I_BaseType(std::vector<GLushort>);
	I_Constructor0(____VectorGLushort,
	               "",
	               "");
	I_Constructor1(IN, const osg::VectorGLushort &, copy,
	               ____VectorGLushort__C5_VectorGLushort_R1,
	               "",
	               "");
	I_Constructor2(IN, GLushort *, beg, IN, GLushort *, end,
	               ____VectorGLushort__GLushort_P1__GLushort_P1,
	               "",
	               "");
	I_Constructor1(IN, unsigned int, n,
	               ____VectorGLushort__unsigned_int,
	               "",
	               "");
END_REFLECTOR

