/* 
    File: task.cpp

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

#include <assert.h>
#include <pthread.h>
#include <errno.h>

#include <string>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>


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
  //  assert(0);
}

std::string Task::Name() {
  return name;
}

void * tfunc(void * args) {
  std::cout << "in tfunc " << std::endl << std::flush;
  Task * task_instance = (Task *)args;
  std::cout << "in tfunc " << task_instance->Name() << std::endl << std::flush;
  task_instance->CarrierForRun();
	delete task_instance;
	return nullptr;
}

int Task::Start() {
  std::cout << "starting task " << Name() << std::endl << std::flush;

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
    std::cout << "Task " << name << " is running now[" << i << "].\n" << std::flush;
    usleep(random()%1000000);
  }
}

void Task::GracefullyExitMainThread() {
  pthread_exit(NULL);
}
