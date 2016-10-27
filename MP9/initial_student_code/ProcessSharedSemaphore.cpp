/*
	ProcessSharedSemaphore.cpp
 
	Created by Joshua Higginbotham on 6/24/16.
 */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "sync_lib.h"

/*--------------------------------------------------------------------------*/
/* IMPLEMENTATION   P r o c e s s S h a r e d S e m a p h o r e  */
/*
	Most of these functions can be completed with 1 or 2 lines,
 	they just wrap around the sem_* functions.
 */
/*--------------------------------------------------------------------------*/

ProcessSharedSemaphore::ProcessSharedSemaphore(std::string _name, int _val) :
sema_name(_name),
max(_val)
{
	throw sync_lib_exception("ProcessSharedSemaphore not yet implemented");
}

ProcessSharedSemaphore::~ProcessSharedSemaphore() {
	
}

int ProcessSharedSemaphore::P() {
	return -1;
}

int ProcessSharedSemaphore::V() {
	return -1;
}

std::string ProcessSharedSemaphore::get_name() {
	return sema_name;
}

bool ProcessSharedSemaphore::initial_construction_failed() {
	return sema == SEM_FAILED;
}

void ProcessSharedSemaphore::set_unlink_on_destruction(bool _unlink_afterwards) {
	unlink_on_destruction = _unlink_afterwards;
}