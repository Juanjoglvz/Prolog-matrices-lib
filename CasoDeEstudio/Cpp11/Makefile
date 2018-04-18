#!/usr/bin/make -f
# -*- mode:makefile -*-
LDLIBS :=
CPPFLAGS := -shared -ld g++ -cc-options[" -std=c++11"
CC := swipl-ld

DIROBJ := obj/
DIRLIB := lib/
DIRHEA := include/
DIRSRC := src/

all: dirs matlog

debug: CCFLAGS += -DDEBUG -g
debug: all

dirs:
	mkdir -p $(DIROBJ) $(DIRLIB)
	
ftest: $(DIRSRC)test.cpp
	$(CC) $(CPPFLAGS) -o $@.so $^
	
matlog: $(DIRSRC)matlog.cpp $(DIRSRC)Matrix.cpp
	$(CC) $(CPPFLAGS) -I$(DIRHEA) -o $@.so $^

$(DIROBJ)%.o: $(DIRSRC)%.c
	$(CC) $(CCFLAGS) -I$(DIRHEA) -c $^ -o $@
	
	
clean: 
	-$(RM) *~
	-$(RM) ./*.so
	-$(RM) -r ./lib
	-$(RM) -r ./obj
