# --------------------------------------------------------------------------- #
# Developer: Andrew Kirfman                                                   #
# Project: CSCE-313 Episode #4                                                #
#                                                                             #
# File: ./Makefile                                                            #
# --------------------------------------------------------------------------- #


CC=g++
STD=-std=c++11
CFLAGS=-O3 -Wall -g

all: proctest.o main.o
	$(CC) $(STD) $(CFLAGS) -o proctest proctest.o main.o

main.o: main.cpp
	$(CC) $(STD) $(CFLAGS) -c main.cpp
	
proctest.o: proctest.h proctest.cpp
	$(CC) $(STD) $(CFLAGS) -c proctest.h proctest.cpp

clean:
	rm -rf proctest
