PROJECT(osgCairo)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/etc/")

# Use a debug postfix to distinguish build products. Mostly important on
# Windows, because linking a debug library into a release executable (or
# vice-versa, or just mixing C++ runtime versions) on Windows will lead
# to crashes if the libraries use the C++ runtime.
SET(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "Add a postfix, usually d on windows")

# On GCC, we need to set these compiler flags.
IF(NOT WIN32)
	SET(CMAKE_CXX_FLAGS "-W -Wall -Wno-unused")
ENDIF(NOT WIN32)

INCLUDE(FindPkgConfig)

FIND_PACKAGE(OpenSceneGraph)

PKG_CHECK_MODULES(CAIRO REQUIRED cairo)
PKG_CHECK_MODULES(POPPLER poppler-glib)

SET(SRC_FILES
	src/Image.cpp
	src/Matrix.cpp
	src/Path.cpp
	src/Pattern.cpp
	src/ReadFile.cpp
	src/Surface.cpp
	src/Types.cpp
)

SET(HEADER_FILES
	${osgCairo_SOURCE_DIR}/include/osgCairo/Export
	${osgCairo_SOURCE_DIR}/include/osgCairo/Glyph
	${osgCairo_SOURCE_DIR}/include/osgCairo/Image
	${osgCairo_SOURCE_DIR}/include/osgCairo/Matrix
	${osgCairo_SOURCE_DIR}/include/osgCairo/Path
	${osgCairo_SOURCE_DIR}/include/osgCairo/Pattern
	${osgCairo_SOURCE_DIR}/include/osgCairo/ReadFile
	${osgCairo_SOURCE_DIR}/include/osgCairo/ScaledFont
	${osgCairo_SOURCE_DIR}/include/osgCairo/Surface
	${osgCairo_SOURCE_DIR}/include/osgCairo/Types
)

SET(OSGCAIROWIDGET FALSE CACHE BOOL "Build the osgWidget/osgCairo example?")

INCLUDE_DIRECTORIES(
	include
	${OPENSCENEGRAPH_INCLUDE_DIR}
	${CAIRO_INCLUDE_DIRS}
	${POPPLER_INCLUDE_DIRS}
)

LINK_DIRECTORIES(
	${OPENSCENEGRAPH_LIB_DIR}
	${CAIRO_LIBRARY_DIRS}
	${POPPLER_LIBRARY_DIRS}
)

ADD_LIBRARY(osgCairo SHARED ${SRC_FILES} ${HEADER_FILES})

# Add debug postfix to OSG libraries so we link to the right ones in debug.
# Cairo is a C-only library so the same one (release) can be linked to both
# debug and release without problems.
TARGET_LINK_LIBRARIES(osgCairo
                      debug     OpenThreads${CMAKE_DEBUG_POSTFIX}
                      optimized OpenThreads
                      debug     osg${CMAKE_DEBUG_POSTFIX}
                      optimized osg
                      debug     osgUtil${CMAKE_DEBUG_POSTFIX}
                      optimized osgUtil
                      debug     osgDB${CMAKE_DEBUG_POSTFIX}
                      optimized osgDB
                      debug     osgViewer${CMAKE_DEBUG_POSTFIX}
                      optimized osgViewer
                      general   cairo)

# The archive/runtime part is so that the lib/dll pair goes into lib and bin
# respectively on Windows.
INSTALL(TARGETS osgCairo
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

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

IF(POPPLER_FOUND)
	TARGET_LINK_LIBRARIES(osgCairo poppler poppler-glib)
	ADD_DEFINITIONS(-DOSGCAIRO_POPPLER)
	ADD_SUBDIRECTORY(examples/osgcairopdf)
ENDIF(POPPLER_FOUND)

CONFIGURE_FILE(
	"${CMAKE_CURRENT_SOURCE_DIR}/etc/uninstall.cmake"
	"${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake"
	IMMEDIATE @ONLY
)

ADD_CUSTOM_TARGET(uninstall
	"${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake"
)

# Packaging information.
SET(CPACK_GENERATOR TGZ)
SET(CPACK_SOURCE_GENERATOR TGZ)
SET(CPACK_SOURCE_IGNORE_FILES
	"~$"
	".*.svn"
	".*build/"
	".*debug/"
	".*release/"
)

SET(OSGCAIRO_VERSION "0.1.0")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "osgCairo-${OSGCAIRO_VERSION}")

INCLUDE(CPack)
