CC = gcc
CFLAGS = -ansi -Wall -g -pedantic

all: fameChecker

fameChecker: main.o
	$(CC) $(CFLAGS) -o fameChecker main.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	rm *.o fameChecker