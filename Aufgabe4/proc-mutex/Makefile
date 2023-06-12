CC = gcc
CFLAGS = -Wall -Wextra

# Target for the first program
proc1: proc1.c
	$(CC) $(CFLAGS) -o proc1 proc1.c -lpthread -lrt

# Target for the second program
proc2: proc2.c
	$(CC) $(CFLAGS) -o proc2 proc2.c -lpthread -lrt

.PHONY: clean

clean:
	rm -f proc1 proc2 *.o
