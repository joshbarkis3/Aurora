CC = cc
CFLAGS = -Wall
LDFLAGS =

SRCDIR = ./src
BUILDDIR = ./build

SRC = $(wildcard $(SRCDIR)/*.c)
INC = $(wildcard $(SRCDIR)/*.h)

OBJ := $(SRC:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)

all: aurora

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

aurora: $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

.PHONY: clean

clean:
	rm -rf $(BUILDDIR)
