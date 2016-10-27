/* 
    File: periodictask.C

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

#include<assert.h>

#include "utils.H"
#include "scheduler.H"
#include "periodictask.H"

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
/* METHODS FOR CLASS   P e r i o d i c T a s k  */
/*--------------------------------------------------------------------------*/

PeriodicTask::PeriodicTask(const        char      _name[],
			   const struct timeval & _period,
			   const struct timeval & _execution_time) 
  : Schedulable(_name) {

  period.tv_sec          = _period.tv_sec;
  period.tv_usec         = _period.tv_usec;

  execution_time.tv_sec  = _execution_time.tv_sec;
  execution_time.tv_usec = _execution_time.tv_usec;

}

PeriodicTask::~PeriodicTask() {
  assert(FALSE);
}

void PeriodicTask::Get_Period(struct timeval & _period) {

  _period.tv_sec          = period.tv_sec;
  _period.tv_usec         = period.tv_usec;

}

void PeriodicTask::Get_Execution_Time(struct timeval & _execution_time) {

  _execution_time.tv_sec  = execution_time.tv_sec;
  _execution_time.tv_usec = execution_time.tv_usec;

}

int PeriodicTask::Execute() {
  for(;;) {
    Schedulable::Execute();
    usleep(period.tv_sec * 1000000 + period.tv_usec);
  }
}
