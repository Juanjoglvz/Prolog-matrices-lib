#!/usr/bin/make -f
# -*- mode:makefile -*-

CCFLAGS := -std=c99 -Wall -pedantic
LDLIBS :=
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
	
$(DIROBJ)%.o: $(DIRSRC)%.c
	$(CC) $(CCFLAGS) -I$(DIRHEA) -c $^ -o $@
	
	
clean: 
	-$(RM) *~
	-$(RM) -r ./lib
	-$(RM) -r ./obj
