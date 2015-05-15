#Makefile for Project
DIR = $(shell pwd)
DIR_TEST = $(shell pwd)/test

COMPILER_OPTIONS = -std=gnu99
COMPILER_DEFINES =

INC_DIR = -I$(DIR)/include -I$(DIR)

COMPILER = gcc $(COMPILER_OPTIONS) $(COMPILER_DEFINES) $(INC_DIR)

all: MYLIB

clean:
	rm -f $(OBJ_FILES)
	rm libproject.a

MYLIB: 
	cd $(DIR)/src; make
	@echo ======Making Library========= 
	ar rcs libproject.a src/comms.o src/collector.o src/analyst.o src/bank.o src/director.o
	ranlib libproject.a



