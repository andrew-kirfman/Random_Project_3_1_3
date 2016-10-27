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
/*--------------------------------------------------------------------------*/

ProcessSharedSemaphore::ProcessSharedSemaphore(std::string _name, int _val) :
sema_name(_name),
max(_val)
{
	empty_constructor_used = false;
	sema = sem_open(sema_name.c_str(), O_RDWR | O_CREAT, MY_CREATION_FLAGS, max);
	if(sema == SEM_FAILED) {
		throw sync_lib_exception("PROCESS_SHARED_SEMAPHORE:" + sema_name + ": failed on sem_open");
	}
}

ProcessSharedSemaphore::~ProcessSharedSemaphore() {
	if(!empty_constructor_used) {
		if(sem_close(sema) < 0) {
			threadsafe_console_output.perror("PROCESS_SHARED_SEMAPHORE:" + sema_name + ": failed on sem_close");
		}
		if(unlink_on_destruction) {
			if(sem_unlink(sema_name.c_str()) < 0 && errno != ENOENT) {
				threadsafe_console_output.perror("PROCESS_SHARED_SEMAPHORE:" + sema_name + ": failed on sem_unlink");
			}
		}
	}
}

int ProcessSharedSemaphore::P() {
	return sem_wait(sema);
}

int ProcessSharedSemaphore::V() {
	return sem_post(sema);
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