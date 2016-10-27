/* 
    File: periodictask.H

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 08/02/11

    Definition of Periodic Tasks. 
    This definition follows very loosely the one in the Real-Time 
    Specification for Java (RTSJ).
    Periodic Tasks are schedulables that have a period and an 
    execution time.
                                     
*/

#ifndef _periodictasks_H_                   // include file only once
#define _periodictasks_H_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <sys/time.h>
#include "schedulable.H"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CLASS   P e r i o d i c T a s k s */
/*--------------------------------------------------------------------------*/

class PeriodicTask : public Schedulable {

protected:

  virtual int Execute();
  /* Packages the run method so that the scheduler can have a handle for
     "scheduling" the task.
  */

  timeval period;
  timeval execution_time;

public:

  /* -- CONSTRUCTOR/DESTRUCTOR */

  PeriodicTask(const char      _name[], 
	       const timeval & _period,
	       const timeval & _execution_time);
  /* Create and initialize the new periodic task. 
     Invoke Task::start() to start execution of task. */

  ~PeriodicTask();

  /* -- ACCESSORS */

  void Get_Period(struct timeval & _period);

  void Get_Execution_Time(struct timeval & _execution_time);

  /* -- THREAD EXECUTION CONTROL */

};


#endif


