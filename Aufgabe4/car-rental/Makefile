CC=gcc
CFLAGS=-I. -lpthread -lrt

car_rental: main.c
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f car_rental
