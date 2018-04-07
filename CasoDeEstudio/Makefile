#!/usr/bin/make -f
# -*- mode:makefile -*-

CCFLAGS := -std=c99 -Wall -pedantic
LDLIBS :=
CPPFLAGS := --std=C++11 -shared -ld g++
CC := swipl-ld

DIROBJ := obj/
DIRLIB := lib/
DIRHEA := include/
DIRSRC := src/

all: dirs helloworld

debug: CCFLAGS += -DDEBUG -g
debug: all

dirs:
	mkdir -p $(DIROBJ) $(DIRLIB)

helloworld: $(DIROBJ)helloworld.o $(DIROBJ)matlog.o $(DIROBJ)debug.o
	$(CC) -shared -o $@ $^
	
test: $(DIRSRC)test.cpp
	$(CC) $(CPPFLAGS) -o $@.so $^

$(DIROBJ)%.o: $(DIRSRC)%.c
	$(CC) $(CCFLAGS) -I$(DIRHEA) -c $^ -o $@
	
	
clean: 
	-$(RM) *~
	-$(RM) -r ./lib
	-$(RM) -r ./obj
