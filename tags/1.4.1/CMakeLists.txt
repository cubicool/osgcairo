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
	SET(CMAKE_C_FLAGS "-Wall -Wextra -Wno-unused -fPIC")
	SET(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused -fPIC")
ENDIF(NOT WIN32)

IF(NOT WIN32)
	INCLUDE(FindPkgConfig)
	PKG_CHECK_MODULES(CAIRO REQUIRED cairo)
ELSE(NOT WIN32)
	FIND_PACKAGE(Win32GTKBundle)
ENDIF(NOT WIN32)

SET(OPENSCENEGRAPH_INCLUDE_DIR "" CACHE PATH "Location of the OSG include headers.")
SET(OPENSCENEGRAPH_LIB_DIR "" CACHE PATH "Location of the the OSG library objects.")

SET(SRC_FILES
	src/Image.cpp
	src/LibraryWrapper.cpp
	src/ReadFile.cpp
	src/Util.cpp
)

SET(CAIROCKS_SRC_FILES
	ext/cairocks/src/distance-field.c
	ext/cairocks/src/emboss.c
	ext/cairocks/src/gaussian-blur.c
	ext/cairocks/src/invert.c
	ext/cairocks/src/map-path-onto.c
	ext/cairocks/src/rounded.c
)

SET(HEADER_PATH ${osgCairo_SOURCE_DIR}/include/osgCairo)
SET(HEADER_FILES
	${HEADER_PATH}/Export
	${HEADER_PATH}/Image
	${HEADER_PATH}/ReadFile
	${HEADER_PATH}/Util
	${HEADER_PATH}/Version
)

SET(CAIROCKS_HEADER_FILES
	ext/cairocks/src/cairocks.h
)

SOURCE_GROUP("Header Files" FILES ${HEADER_FILES})
SOURCE_GROUP("Source Files" FILES ${SRC_FILES})

INCLUDE_DIRECTORIES(
	include
	ext/cairocks/src
	${OPENSCENEGRAPH_INCLUDE_DIR}
	${CAIRO_INCLUDE_DIRS}
)

LINK_DIRECTORIES(
	${OPENSCENEGRAPH_LIB_DIR}
	${CAIRO_LIBRARY_DIRS}
)

ADD_LIBRARY(osgCairo SHARED ${HEADER_FILES} ${SRC_FILES})

SOURCE_GROUP("Header Files" FILES ${CAIROCKS_HEADER_FILES})
SOURCE_GROUP("Source Files" FILES ${CAIROCKS_SRC_FILES})

ADD_LIBRARY(cairocks STATIC ${CAIROCKS_HEADER_FILES} ${CAIROCKS_SRC_FILES})

# Add debug postfix to OSG libraries so we link to the right ones in debug.
# Cairo is a C-only library so the same one (release) can be linked to both
# debug and release without problems.
TARGET_LINK_LIBRARIES(osgCairo
	debug     OpenThreads${CMAKE_DEBUG_POSTFIX}
	optimized OpenThreads
	debug     osg${CMAKE_DEBUG_POSTFIX}
	optimized osg
	debug     osgDB${CMAKE_DEBUG_POSTFIX}
	optimized osgDB
	debug     cairo
	optimized cairo
	cairocks
)

# The archive/runtime part is so that the lib/dll pair goes into lib and bin
# respectively on Windows.
INSTALL(TARGETS osgCairo
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
)

FOREACH(INCLUDEFILE ${HEADER_FILES})
	FILE(RELATIVE_PATH REL_INCLUDEFILE ${osgCairo_SOURCE_DIR}/include/osgCairo ${INCLUDEFILE})
	GET_FILENAME_COMPONENT(REL_INCLUDE_PATH ${REL_INCLUDEFILE} PATH)
	INSTALL(
		FILES ${INCLUDEFILE}
		DESTINATION ${CMAKE_INSTALL_PREFIX}/include/osgCairo/${REL_INCLUDE_PATH}
	)
ENDFOREACH(INCLUDEFILE)

ADD_SUBDIRECTORY(examples/osgcairodistancefield)
ADD_SUBDIRECTORY(examples/osgcairodrawspeed)
ADD_SUBDIRECTORY(examples/osgcairoroads)
ADD_SUBDIRECTORY(examples/osgcairotests)
ADD_SUBDIRECTORY(examples/osgcairoutil)
ADD_SUBDIRECTORY(examples/osgcairoviewer)

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

SET(OSGCAIRO_VERSION "1.4.1")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "osgCairo-${OSGCAIRO_VERSION}")

INCLUDE(CPack)

