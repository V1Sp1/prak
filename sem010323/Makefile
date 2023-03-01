CC = g++
CFLAGS = -Wall -g
VFLAGS = --leak-check=full --show-leak-kinds=all

prog: main.cpp list.cpp sorter.cpp
	$(CC) $(CFLAGS) list.cpp sorter.cpp main.cpp -o $@

clean:
	rm -f *.o prog

valgrind:
	valgrind $(VFLAGS) ./prog

