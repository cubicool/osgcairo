PROJECT(osgcairoutil)

LINK_LIBRARIES(osgCairo
	debug     osgViewer${CMAKE_DEBUG_POSTFIX}
	optimized osgViewer
)

ADD_EXECUTABLE(osgcairoutil osgcairoutil.cpp)

INSTALL(TARGETS osgcairoutil DESTINATION ${CMAKE_INSTALL_PREFIX}/share/OpenSceneGraph/bin)
SET_TARGET_PROPERTIES(osgcairoutil PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")
