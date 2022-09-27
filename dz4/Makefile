CC = gcc
CFLAGS = -Wall

matrix: main.c matrix.o
	$(CC) $(CFLAGS) main.c matrix.o -o $@

matrix.o: matrix.c matrix.h
	$(CC) $(CFLAGS) -c $< -o $@

run: matrix
	./matrix

clean:
	rm -f *.o matrix
