/* 
    File: scheduler.C

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 04/12/13

    Implementation of the Scheduler.

    This file provides the implementation of a FIFO Scheduler.

*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "scheduler.h"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* METHODS FOR CLASS   S c h e d u l e r  */
/*--------------------------------------------------------------------------*/


Scheduler::Scheduler(std::string _name) {
	current_task = nullptr;
	name = _name;
	/*
	 	scheduler_mutex is now constructed
	 	inside the scheduler class.
	 */
	//scheduler_mutex = new PthreadSemaphore(1);
}

Scheduler::~Scheduler() {
	threadsafe_console_output.println("Scheduler::~Scheduler");
	/*
	 	scheduler_mtex is now destructed
	 	automatically.
	 */
	//delete scheduler_mutex;
}

Schedulable * Scheduler::Current_Task() {
	return current_task;
}

/* FIFO IMPLEMENTATION! */

int Scheduler::enqueue(Schedulable * _task) {
	threadsafe_console_output.println("Scheduler::enqueue");
	//scheduler_mutex->P();
	scheduler_mutex.P();
	ready_queue.push_back(_task);
	//scheduler_mutex->V();
	scheduler_mutex.V();
	return 0;
}

int Scheduler::Resume(Schedulable * _task) {
	threadsafe_console_output.println("Resume (" + _task->name + ")");
	return(enqueue(_task));
}

int Scheduler::Start(Schedulable * _task) {
	threadsafe_console_output.println("START (" + _task->name + ")");
	if (Resume(_task)) 
		return -1;
	else
		return(_task->Block());
}

Schedulable * Scheduler::dequeue() {
	threadsafe_console_output.print("Scheduler::dequeue (");
	Schedulable * t;
	//scheduler_mutex->P();
	scheduler_mutex.P();
	if (ready_queue.empty())
		t = nullptr;
	else {
		t = ready_queue.front();
		ready_queue.pop_front();
	}
	if (t) threadsafe_console_output.print(t->name); else threadsafe_console_output.print("NULL");
	threadsafe_console_output.println(")");
	//scheduler_mutex->V();
	scheduler_mutex.V();
	return t;
}

int Scheduler::Yield() {
	assert(current_task);
	threadsafe_console_output.println("Yield (" + current_task->name + ")");
	Schedulable * new_task = dequeue();
	Schedulable * old_task = current_task;

	if ((!new_task) || (new_task == current_task)) 
		/* DO NOTHING */
		return 0;

	current_task = new_task;
	new_task->Unblock();
	old_task->Block();

	return 0;
}

int Scheduler::Finish_Task() {
	assert(current_task);
	threadsafe_console_output.println("Finishing task (" + current_task->name + ")");
	Schedulable * new_task = dequeue();
	Schedulable * old_task = current_task;
	
	if (new_task == current_task) {
		/* 
		 	If task is still in the queue,
		 	finish recursively until all
		 	entries are deleted. 
		 
		 	This assumes that no entries are
		 	out of sequence, so it is not
		 	foolproof, but it seems sufficient
		 	for the time being.
		 */
		Finish_Task();
		return 0;
	}
	
	current_task = new_task;
	
	if(current_task == nullptr) {
		/* NO MORE TASKS, FINISH UP */
		/*
		 	Note that if all threads are
		 	now finished, but the Scheduler
		 	is on the heap and not in its
		 	own thread to be deallocated,
		 	simply returning 0 (as does the
		 	original code by using Yield)
		 	is a memory leak.
		 */
		delete this;
		return 0;
	}
	else {
		/*
		 	The original code blocked the
		 	old task in Yield, even if the old task
		 	was finished. This prevents their
		 	destructors from ever being called,
		 	and we have a second memory leak.
		 */
		new_task->Unblock();
	}
	
	return 0;
}

int Scheduler::Kick_Off() {
	threadsafe_console_output.println("Scheduler::Kick_Off");
	current_task = dequeue();
	assert(current_task);
	current_task->Unblock();
}
