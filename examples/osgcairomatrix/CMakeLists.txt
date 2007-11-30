PROJECT(osgcairomatrix)

LINK_LIBRARIES(osgCairo)

ADD_EXECUTABLE(osgcairomatrix osgcairomatrix.cpp)

INSTALL(TARGETS osgcairomatrix DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
