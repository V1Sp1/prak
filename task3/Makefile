CC = gcc
CFLAGS = -Wall

task3: main.c lex.o
	$(CC) $(CFLAGS) main.c lex.o -o $@

lex.o: lex.c lex.h
	$(CC) $(CFLAGS) -c lex.c -o $@

run: task3
	./task3

val: task3
	valgrind ./task3

clean:
	rm -f *.o lex
