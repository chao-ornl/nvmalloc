
CC=gcc
CFLAGS=-I.

SRC=./src

DEPS = $(SRC)/nvm.h 
OBJ = $(SRC)/nvm.o $(SRC)/namespace.o $(SRC)/ssd_device.o

all: baseline 

baseline: $(OBJ)
	ar rcs libnvm.a ${SRC}/nvm.o ${SRC}/namespace.o $(SRC)/ssd_device.o

clean: 
	rm -f ${SRC}/*.o libnvm.a

