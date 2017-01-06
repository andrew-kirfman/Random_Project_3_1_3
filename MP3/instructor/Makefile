# ---------------------------------------------------------------------------- #
# Developer: Andrew Kirfman                                                    #
# Project: CSCE-313 Machine Problem #3                                         #
#                                                                              #
# File: ./Makefile                                                             #
# ---------------------------------------------------------------------------- #

CC=g++
STD=-std=c++11
CFLAGS=-Ofast
T1=mp3_part1.cpp
T2=mp3_part2.cpp
T3=mp3_part3.cpp
T4=mp3_part4.cpp
T5=mp3_part5.cpp
T6=mp3_part6.cpp
T7=mp3_part7.cpp


all: $(T1) $(T2) $(T3) $(T4) $(T5) $(T6) $(T7)
	make part1
	make part2
	make part3
	make part4
	make part5
	make part6
	make part7

part1: $(T1)
	$(CC) $(STD) $(CFLAGS) -o part1 mp3_part1.cpp

part2: $(T2)
	$(CC) $(STD) $(CFLAGS) -o part2 mp3_part2.cpp

part3: $(T3)
	$(CC) $(STD) $(CFLAGS) -o part3 mp3_part3.cpp

part4: $(T4)
	$(CC) $(STD) $(CFLAGS) -o part4 mp3_part4.cpp

part5: $(T5) 
	$(CC) $(STD) $(CFLAGS) -o part5 mp3_part5.cpp

part6: $(T6)
	$(CC) $(STD) $(CFLAGS) -o part6 mp3_part6.cpp

part7: $(T7)
	$(CC) $(STD) $(CFLAGS) -o part7 mp3_part7.cpp


clean:
	rm -rf *.o *.gch part1 part2 part3 part4 part5\
		part6 part7 
