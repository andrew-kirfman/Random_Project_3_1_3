/* 
    File: timer.h

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 08/03/25

*/

#ifndef _timer_h_                   // include file only once
#define _timer_h_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <pthread.h>
#include <vector.h>
#include <list.h>
#include <queue.h>
#include "semaphore.h"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */ 
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CLASS   T i m e r  */
/*--------------------------------------------------------------------------*/

class Timer {
private:
  static pthread_t timer_thread_id;
  static long clock_interval; /* in musec */

  /* LIST OF TIMERS WAITING TO BE TRIGGERED */
  friend class PrioritizeTimers;
  static priority_queue<Timer, vector<Timer>, PrioritizeTimers> timer_queue;

  static void * TimerHandlerThread(void * args);
  static Semaphore mutex;

  /* WHEN SHOULD THE TIMER FIRE? */
  timeval tv;

public:

  static int Init(long _clock_interval_musec);
  /* Initialize the timer handling. This should be called by the main 
     thread early on, certainly before it creates the first timers.
     An implementation could look as follows: First, the main thread masks the
     SIGALARM (or SIGVTALARM) signals; this should be done before any 
     tasks are created, in order for the tasks to have the signals masked. 
     Then, method "Init()" would create a new timer thread, which would 
     process the requests in the timer queue. For this thread, the 
     signal would be unblocked. */
 
  Timer();
  /* Construct timer that calles "event_handler" when it fires. */

  virtual void Set(long _imusec);
  /* Set this timer to fire after time '_imusec' from now. */

  virtual void Clear();
  /* Cancel the timer. */

  virtual void Event_Handler() {}
  /* This function is called when the timer expires. */

  static void Print_Now();
  /* For debugging purposes. Prints current time. */

};

class PrioritizeTimers {
  /* This class is used to keep the priority queue of Timers sorted. */
public :
  int operator()( const Timer &x, const Timer &y );
};
#endif


