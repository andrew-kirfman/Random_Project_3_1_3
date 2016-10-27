/*
	ThreadSharedSemaphore.cpp
 
	Created by Joshua Higginbotham on 6/24/16.
 */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "sync_lib.h"

/*--------------------------------------------------------------------------*/
/* IMPLEMENTATION   T h r e a d S h a r e d S e m a p h o r e  */
/*
	Good ole semaphore from MP7/MP8, now using
	the Semaphore interface.
*/
/*--------------------------------------------------------------------------*/

ThreadSharedSemaphore::ThreadSharedSemaphore(int _val) {
	value = _val;
	pthread_cond_init(&wait_queue, NULL);
	pthread_mutex_init(&value_access_lock, NULL);
}

ThreadSharedSemaphore::~ThreadSharedSemaphore() {
	pthread_cond_destroy(&wait_queue);
	pthread_mutex_destroy(&value_access_lock);
	value = 0;
}

int ThreadSharedSemaphore::P() {
	pthread_mutex_lock(&value_access_lock);
	--value;
	if(value < 0) pthread_cond_wait(&wait_queue, &value_access_lock);
	pthread_mutex_unlock(&value_access_lock);
	
	return 1;
}

int ThreadSharedSemaphore::V() {
	pthread_mutex_lock(&value_access_lock);
	++value;
	if(value <= 0){ pthread_cond_signal(&wait_queue); }
	pthread_mutex_unlock(&value_access_lock);
	
	return 1;
}
