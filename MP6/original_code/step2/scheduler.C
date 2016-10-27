/* 
    File: scheduler.C

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 04/12/13

    Implementation of the Scheduler.

    This file provides the implementation of a FIFO Scheduler.

*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include<assert.h>
#include<iostream>
#include<list>
#include<string>

#include "utils.H"

#include "scheduler.H"

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
/* METHODS FOR CLASS   S c h e d u l e r  */
/*--------------------------------------------------------------------------*/


Scheduler::Scheduler(char _name[]) {
  current_task = NULL;
  strncpy(name, _name, MAX_NAME_LEN);
  scheduler_mutex = new Semaphore(1);
}

Scheduler::~Scheduler() {
}

Schedulable * Scheduler::Current_Task() {
  return current_task;
}

/* FIFO IMPLEMENTATION! */

int Scheduler::enqueue(Schedulable * _task) {
  cout << "Scheduler::enqueue\n" << flush;
  scheduler_mutex->P();
  ready_queue.push_back(_task);
  scheduler_mutex->V();
  return 0;
}

int Scheduler::Resume(Schedulable * _task) {
  cout << "Resume (" << _task->name << ")\n" << flush;
  return(enqueue(_task));
}

int Scheduler::Start(Schedulable * _task) {
  cout << "START (" << _task->name << ")\n" << flush;
  if (Resume(_task)) 
    return -1;
  else
    return(_task->Block());
}

Schedulable * Scheduler::dequeue() {
  cout << "Scheduler::dequeue (" << flush;
  Schedulable * t;
  scheduler_mutex->P();
  if (ready_queue.empty())
    t = NULL;
  else {
    t = ready_queue.front();
    ready_queue.pop_front();
  }
  if (t) cout << t->name; else cout << "NULL";
  cout << ")\n" << flush;
  scheduler_mutex->V();
  return t;
}

int Scheduler::Yield() {
  assert(current_task);
  cout << "Yield (" << current_task->name << ")\n" << flush;
  Schedulable * new_task = dequeue();
  Schedulable * old_task = current_task;

  if ((!new_task) || (new_task == current_task)) 
    /* DO NOTHING */
    return 0;

  current_task = new_task;
  new_task->Unblock();
  old_task->Block();

  return 0;
}

int Scheduler::Kick_Off() {
  cout << "Scheduler::Kick_Off\n" << flush;
  current_task = dequeue();
  assert(current_task);
  current_task->Unblock();
}
