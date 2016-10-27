//
//  KernelSemaphore.cpp
//  
//
//  Created by Joshua Higginbotham on 6/24/16.
//
//

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "sync_lib.h"

/*--------------------------------------------------------------------------*/
/* IMPLEMENTATION   K e r n e l S e m a p h o r e  */
/*
	Notes will go here when I think of some to write.
 */
/*--------------------------------------------------------------------------*/

KernelSemaphore::KernelSemaphore(std::string _name, int _val) :
sema_name(_name),
max(_val)
{
	sema = sem_open(sema_name.c_str(), O_RDWR | O_CREAT, MY_CREATION_FLAGS, max);
}

KernelSemaphore::~KernelSemaphore() {
	if(!empty_constructor_used) {
		if(sem_close(sema) < 0) {
			threadsafe_console_output.perror("KERNEL_SEMAPHORE:" + sema_name + ": failed on sem_close");
		}
		if(unlink_on_destruction) {
			if(sem_unlink(sema_name.c_str()) < 0 && errno != ENOENT) {
				threadsafe_console_output.perror("KERNEL_SEMAPHORE:" + sema_name + ": failed on sem_unlink");
			}
		}
	}
}

int KernelSemaphore::P() {
	return sem_wait(sema);
}

int KernelSemaphore::V() {
	return sem_post(sema);
}

std::string KernelSemaphore::get_name() {
	return sema_name;
}

bool KernelSemaphore::initial_construction_failed() {
	return sema == SEM_FAILED;
}

void KernelSemaphore::set_unlink_on_destruction(bool _unlink_afterwards) {
	unlink_on_destruction = _unlink_afterwards;
}