CC = gcc
CFLAGS = -Wall # -g
VFLAGS = --leak-check=full --show-leak-kinds=all
SERVIP = 127.0.0.1
SERVPORT = 7777
SERVLOG = a.log

all: server client

sessaddr.o: sessaddr.c sessaddr.h
	$(CC) $(CFLAGS) -c $< -o $@

carlist.o: carlist.c carlist.h sessaddr.h
	$(CC) $(CFLAGS) -c $< -o $@

session.o: session.c session.h sessaddr.h
	$(CC) $(CFLAGS) -c $< -o $@

server: server.c carlist.o session.o sessaddr.o
	$(CC) $(CFLAGS) server.c carlist.o session.o sessaddr.o -o $@

client: client.c session.o sessaddr.o
	$(CC) $(CFLAGS) client.c session.o sessaddr.o -o $@

clean:
	rm -f *.o server client $(SERVLOG)

runserver:
	./server $(SERVPORT) $(SERVLOG)

runclient:
	./client $(SERVIP) $(SERVPORT)

valserver:
	valgrind $(VFLAGS) ./server $(SERVPORT) $(SERVLOG)

valclient:
	valgrind $(VFLAGS) ./client $(SERVIP) $(SERVPORT)
