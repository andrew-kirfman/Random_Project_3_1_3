# --------------------------------------------------------------------------- #
# Developer: Andrew Kirfman                                                   #
# Project: CSCE-313 Machine Problem #6                                        #
#                                                                             #
# File: ./MP6/Makefile                                                        #
# --------------------------------------------------------------------------- #


CC=g++
STD=-std=c++11
CFLAGS=-O3 -Wall -g


all: 
	$(CC) $(CFLAGS) $(STD) main.cpp shell.cpp -o scheduler
	
main.o: main.cpp
	$(CC) $(CFLAGS) $(STD) -c main.cpp 

shell.o: shell.cpp
	$(CC) $(CFLAGS) $(STD) -c shell.cpp

clean:
	rm -rf scheduler
