# makefile

all: task_test_step2

task.o: task.H task.C
	g++ -c -g task.C

semaphore.o: semaphore.H semaphore.C
	g++ -c -g semaphore.C

schedulable.o: schedulable.H scheduler.H task.H semaphore.H schedulable.C
	g++ -c -g schedulable.C

scheduler.o: scheduler.H task.H schedulable.H scheduler.C
	g++ -c -g scheduler.C

task_test_step2: task_test_step2.C task.o schedulable.o scheduler.o \
                 semaphore.o utils.H
	g++ -o task_test_step2 task_test_step2.C task.o schedulable.o \
               scheduler.o semaphore.o -lpthread

clean:
	rm *.o task_test_step2