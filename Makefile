
CC = gcc
AR = ar

CFLAGS = -std=c90 -I. -fPIC

SRC=./src

DEPS = $(SRC)/nvm.h 

SOURCES = $(SRC)/nvm.c $(SRC)/namespace.c $(SRC)/ssd_device.c
OBJS    = $(SOURCES:.c=.o)

TARGET          = libnvm.so
STATIC_TARGET   = libnvm.a

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

all: $(OBJS) $(TARGET) $(TARGET_STATIC)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -shared

$(TARGET_STATIC): $(OBJS) $(TARGET)
	$(AR) rcs $(TARGET_STATIC) $(OBJS)

clean: 
	rm -f ${SRC}/*.o libnvm.a libnvm.so*

