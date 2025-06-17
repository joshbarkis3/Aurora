cc=clang
CFLAGS = -Wall
LDFLAGS =

SRC = $(wildcard src/*.c)
INC = $(wildcard src/*.h)

OBJ = $(SRC:.c=.o)

BUILDDIR = ./build

all: aurora clean

%.o: %.c %.h
	$(CC) -o $@ -c $< $(CFLAGS)

build:
	mkdir -p $(BUILDDIR)

aurora: $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

clean:
	rm -rf $(wildcard src/*.o)
