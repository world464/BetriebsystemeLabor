CC=gcc
CFLAGS=-I. -lpthread -lrt

miners: main.c
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f miners