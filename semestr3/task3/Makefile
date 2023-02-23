CC = gcc
CFLAGS = -Wall
MACRO_FLAG = 0


task3: main.c lex.o
	$(CC) $(CFLAGS) main.c lex.o -o $@

lex.o: lex.c lex.h
	$(CC) $(CFLAGS) -D USE_FSCANF_IN_GETSYM=$(MACRO_FLAG) $()-c lex.c -o $@

run: task3
	./task3

val: task3
	valgrind ./task3

clean:
	rm -f *.o lex
