# --------------------------------------------------------------------------- #
# Developer: Andrew Kirfman                                                   #
# Project: CSCE-313 Machine Problem #6                                        #
#                                                                             #
# File: ./MP6/Makefile                                                        #
# --------------------------------------------------------------------------- #


CC=g++
STD=-std=c++11
CFLAGS=-O3 -Wall -g


all: Running_process.o Scheduler.o shell.o main.o
	$(CC) $(CFLAGS) $(STD) -o scheduler Running_process.o Scheduler.o shell.o main.o

test: Running_process.o Scheduler.o shell.o test_main.o
	$(CC) $(CFLAGS) $(STD) -o test_scheduler Running_process.o Scheduler.o shell.o test_main.o

test_main.o: test_main.cpp
	$(CC) $(CFLAGS) $(STD) -c test_main.cpp

main.o: main.cpp 
	$(CC) $(CFLAGS) $(STD) -c main.cpp

shell.o: shell.cpp shell.h
	$(CC) $(CFLAGS) $(STD) -c shell.cpp shell.h

Scheduler.o: Scheduler.cpp Scheduler.h Running_process.o
	$(CC) $(CFLAGS) $(STD) -c Scheduler.cpp Scheduler.h

Running_process.o: Running_process.cpp Running_process.h
	$(CC) $(CFLAGS) $(STD) -c Running_process.cpp Running_process.h

clean:
	rm -rf scheduler *.o *.gch test_scheduler
