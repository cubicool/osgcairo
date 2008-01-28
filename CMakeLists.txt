PROJECT(osgCairo)

INCLUDE(FindPkgConfig)

PKG_CHECK_MODULES(CAIRO REQUIRED cairo)

SET(SRC_FILES
	src/Matrix.cpp
	src/Path.cpp
	src/Pattern.cpp
	src/Surface.cpp
	src/SurfaceImage.cpp
	src/Types.cpp
)

SET(HEADER_FILES
	${osgCairo_SOURCE_DIR}/include/osgCairo/Glyph
	${osgCairo_SOURCE_DIR}/include/osgCairo/Matrix
	${osgCairo_SOURCE_DIR}/include/osgCairo/Path
	${osgCairo_SOURCE_DIR}/include/osgCairo/Pattern
	${osgCairo_SOURCE_DIR}/include/osgCairo/Surface
	${osgCairo_SOURCE_DIR}/include/osgCairo/SurfaceImage
	${osgCairo_SOURCE_DIR}/include/osgCairo/Types
)

SET(CMAKE_INCLUDE_PATH /usr/local/include CACHE STRING "Put your OSG include dir here...")
SET(CMAKE_LIBRARY_PATH /usr/local/lib CACHE STRING "Put your OSG lib dir here...")
SET(OSGCAIROWIDGET FALSE CACHE BOOL "Build the osgWidget/osgCairo example?")

INCLUDE_DIRECTORIES(include ${CMAKE_INCLUDE_PATH} ${CAIRO_INCLUDE_DIRS})

LINK_DIRECTORIES(${CMAKE_LIBRARY_PATH} ${CAIRO_LIBRARY_DIRS})

LINK_LIBRARIES(osg osgUtil osgViewer cairo)

ADD_LIBRARY(osgCairo SHARED ${SRC_FILES} ${HEADER_FILES})

INSTALL(TARGETS osgCairo DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

FOREACH(INCLUDEFILE ${HEADER_FILES})
	FILE(RELATIVE_PATH REL_INCLUDEFILE ${osgCairo_SOURCE_DIR}/include/osgCairo ${INCLUDEFILE})
	GET_FILENAME_COMPONENT(REL_INCLUDE_PATH ${REL_INCLUDEFILE} PATH)
	INSTALL(
		FILES ${INCLUDEFILE}
		DESTINATION ${CMAKE_INSTALL_PREFIX}/include/osgCairo/${REL_INCLUDE_PATH}
	)
ENDFOREACH(INCLUDEFILE)

ADD_SUBDIRECTORY(examples/osgcairoviewer)
ADD_SUBDIRECTORY(examples/osgcairomatrix)

IF(OSGCAIROWIDGET)
	ADD_SUBDIRECTORY(examples/osgcairowidget)
ENDIF(OSGCAIROWIDGET)
