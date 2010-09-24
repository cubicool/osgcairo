PROJECT(osgcairodrawspeed)

# osgCairo gets the proper debug postfix automatically since it's part of the
# current project, but we need to add the postfix for the other libs.
LINK_LIBRARIES(osgCairo
               debug     OpenThreads${CMAKE_DEBUG_POSTFIX}
               optimized OpenThreads)

ADD_EXECUTABLE(osgcairodrawspeed osgcairodrawspeed.cpp)

INSTALL(TARGETS osgcairodrawspeed DESTINATION ${CMAKE_INSTALL_PREFIX}/share/OpenSceneGraph/bin)
SET_TARGET_PROPERTIES(osgcairodrawspeed PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")
