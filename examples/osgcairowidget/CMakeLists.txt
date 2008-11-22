PROJECT(osgcairowidget)

# osgCairo gets the proper debug postfix automatically since it's part of the
# current project, but we need to add the postfix for the other libs.
LINK_LIBRARIES(osgCairo
               debug     OpenThreads${CMAKE_DEBUG_POSTFIX}
               optimized OpenThreads
               debug     osgGA${CMAKE_DEBUG_POSTFIX}
               optimized osgGA
               debug     osgWidget${CMAKE_DEBUG_POSTFIX}
               optimized osgWidget)

ADD_EXECUTABLE(osgcairowidget osgcairowidget.cpp)

INSTALL(TARGETS osgcairowidget DESTINATION ${CMAKE_INSTALL_PREFIX}/share/openscenegraph/bin)
SET_TARGET_PROPERTIES(osgcairowidget PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
