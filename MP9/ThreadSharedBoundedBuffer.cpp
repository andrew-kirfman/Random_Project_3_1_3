/*
	ThreadSharedBoundedBuffer.cpp

	Created by Joshua Higginbotham on 6/24/16.
 */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "sync_lib.h"

/*--------------------------------------------------------------------------*/
/* IMPLEMENTATION   T h r e a d S h a r e d B u f f e r */
/*
	Good ole BoundedBuffer from MP7/MP8, now using
	the BoundedBuffer interface.
*/
/*--------------------------------------------------------------------------*/

ThreadSharedBoundedBuffer::ThreadSharedBoundedBuffer(int _size) : max_items(_size), full(0), empty(_size) { pthread_mutex_init(&item_access_lock, NULL);
}

ThreadSharedBoundedBuffer::~ThreadSharedBoundedBuffer() {
	
}

std::string ThreadSharedBoundedBuffer::push_back(std::string req) {
	empty.P();
	pthread_mutex_lock(&item_access_lock);
	items.push(req);
	pthread_mutex_unlock(&item_access_lock);
	full.V();
	
	return "PUSH_SUCCESS";
}

std::string ThreadSharedBoundedBuffer::retrieve_front() {
	full.P();
	pthread_mutex_lock(&item_access_lock);
	std::string retrieval = items.front();
	items.pop();
	pthread_mutex_unlock(&item_access_lock);
	empty.V();
	return retrieval;
}

int ThreadSharedBoundedBuffer::size() {
	pthread_mutex_lock(&item_access_lock);
	int i = items.size();
	pthread_mutex_unlock(&item_access_lock);
	return i;
}