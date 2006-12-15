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

#include <osg/Node>
#include <osg/Vec3d>
#include <osgSim/LineOfSight>
#include <osgUtil/IntersectionVisitor>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

BEGIN_OBJECT_REFLECTOR(osgSim::DatabaseCacheReadCallback)
	I_BaseType(osgUtil::IntersectionVisitor::ReadCallback);
	I_Constructor0(____DatabaseCacheReadCallback,
	               "",
	               "");
	I_Method1(void, setMaximumNumOfFilesToCache, IN, unsigned int, maxNumFilesToCache,
	          __void__setMaximumNumOfFilesToCache__unsigned_int,
	          "",
	          "");
	I_Method0(unsigned int, getMaximumNumOfFilesToCache,
	          __unsigned_int__getMaximumNumOfFilesToCache,
	          "",
	          "");
	I_Method0(void, clearDatabaseCache,
	          __void__clearDatabaseCache,
	          "",
	          "");
	I_Method0(void, pruneUnusedDatabaseCache,
	          __void__pruneUnusedDatabaseCache,
	          "",
	          "");
	I_Method1(osg::Node *, readNodeFile, IN, const std::string &, filename,
	          __osg_Node_P1__readNodeFile__C5_std_string_R1,
	          "",
	          "");
	I_SimpleProperty(unsigned int, MaximumNumOfFilesToCache, 
	                 __unsigned_int__getMaximumNumOfFilesToCache, 
	                 __void__setMaximumNumOfFilesToCache__unsigned_int);
END_REFLECTOR

TYPE_NAME_ALIAS(std::vector< osg::Vec3d >, osgSim::LineOfSight::Intersections);

BEGIN_VALUE_REFLECTOR(osgSim::LineOfSight)
	I_Constructor0(____LineOfSight,
	               "",
	               "");
	I_Method0(void, clear,
	          __void__clear,
	          "Clear the internal LOS List so it contains no line of sight tests. ",
	          "");
	I_Method2(unsigned int, addLOS, IN, const osg::Vec3d &, start, IN, const osg::Vec3d &, end,
	          __unsigned_int__addLOS__C5_osg_Vec3d_R1__C5_osg_Vec3d_R1,
	          "Add a line of sight test, consisting of start and end point. ",
	          "Returns the index number of the newly adding LOS test. ");
	I_Method0(unsigned int, getNumLOS,
	          __unsigned_int__getNumLOS,
	          "Get the number of line of sight tests. ",
	          "");
	I_Method2(void, setStartPoint, IN, unsigned int, i, IN, const osg::Vec3d &, start,
	          __void__setStartPoint__unsigned_int__C5_osg_Vec3d_R1,
	          "Set the start point of signel line of sight test. ",
	          "");
	I_Method1(const osg::Vec3d &, getStartPoint, IN, unsigned int, i,
	          __C5_osg_Vec3d_R1__getStartPoint__unsigned_int,
	          "Get the start point of single line of sight test. ",
	          "");
	I_Method2(void, setEndPoint, IN, unsigned int, i, IN, const osg::Vec3d &, end,
	          __void__setEndPoint__unsigned_int__C5_osg_Vec3d_R1,
	          "Set the end point of single line of sight test. ",
	          "");
	I_Method1(const osg::Vec3d &, getEndPoint, IN, unsigned int, i,
	          __C5_osg_Vec3d_R1__getEndPoint__unsigned_int,
	          "Get the end point of single line of sight test. ",
	          "");
	I_Method1(const osgSim::LineOfSight::Intersections &, getIntersections, IN, unsigned int, i,
	          __C5_Intersections_R1__getIntersections__unsigned_int,
	          "Get the intersection points for a single line of sight test. ",
	          "");
	I_Method1(void, computeIntersections, IN, osg::Node *, scene,
	          __void__computeIntersections__osg_Node_P1,
	          "Compute the LOS intersections with the specified scene graph. ",
	          "The results are all stored in the form of Intersections list, one per LOS test. ");
	I_Method0(void, clearDatabaseCache,
	          __void__clearDatabaseCache,
	          "Clear the database cache. ",
	          "");
	I_Method1(void, setDatabaseCacheReadCallback, IN, osgSim::DatabaseCacheReadCallback *, dcrc,
	          __void__setDatabaseCacheReadCallback__DatabaseCacheReadCallback_P1,
	          "Set the ReadCallback that does the reading of external PagedLOD models, and caching of loaded subgraphs. ",
	          "Note, if you have mulitple LineOfSight or HeightAboveTerrain objects in use at one time then you should share a single DatabaseCacheReadCallback between all of them. ");
	I_Method0(osgSim::DatabaseCacheReadCallback *, getDatabaseCacheReadCallback,
	          __DatabaseCacheReadCallback_P1__getDatabaseCacheReadCallback,
	          "Get the ReadCallback that does the reading of external PagedLOD models, and caching of loaded subgraphs. ",
	          "");
	I_Method0(osgUtil::IntersectionVisitor &, getIntersectionVisitor,
	          __osgUtil_IntersectionVisitor_R1__getIntersectionVisitor,
	          "Get the IntersectionVistor that does the intersection traversal over the scene. ",
	          "Note, if you want to customized the traversal then you can use the IntersectionVisitor's method to alter its behavior. ");
	I_StaticMethod3(osgSim::LineOfSight::Intersections, computeIntersections, IN, osg::Node *, scene, IN, const osg::Vec3d &, start, IN, const osg::Vec3d &, end,
	                __Intersections__computeIntersections__osg_Node_P1__C5_osg_Vec3d_R1__C5_osg_Vec3d_R1_S,
	                "Compute the intersection between the specified scene graph and a single LOS start,end pair. ",
	                "Returns an IntersectionList, of all the points intersected. ");
	I_SimpleProperty(osgSim::DatabaseCacheReadCallback *, DatabaseCacheReadCallback, 
	                 __DatabaseCacheReadCallback_P1__getDatabaseCacheReadCallback, 
	                 __void__setDatabaseCacheReadCallback__DatabaseCacheReadCallback_P1);
	I_IndexedProperty(const osg::Vec3d &, EndPoint, 
	                  __C5_osg_Vec3d_R1__getEndPoint__unsigned_int, 
	                  __void__setEndPoint__unsigned_int__C5_osg_Vec3d_R1, 
	                  0);
	I_SimpleProperty(osgUtil::IntersectionVisitor &, IntersectionVisitor, 
	                 __osgUtil_IntersectionVisitor_R1__getIntersectionVisitor, 
	                 0);
	I_IndexedProperty(const osg::Vec3d &, StartPoint, 
	                  __C5_osg_Vec3d_R1__getStartPoint__unsigned_int, 
	                  __void__setStartPoint__unsigned_int__C5_osg_Vec3d_R1, 
	                  0);
END_REFLECTOR
