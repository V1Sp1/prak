CC = gcc
CFLAGS = -Wall -g
VFLAGS1 = --leak-check=full --child-silent-after-fork=yes
VFLAGS2 = --leak-check=full --show-leak-kinds=all
PROG = shell

$(PROG): main.c error.o darr.o lex.o tree.o exec.o intcmds.o
	$(CC) $(CFLAGS) main.c error.o darr.o lex.o tree.o exec.o intcmds.o -lm -o $@

error.o: error.c error.h
	$(CC) $(CFLAGS) -c $< -o $@

darr.o: darr.c darr.h error.o
	$(CC) $(CFLAGS) -c $< -o $@

lex.o: lex.c lex.h error.o darr.o
	$(CC) $(CFLAGS) -c $< -o $@

tree.o: tree.c tree.h error.o darr.o
	$(CC) $(CFLAGS) -c $< -o $@

exec.o: exec.c exec.h error.o tree.o
	$(CC) $(CFLAGS) -c $< -o $@

intcmds.o: intcmds.c intcmds.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(PROG)

run:
	rlwrap ./$(PROG)

val:
	valgrind $(VFLAGS1) ./$(PROG)

valall:
	valgrind $(VFLAGS2) ./$(PROG)
