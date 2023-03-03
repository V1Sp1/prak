CC = gcc
CFLAGS = -Wall -g
VFLAGS = --leak-check=full --show-leak-kinds=all
SERVPORT = 7777
SERVLOG = a.log


all: server #client

sessaddr.o: sessaddr.c sessaddr.h
	$(CC) $(CFLAGS) -c $< -o $@

carlist.o: carlist.c carlist.h sessaddr.h
	$(CC) $(CFLAGS) -c $< -o $@

session.o: session.c session.h sessaddr.h
	$(CC) $(CFLAGS) -c $< -o $@

server: server.c carlist.o session.o sessaddr.o
	$(CC) $(CFLAGS) server.c carlist.o session.o sessaddr.o -o $@

#client: client.c session.o 
#	$(CC) $(CFLAGS) client.c session.o sessadr.o -o $@

clean:
	rm -f *.o server client

valserver:
	valgrind $(VFLAGS) ./server $(SERVPORT) $(SERVLOG)

valclient:
	valgrind $(VFLAGS) ./client
