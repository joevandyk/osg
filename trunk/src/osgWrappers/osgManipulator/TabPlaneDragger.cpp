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

#include <osg/Vec4>
#include <osgGA/GUIActionAdapter>
#include <osgGA/GUIEventAdapter>
#include <osgManipulator/Dragger>
#include <osgManipulator/TabPlaneDragger>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

BEGIN_OBJECT_REFLECTOR(osgManipulator::TabPlaneDragger)
	I_BaseType(osgManipulator::CompositeDragger);
	I_Constructor0(____TabPlaneDragger,
	               "",
	               "");
	I_Method3(bool, handle, IN, const osgManipulator::PointerInfo &, pi, IN, const osgGA::GUIEventAdapter &, ea, IN, osgGA::GUIActionAdapter &, us,
	          Properties::VIRTUAL,
	          __bool__handle__C5_PointerInfo_R1__C5_osgGA_GUIEventAdapter_R1__osgGA_GUIActionAdapter_R1,
	          "",
	          "");
	I_MethodWithDefaults1(void, setupDefaultGeometry, IN, bool, twoSidedHandle, true,
	                      Properties::NON_VIRTUAL,
	                      __void__setupDefaultGeometry__bool,
	                      "Setup default geometry for dragger. ",
	                      "");
	I_Method1(void, setPlaneColor, IN, const osg::Vec4 &, color,
	          Properties::NON_VIRTUAL,
	          __void__setPlaneColor__C5_osg_Vec4_R1,
	          "",
	          "");
	I_SimpleProperty(const osg::Vec4 &, PlaneColor, 
	                 0, 
	                 __void__setPlaneColor__C5_osg_Vec4_R1);
END_REFLECTOR

