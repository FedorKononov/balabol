TARGET = libkaitalk.so
OBJECTS = src/gsocket.o
CFLAGS = -fPIC -c -Wall -Iinclude
LDFLAGS = -shared
CC = gcc
LD = $(CC)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(LD) -o $@ $^ $(LDFLAGS)

kaitalk: examples/test.o $(TARGET)
	$(LD) -o $@ $< -lkaitalk

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $^

install: $(TARGET)
	cp $(TARGET) /usr/lib/
	cp -r include/kaitalk /usr/include/

clean:
	rm -f $(TARGET) kaitalk src/*.o examples/*.o *~

.PHONY: all clean install test