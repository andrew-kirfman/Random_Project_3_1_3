/* 
    File: timer.C

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 08/3/28

*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

#define TIMER_SIGNAL SIGALRM
#define TIMER_TYPE   ITIMER_REAL

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/
#include <assert.h>
#include <pthread.h>
#include <errno.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>

#include <list>
#include <iostream>
#include <cstring>


#include "semaphore.H"
#include "timer.H"
#include <stdio.h>

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
/* auxiliary functions */
/*--------------------------------------------------------------------------*/

bool greater_timeval(const timeval  tv1, const timeval  tv2) {
  return ((tv1.tv_sec > tv2.tv_sec) 
	  || ((tv1.tv_sec == tv2.tv_sec) && (tv1.tv_usec > tv2.tv_usec)));
}

bool time_passed(const timeval tp) {
  timeval now;
  gettimeofday(&now, NULL);
  return(greater_timeval(now, tp));
}

void add_to_timeval(timeval * tv, long musec) {
  tv->tv_sec  += musec / 1000000;
  tv->tv_usec += musec % 1000000;
}

void print_now() {
  timeval now;
  gettimeofday(&now, NULL);

  printf("time now: (%ld sec %ld usec)", now.tv_sec, now.tv_usec);
}

/*--------------------------------------------------------------------------*/
/* LOCAL VARIABLES  */
/*--------------------------------------------------------------------------*/

list<Timer*> timer_queue;
/* Queue of waiting timer events. */

Semaphore Timer::mutex(1);
/* This mutex enforces mutual exclusive access to timer queue. */

long      Timer::clock_interval;

/*--------------------------------------------------------------------------*/
/* MANAGING THE TIMER QUEUE  */
/*--------------------------------------------------------------------------*/

bool Timer::compare_timers(const Timer * t1, const Timer * t2) {
  /* This one is a static method of class timer because it accesses private 
     variables. */
  return (greater_timeval(t2->tv, t1->tv));
}

Timer * first_timer_in_queue() {
  if (!timer_queue.empty())
    return timer_queue.front();
  else 
    return NULL;
}

/*--------------------------------------------------------------------------*/
/* SEPARATE THREAD TO MANAGE TIMER EVENTS. */
/*--------------------------------------------------------------------------*/

void * Timer::TimerHandlerThread(void * args) {

  /* -- SET UP THE TIMER SIGNAL */
  cout << "Timer::TimerHandlerThread: set up timer signal..." << flush;
  long sec  = clock_interval / 1000000;
  long usec = clock_interval % 1000000;
  struct itimerval itv;
  itv.it_value.tv_sec     = sec;
  itv.it_value.tv_usec    = usec;
  itv.it_interval.tv_sec  = sec;
  itv.it_interval.tv_usec = usec;
  setitimer(TIMER_TYPE, &itv, NULL);
  cout << "done\n" << flush;
 
  /* -- START CATCHING SIGNALS */
  sigset_t intmask;
  int      signo;

  assert(sigemptyset(&intmask) != -1);
  assert(sigaddset(&intmask, TIMER_SIGNAL) != -1);
  
  for(;;) {
    cout << "Timer::TimerHandlerThread: wait for signal..." << flush;
    assert(sigwait(&intmask, &signo) == 0);
    print_now();
    cout << "received "; 
    if (signo == TIMER_SIGNAL) cout << "TIMER_SIGNAL. \n";
    else printf("some signal (%d). \n", signo);
    cout << flush;

    /* -- CHECK FOR EXPIRED TIMER EVENTS */
    if (Timer * t = first_timer_in_queue()) {
      if (time_passed(t->tv)) {
        
	mutex.P();
	timer_queue.pop_front();
	mutex.V();
	t->Handle_Event();
      }
    }
  }
}

/*--------------------------------------------------------------------------*/
/* INITIALIZE TIMER MANAGEMENT: START TIMER PROCESSING THREAD etc. */
/*--------------------------------------------------------------------------*/

int Timer::Init(long _clock_interval_musec) {

  clock_interval = _clock_interval_musec;

  /* -- FIRST, MASK THE SIGNAL */
  sigset_t s;
  sigemptyset(&s);
  sigaddset(&s, TIMER_SIGNAL);
  assert(sigprocmask(SIG_BLOCK, &s, NULL) == 0);

  /* -- CREATE A THREAD THAT HANDLES THE PERIODIC SIGNALS */
  int error;
  pthread_t timer_thread_id;
  if (error = pthread_create(&timer_thread_id, NULL, TimerHandlerThread, NULL)) {
    fprintf(stderr, "p_create failed: %s\n", strerror(error));
    return error;
  }

  if (error = pthread_detach(timer_thread_id)) {
    fprintf(stderr, "p_detach failed: %s\n", strerror(error));
    return error;
  } 
  /* -- NOTHING MORE TO DO HERE. */
  return 0;
}

/*--------------------------------------------------------------------------*/
/* TIMER MANAGEMENT METHODS: CONSTRUCTOR, SET, CLEAR */
/*--------------------------------------------------------------------------*/

Timer::Timer() {
  
}

void Timer::Set(long _imusec) {
  gettimeofday(&tv, NULL);
  add_to_timeval(&tv, _imusec);

  mutex.P();
  timer_queue.push_back(this);
  timer_queue.sort(compare_timers);
  mutex.V();
}

int Timer::Clear() {
  int result = 0;

  mutex.P();
  if (timer_queue.empty())
    result = -1;
  timer_queue.remove(this);
  mutex.V();
  return result;
}


