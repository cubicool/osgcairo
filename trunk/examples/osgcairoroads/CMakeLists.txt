PROJECT(osgcairoroads)

# osgCairo gets the proper debug postfix automatically since it's part of the
# current project, but we need to add the postfix for the other libs.
LINK_LIBRARIES(osgCairo
               debug     OpenThreads${CMAKE_DEBUG_POSTFIX}
               optimized OpenThreads)

ADD_EXECUTABLE(osgcairoroads osgcairoroads.cpp)

INSTALL(TARGETS osgcairoroads DESTINATION ${CMAKE_INSTALL_PREFIX}/share/OpenSceneGraph/bin)
SET_TARGET_PROPERTIES(osgcairoroads PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")
