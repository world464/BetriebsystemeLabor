CC=gcc
CFLAGS=-Wall

all: main

main: fifo_recv.c
	$(CC) $(CFLAGS) -o main fifo_recv.c

clean:
	rm -f main

main: fifo_send.c
	$(CC) $(CFLAGS) -o main fifo_send.c