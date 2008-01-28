PROJECT(osgcairowidget)

LINK_LIBRARIES(osgCairo osgWidget)

ADD_EXECUTABLE(osgcairowidget osgcairowidget.cpp)

INSTALL(TARGETS osgcairowidget DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
