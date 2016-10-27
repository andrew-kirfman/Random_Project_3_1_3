/* 
    File: task.C

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 04/12/13

*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "task.h"

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
/* METHODS FOR CLASS   T a s k  */
/*--------------------------------------------------------------------------*/


Task::~Task() {
	threadsafe_console_output.println("task [" + name + "] destructing");
}

std::string Task::Name() {
	return name;
}

void * Task::tfunc(void * args) {
	threadsafe_console_output.println("in tfunc ");
	Task * task_instance = (Task *)args;
	threadsafe_console_output.println("in tfunc " + task_instance->Name());
	task_instance->CarrierForRun();
	/*
	 	Task objects are assumed to be on the heap,
	 	and the thread that created them can't
	 	be assumed to have responsibility for them
	 	in this context. In fact, we know that 
	 	that it hands that responsibility over
	 	to the Scheduler-related classes because
	 	the main thread gracefully exits. Thus,
	 	it became necessary to insert the delete line.
	 
	 	NULL was changed to nullptr because the latter
	 	tends to be safer, and it's available because
	 	C++ is now being used instead of C.
	 */
	delete task_instance;
	//return NULL;
	return nullptr;
}

int Task::Start() {
	threadsafe_console_output.println("starting task " + Name());

	/*
	 	The original code use fprintf with stderr
	 	and strerror(error) where error was the
	 	return value of pthread_create and pthread_detach.
	 	This is equivalent and involves less typing. :)
	 */
	if ((errno = pthread_create(&thread_id, NULL, tfunc, this)) != 0) {
		perror("pthread_create failed");
		return errno;
	}
	if ((errno = pthread_detach(thread_id)) != 0) {
		perror("pthread_detach failed");
		return errno;
	}
	
	return 0;
}


void Task::Run() {
	for (int i = 0; i < DEFAULT_TASK_ITERATIONS; i++) {
		threadsafe_console_output.println("Task " + name + " is running now[" + std::to_string(i) + "].");
		usleep(random()%1000000);
	}
}

void Task::GracefullyExitMainThread() {
	pthread_exit(NULL);
}
