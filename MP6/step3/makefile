# makefile

all: task_test_step3

task.o: task.h task.cpp
	g++ -std=c++11 -c -g task.cpp

semaphore.o: semaphore.h semaphore.cpp
	g++ -std=c++11 -c -g semaphore.cpp

timer.o: timer.h timer.cpp
	g++ -std=c++11 -c -g timer.cpp

schedulable.o: schedulable.h scheduler.h task.h semaphore.h schedulable.cpp
	g++ -std=c++11 -c -g schedulable.cpp

scheduler.o: scheduler.h task.h schedulable.h scheduler.cpp
	g++ -std=c++11 -c -g scheduler.cpp

rrscheduler.o: rrscheduler.h timer.h scheduler.h schedulable.h rrscheduler.cpp
	g++ -std=c++11 -c -g rrscheduler.cpp

task_test_step3: task_test_step3.cpp task.o timer.o schedulable.o scheduler.o \
                rrscheduler.o  semaphore.o utils.h
	g++ -std=c++11 -o task_test_step3 task_test_step3.cpp task.o schedulable.o \
               scheduler.o rrscheduler.o semaphore.o timer.o  -lpthread

clean:
	rm *.o task_test_step3
