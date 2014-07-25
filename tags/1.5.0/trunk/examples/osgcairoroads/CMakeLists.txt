PROJECT(osgcairoroads)

LINK_LIBRARIES(osgCairo
	debug     osgViewer${CMAKE_DEBUG_POSTFIX}
	optimized osgViewer
)

ADD_EXECUTABLE(osgcairoroads osgcairoroads.cpp)

INSTALL(TARGETS osgcairoroads DESTINATION ${CMAKE_INSTALL_PREFIX}/share/OpenSceneGraph/bin)
SET_TARGET_PROPERTIES(osgcairoroads PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")
