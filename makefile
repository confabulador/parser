FLAGS = -Wall -pedantic

CC = gcc

all: *.c *.h
	$(CC) $(FLAGS) -o parser.out $?

memory-test:
	valgrind -s ./parser.out
