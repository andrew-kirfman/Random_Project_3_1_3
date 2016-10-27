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

#include <assert.h>
#include <string.h>
#include <iostream>

#include <pthread.h>
#include <errno.h>

#include "task.H"

using namespace std;

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
  //  assert(0);
}

char * Task::Name() {
  return name;
}

void * tfunc(void * args) {
  cout << "in tfunc " << endl << flush;
  Task * task_instance = (Task *)args;
  cout << "in tfunc " << task_instance->Name() << endl << flush;
  task_instance->Run();
  return NULL;
}

void Task::Start() {
  int error;
  cout << "starting task " << Name() << endl << flush;

  if (error = pthread_create(&thread_id, NULL, tfunc, this))
    fprintf(stderr, "p_create failed: %s\n", strerror(error));
  if (error = pthread_detach(thread_id))
    fprintf(stderr, "p_detach failed: %s\n", strerror(error));
}


void Task::Run() {
  for (int i = 0; i < 100; i++) {
    cout << "Task " << name << " is running now[" << i << "].\n" << flush;
    usleep(random()%1000000);
  }
}

void Task::GracefullyExitMainThread() {
  pthread_exit(NULL);
}
