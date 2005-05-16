// ***************************************************************************
//
//   Generated automatically by genwrapper.
//   Please DO NOT EDIT this file!
//
// ***************************************************************************

#include <osgIntrospection/ReflectionMacros>
#include <osgIntrospection/TypedMethodInfo>
#include <osgIntrospection/Attributes>

#include <osg/CollectOccludersVisitor>
#include <osg/DisplaySettings>
#include <osg/FrameStamp>
#include <osg/Light>
#include <osg/Matrixd>
#include <osg/Matrixf>
#include <osg/Node>
#include <osg/NodeVisitor>
#include <osg/State>
#include <osg/StateSet>
#include <osg/Vec3>
#include <osg/Vec4>
#include <osg/Viewport>
#include <osgUtil/CullVisitor>
#include <osgUtil/RenderGraph>
#include <osgUtil/RenderStage>
#include <osgUtil/SceneView>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

TYPE_NAME_ALIAS(osgUtil::SceneView::Options, osgUtil::SceneView::LightingMode);

BEGIN_ENUM_REFLECTOR(osgUtil::SceneView::Options)
	I_EnumLabel(osgUtil::SceneView::NO_SCENEVIEW_LIGHT);
	I_EnumLabel(osgUtil::SceneView::HEADLIGHT);
	I_EnumLabel(osgUtil::SceneView::SKY_LIGHT);
	I_EnumLabel(osgUtil::SceneView::COMPILE_GLOBJECTS_AT_INIT);
	I_EnumLabel(osgUtil::SceneView::STANDARD_SETTINGS);
END_REFLECTOR

BEGIN_ENUM_REFLECTOR(osgUtil::SceneView::ActiveUniforms)
	I_EnumLabel(osgUtil::SceneView::FRAME_NUMBER_UNIFORM);
	I_EnumLabel(osgUtil::SceneView::FRAME_TIME_UNIFORM);
	I_EnumLabel(osgUtil::SceneView::DELTA_FRAME_TIME_UNIFORM);
	I_EnumLabel(osgUtil::SceneView::VIEW_MATRIX_UNIFORM);
	I_EnumLabel(osgUtil::SceneView::INVERSE_VIEW_MATRIX_UNIFORM);
	I_EnumLabel(osgUtil::SceneView::DEFAULT_UNIFORMS);
	I_EnumLabel(osgUtil::SceneView::ALL_UNIFORMS);
END_REFLECTOR

BEGIN_ENUM_REFLECTOR(osgUtil::SceneView::FusionDistanceMode)
	I_EnumLabel(osgUtil::SceneView::USE_FUSION_DISTANCE_VALUE);
	I_EnumLabel(osgUtil::SceneView::PROPORTIONAL_TO_SCREEN_DISTANCE);
END_REFLECTOR

BEGIN_OBJECT_REFLECTOR(osgUtil::SceneView)
	I_BaseType(osg::Referenced);
	I_BaseType(osg::CullSettings);
	I_ConstructorWithDefaults1(IN, osg::DisplaySettings *, ds, NULL);
	I_MethodWithDefaults1(void, setDefaults, IN, unsigned int, options, osgUtil::SceneView::STANDARD_SETTINGS);
	I_Method1(void, setSceneData, IN, osg::Node *, node);
	I_Method0(osg::Node *, getSceneData);
	I_Method0(const osg::Node *, getSceneData);
	I_Method1(void, setViewport, IN, osg::Viewport *, viewport);
	I_Method4(void, setViewport, IN, int, x, IN, int, y, IN, int, width, IN, int, height);
	I_Method0(const osg::Viewport *, getViewport);
	I_Method0(osg::Viewport *, getViewport);
	I_Method4(void, getViewport, IN, int &, x, IN, int &, y, IN, int &, width, IN, int &, height);
	I_Method1(void, setDisplaySettings, IN, osg::DisplaySettings *, vs);
	I_Method0(const osg::DisplaySettings *, getDisplaySettings);
	I_Method0(osg::DisplaySettings *, getDisplaySettings);
	I_Method1(void, setClearColor, IN, const osg::Vec4 &, color);
	I_Method0(const osg::Vec4 &, getClearColor);
	I_Method1(void, setGlobalStateSet, IN, osg::StateSet *, state);
	I_Method0(osg::StateSet *, getGlobalStateSet);
	I_Method0(const osg::StateSet *, getGlobalStateSet);
	I_Method1(void, setLocalStateSet, IN, osg::StateSet *, state);
	I_Method0(osg::StateSet *, getLocalStateSet);
	I_Method0(const osg::StateSet *, getLocalStateSet);
	I_Method1(void, setActiveUniforms, IN, int, activeUniforms);
	I_Method1(int, getActiveUniforms, IN, int, activeUniforms);
	I_Method0(void, updateUniforms);
	I_Method1(void, setLightingMode, IN, osgUtil::SceneView::LightingMode, mode);
	I_Method0(osgUtil::SceneView::LightingMode, getLightingMode);
	I_Method1(void, setLight, IN, osg::Light *, light);
	I_Method0(osg::Light *, getLight);
	I_Method0(const osg::Light *, getLight);
	I_Method1(void, setState, IN, osg::State *, state);
	I_Method0(osg::State *, getState);
	I_Method0(const osg::State *, getState);
	I_Method1(void, setProjectionMatrix, IN, const osg::Matrixf &, matrix);
	I_Method1(void, setProjectionMatrix, IN, const osg::Matrixd &, matrix);
	I_Method6(void, setProjectionMatrixAsOrtho, IN, double, left, IN, double, right, IN, double, bottom, IN, double, top, IN, double, zNear, IN, double, zFar);
	I_Method4(void, setProjectionMatrixAsOrtho2D, IN, double, left, IN, double, right, IN, double, bottom, IN, double, top);
	I_Method6(void, setProjectionMatrixAsFrustum, IN, double, left, IN, double, right, IN, double, bottom, IN, double, top, IN, double, zNear, IN, double, zFar);
	I_Method4(void, setProjectionMatrixAsPerspective, IN, double, fovy, IN, double, aspectRatio, IN, double, zNear, IN, double, zFar);
	I_Method0(osg::Matrixd &, getProjectionMatrix);
	I_Method0(const osg::Matrixd &, getProjectionMatrix);
	I_Method6(bool, getProjectionMatrixAsOrtho, IN, double &, left, IN, double &, right, IN, double &, bottom, IN, double &, top, IN, double &, zNear, IN, double &, zFar);
	I_Method6(bool, getProjectionMatrixAsFrustum, IN, double &, left, IN, double &, right, IN, double &, bottom, IN, double &, top, IN, double &, zNear, IN, double &, zFar);
	I_Method4(bool, getProjectionMatrixAsPerspective, IN, double &, fovy, IN, double &, aspectRatio, IN, double &, zNear, IN, double &, zFar);
	I_Method1(void, setViewMatrix, IN, const osg::Matrixf &, matrix);
	I_Method1(void, setViewMatrix, IN, const osg::Matrixd &, matrix);
	I_Method3(void, setViewMatrixAsLookAt, IN, const osg::Vec3 &, eye, IN, const osg::Vec3 &, center, IN, const osg::Vec3 &, up);
	I_Method0(osg::Matrixd &, getViewMatrix);
	I_Method0(const osg::Matrixd &, getViewMatrix);
	I_MethodWithDefaults4(void, getViewMatrixAsLookAt, IN, osg::Vec3 &, eye, , IN, osg::Vec3 &, center, , IN, osg::Vec3 &, up, , IN, float, lookDistance, 1.0f);
	I_Method1(void, setInitVisitor, IN, osg::NodeVisitor *, av);
	I_Method0(osg::NodeVisitor *, getInitVisitor);
	I_Method0(const osg::NodeVisitor *, getInitVisitor);
	I_Method1(void, setUpdateVisitor, IN, osg::NodeVisitor *, av);
	I_Method0(osg::NodeVisitor *, getUpdateVisitor);
	I_Method0(const osg::NodeVisitor *, getUpdateVisitor);
	I_Method1(void, setCullVisitor, IN, osgUtil::CullVisitor *, cv);
	I_Method0(osgUtil::CullVisitor *, getCullVisitor);
	I_Method0(const osgUtil::CullVisitor *, getCullVisitor);
	I_Method1(void, setCullVisitorLeft, IN, osgUtil::CullVisitor *, cv);
	I_Method0(osgUtil::CullVisitor *, getCullVisitorLeft);
	I_Method0(const osgUtil::CullVisitor *, getCullVisitorLeft);
	I_Method1(void, setCullVisitorRight, IN, osgUtil::CullVisitor *, cv);
	I_Method0(osgUtil::CullVisitor *, getCullVisitorRight);
	I_Method0(const osgUtil::CullVisitor *, getCullVisitorRight);
	I_Method1(void, setCollectOccludersVisitor, IN, osg::CollectOccludersVisitor *, cov);
	I_Method0(osg::CollectOccludersVisitor *, getCollectOccludersVisitor);
	I_Method0(const osg::CollectOccludersVisitor *, getCollectOccludersVisitor);
	I_Method1(void, setRenderGraph, IN, osgUtil::RenderGraph *, rg);
	I_Method0(osgUtil::RenderGraph *, getRenderGraph);
	I_Method0(const osgUtil::RenderGraph *, getRenderGraph);
	I_Method1(void, setRenderGraphLeft, IN, osgUtil::RenderGraph *, rg);
	I_Method0(osgUtil::RenderGraph *, getRenderGraphLeft);
	I_Method0(const osgUtil::RenderGraph *, getRenderGraphLeft);
	I_Method1(void, setRenderGraphRight, IN, osgUtil::RenderGraph *, rg);
	I_Method0(osgUtil::RenderGraph *, getRenderGraphRight);
	I_Method0(const osgUtil::RenderGraph *, getRenderGraphRight);
	I_Method1(void, setRenderStage, IN, osgUtil::RenderStage *, rs);
	I_Method0(osgUtil::RenderStage *, getRenderStage);
	I_Method0(const osgUtil::RenderStage *, getRenderStage);
	I_Method1(void, setRenderStageLeft, IN, osgUtil::RenderStage *, rs);
	I_Method0(osgUtil::RenderStage *, getRenderStageLeft);
	I_Method0(const osgUtil::RenderStage *, getRenderStageLeft);
	I_Method1(void, setRenderStageRight, IN, osgUtil::RenderStage *, rs);
	I_Method0(osgUtil::RenderStage *, getRenderStageRight);
	I_Method0(const osgUtil::RenderStage *, getRenderStageRight);
	I_Method1(void, setDrawBufferValue, IN, GLenum, drawBufferValue);
	I_Method0(GLenum, getDrawBufferValue);
	I_MethodWithDefaults2(void, setFusionDistance, IN, osgUtil::SceneView::FusionDistanceMode, mode, , IN, float, value, 1.0f);
	I_Method0(osgUtil::SceneView::FusionDistanceMode, getFusionDistanceMode);
	I_Method0(float, getFusionDistanceValue);
	I_Method1(void, setPrioritizeTextures, IN, bool, pt);
	I_Method0(bool, getPrioritizeTextures);
	I_Method1(void, setComputeStereoMatricesCallback, IN, osgUtil::SceneView::ComputeStereoMatricesCallback *, callback);
	I_Method0(osgUtil::SceneView::ComputeStereoMatricesCallback *, getComputeStereoMatricesCallback);
	I_Method0(const osgUtil::SceneView::ComputeStereoMatricesCallback *, getComputeStereoMatricesCallback);
	I_Method2(bool, projectWindowIntoObject, IN, const osg::Vec3 &, window, IN, osg::Vec3 &, object);
	I_Method4(bool, projectWindowXYIntoObject, IN, int, x, IN, int, y, IN, osg::Vec3 &, near_point, IN, osg::Vec3 &, far_point);
	I_Method2(bool, projectObjectIntoWindow, IN, const osg::Vec3 &, object, IN, osg::Vec3 &, window);
	I_Method1(void, setFrameStamp, IN, osg::FrameStamp *, fs);
	I_Method0(const osg::FrameStamp *, getFrameStamp);
	I_Method1(osg::Matrixd, computeLeftEyeProjection, IN, const osg::Matrixd &, projection);
	I_Method1(osg::Matrixd, computeLeftEyeView, IN, const osg::Matrixd &, view);
	I_Method1(osg::Matrixd, computeRightEyeProjection, IN, const osg::Matrixd &, projection);
	I_Method1(osg::Matrixd, computeRightEyeView, IN, const osg::Matrixd &, view);
	I_Method1(osg::Matrixd, computeLeftEyeProjectionImplementation, IN, const osg::Matrixd &, projection);
	I_Method1(osg::Matrixd, computeLeftEyeViewImplementation, IN, const osg::Matrixd &, view);
	I_Method1(osg::Matrixd, computeRightEyeProjectionImplementation, IN, const osg::Matrixd &, projection);
	I_Method1(osg::Matrixd, computeRightEyeViewImplementation, IN, const osg::Matrixd &, view);
	I_Method0(void, init);
	I_Method0(void, update);
	I_Method0(void, cull);
	I_Method0(void, draw);
	I_Method0(void, releaseAllGLObjects);
	I_Method0(void, flushAllDeletedGLObjects);
	I_Method1(void, flushDeletedGLObjects, IN, double &, availableTime);
	I_WriteOnlyProperty(int, ActiveUniforms);
	I_Property(const osg::Vec4 &, ClearColor);
	I_Property(osg::CollectOccludersVisitor *, CollectOccludersVisitor);
	I_Property(osgUtil::SceneView::ComputeStereoMatricesCallback *, ComputeStereoMatricesCallback);
	I_Property(osgUtil::CullVisitor *, CullVisitor);
	I_Property(osgUtil::CullVisitor *, CullVisitorLeft);
	I_Property(osgUtil::CullVisitor *, CullVisitorRight);
	I_WriteOnlyProperty(unsigned int, Defaults);
	I_Property(osg::DisplaySettings *, DisplaySettings);
	I_Property(GLenum, DrawBufferValue);
	I_WriteOnlyProperty(osg::FrameStamp *, FrameStamp);
	I_ReadOnlyProperty(osgUtil::SceneView::FusionDistanceMode, FusionDistanceMode);
	I_ReadOnlyProperty(float, FusionDistanceValue);
	I_Property(osg::StateSet *, GlobalStateSet);
	I_Property(osg::NodeVisitor *, InitVisitor);
	I_Property(osg::Light *, Light);
	I_Property(osgUtil::SceneView::LightingMode, LightingMode);
	I_Property(osg::StateSet *, LocalStateSet);
	I_Property(bool, PrioritizeTextures);
	I_Property(const osg::Matrixd &, ProjectionMatrix);
	I_Property(osgUtil::RenderGraph *, RenderGraph);
	I_Property(osgUtil::RenderGraph *, RenderGraphLeft);
	I_Property(osgUtil::RenderGraph *, RenderGraphRight);
	I_Property(osgUtil::RenderStage *, RenderStage);
	I_Property(osgUtil::RenderStage *, RenderStageLeft);
	I_Property(osgUtil::RenderStage *, RenderStageRight);
	I_Property(osg::Node *, SceneData);
	I_Property(osg::State *, State);
	I_Property(osg::NodeVisitor *, UpdateVisitor);
	I_Property(const osg::Matrixd &, ViewMatrix);
	I_Property(osg::Viewport *, Viewport);
END_REFLECTOR

BEGIN_ABSTRACT_OBJECT_REFLECTOR(osgUtil::SceneView::ComputeStereoMatricesCallback)
	I_BaseType(osg::Referenced);
	I_Constructor0();
	I_Method1(osg::Matrixd, computeLeftEyeProjection, IN, const osg::Matrixd &, projection);
	I_Method1(osg::Matrixd, computeLeftEyeView, IN, const osg::Matrixd &, view);
	I_Method1(osg::Matrixd, computeRightEyeProjection, IN, const osg::Matrixd &, projection);
	I_Method1(osg::Matrixd, computeRightEyeView, IN, const osg::Matrixd &, view);
END_REFLECTOR

