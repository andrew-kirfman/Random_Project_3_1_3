# makefile

all: task.o task_test_step1

task.o: task.h task.cpp
	g++ -std=c++11 -c task.cpp

task_test_step1: task_test_step1.cpp task.o utils.h
	g++ -std=c++11 -o task_test_step1 task_test_step1.cpp task.o -lpthread

clean:
	rm *.o task_test_step1