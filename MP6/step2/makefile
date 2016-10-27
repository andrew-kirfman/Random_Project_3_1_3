# makefile

all: task_test_step2

PthreadSemaphore.o: sync_lib.h PthreadSemaphore.cpp
	g++ -std=c++11 -c -g PthreadSemaphore.cpp

KernelSemaphore.o: sync_lib.h KernelSemaphore.cpp
	g++ -std=c++11 -c -g KernelSemaphore.cpp

task.o: sync_lib.h task.h task.cpp
	g++ -std=c++11 -c -g task.cpp

schedulable.o: sync_lib.h schedulable.h scheduler.h task.h schedulable.cpp
	g++ -std=c++11 -c -g schedulable.cpp

scheduler.o: sync_lib.h scheduler.h task.h schedulable.h scheduler.cpp
	g++ -std=c++11 -c -g scheduler.cpp

task_test_step2: sync_lib.h task_test_step2.cpp task.o schedulable.o scheduler.o \
                 PthreadSemaphore.o KernelSemaphore.o
	g++ -std=c++11 -o task_test_step2 task_test_step2.cpp task.o schedulable.o \
               scheduler.o PthreadSemaphore.o KernelSemaphore.o -lpthread

clean:
	rm *.o task_test_step2