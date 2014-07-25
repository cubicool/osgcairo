PROJECT(osgcairoviewer)

LINK_LIBRARIES(osgCairo
	debug     osgViewer${CMAKE_DEBUG_POSTFIX}
	optimized osgViewer
	debug     osgGA${CMAKE_DEBUG_POSTFIX}
	optimized osgGA
)

ADD_EXECUTABLE(osgcairoviewer osgcairoviewer.cpp)

INSTALL(TARGETS osgcairoviewer DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
SET_TARGET_PROPERTIES(osgcairoviewer PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")
