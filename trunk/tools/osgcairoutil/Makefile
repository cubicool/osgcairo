ifeq ($(OSG_DIR), )
OSG_DIR=/home/cubicool/local
endif

_osgcairoutil.so: osgcairoutil.cpp
	@g++ -shared -W -Wall -Wno-unused \
	`pkg-config --libs pycairo --cflags pycairo` \
	-I/usr/include/python2.6 -I$(OSG_DIR)/include \
	-L$(OSG_DIR)/lib -lpython2.6 -losgCairo \
	$(<) -o $(@)

