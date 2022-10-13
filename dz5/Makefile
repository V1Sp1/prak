CC = gcc
CFLAGS = -Wall -g

6p45e: 6p45e.c 6p42.o
	$(CC) $(CFLAGS) 6p45e.c 6p42.o -o $@

6p42.o: 6p42.c 6p42.h
	$(CC) $(CFLAGS) -c $< -o $@
