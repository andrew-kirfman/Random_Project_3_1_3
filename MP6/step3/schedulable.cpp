/* 
    File: schedulable.cpp

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 08/02/25

*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include<assert.h>

#include <iostream>

#include "utils.h"
#include "scheduler.h"
#include "schedulable.h"



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
/* METHODS FOR CLASS   S c h e d u l a b l e  */
/*--------------------------------------------------------------------------*/

//Schedulable::Schedulable(const char _name[], Scheduler * _sched) : Task(_name) {
Schedulable::Schedulable(std::string _name, Scheduler * _sched) : Task(_name) {
	std::cout << "Schedulable (" << _name << ") constructor\n" << std::flush;
  sched = _sched;
  //block_semaphore = new Semaphore(0);
}

Schedulable::~Schedulable() {
  assert(FALSE);
}

void Schedulable::CarrierForRun() {

  /* -- PUT THE TASK ON THE WAITING QUEUE OF THE SCHEDULER. */
  assert(sched->Start(this) == 0);

 
  /* -- NOW YOU HAVE THE CPU. EXECUTE YOUR CODE. */
  Run();

  /* -- TASK IS DONE WITH EXECUTING. GIVE UP THE CPU. 
        (i.e. HAVE THE SCHEDULER PICK THE NEXT TASK.) */
  sched->Yield();
   
}

int Schedulable::Start() {
  /* This is a simple schedulable task with a single execution of the 
     "Run" function. More sophisticated Schedulables, such as "Periodic_Task",
     periodically invoke the "Run" function.
 */
 
  /* -- START THE TASK */
  return Task::Start();
  
}

int Schedulable::Block() {
  return(block_semaphore.P());
}

int Schedulable::Unblock() {
  return(block_semaphore.V());
}
