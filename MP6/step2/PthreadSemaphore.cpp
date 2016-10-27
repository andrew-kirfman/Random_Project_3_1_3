//
//  PthreadSemaphore.cpp
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
/* IMPLEMENTATION   P t h r e a d S e m a p h o r e  */
/*
	Good ole semaphore from 313, now using
	the semaphore interface.
*/
/*--------------------------------------------------------------------------*/


PthreadSemaphore::PthreadSemaphore(int _val) {
	value = _val;
	pthread_cond_init(&wait_queue, NULL);
	pthread_mutex_init(&value_access_lock, NULL);
}

PthreadSemaphore::~PthreadSemaphore() {
	pthread_cond_destroy(&wait_queue);
	pthread_mutex_destroy(&value_access_lock);
	value = 0;
}

int PthreadSemaphore::P() {
	pthread_mutex_lock(&value_access_lock);
	--value;
	if(value < 0) pthread_cond_wait(&wait_queue, &value_access_lock);
	pthread_mutex_unlock(&value_access_lock);
	
	return 0;
}

int PthreadSemaphore::V() {
	pthread_mutex_lock(&value_access_lock);
	++value;
	if(value <= 0){ pthread_cond_signal(&wait_queue); }
	pthread_mutex_unlock(&value_access_lock);
	
	return 0;
}
