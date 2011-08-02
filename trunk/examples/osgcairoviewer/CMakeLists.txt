PROJECT(osgcairoviewer)

LINK_LIBRARIES(osgCairo
	debug     osgGA${CMAKE_DEBUG_POSTFIX}
	optimized osgGA
	debug     osgViewer${CMAKE_DEBUG_POSTFIX}
	optimized osgViewer
)

ADD_EXECUTABLE(osgcairoviewer osgcairoviewer.cpp)

INSTALL(TARGETS osgcairoviewer DESTINATION ${CMAKE_INSTALL_PREFIX}/share/OpenSceneGraph/bin)
SET_TARGET_PROPERTIES(osgcairoviewer PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")
