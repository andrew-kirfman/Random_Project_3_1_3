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
#include <iostream.h>

#include <pthread.h>
#include <errno.h>

#include "task.h"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

template<class T, void(T::*mem_fn)()>
void* thunk(void* p)
{
(static_cast<T*>(p)->*mem_fn)();
return 0;
}


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

Task::Task(const char _name[]) {

  /* -- INITIALIZE TASK NAME */
  strncpy(name, _name, MAX_NAME_LEN);
  Start();

}

Task::~Task() {
  //  assert(0);
}

char * Task::Name() {
  return name;
}

void Task::Start() {
  int error;
  if (error = pthread_create(&thread_id, NULL, thunk<Task, &Task::Run>, this))
    fprintf(stderr, "p_create failed: %s\n", strerror(error));
  if (error = pthread_detach(thread_id))
    fprintf(stderr, "p_detach failed: %s\n", strerror(error));
}


void Task::Run() {
  cout << "Task " << name << " is running now.\n" << flush;
}

