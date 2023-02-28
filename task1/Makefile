CC = gcc
CFLAGS = -Wall -g
VFLAGS = --leak-check=full --show-leak-kinds=all

all: server client

carlist.o: carlist.c carlist.h
	$(CC) $(CFLAGS) -c $< -o $@

session.o: session.c session.h
	$(CC) $(CFLAGS) -c $< -o $@

server: server.c carlist.o session.o
	$(CC) $(CFLAGS) server.c carlist.o session.o -o $@

client: server.c session.o 
	$(CC) $(CFLAGS) client.c session.o -o $@

clean:
	rm -f *.o server client

valserver:
	valgrind $(VFLAGS) ./server

valclient:
	valgrind $(VFLAGS) ./client
