
IF   (DYNAMIC_OPENSCENEGRAPH)
	ADD_DEFINITIONS(-DOSGSHADOW_LIBRARY)
ELSE (DYNAMIC_OPENSCENEGRAPH)
	ADD_DEFINITIONS(-DOSG_LIBRARY_STATIC)
ENDIF(DYNAMIC_OPENSCENEGRAPH)

SET(LIB_NAME osgShadow)
SET(HEADER_PATH ${OpenSceneGraph_SOURCE_DIR}/include/${LIB_NAME})
SET(LIB_PUBLIC_HEADERS
	${HEADER_PATH}/Export
	${HEADER_PATH}/OccluderGeometry
	${HEADER_PATH}/ParallelSplitShadowMap
	${HEADER_PATH}/ShadowMap
	${HEADER_PATH}/ShadowTechnique
	${HEADER_PATH}/ShadowTexture
	${HEADER_PATH}/ShadowVolume
	${HEADER_PATH}/ShadowedScene
	${HEADER_PATH}/Version
)

# FIXME: For OS X, need flag for Framework or dylib
ADD_LIBRARY(${LIB_NAME}
	${OPENSCENEGRAPH_USER_DEFINED_DYNAMIC_OR_STATIC}
	${LIB_PUBLIC_HEADERS}
	OccluderGeometry.cpp
	ParallelSplitShadowMap.cpp
	ShadowMap.cpp
	ShadowTechnique.cpp
	ShadowTexture.cpp
	ShadowVolume.cpp
	ShadowedScene.cpp
	Version.cpp
)

TARGET_LINK_LIBRARIES(${LIB_NAME}
	osgUtil
	osgDB
	osg
	${OPENTHREADS_LIBRARY}
	${OPENGL_LIBRARIES}
)

INCLUDE(ModuleInstall OPTIONAL)