#Makefile for Project
DIR = $(shell pwd)
DIR_TEST = $(shell pwd)/test

COMPILER_OPTIONS = -std=gnu99
COMPILER_DEFINES =

SRC_FILES_NAMES = src/comms.c src/collector.c src/analyst.c src/bank.c src/director.c

OBJ_FILES = obj/comms.o obj/collector.o obj/analyst.o obj/bank.o obj/director.o

INC_DIR = -I$(DIR)/include -I$(DIR)

COMPILER = gcc $(COMPILER_OPTIONS) $(COMPILER_DEFINES) $(INC_DIR)

all: MYLIB

clean:
	rm -f $(OBJ_FILES)

MYLIB: 
	@echo ======Making Library========= 
	ar rcs libproject.a obj/comms.o obj/collector.o obj/analyst.o obj/bank.o obj/director.o
	ranlib libproject.a

comms.o: src/comms.c
	$(COMPILER) -c -o obj/comms.o src/comms.c

collector.o: src/collector.c
	$(COMPILER) -c -o obj/collector.o src/collector.c

analyst.o: src/analyst.c
	$(COMPILER) -c -o obj/analyst.o src/analyst.c

bank.o: src/bank.c
	$(COMPILER) -c -o obj/bank.o src/bank.c

director.o: src/director.c
	$(COMPILER) -c -o obj/director.o src/director.c
