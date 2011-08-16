PROJECT(osgcairodistancefield)

LINK_LIBRARIES(osgCairo
	debug     osgViewer${CMAKE_DEBUG_POSTFIX}
	optimized osgViewer
	debug     osgGA${CMAKE_DEBUG_POSTFIX}
	optimized osgGA
)

ADD_EXECUTABLE(osgcairodistancefield osgcairodistancefield.cpp)

INSTALL(TARGETS osgcairodistancefield DESTINATION ${CMAKE_INSTALL_PREFIX}/share/OpenSceneGraph/bin)
SET_TARGET_PROPERTIES(osgcairodistancefield PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")
