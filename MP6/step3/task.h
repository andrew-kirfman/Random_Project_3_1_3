/* 
    File: task.h

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 08/02/11

*/

#ifndef _task_h_                   // include file only once
#define _task_h_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "utils.h"
#include <cstring>
#include <pthread.h>



/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CLASS   T a s k */
/*--------------------------------------------------------------------------*/

class Task {
  friend void * tfunc(void * args);

protected:

  /* -- NAME */
  static const int   MAX_NAME_LEN = 15;
	static const int	DEFAULT_TASK_ITERATIONS = 100;
	std::string name	= "";

  /* -- IMPLEMENTATION */
  pthread_t thread_id;
  /* If you implement tasks using pthread, you may need to store
     the thread_id of the thread associated with this task.
  */

  virtual void CarrierForRun() { Run(); }
  /* This method is called by 'tfunc' to invoke the 'Run()' method
     of the particular task implementation. 
     We need to package the 'Run()' function in this way because 
     more complicated tasks, for example schedulable tasks, need to 
     set up things as the newly created thread before they can 
     invoke the 'Run()' method. All that functionality can be implemented
     in the 'CarrierForRun()' method of the derived task class. 
  */

public:

  /* -- CONSTRUCTOR/DESTRUCTOR */

	Task(std::string _name) {
    /* Create, initialize the new task. The task is started
       with a separate invocation of the Start() method. */
	  name = _name;
  }


  ~Task();

  /* -- ACCESSORS */
  
	std::string Name();
  /* Return the name of the task. */

  /* -- TASK LAUNCH */

  virtual int Start();
  /* This method is used to start the thread. For basic tasks implemented
     using pthreads, this is where the thread is created and started.
     For schedulable tasks (derived from class Task) this is where the 
     thread is created and handed over to the scheduler for execution.
     The functionality of the task is defined in "Run()"; see below.
  */

  /* -- TASK FUNCTIONALITY */

  virtual void Run() = 0;
  /* The method that is executed when the task object is started.
     When the method returns, the thread can be terminated.
     The method returns 0 if no error. */

  /* -- MAIN THREAD TERMINATION */
  static void GracefullyExitMainThread();
  /* This function is called at the end of the main() function. 
     Depending on the particular thread implementation, we have to
     make sure that the main thread (i.e., the thread that runs 
     executes the main function) either waits until all other threads
     are done or exits in a way that does not terminate them. 
  */


};


#endif


