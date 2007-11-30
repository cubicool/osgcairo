PROJECT(osgcairoviewer)

LINK_LIBRARIES(osgCairo)

ADD_EXECUTABLE(osgcairoviewer osgcairoviewer.cpp)

INSTALL(TARGETS osgcairoviewer DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
