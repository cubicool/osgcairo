PROJECT(osgcairopdf)

LINK_LIBRARIES(osgCairo)

ADD_EXECUTABLE(osgcairopdf osgcairopdf.cpp)

INSTALL(TARGETS osgcairopdf DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
