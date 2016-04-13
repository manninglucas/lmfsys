CC=gcc
CFLAGS=-Wall
LDFLAGS=
OBJECTS=
EXECUTABLE=lmfsys
OBJDIR=obj
SRCDIR=src
sources= $(wildcard src/*.c)
objects= $(patsubst src/%.c,obj/%.o,$(wildcard src/*.c))
vpath %.c src
vpath %.h src
vpath %.o obj

$(OBJDIR)/%.o: %.c
	$(CC) -c $(CFLAGS) -c -o $@ $<

build/$(EXECUTABLE): $(objects)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

all: 
	lmfsys

clean:
	rm -f obj/*.o


