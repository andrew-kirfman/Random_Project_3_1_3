# -------------------------------------------------------------------------- #
# Developer: Andrew Kirfman, Dr. Betatti                                     #
# Project: CSCE-313 Machine Problem 2                                        #
#                                                                            #
# File: ./makefile                                                           #
# -------------------------------------------------------------------------- #

CC=g++
STD=-std=c++11
CFLAGS=

all: memtest

my_allocator.o : my_allocator.c
	$(CC) $(STD) $(CFLAGS) -c -g my_allocator.c

memtest.o : memtest.c
	$(CC) $(STD) $(CFLAGS) -c -g memtest.c

memtest: memtest.o my_allocator.o
	$(CC) $(STD) $(CFLAGS) -o memtest memtest.o my_allocator.o

clean:
	rm -rf *.o *.gch memtest

