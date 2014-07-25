PROJECT(osgcairotests)

LINK_LIBRARIES(osgCairo
	debug     osgViewer${CMAKE_DEBUG_POSTFIX}
	optimized osgViewer
)

ADD_EXECUTABLE(osgcairotests osgcairotests.cpp)

INSTALL(TARGETS osgcairotests DESTINATION ${CMAKE_INSTALL_PREFIX}/share/OpenSceneGraph/bin)
SET_TARGET_PROPERTIES(osgcairotests PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")
