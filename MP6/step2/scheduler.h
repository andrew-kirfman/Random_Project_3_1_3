/* 
    File: scheduler.H

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 08/02/11

    Definition of the system scheduler.
                                     
*/

#ifndef _scheduler_h_                   // include file only once
#define _scheduler_h_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "sync_lib.h"
#include "schedulable.h"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CLASS   S c h e d u l e r  */
/*--------------------------------------------------------------------------*/

class Scheduler  {
private:
	//PthreadSemaphore*			scheduler_mutex;
	/*
	 	scheduler_mute has been moved off the heap
	 	as part of cleaning up parts of the code.
	 */
	PthreadSemaphore scheduler_mutex = PthreadSemaphore(1);
	static const int	MAX_NAME_LEN = 15;
	std::string			name = "";
protected:

	Schedulable * current_task;

	/* -- READY QUEUE MANAGEMENT */
	std::list<Schedulable*> ready_queue;
	/* 
	 	This is a simple example of a ready queue that could be used for
	 	a FIFO scheduler. 
	 */

	virtual int           enqueue(Schedulable * _task);
	virtual Schedulable * dequeue();

public:

	/* -- CONSTRUCTOR/DESTRUCTOR */

	Scheduler(std::string _name);

	~Scheduler();

	/* -- THE CURRENTLY RUNNING TASK */

	Schedulable * Current_Task();
	/* Return a pointer to the currently running task. */


	/* -- START SCHEDULING THE TASKS */
	virtual int Kick_Off();

	/* -- SCHEDULING OPERATIONS */

	virtual int Start(Schedulable * _task);
	/* 
	 	Start scheduling this task. This method is called in method
	 	Schedulable::Start() that starts execution of the task, more
	 	specifically, inside method Schedulable::CarrierForRun().
	 	If return value is not zero, task could not be successfully started. 
	 */

	virtual int Stop(Schedulable * _task) {}
	/* Stop scheduling this task. (We don't need to implement it for now.) */

	virtual int Yield();
	/* The calling task gives up the CPU. */
	
	virtual int Finish_Task();
	/* 
	 	The calling task gives up the CPU, but does
	 	not block as in Yield. Tasks are supposed to 
	 	have already completed execution by the time 
	 	this function is called, and are expected to 
	 	clean up their resources immediately afterwards.
	 
	 	Full explanation of this method's purpose is
	 	in task_test_step2.cpp and scheduler.cpp, 
	 	where the code makes certain issues clearer.
	 */

	virtual int Resume(Schedulable * _task);
	/* Resume the given _task. */

};


#endif


