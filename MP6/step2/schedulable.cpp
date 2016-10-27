/* 
    File: schedulable.C

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 08/02/25

*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "scheduler.h"
#include "schedulable.h"

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
/* METHODS FOR CLASS   S c h e d u l a b l e  */
/*--------------------------------------------------------------------------*/

Schedulable::Schedulable(std::string _name, Scheduler * _sched) : Task(_name) {
	threadsafe_console_output.println("Schedulable (" + _name + ") constructor");
	sched = _sched;
	/*
	 	Construction of this semaphore has been
	 	moved inside the class.
	 */
	//block_semaphore = new PthreadSemaphore(0);
}

Schedulable::~Schedulable() {
	/*
	 	No need to kill the process,
	 	seems a bit extreme...
	 */
//	assert(FALSE);
}

void Schedulable::CarrierForRun() {

	/* -- PUT THE TASK ON THE WAITING QUEUE OF THE SCHEDULER. */
	assert(sched->Start(this) == 0);


	/* -- NOW YOU HAVE THE CPU. EXECUTE YOUR CODE. */
	Run();

	/* -- TASK IS DONE WITH EXECUTING. GIVE UP THE CPU. 
		(i.e. HAVE THE SCHEDULER PICK THE NEXT TASK.) */
	
	/*
	 	Finish_Task was written solely to be used
	 	instead of Yield at this point in the code.
	 	Refer to comments in scheduler.h and
	 	task_test_step2.cpp for full explanation.
	 
	 	Short explanation: Yield blocks the calling thread, which is
	 	fine most places except here since here it
	 	prevents the thread from finishing and risks
	 	leaking resources. Finish_Task mimics Yield's
	 	behavior, but allows the calling thread to terminate.
	 */
	//sched->Yield();
	sched->Finish_Task();
	
}

int Schedulable::Start() {
	/* 
	 	This is a simple schedulable task with a single execution of the
	 	"Run" function. More sophisticated Schedulables, such as "Periodic_Task",
		 periodically invoke the "Run" function.
	*/

	/* -- START THE TASK */
	
	return Task::Start();
	
}

/*
 	Changed arrows to dots, since block_semaphore
 	no longer refers to a pointer. See comments
 	in schedulable.h for full explanation.
 
 	Short explanation: This allows object 
 	destruction be taken care of automatically.
 */
int Schedulable::Block() {
//	return(block_semaphore->P());
	return(block_semaphore.P());
}

int Schedulable::Unblock() {
//	return(block_semaphore->V());
	return(block_semaphore.V());
}
