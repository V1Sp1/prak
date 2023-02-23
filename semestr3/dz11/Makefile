CC = gcc
CFLAGS = -Wall

all: msgserver msgclient

msgserver: chat_setings.h msgserver.c list.o
	$(CC) $(CFLAGS) msgserver.c list.o -o $@

msgclient: chat_setings.h msgclient.c
	$(CC) $(CFLAGS) msgclient.c -o $@

list.o: list.c list.h
	$(CC) $(CFLAGS) -c list.c -o $@

val: task3
	valgrind ./task3
