PROJECT(osgcairoutil)

# osgCairo gets the proper debug postfix automatically since it's part of the
# current project, but we need to add the postfix for the other libs.
LINK_LIBRARIES(osgCairo
               debug     OpenThreads${CMAKE_DEBUG_POSTFIX}
               optimized OpenThreads)

ADD_EXECUTABLE(osgcairoutil osgcairoutil.cpp)

INSTALL(TARGETS osgcairoutil DESTINATION ${CMAKE_INSTALL_PREFIX}/share/OpenSceneGraph/bin)
SET_TARGET_PROPERTIES(osgcairoutil PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
