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
	std::cout << "task [" << name << "] destructing" << std::endl << std::flush;
}

std::string Task::Name() {
	return name;
}

void * tfunc(void * args) {
	std::cout << "in tfunc " << std::endl << std::flush;
	Task * task_instance = (Task *)args;
	std::cout << "in tfunc " << task_instance->Name() << std::endl << std::flush;
	task_instance->Run();
	delete task_instance;
	return nullptr;
}

void Task::Start() {
	std::cout << "starting task " << Name() << std::endl << std::flush;

	if ((errno = pthread_create(&thread_id, NULL, tfunc, this)) != 0) {
		perror("p_create failed");
	}
	if ((errno = pthread_detach(thread_id)) != 0) {
		perror("p_detach failed");
	}
}


void Task::Run() {
	for (int i = 0; i < MAX_TASK_ITERATIONS; i++) {
		std::cout << "Task " << name << " is running now[" << i << "].\n" << std::flush;
		usleep(random()%1000000);
	}
}

void Task::GracefullyExitMainThread() {
	pthread_exit(NULL);
}
