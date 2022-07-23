#/*!
#* \file Makefile
#* \brief Makefile for compiling
#*
#* Copyright of Timo hannukkala, Inc. All rights reserved.
#*
#* \author Timo Hannukkala <timohannukkala@hotmail.com>
#*/
TARGET:=functional_testapp_button
CXX:=$(if $(CXX),$(CXX),g++)
PKGFLAGS=`pkg-config --cflags --libs prj-tg-ui-lib`
CXXFLAGS+=-g -Wall -pedantic -c -pipe -std=gnu++17 -W -D_REENTRANT -fPIC
CXXFLAGS+=-I./src
CXXFLAGS+=$(PKGFLAGS)
CXXFLAGS+=-Wno-unused-parameter -Wuninitialized -Wconversion -Wshadow -Wpointer-arith \
	 -Wswitch-default -Wswitch-enum -Wcast-align \
	 -Winline -Wundef -Wcast-qual -Wunreachable-code -Wlogical-op -Wfloat-equal \
	 -Wredundant-decls -Werror \
	 -Wno-unused-const-variable
CXXFLAGS+=-DFUNCIONAL_TEST
LDFLAGS:=$(PKGFLAGS)
LDFLAGS+=-lpthread
LDFLAGS+=-lX11
LDFLAGS+=-lpng
# set current make dir
CURRENT_DIR=$(dir $(abspath $(lastword $(MAKEFILE_LIST))))

src_SRCDIR:=$(CURRENT_DIR)src
src_SRCS:=$(wildcard $(src_SRCDIR)/*.cpp)
src_OBJS:=$(src_SRCS:.cpp=.o)

IMAGES_TO_COMPARE_DIR=$(CURRENT_DIR)images_to_compare
CXXFLAGS+=-DIMAGES_TO_COMPARE_DIR=\"$(IMAGES_TO_COMPARE_DIR)\"

ORDERS_FILE=$(CURRENT_DIR)orders/orders.txt
CXXFLAGS+=-DORDERS_FILE=\"$(ORDERS_FILE)\"

all: default

default: $(src_OBJS)
	$(CXX) $(src_OBJS) $(LDFLAGS) -o $(TARGET)

$(src_OBJS):%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET)
	rm -f src/*.o
