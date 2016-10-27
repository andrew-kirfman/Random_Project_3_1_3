/* 
    File: RRscheduler.C

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 08/03/29

    Implementation of the Round Robin Scheduler.

*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <iostream>

#include "schedulable.H"
#include "timer.H"
#include "rrscheduler.H"

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
/* METHODS FOR CLASS   R R S c h e d u l e r  */
/*--------------------------------------------------------------------------*/


RRScheduler::RRScheduler(char _name[], long _quantum) : Scheduler(_name){
  time_quantum  = _quantum;
  quantum_timer = new EndOfQuantumTimer(this);
}

int RRScheduler::Resume(Schedulable* _task) {
  return(Scheduler::Resume(_task));
}

int RRScheduler::Start(Schedulable * _task) {
  return(Scheduler::Start(_task));
}

int RRScheduler::Yield() {
  quantum_timer->Clear();
  Scheduler::Yield();
  return 0;
}

int RRScheduler::Kick_Off() {
  Scheduler::Kick_Off();
}

void RRScheduler::handle_end_of_quantum() {
  cout << "END OF QUANTUM: resume task" << current_task->Name() << endl;
  Scheduler::Resume(current_task);
  cout << "END OF QUANTUM: set time" << endl;
  quantum_timer->Set(time_quantum);
  cout << "END OF QUANTUM: yield ..." << endl;
  Scheduler::Yield();
}

