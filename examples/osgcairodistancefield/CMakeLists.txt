PROJECT(osgcairodistancefield)

# osgCairo gets the proper debug postfix automatically since it's part of the
# current project, but we need to add the postfix for the other libs.
LINK_LIBRARIES(osgCairo
	debug     OpenThreads${CMAKE_DEBUG_POSTFIX}
	optimized OpenThreads
	debug     osgText${CMAKE_DEBUG_POSTFIX}
	optimized osgText
)

ADD_EXECUTABLE(osgcairodistancefield osgcairodistancefield.cpp)

INSTALL(TARGETS osgcairodistancefield DESTINATION ${CMAKE_INSTALL_PREFIX}/share/OpenSceneGraph/bin)
SET_TARGET_PROPERTIES(osgcairodistancefield PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")
