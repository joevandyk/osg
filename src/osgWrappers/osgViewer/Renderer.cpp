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

#include <osg/Camera>
#include <osg/GraphicsContext>
#include <osg/Object>
#include <osgUtil/SceneView>
#include <osgViewer/Renderer.orig>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

BEGIN_OBJECT_REFLECTOR(osgViewer::Renderer)
	I_DeclaringFile("osgViewer/Renderer.orig");
	I_BaseType(osg::GraphicsOperation);
	I_BaseType(osgViewer::OpenGLQuerySupport);
	I_BaseType(osg::GraphicsOperation);
	I_BaseType(osgViewer::OpenGLQuerySupport);
	I_Constructor1(IN, osg::Camera *, camera,
	               Properties::NON_EXPLICIT,
	               ____Renderer__osg_Camera_P1,
	               "",
	               "");
	I_Method1(osgUtil::SceneView *, getSceneView, IN, unsigned int, i,
	          Properties::NON_VIRTUAL,
	          __osgUtil_SceneView_P1__getSceneView__unsigned_int,
	          "",
	          "");
	I_Method1(void, setDone, IN, bool, done,
	          Properties::NON_VIRTUAL,
	          __void__setDone__bool,
	          "",
	          "");
	I_Method0(bool, getDone,
	          Properties::NON_VIRTUAL,
	          __bool__getDone,
	          "",
	          "");
	I_Method1(void, setGraphicsThreadDoesCull, IN, bool, flag,
	          Properties::NON_VIRTUAL,
	          __void__setGraphicsThreadDoesCull__bool,
	          "",
	          "");
	I_Method0(bool, getGraphicsThreadDoesCull,
	          Properties::NON_VIRTUAL,
	          __bool__getGraphicsThreadDoesCull,
	          "",
	          "");
	I_Method0(void, cull,
	          Properties::NON_VIRTUAL,
	          __void__cull,
	          "",
	          "");
	I_Method0(void, draw,
	          Properties::NON_VIRTUAL,
	          __void__draw,
	          "",
	          "");
	I_Method0(void, cull_draw,
	          Properties::NON_VIRTUAL,
	          __void__cull_draw,
	          "",
	          "");
	I_Method0(void, release,
	          Properties::VIRTUAL,
	          __void__release,
	          "if this operation is a barrier then release it. ",
	          "");
	I_Constructor1(IN, osg::Camera *, camera,
	               Properties::NON_EXPLICIT,
	               ____Renderer__osg_Camera_P1,
	               "",
	               "");
	I_Method1(osgUtil::SceneView *, getSceneView, IN, unsigned int, i,
	          Properties::NON_VIRTUAL,
	          __osgUtil_SceneView_P1__getSceneView__unsigned_int,
	          "",
	          "");
	I_Method1(void, setDone, IN, bool, done,
	          Properties::NON_VIRTUAL,
	          __void__setDone__bool,
	          "",
	          "");
	I_Method0(bool, getDone,
	          Properties::NON_VIRTUAL,
	          __bool__getDone,
	          "",
	          "");
	I_Method1(void, setGraphicsThreadDoesCull, IN, bool, flag,
	          Properties::NON_VIRTUAL,
	          __void__setGraphicsThreadDoesCull__bool,
	          "",
	          "");
	I_Method0(bool, getGraphicsThreadDoesCull,
	          Properties::NON_VIRTUAL,
	          __bool__getGraphicsThreadDoesCull,
	          "",
	          "");
	I_Method0(void, cull,
	          Properties::NON_VIRTUAL,
	          __void__cull,
	          "",
	          "");
	I_Method0(void, draw,
	          Properties::NON_VIRTUAL,
	          __void__draw,
	          "",
	          "");
	I_Method0(void, cull_draw,
	          Properties::NON_VIRTUAL,
	          __void__cull_draw,
	          "",
	          "");
	I_Method0(void, release,
	          Properties::VIRTUAL,
	          __void__release,
	          "if this operation is a barrier then release it. ",
	          "");
	I_ProtectedMethod1(void, updateSceneView, IN, osgUtil::SceneView *, sceneView,
	                   Properties::NON_VIRTUAL,
	                   Properties::NON_CONST,
	                   __void__updateSceneView__osgUtil_SceneView_P1,
	                   "",
	                   "");
	I_ProtectedMethod1(void, updateSceneView, IN, osgUtil::SceneView *, sceneView,
	                   Properties::NON_VIRTUAL,
	                   Properties::NON_CONST,
	                   __void__updateSceneView__osgUtil_SceneView_P1,
	                   "",
	                   "");
	I_SimpleProperty(bool, Done, 
	                 __bool__getDone, 
	                 __void__setDone__bool);
	I_SimpleProperty(bool, GraphicsThreadDoesCull, 
	                 __bool__getGraphicsThreadDoesCull, 
	                 __void__setGraphicsThreadDoesCull__bool);
END_REFLECTOR

BEGIN_OBJECT_REFLECTOR(osgViewer::Renderer)
	I_DeclaringFile("osgViewer/Renderer.orig");
	I_BaseType(osg::GraphicsOperation);
	I_BaseType(osgViewer::OpenGLQuerySupport);
	I_BaseType(osg::GraphicsOperation);
	I_BaseType(osgViewer::OpenGLQuerySupport);
	I_Constructor1(IN, osg::Camera *, camera,
	               Properties::NON_EXPLICIT,
	               ____Renderer__osg_Camera_P1,
	               "",
	               "");
	I_Method1(osgUtil::SceneView *, getSceneView, IN, unsigned int, i,
	          Properties::NON_VIRTUAL,
	          __osgUtil_SceneView_P1__getSceneView__unsigned_int,
	          "",
	          "");
	I_Method1(void, setDone, IN, bool, done,
	          Properties::NON_VIRTUAL,
	          __void__setDone__bool,
	          "",
	          "");
	I_Method0(bool, getDone,
	          Properties::NON_VIRTUAL,
	          __bool__getDone,
	          "",
	          "");
	I_Method1(void, setGraphicsThreadDoesCull, IN, bool, flag,
	          Properties::NON_VIRTUAL,
	          __void__setGraphicsThreadDoesCull__bool,
	          "",
	          "");
	I_Method0(bool, getGraphicsThreadDoesCull,
	          Properties::NON_VIRTUAL,
	          __bool__getGraphicsThreadDoesCull,
	          "",
	          "");
	I_Method0(void, cull,
	          Properties::NON_VIRTUAL,
	          __void__cull,
	          "",
	          "");
	I_Method0(void, draw,
	          Properties::NON_VIRTUAL,
	          __void__draw,
	          "",
	          "");
	I_Method0(void, cull_draw,
	          Properties::NON_VIRTUAL,
	          __void__cull_draw,
	          "",
	          "");
	I_Method0(void, release,
	          Properties::VIRTUAL,
	          __void__release,
	          "if this operation is a barrier then release it. ",
	          "");
	I_Constructor1(IN, osg::Camera *, camera,
	               Properties::NON_EXPLICIT,
	               ____Renderer__osg_Camera_P1,
	               "",
	               "");
	I_Method1(osgUtil::SceneView *, getSceneView, IN, unsigned int, i,
	          Properties::NON_VIRTUAL,
	          __osgUtil_SceneView_P1__getSceneView__unsigned_int,
	          "",
	          "");
	I_Method1(void, setDone, IN, bool, done,
	          Properties::NON_VIRTUAL,
	          __void__setDone__bool,
	          "",
	          "");
	I_Method0(bool, getDone,
	          Properties::NON_VIRTUAL,
	          __bool__getDone,
	          "",
	          "");
	I_Method1(void, setGraphicsThreadDoesCull, IN, bool, flag,
	          Properties::NON_VIRTUAL,
	          __void__setGraphicsThreadDoesCull__bool,
	          "",
	          "");
	I_Method0(bool, getGraphicsThreadDoesCull,
	          Properties::NON_VIRTUAL,
	          __bool__getGraphicsThreadDoesCull,
	          "",
	          "");
	I_Method0(void, cull,
	          Properties::NON_VIRTUAL,
	          __void__cull,
	          "",
	          "");
	I_Method0(void, draw,
	          Properties::NON_VIRTUAL,
	          __void__draw,
	          "",
	          "");
	I_Method0(void, cull_draw,
	          Properties::NON_VIRTUAL,
	          __void__cull_draw,
	          "",
	          "");
	I_Method0(void, release,
	          Properties::VIRTUAL,
	          __void__release,
	          "if this operation is a barrier then release it. ",
	          "");
	I_ProtectedMethod1(void, updateSceneView, IN, osgUtil::SceneView *, sceneView,
	                   Properties::NON_VIRTUAL,
	                   Properties::NON_CONST,
	                   __void__updateSceneView__osgUtil_SceneView_P1,
	                   "",
	                   "");
	I_ProtectedMethod1(void, updateSceneView, IN, osgUtil::SceneView *, sceneView,
	                   Properties::NON_VIRTUAL,
	                   Properties::NON_CONST,
	                   __void__updateSceneView__osgUtil_SceneView_P1,
	                   "",
	                   "");
	I_SimpleProperty(bool, Done, 
	                 __bool__getDone, 
	                 __void__setDone__bool);
	I_SimpleProperty(bool, GraphicsThreadDoesCull, 
	                 __bool__getGraphicsThreadDoesCull, 
	                 __void__setGraphicsThreadDoesCull__bool);
END_REFLECTOR

