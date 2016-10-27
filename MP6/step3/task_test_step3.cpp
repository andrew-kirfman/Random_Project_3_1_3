/* TEST PROGRAM FOR STEP 3 of MP2 */

#include <iostream>


#include "task.h"
#include "schedulable.h"
#include "rrscheduler.h"

#include "timer.h"
#include <unistd.h>

/* --------------------------------------------------------------------------*/
/* Class DISK */
/* --------------------------------------------------------------------------*/

class Disk {
  /* This class simulates the behavior of a disk device. 
     The program issues a disk operation (the class supports only
     read() operations), then self suspends (just like with a real disk,) and 
     waits for a interrupt to know that operation completed. 
     In this implementation, we emulate the I/O hardware interrupt by
     a timer interrupt, which fires at the end of the "disk operation".
     When the timer fires, the task is resumed, and the disk operation returns.
  */
private:

  class DiskOperationTimer : public Timer {
  private:
    Disk        * disk;
    Schedulable * task;
  public:
    DiskOperationTimer(Disk * _disk, Schedulable * _task) : Timer() {
      disk = _disk;
      task = _task;
    }
    virtual void Handle_Event() {
      printf("END-OF-EVENT TIMER to indicate END OF DISK OPERATION (disk %s)\n", 
	     disk->name.c_str());
      /* -- DISK OPERATION COMPLETED. RESUME TASK. */
      disk->sched->Resume(task);
    }
  };

  int          latency;		/* latency of disk operation in usec.          */
  Scheduler  * sched;		/* need a pointer to scheduler to resume task. */
  std::string name = "";	/* give the disk a name!                       */


public:

	Disk(std::string _name, int _latency, Scheduler * _sched) {
		name = _name;
    latency = _latency;
    sched   = _sched;
  }

  void read() {
    DiskOperationTimer t(this, sched->Current_Task());
    /* -- HERE WE WOULD SEND THE COMMANDS TO THE DISK, AND THE DISK WOULD
          RETURN WITH AN INTERRUPT WHEN IT IS DONE. WE SIMULATE THIS BY 
          SETTING A TIMER. */
    t.Set(latency);
    /* -- GIVE UP THE CPU */
    sched->Yield();
    /* -- THE TASK WILL BE RESUMED HERE AFTER THE TIMER FIRES. */
  }

};

/* --------------------------------------------------------------------------*/
/* HERE COME THE TASKS */
/* --------------------------------------------------------------------------*/

/* -- THIS TASK DOES A LOT OF DISK ACCESSES. */
class IOBoundTask : public Schedulable {
private:
	Disk disk;
public:
	IOBoundTask(std::string _name, Scheduler * _sched) :
	Schedulable(_name, _sched), disk("disk", 100000, _sched) {}
	
	/* This task has a very slow disk, at 100msec latency. */

  void Run() {
    std::cout << "IOBoundTask [" << name << "] running\n" << std::flush;
    for (int i = 0; i < DEFAULT_TASK_ITERATIONS; i++) {
      if (i % 20 == 1) {
	std::cout << name << " reading data from disk\n" << std::flush;
        disk.read();
      }
      std::cout << name << " doing some computation " << i << "\n" << std::flush;
      usleep(10000);
    }
    std::cout << "IOBoundTask [" << name << "] done!\n" << std::flush;
  }
};

/* -- WE KNOW THIS TASK FROM EARLIER. */
class AvionicsTask : public Schedulable {
public:
	AvionicsTask(std::string _name, Scheduler * _sched) :
	Schedulable(_name, _sched) {}
	
  void Run() {
    std::cout << "Avionics System [" << name << "] running\n" << std::flush;
    std::cout << "STOP WITH ^C\n" << std::flush;

    for (int i = 0; i < DEFAULT_TASK_ITERATIONS; i++) {
      std::cout << name << " waiting for next refresh interval\n" << std::flush;
      if (i % 10 == 1) {
	sched->Resume(this);
	sched->Yield();
      }
      std::cout << name << " refreshing avionics screen\n" << std::flush;
      usleep(12000);
    }
    std::cout << "Avionics System [" << name << "] done!\n" << std::flush;
  }
};

/* --------------------------------------------------------------------------*/
/* MAIN PROGRAM */
/* --------------------------------------------------------------------------*/

int main(int argc, char * argv[]) {

  /* -- INITIALIZE THE TIMER "CLOCK" TO WHATEVER INTERVAL IS APPROPRIATE. */
  Timer::Init(500000);

  /* -- CREATE SCHEDULER */
  char sche[] = "scheduler";
  RRScheduler * system_scheduler = new RRScheduler(sche, 4000000);

  /* -- CREATE TASKS */
//  char a1[] = "io task";
//  char a2[] = "avionics task";
//  IOBoundTask  task1(a1,       system_scheduler);
//  AvionicsTask task2(a2, system_scheduler);
	Schedulable * task1 = new IOBoundTask(std::string("io task"), system_scheduler);
	Schedulable * task2 = new AvionicsTask(std::string("avionics task"), system_scheduler);
	
  /* -- LAUNCH TASKS */
//  task1.Start();
//  task2.Start();
	task1->Start();
	task2->Start();

  std::cout << "end of launch" << std::flush;

  /* -- MAKE SURE THAT EVERYBODY IS SAFELY STARTED (CLUDGE!) */
  usleep(1000000);

  /* -- START SCHEDULING */
  system_scheduler->Kick_Off();
  usleep(100000000);
  for(;;)
    pause();

  /* -- Have the parent thread get out of the way. */
  Task::GracefullyExitMainThread();
}
