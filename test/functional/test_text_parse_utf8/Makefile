#/*!
#* \file Makefile
#* \brief Makefile for compiling
#*
#* Copyright of Timo hannukkala, Inc. All rights reserved.
#*
#* \author Timo Hannukkala <timohannukkala@hotmail.com>
#*/
TARGET:=functional_text_parse_utf8
CXX:=$(if $(CXX),$(CXX),g++)
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

text_parse_utf8_SRCDIR:=$(CURRENT_DIR)../../../lib/src/font/text
text_parse_utf8_SRCS:=$(wildcard $(text_parse_utf8_SRCDIR)/tg_text_parse_utf8.cpp)
text_parse_utf8_OBJS:=$(text_parse_utf8_SRCS:.cpp=.o)

ORDERS_FILE=$(CURRENT_DIR)orders/orders.txt
CXXFLAGS+=-DORDERS_FILE=\"$(ORDERS_FILE)\"

all: default

default: $(src_OBJS) $(text_parse_utf8_OBJS)
	$(CXX) $(src_OBJS) $(text_parse_utf8_OBJS) $(LDFLAGS) -o $(TARGET)

$(src_OBJS):%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(text_parse_utf8_OBJS):%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET)
	rm -f $(src_SRCDIR)/*.o
	rm -f $(text_parse_utf8_SRCDIR)/*.o
