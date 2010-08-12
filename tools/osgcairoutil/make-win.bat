@echo off

call "%VS90COMNTOOLS%vsvars32.bat" >NUL:

set PYTHON_DIR=C:\Python25
set GTKBUNDLE_DIR=C:\users\cubicool\sources\gtk+-bundle_2.14.7-20090119_win32

set PYTHON_LIB="%PYTHON_DIR%\libs\python25.lib"
set CAIRO_LIB="%GTKBUNDLE_DIR%\lib\cairo.lib"

set INCLUDE_DIRS=
set INCLUDE_DIRS=%INCLUDE_DIRS% /I"%PYTHON_DIR%\include"
set INCLUDE_DIRS=%INCLUDE_DIRS% /I"%PYTHON_DIR%\include\pycairo"
set INCLUDE_DIRS=%INCLUDE_DIRS% /I"%GTKBUNDLE_DIR%\include\cairo"

set LIBS=%PYTHON_LIB% %CAIRO_LIB% %SDL_LIB%

cl /Fe_osgcairoutil /Fo_osgcairoutil %INCLUDE_DIRS% /LD osgcairoutil.cpp /link %LIBS%

move _osgcairoutil.dll _osgcairoutil.pyd

pause
