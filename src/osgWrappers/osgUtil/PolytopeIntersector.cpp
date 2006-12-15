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

#include <osg/Drawable>
#include <osg/Node>
#include <osg/Polytope>
#include <osgUtil/IntersectionVisitor>
#include <osgUtil/PolytopeIntersector>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

TYPE_NAME_ALIAS(std::set< osgUtil::PolytopeIntersector::Intersection >, osgUtil::PolytopeIntersector::Intersections);

BEGIN_OBJECT_REFLECTOR(osgUtil::PolytopeIntersector)
	I_BaseType(osgUtil::Intersector);
	I_Constructor1(IN, const osg::Polytope &, polytope,
	               ____PolytopeIntersector__C5_osg_Polytope_R1,
	               "Construct a PolytopeIntersector using speified polytope in MODEL coordinates. ",
	               "");
	I_Constructor2(IN, osgUtil::Intersector::CoordinateFrame, cf, IN, const osg::Polytope &, polytope,
	               ____PolytopeIntersector__CoordinateFrame__C5_osg_Polytope_R1,
	               "Construct a PolytopeIntersector using speified polytope in specified coordinate frame. ",
	               "");
	I_Constructor5(IN, osgUtil::Intersector::CoordinateFrame, cf, IN, double, xMin, IN, double, yMin, IN, double, xMax, IN, double, yMax,
	               ____PolytopeIntersector__CoordinateFrame__double__double__double__double,
	               "Convinience constructor for supporting picking in WINDOW, or PROJECTION coorindates In WINDOW coordinates (clip space cube) creates a five sided polytope box that has a front face at 0.0 and sides around box xMin, yMin, xMax, yMax. ",
	               "In PROJECTION coordinates (clip space cube) creates a five sided polytope box that has a front face at -1 and sides around box xMin, yMin, xMax, yMax. In VIEW and MODEL coordinates (clip space cube) creates a five sided polytope box that has a front face at 0.0 and sides around box xMin, yMin, xMax, yMax. ");
	I_Method1(void, insertIntersection, IN, const osgUtil::PolytopeIntersector::Intersection &, intersection,
	          __void__insertIntersection__C5_Intersection_R1,
	          "",
	          "");
	I_Method0(osgUtil::PolytopeIntersector::Intersections &, getIntersections,
	          __Intersections_R1__getIntersections,
	          "",
	          "");
	I_Method0(osgUtil::PolytopeIntersector::Intersection, getFirstIntersection,
	          __Intersection__getFirstIntersection,
	          "",
	          "");
	I_Method1(osgUtil::Intersector *, clone, IN, osgUtil::IntersectionVisitor &, iv,
	          __Intersector_P1__clone__osgUtil_IntersectionVisitor_R1,
	          "",
	          "");
	I_Method1(bool, enter, IN, const osg::Node &, node,
	          __bool__enter__C5_osg_Node_R1,
	          "",
	          "");
	I_Method0(void, leave,
	          __void__leave,
	          "",
	          "");
	I_Method2(void, intersect, IN, osgUtil::IntersectionVisitor &, iv, IN, osg::Drawable *, drawable,
	          __void__intersect__osgUtil_IntersectionVisitor_R1__osg_Drawable_P1,
	          "",
	          "");
	I_Method0(void, reset,
	          __void__reset,
	          "",
	          "");
	I_Method0(bool, containsIntersections,
	          __bool__containsIntersections,
	          "",
	          "");
	I_SimpleProperty(osgUtil::PolytopeIntersector::Intersection, FirstIntersection, 
	                 __Intersection__getFirstIntersection, 
	                 0);
	I_SimpleProperty(osgUtil::PolytopeIntersector::Intersections &, Intersections, 
	                 __Intersections_R1__getIntersections, 
	                 0);
END_REFLECTOR

BEGIN_VALUE_REFLECTOR(osgUtil::PolytopeIntersector::Intersection)
	I_Constructor0(____Intersection,
	               "",
	               "");
	I_PublicMemberProperty(osg::NodePath, nodePath);
	I_PublicMemberProperty(osg::ref_ptr< osg::Drawable >, drawable);
END_REFLECTOR

STD_SET_REFLECTOR(std::set< osgUtil::PolytopeIntersector::Intersection >);
