
# FIXME: For OS X, need flag for Framework or dylib
IF   (DYNAMIC_OPENSCENEGRAPH)
	ADD_DEFINITIONS(-DOSGUTIL_LIBRARY)
ELSE (DYNAMIC_OPENSCENEGRAPH)
	ADD_DEFINITIONS(-DOSG_LIBRARY_STATIC)
ENDIF(DYNAMIC_OPENSCENEGRAPH)

SET(LIB_NAME osgUtil)
SET(HEADER_PATH ${OpenSceneGraph_SOURCE_DIR}/include/${LIB_NAME})
SET(LIB_PUBLIC_HEADERS
	${HEADER_PATH}/CubeMapGenerator
	${HEADER_PATH}/CullVisitor
	${HEADER_PATH}/DelaunayTriangulator
	${HEADER_PATH}/DisplayRequirementsVisitor
	${HEADER_PATH}/Export
	${HEADER_PATH}/GLObjectsVisitor
	${HEADER_PATH}/HalfWayMapGenerator
	${HEADER_PATH}/HighlightMapGenerator
	${HEADER_PATH}/IntersectVisitor
	${HEADER_PATH}/IntersectionVisitor
	${HEADER_PATH}/LineSegmentIntersector
	${HEADER_PATH}/Optimizer
	${HEADER_PATH}/PlaneIntersector
	${HEADER_PATH}/PolytopeIntersector
	${HEADER_PATH}/PositionalStateContainer
	${HEADER_PATH}/ReflectionMapGenerator
	${HEADER_PATH}/RenderBin
	${HEADER_PATH}/RenderLeaf
	${HEADER_PATH}/RenderStage
	${HEADER_PATH}/SceneView
	${HEADER_PATH}/Simplifier
	${HEADER_PATH}/SmoothingVisitor
	${HEADER_PATH}/StateGraph
	${HEADER_PATH}/Statistics
	${HEADER_PATH}/TangentSpaceGenerator
	${HEADER_PATH}/Tessellator
	${HEADER_PATH}/TransformAttributeFunctor
	${HEADER_PATH}/TransformCallback
	${HEADER_PATH}/TriStripVisitor
	${HEADER_PATH}/UpdateVisitor
	${HEADER_PATH}/Version
)

ADD_LIBRARY(${LIB_NAME}
	${OPENSCENEGRAPH_USER_DEFINED_DYNAMIC_OR_STATIC}
	${LIB_PUBLIC_HEADERS}
	CubeMapGenerator.cpp
	CullVisitor.cpp
	DelaunayTriangulator.cpp
	DisplayRequirementsVisitor.cpp
	GLObjectsVisitor.cpp
	HalfWayMapGenerator.cpp
	HighlightMapGenerator.cpp
	IntersectVisitor.cpp
	IntersectionVisitor.cpp
	LineSegmentIntersector.cpp
	Optimizer.cpp
	PlaneIntersector.cpp
	PolytopeIntersector.cpp
	PositionalStateContainer.cpp
	RenderBin.cpp
	RenderLeaf.cpp
	RenderStage.cpp
	SceneView.cpp
	Simplifier.cpp
	SmoothingVisitor.cpp
	StateGraph.cpp
	Statistics.cpp
	TangentSpaceGenerator.cpp
	Tessellator.cpp
	TransformAttributeFunctor.cpp
	TransformCallback.cpp
	TriStripVisitor.cpp
	TriStrip_graph_array.h
	TriStrip_heap_array.h
	TriStrip_tri_stripper.cpp
	TriStrip_tri_stripper.h
	UpdateVisitor.cpp
	Version.cpp
)

TARGET_LINK_LIBRARIES(${LIB_NAME}
	osg
	${OPENTHREADS_LIBRARY}
	${OPENGL_LIBRARIES}
)

INCLUDE(ModuleInstall OPTIONAL)