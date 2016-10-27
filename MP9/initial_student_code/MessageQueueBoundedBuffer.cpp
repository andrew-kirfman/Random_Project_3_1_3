/*
	MessageQueueBoundedBuffer.cpp
 
	Created by Joshua Higginbotham on 6/24/16.
 */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "sync_lib.h"

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

/* --- none --- */

/*--------------------------------------------------------------------------*/
/* HELPER FUNCTIONS */
/*--------------------------------------------------------------------------*/

/* --- none --- */

/*--------------------------------------------------------------------------*/
/* IMPLEMENTATION   M e s s a g e Q u e u e B o u n d e d B u f f e r 		*/
/*--------------------------------------------------------------------------*/

MessageQueueBoundedBuffer::MessageQueueBoundedBuffer(std::string _name, int _num_msgs, int _msg_size)
{
	throw sync_lib_exception("MessageQueueBoundedBuffer not implemented");
}

MessageQueueBoundedBuffer::~MessageQueueBoundedBuffer() {
	
}

std::string MessageQueueBoundedBuffer::push_back(std::string req) {
	return "";
}

std::string MessageQueueBoundedBuffer::retrieve_front() {
	return "";
}

int MessageQueueBoundedBuffer::size() {
	return -1;
}

int MessageQueueBoundedBuffer::get_msg_size() {
	return msg_size;
}

std::string MessageQueueBoundedBuffer::get_name() {
	return my_name;
}

void MessageQueueBoundedBuffer::set_unlink_on_destruction(bool _unlink_afterwards) {
	unlink_on_destruction = _unlink_afterwards;
}
