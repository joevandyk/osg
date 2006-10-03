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

#include <osg/ArgumentParser>
#include <osg/CullSettings>
#include <osg/Matrixd>
#include <osg/Matrixf>
#include <osg/Node>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

TYPE_NAME_ALIAS(unsigned int, osg::CullSettings::CullingMode);

BEGIN_ENUM_REFLECTOR(osg::CullSettings::VariablesMask)
	I_EnumLabel(osg::CullSettings::COMPUTE_NEAR_FAR_MODE);
	I_EnumLabel(osg::CullSettings::CULLING_MODE);
	I_EnumLabel(osg::CullSettings::LOD_SCALE);
	I_EnumLabel(osg::CullSettings::SMALL_FEATURE_CULLING_PIXEL_SIZE);
	I_EnumLabel(osg::CullSettings::CLAMP_PROJECTION_MATRIX_CALLBACK);
	I_EnumLabel(osg::CullSettings::NEAR_FAR_RATIO);
	I_EnumLabel(osg::CullSettings::IMPOSTOR_ACTIVE);
	I_EnumLabel(osg::CullSettings::DEPTH_SORT_IMPOSTOR_SPRITES);
	I_EnumLabel(osg::CullSettings::IMPOSTOR_PIXEL_ERROR_THRESHOLD);
	I_EnumLabel(osg::CullSettings::NUM_FRAMES_TO_KEEP_IMPOSTORS_SPRITES);
	I_EnumLabel(osg::CullSettings::CULL_MASK);
	I_EnumLabel(osg::CullSettings::CULL_MASK_LEFT);
	I_EnumLabel(osg::CullSettings::CULL_MASK_RIGHT);
	I_EnumLabel(osg::CullSettings::CLEAR_COLOR);
	I_EnumLabel(osg::CullSettings::NO_VARIABLES);
	I_EnumLabel(osg::CullSettings::ALL_VARIABLES);
END_REFLECTOR

BEGIN_ENUM_REFLECTOR(osg::CullSettings::ComputeNearFarMode)
	I_EnumLabel(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
	I_EnumLabel(osg::CullSettings::COMPUTE_NEAR_FAR_USING_BOUNDING_VOLUMES);
	I_EnumLabel(osg::CullSettings::COMPUTE_NEAR_FAR_USING_PRIMITIVES);
END_REFLECTOR

BEGIN_ENUM_REFLECTOR(osg::CullSettings::CullingModeValues)
	I_EnumLabel(osg::CullSettings::NO_CULLING);
	I_EnumLabel(osg::CullSettings::VIEW_FRUSTUM_SIDES_CULLING);
	I_EnumLabel(osg::CullSettings::NEAR_PLANE_CULLING);
	I_EnumLabel(osg::CullSettings::FAR_PLANE_CULLING);
	I_EnumLabel(osg::CullSettings::VIEW_FRUSTUM_CULLING);
	I_EnumLabel(osg::CullSettings::SMALL_FEATURE_CULLING);
	I_EnumLabel(osg::CullSettings::SHADOW_OCCLUSION_CULLING);
	I_EnumLabel(osg::CullSettings::CLUSTER_CULLING);
	I_EnumLabel(osg::CullSettings::DEFAULT_CULLING);
	I_EnumLabel(osg::CullSettings::ENABLE_ALL_CULLING);
END_REFLECTOR

BEGIN_VALUE_REFLECTOR(osg::CullSettings)
	I_Constructor0();
	I_Constructor1(IN, osg::ArgumentParser &, arguments);
	I_Constructor1(IN, const osg::CullSettings &, cs);
	I_Method0(void, setDefaults);
	I_Method1(void, setInheritanceMask, IN, unsigned int, mask);
	I_Method0(unsigned int, getInheritanceMask);
	I_Method1(void, setCullSettings, IN, const osg::CullSettings &, settings);
	I_Method1(void, inheritCullSettings, IN, const osg::CullSettings &, settings);
	I_Method2(void, inheritCullSettings, IN, const osg::CullSettings &, settings, IN, unsigned int, inheritanceMask);
	I_Method0(void, readEnvironmentalVariables);
	I_Method1(void, readCommandLine, IN, osg::ArgumentParser &, arguments);
	I_Method1(void, setImpostorsActive, IN, bool, active);
	I_Method0(bool, getImpostorsActive);
	I_Method1(void, setImpostorPixelErrorThreshold, IN, float, numPixels);
	I_Method0(float, getImpostorPixelErrorThreshold);
	I_Method1(void, setDepthSortImpostorSprites, IN, bool, doDepthSort);
	I_Method0(bool, getDepthSortImpostorSprites);
	I_Method1(void, setNumberOfFrameToKeepImpostorSprites, IN, int, numFrames);
	I_Method0(int, getNumberOfFrameToKeepImpostorSprites);
	I_Method1(void, setComputeNearFarMode, IN, osg::CullSettings::ComputeNearFarMode, cnfm);
	I_Method0(osg::CullSettings::ComputeNearFarMode, getComputeNearFarMode);
	I_Method1(void, setNearFarRatio, IN, double, ratio);
	I_Method0(double, getNearFarRatio);
	I_Method1(void, setCullingMode, IN, osg::CullSettings::CullingMode, mode);
	I_Method0(osg::CullSettings::CullingMode, getCullingMode);
	I_Method1(void, setCullMask, IN, osg::Node::NodeMask, nm);
	I_Method0(osg::Node::NodeMask, getCullMask);
	I_Method1(void, setCullMaskLeft, IN, osg::Node::NodeMask, nm);
	I_Method0(osg::Node::NodeMask, getCullMaskLeft);
	I_Method1(void, setCullMaskRight, IN, osg::Node::NodeMask, nm);
	I_Method0(osg::Node::NodeMask, getCullMaskRight);
	I_Method1(void, setLODScale, IN, float, bias);
	I_Method0(float, getLODScale);
	I_Method1(void, setSmallFeatureCullingPixelSize, IN, float, value);
	I_Method0(float, getSmallFeatureCullingPixelSize);
	I_Method1(void, setClampProjectionMatrixCallback, IN, osg::CullSettings::ClampProjectionMatrixCallback *, cpmc);
	I_Method0(osg::CullSettings::ClampProjectionMatrixCallback *, getClampProjectionMatrixCallback);
	I_Method0(const osg::CullSettings::ClampProjectionMatrixCallback *, getClampProjectionMatrixCallback);
	I_Property(osg::CullSettings::ClampProjectionMatrixCallback *, ClampProjectionMatrixCallback);
	I_Property(osg::CullSettings::ComputeNearFarMode, ComputeNearFarMode);
	I_Property(osg::Node::NodeMask, CullMask);
	I_Property(osg::Node::NodeMask, CullMaskLeft);
	I_Property(osg::Node::NodeMask, CullMaskRight);
	I_WriteOnlyProperty(const osg::CullSettings &, CullSettings);
	I_Property(osg::CullSettings::CullingMode, CullingMode);
	I_Property(bool, DepthSortImpostorSprites);
	I_Property(float, ImpostorPixelErrorThreshold);
	I_Property(bool, ImpostorsActive);
	I_Property(unsigned int, InheritanceMask);
	I_Property(float, LODScale);
	I_Property(double, NearFarRatio);
	I_Property(int, NumberOfFrameToKeepImpostorSprites);
	I_Property(float, SmallFeatureCullingPixelSize);
END_REFLECTOR

BEGIN_ABSTRACT_OBJECT_REFLECTOR(osg::CullSettings::ClampProjectionMatrixCallback)
	I_BaseType(osg::Referenced);
	I_Constructor0();
	I_Method3(bool, clampProjectionMatrixImplementation, IN, osg::Matrixf &, projection, IN, double &, znear, IN, double &, zfar);
	I_Method3(bool, clampProjectionMatrixImplementation, IN, osg::Matrixd &, projection, IN, double &, znear, IN, double &, zfar);
END_REFLECTOR

