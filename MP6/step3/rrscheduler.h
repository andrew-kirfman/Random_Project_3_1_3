/* 
    File: RRscheduler.h

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University

    Date  : 08/03/28

    Header file for Round-Robin extension to Scheduler.

*/

#ifndef _rrscheduler_h_                   // include file only once
#define _rrscheduler_h_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "timer.h"
#include "scheduler.h"
#include "schedulable.h"
#include "stdio.h"

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

class EndOfQuantumTimer;

/*--------------------------------------------------------------------------*/
/* CLASS    R R S c h e d u l e r   :              class for a RR scheduler */
/*--------------------------------------------------------------------------*/


class RRScheduler : public Scheduler {

protected:
  long           time_quantum;  /* in musec */
  Timer        * quantum_timer;


  friend class EndOfQuantumTimer;

  virtual void handle_end_of_quantum();

public:

  RRScheduler(char _name[], long _quantum);
  /* Instantiate a new scheduler. The quantum is given in musec. */

  ~RRScheduler();
  /* Don't need to implement for now... */

  /* -- START SCHEDULING THE TASKS */
  virtual int Kick_Off();

  /* -- SCHEDULING OPERATIONS */

  virtual int Start(Schedulable * _task);
  /* Start scheduling this task. See Scheduler::Start(). */

  /*  virtual int Stop(Schedulable * _task) {} */
  /* Stop scheduling this task. (We don't need to implement it for now.) */

  virtual int Yield();
  /* The calling task gives up the CPU. 
     Basically the same as Scheduler::Yield(), but we need to 
     take care of the quantum timer first. */

  virtual int Resume(Schedulable * _task);
  /* Resume the given _task. */

};
    
/*--------------------------------------------------------------------------*/
/* CLASS    E o Q T i m e r   :  end-of-quantum management for RR scheduler */
/*--------------------------------------------------------------------------*/

class EndOfQuantumTimer : public Timer {

private:
  RRScheduler * sched;

public:
  EndOfQuantumTimer(RRScheduler * _sched) {
//EndOfQuantumTimer(RRScheduler * _sched) : Timer() {
    sched = _sched;
  }      

  virtual void Handle_Event() {
    printf("---------------------END-OF-QUANTUM TIMER: -----------------------------");
    printf("\n");
    sched->handle_end_of_quantum();
  }   

};

#endif




