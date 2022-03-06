# prj-tg-ui-lib

prj-tg-ui-lib (library) is my personal project to graphical user interface with opengl.

There is plenty of things that would be nice to do, as listed in TODO.md

prj-tg-ui-lib was done on ubuntu 20.04.

## prj-tg-ui-lib library

prj-tg-ui-lib library uses X11 or glfw to create window and get mouse/keyboard inputs.

### Development Environment

Required libraries: libglew-dev, libpng-dev, make, build-essential  
prj-ttf-reader from https://github.com/Timohan/prj-ttf-reader

If using glfw, required library is also: libglfw3-dev

### Build and Installation

cd lib  
make  
or if using glfw: make USE_GLFW=on  
sudo make install

### Clean and Uninstallation

make clean  
sudo make uninstall

## prj-tg-ui-lib examples and tests

Tests are in test/manual folder.

## possible errors

when using X11 (default build without glfw):  
error: "X Error of failed request: GLXBadFBConfig"  
solution: export MESA_GL_VERSION_OVERRIDE=4.0  
