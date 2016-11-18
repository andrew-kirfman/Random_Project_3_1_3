# ---------------------------------------------------------------------------- #
# Developer: Andrew Kirfman, Margaret Baxter                                   #
# Project: CSCE-313 Machine Problem #1                                         #
#                                                                              #
# File: ./MP1/Part_2/Makefile                                                  #
# ---------------------------------------------------------------------------- #

CC=g++
STD=-std=c++11
CFLAGS=

all: main.o linked_list2.o
	$(CC) $(STD) -o List_2 main.o linked_list2.o
	
main.o: main.cpp
	$(CC) $(STD) -c main.cpp
	
linked_list2.o: linked_list2.cpp linked_list2.h
	$(CC) $(STD) -c linked_list2.cpp linked_list2.h
	
clean:
	rm -rf *.o List_2 *.gch
