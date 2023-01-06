#/*!
#* \file Makefile
#* \brief Makefile for compiling prj-tg-ui-lib library
#* and generating pkgconfig file
#* and making install & uninstall
#*
#* Copyright of Timo Hannukkala. All rights reserved.
#*
#* \author Timo Hannukkala <timohannukkala@hotmail.com>
#*/
TARGET:=libprj-tg-ui-lib.so
VERSION:=0.1
PREFIX:=/usr
LIBDIR:= $(PREFIX)/lib
PKGCONFIGDIR:=$(LIBDIR)/pkgconfig
PKGCONFIGFILE:=prj-tg-ui-lib
CCX:=$(if $(CCX),$(CCX),g++)
PKGFLAGS=`pkg-config --cflags --libs prj-ttf-reader`
CXXFLAGS+=$(PKGFLAGS)
CXXFLAGS+=-Wuninitialized -Wconversion -Wshadow -Wpointer-arith \
	-Wswitch-default -Wswitch-enum -Wcast-align \
	-Wundef -Wcast-qual -Wunreachable-code -Wfloat-equal \
	-Wredundant-decls -Wextra -Wall
CXXFLAGS+=-Werror
CXXFLAGS+=-O2
CXXFLAGS+=-g -fPIC -std=c++17
LDFLAGS+=-shared -lpng -lGL -lGLEW $(PKGFLAGS)
GLFW:=$(if $(USE_GLFW),$(USE_GLFW),off)

CURRENT_DIR=$(dir $(abspath $(lastword $(MAKEFILE_LIST))))

application_SRCDIR:=$(CURRENT_DIR)src/application
application_SRCS:=$(wildcard $(application_SRCDIR)/*.cpp)
application_OBJS:=$(application_SRCS:.cpp=.o)

global_SRCDIR:=$(CURRENT_DIR)src/global
global_SRCS:=$(wildcard $(global_SRCDIR)/*.cpp)
global_OBJS:=$(global_SRCS:.cpp=.o)

global_private_SRCDIR:=$(CURRENT_DIR)src/global/private
global_private_SRCS:=$(wildcard $(global_private_SRCDIR)/*.cpp)
global_private_OBJS:=$(global_private_SRCS:.cpp=.o)

window_SRCDIR:=$(CURRENT_DIR)src/window
window_SRCS:=$(wildcard $(window_SRCDIR)/*.cpp)
window_OBJS:=$(window_SRCS:.cpp=.o)

window_private_SRCDIR:=$(CURRENT_DIR)src/window/private
window_private_SRCS:=$(wildcard $(window_private_SRCDIR)/*.cpp)
window_private_OBJS:=$(window_private_SRCS:.cpp=.o)

shader_SRCDIR:=$(CURRENT_DIR)src/shader
shader_SRCS:=$(wildcard $(shader_SRCDIR)/*.cpp)
shader_OBJS:=$(shader_SRCS:.cpp=.o)

item2d_SRCDIR:=$(CURRENT_DIR)src/item2d
item2d_SRCS:=$(wildcard $(item2d_SRCDIR)/*.cpp)
item2d_OBJS:=$(item2d_SRCS:.cpp=.o)

item2d_private_SRCDIR:=$(CURRENT_DIR)src/item2d/private
item2d_private_SRCS:=$(wildcard $(item2d_private_SRCDIR)/*.cpp)
item2d_private_OBJS:=$(item2d_private_SRCS:.cpp=.o)

item2d_private_mouse_capture_SRCDIR:=$(CURRENT_DIR)src/item2d/private/mouse_capture
item2d_private_mouse_capture_SRCS:=$(wildcard $(item2d_private_mouse_capture_SRCDIR)/*.cpp)
item2d_private_mouse_capture_OBJS:=$(item2d_private_mouse_capture_SRCS:.cpp=.o)

item2d_private_item2d_SRCDIR:=$(CURRENT_DIR)src/item2d/private/item2d
item2d_private_item2d_SRCS:=$(wildcard $(item2d_private_item2d_SRCDIR)/*.cpp)
item2d_private_item2d_OBJS:=$(item2d_private_item2d_SRCS:.cpp=.o)

math_SRCDIR:=$(CURRENT_DIR)src/math
math_SRCS:=$(wildcard $(math_SRCDIR)/*.cpp)
math_OBJS:=$(math_SRCS:.cpp=.o)

image_SRCDIR:=$(CURRENT_DIR)src/image
image_SRCS:=$(wildcard $(image_SRCDIR)/*.cpp)
image_OBJS:=$(image_SRCS:.cpp=.o)

font_SRCDIR:=$(CURRENT_DIR)src/font
font_SRCS:=$(wildcard $(font_SRCDIR)/*.cpp)
font_OBJS:=$(font_SRCS:.cpp=.o)

font_cache_SRCDIR:=$(CURRENT_DIR)src/font/cache
font_cache_SRCS:=$(wildcard $(font_cache_SRCDIR)/*.cpp)
font_cache_OBJS:=$(font_cache_SRCS:.cpp=.o)

event_SRCDIR:=$(CURRENT_DIR)src/event
event_SRCS:=$(wildcard $(event_SRCDIR)/*.cpp)
event_OBJS:=$(event_SRCS:.cpp=.o)

common_SRCDIR:=$(CURRENT_DIR)src/common
common_SRCS:=$(wildcard $(common_SRCDIR)/*.cpp)
common_OBJS:=$(common_SRCS:.cpp=.o)

render_SRCDIR:=$(CURRENT_DIR)src/render
render_SRCS:=$(wildcard $(render_SRCDIR)/*.cpp)
render_OBJS:=$(render_SRCS:.cpp=.o)

ifeq ($(USE_FUNCTIONAL_TEST),on)
CXXFLAGS+=-DFUNCIONAL_TEST
ifeq ($(DISABLE_CPU_OPTIMIZE),on)
CXXFLAGS+=-DDEFAULT_RENDER_WAIT_MAX_TIMEOUT=0
else
CXXFLAGS+=-DDEFAULT_RENDER_WAIT_MAX_TIMEOUT=1000000
endif
else
ifeq ($(DISABLE_CPU_OPTIMIZE),on)
CXXFLAGS+=-DDEFAULT_RENDER_WAIT_MAX_TIMEOUT=0
endif
endif

ifeq ($(USE_GLFW),on)
LDFLAGS+=-lglfw
mainwindow_window_manager_SRCDIR:=$(CURRENT_DIR)src/window/glfw
mainwindow_window_manager_SRCS:=$(wildcard $(mainwindow_window_manager_SRCDIR)/*.cpp)
mainwindow_window_manager_OBJS:=$(mainwindow_window_manager_SRCS:.cpp=.o)
CXXFLAGS+=-DUSE_GLFW
else
mainwindow_window_manager_SRCDIR:=$(CURRENT_DIR)src/window/x11
mainwindow_window_manager_SRCS:=$(wildcard $(mainwindow_window_manager_SRCDIR)/*.cpp)
mainwindow_window_manager_OBJS:=$(mainwindow_window_manager_SRCS:.cpp=.o)
endif

all: default

default: $(render_OBJS) $(global_private_OBJS) $(font_cache_OBJS) $(window_private_OBJS) $(mainwindow_window_manager_OBJS) $(application_OBJS) $(item2d_private_item2d_OBJS) $(item2d_private_mouse_capture_OBJS) $(common_OBJS) $(event_OBJS) $(font_OBJS) $(global_OBJS) $(window_OBJS) $(shader_OBJS) $(item2d_OBJS) $(item2d_private_OBJS) $(math_OBJS) $(image_OBJS)
	$(CCX) $(render_OBJS) $(global_private_OBJS) $(font_cache_OBJS) $(window_private_OBJS) $(mainwindow_window_manager_OBJS) $(application_OBJS) $(item2d_private_item2d_OBJS) $(item2d_private_mouse_capture_OBJS) $(common_OBJS) $(event_OBJS) $(font_OBJS) $(global_OBJS) $(window_OBJS) $(shader_OBJS) $(item2d_OBJS) $(item2d_private_OBJS) $(math_OBJS) $(image_OBJS) $(LDFLAGS) -o $(TARGET)

$(mainwindow_window_manager_OBJS):%.o: %.cpp
	$(CCX) $(CXXFLAGS) -c $< -o $@

$(application_OBJS):%.o: %.cpp
	$(CCX) $(CXXFLAGS) -c $< -o $@

$(render_OBJS):%.o: %.cpp
	$(CCX) $(CXXFLAGS) -c $< -o $@

$(global_OBJS):%.o: %.cpp
	$(CCX) $(CXXFLAGS) -c $< -o $@

$(global_private_OBJS):%.o: %.cpp
	$(CCX) $(CXXFLAGS) -c $< -o $@

$(window_OBJS):%.o: %.cpp
	$(CCX) $(CXXFLAGS) -c $< -o $@

$(window_private_OBJS):%.o: %.cpp
	$(CCX) $(CXXFLAGS) -c $< -o $@

$(shader_OBJS):%.o: %.cpp
	$(CCX) $(CXXFLAGS) -c $< -o $@

$(item2d_OBJS):%.o: %.cpp
	$(CCX) $(CXXFLAGS) $(LDFLAGS) -c $< -o $@

$(item2d_private_OBJS):%.o: %.cpp
	$(CCX) $(CXXFLAGS) -c $< -o $@

$(item2d_private_mouse_capture_OBJS):%.o: %.cpp
	$(CCX) $(CXXFLAGS) -c $< -o $@

$(item2d_private_item2d_OBJS):%.o: %.cpp
	$(CCX) $(CXXFLAGS) -c $< -o $@

$(math_OBJS):%.o: %.cpp
	$(CCX) $(CXXFLAGS) -c $< -o $@

$(image_OBJS):%.o: %.cpp
	$(CCX) $(CXXFLAGS) -c $< -o $@

$(font_OBJS):%.o: %.cpp
	$(CCX) $(CXXFLAGS) -c $< -o $@

$(font_cache_OBJS):%.o: %.cpp
	$(CCX) $(CXXFLAGS) -c $< -o $@

$(event_OBJS):%.o: %.cpp
	$(CCX) $(CXXFLAGS) -c $< -o $@

$(common_OBJS):%.o: %.cpp
	$(CCX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f src/*.o
	rm -f src/application/*.o
	rm -f src/global/*.o
	rm -f src/global/private/*.o
	rm -f src/window/*.o
	rm -f src/window/x11/*.o
	rm -f src/window/glfw/*.o
	rm -f src/shader/*.o
	rm -f src/item2d/*.o
	rm -f src/item2d/private/*.o
	rm -f src/item2d/private/mouse_capture/*.o
	rm -f src/item2d/private/item2d/*.o
	rm -f src/math/*.o
	rm -f src/image/*.o
	rm -f src/font/*.o
	rm -f src/font/cache/*.o
	rm -f src/event/*.o
	rm -f src/common/*.o
	rm -f src/render/*.o
	rm -f *.so

install: $(PKGCONFIGDIR)/$(PKGCONFIGFILE).pc
	mkdir -p $(LIBDIR)/prj-tg-ui-lib
	cp lib*.so $(LIBDIR)/prj-tg-ui-lib
	mkdir -p $(PREFIX)/include/prj-tg-ui-lib/application
	cp src/application/tg_application.h $(PREFIX)/include/prj-tg-ui-lib/application
	mkdir -p $(PREFIX)/include/prj-tg-ui-lib/global
	cp src/global/tg_global_macros.h $(PREFIX)/include/prj-tg-ui-lib/global
	mkdir -p $(PREFIX)/include/prj-tg-ui-lib/window
	cp src/window/tg_mainwindow.h $(PREFIX)/include/prj-tg-ui-lib/window
	mkdir -p $(PREFIX)/include/prj-tg-ui-lib/item2d
	cp src/item2d/*.h $(PREFIX)/include/prj-tg-ui-lib/item2d
	mkdir -p $(PREFIX)/include/prj-tg-ui-lib/event
	cp src/event/tg_event_data.h $(PREFIX)/include/prj-tg-ui-lib/event
	mkdir -p $(PREFIX)/share/prj-tg-ui-lib
	cp -r ../images $(PREFIX)/share/prj-tg-ui-lib

$(PKGCONFIGDIR)/$(PKGCONFIGFILE).pc:
	@echo "installing $(PKGCONFIGDIR)/$(PKGCONFIGFILE).pc"
	@echo "prefix=$(PREFIX)" > $@
	@echo "exec_prefix=\$${prefix}" >> $@
	@echo "libdir=\$${prefix}/lib/prj-tg-ui-lib" >> $@
	@echo "includedir=\$${prefix}/include/prj-tg-ui-lib" >> $@
	@echo "" >> $@
	@echo "Name: prj-tg-ui-lib" >> $@
	@echo "Description: prj-tg-ui-lib lib parses ttf" >> $@
	@echo "Version: $(VERSION)" >> $@
	@echo "Cflags: -I\$${includedir}" >> $@
	@echo "Libs: -L\$${libdir} -lprj-tg-ui-lib -Wl,--rpath=\$${libdir}" >> $@

uninstall:
	rm -rf $(LIBDIR)/prj-tg-ui-lib
	rm -rf $(PREFIX)/include/prj-tg-ui-lib
	rm -f $(PKGCONFIGDIR)/$(PKGCONFIGFILE).pc
	rm -rf $(PREFIX)/share/prj-tg-ui-lib
