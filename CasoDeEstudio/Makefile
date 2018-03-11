#!/usr/bin/make -f
# -*- mode:makefile -*-

CFLAGS :=
LDLIBS :=
CC := swipl-ld

DIROBJ := obj/
DIRLIB := lib/
DIRHEA := include/
DIRSRC := src/

all: dirs helloworld

dirs:
	mkdir -p $(DIROBJ) $(DIRLIB)

helloworld: $(DIROBJ)helloworld.o
	$(CC) -shared -o lib/$@ $^
	
$(DIROBJ)%.o: $(DIRSRC)%.c
	$(CC) -c $^ -o $@
	
	
clean: 
	rm -rf *~ core $(DIROBJ) $(DIRLIB)
