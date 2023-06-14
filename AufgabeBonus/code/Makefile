CC=gcc
CFLAGS=-I./include -Wall -Wextra -g
DEPS = $(wildcard include/*.h)
SRC = $(wildcard src/*.c)
OBJDIR = obj
OBJS = $(patsubst src/%.c, $(OBJDIR)/%.o, $(SRC))

all: myrle

myrle: $(OBJS)
	$(CC) $^ -o $@ $(CFLAGS)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: src/%.c $(DEPS) | $(OBJDIR)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJS) myrle
	rm -rf $(OBJDIR)
