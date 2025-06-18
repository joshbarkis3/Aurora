CC = cc
CFLAGS = -Wall -g
LDFLAGS =

EXEC = aurora

SRCDIR = ./src
BUILDDIR = ./build

SRC = $(wildcard $(SRCDIR)/*.c)
INC = $(wildcard $(SRCDIR)/*.h)

OBJ := $(SRC:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)

all: aurora

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(EXEC): $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

.PHONY: clean

clean:
	rm -rf $(BUILDDIR)
	rm $(EXEC)
