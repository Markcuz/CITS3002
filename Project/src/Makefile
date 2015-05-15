#Makefile for Project
DIR = $(shell pwd)

COMPILER_OPTIONS = -std=gnu99
COMPILER_DEFINES =

SRC_FILES_NAMES = comms.c collector.c analyst.c bank.c director.c

OBJ_FILES = comms.o collector.o analyst.o bank.o director.o

INC_DIR = -I$(DIR)/../include -I$(DIR)

COMPILER = gcc $(COMPILER_OPTIONS) $(COMPILER_DEFINES) $(INC_DIR)

all: ALL

clean:
	rm -f $(OBJ_FILES)

ALL:	$(OBJ_FILES)

comms.o: comms.c
	$(COMPILER) -c -o comms.o comms.c

collector.o: collector.c
	$(COMPILER) -c -o collector.o collector.c

analyst.o: analyst.c
	$(COMPILER) -c -o analyst.o analyst.c

bank.o: bank.c
	$(COMPILER) -c -o bank.o bank.c

director.o: director.c
	$(COMPILER) -c -o director.o director.c

