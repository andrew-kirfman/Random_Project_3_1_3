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

#include <pthread.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include "timer.H"

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

bool greater_timeval(const timeval * tv1, const timeval * tv2) {
  return ((tv1->tv_sec > tv2->tv_sec) 
	  || ((tv1->tv_sec == tv2->tv_sec) && (tv1->tv_usec > tv2->tv_usec)));
}

bool passed(const timeval * tp) {
  timeval now;
  gettimeofday(&now, NULL);
  return(greater_timeval(&now, tp));
}

void add_to_timeval(timeval * tv, long musec) {
  tv->tv_sec  += musec / 1000000;
  tv->tv_usec += musec % 1000000;
}


/*--------------------------------------------------------------------------*/
/* METHODS FOR CLASS   T i m e r  */
/*--------------------------------------------------------------------------*/
pthread_t Timer::timer_thread_id;
long      Timer::clock_interval;
Semaphore Timer::mutex(1);
priority_queue<Timer, vector<Timer>, PrioritizeTimers> Timer::timer_queue;

void * Timer::TimerHandlerThread(void * args) {

  /* -- FIRST, UNMASK THE SIGNAL */
  cout << "Timer::TimerHandlerThread: unmask signal..." << flush;
  sigset_t s;
  sigemptyset(&s);
  sigaddset(&s, TIMER_SIGNAL);
  //  assert(sigprocmask(SIG_UNBLOCK, &s, NULL) == 0);
  cout << "done\n" << flush;

  /* -- SET UP THE TIMER SIGNAL */
  cout << "Timer::TimerHandlerThread: unmask signal..." << flush;
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
    Print_Now();
    printf("received "); 
    if (signo == TIMER_SIGNAL) printf("TIMER_SIGNAL. \n");
    else printf("some signal (%d). \n", signo);
    fflush(stdout);
  }

}

int Timer::Init(long _clock_interval_musec) {

  clock_interval = _clock_interval_musec;

  /* -- FIRST, MASK THE SIGNAL */
  sigset_t s;
  sigemptyset(&s);
  sigaddset(&s, TIMER_SIGNAL);
  assert(sigprocmask(SIG_BLOCK, &s, NULL) == 0);

  /* -- CREATE A THREAD THAT HANDLES THE PERIODIC SIGNALS */
  int error;
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

Timer::Timer() {
  
}

void Timer::Set(long _imusec) {
  mutex.P();
  gettimeofday(&tv, NULL);
  add_to_timeval(&tv, _imusec);

  timer_queue.push(*this);
  mutex.V();
}

void Timer::Clear() {
  mutex.P();
  
  mutex.V();
}

void Timer::Print_Now() {
  timeval now;
  assert (gettimeofday(&now, NULL) == 0);

  printf("time now: %10ld sec / %10ld musec", now.tv_sec, now.tv_usec);
}

int PrioritizeTimers::operator()( const Timer &x, const Timer &y ) {
  return greater_timeval(&(y.tv), &(x.tv));
}

  
