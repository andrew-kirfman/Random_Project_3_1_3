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

const bool VERBOSE = false;

/*--------------------------------------------------------------------------*/
/* HELPER FUNCTIONS */
/*--------------------------------------------------------------------------*/

/* --- none --- */

/*--------------------------------------------------------------------------*/
/* IMPLEMENTATION   M e s s a g e Q u e u e B o u n d e d B u f f e r 		*/
/*
 	Checking against system message queue limits is
 	left to the application, since the result of failure
 	is an exit anyway.
*/
/*--------------------------------------------------------------------------*/

MessageQueueBoundedBuffer::MessageQueueBoundedBuffer(std::string _name, int _num_msgs, int _msg_size) :
max_msgs(_num_msgs),
my_name(_name),
msg_size(_msg_size),
mq_name("/" + my_name + "_mq")
{
	struct mq_attr attr;
	attr.mq_flags = 0;
	attr.mq_maxmsg = max_msgs;
	attr.mq_msgsize = msg_size;
	attr.mq_curmsgs = 0;
	mq_descriptor = mq_open(mq_name.c_str(), O_RDWR | O_CREAT | O_EXCL, MY_CREATION_FLAGS, &attr);
	if(errno == EEXIST) {
		mq_descriptor = mq_open(mq_name.c_str(), O_RDWR, 0, NULL);
		if(mq_descriptor == (mqd_t) -1) {
			throw sync_lib_exception("MESSAGE_QUEUE_BOUNDED_BUFFER:" + mq_name + ": failed to open pre-existing mq for reading and writing");
		}
	}
	else if(mq_descriptor == (mqd_t) -1){
		throw sync_lib_exception("MESSAGE_QUEUE_BOUNDED_BUFFER:" + mq_name + ": failed to open pre-existing mq for reading and writing");
	}
}

MessageQueueBoundedBuffer::~MessageQueueBoundedBuffer() {
	
	if(!empty_constructur_used) {
		if(mq_close(mq_descriptor) < 0) {
			threadsafe_console_output.perror("MESSAGE_QUEUE_BOUNDED_BUFFER:" + mq_name + ": failed to close mq with mqd == " + std::to_string(mq_descriptor));
		}
		if(unlink_on_destruction) {
			if(VERBOSE) threadsafe_console_output.println("MESSAGE_QUEUE_BOUNDED_BUFFER:" + mq_name + ": close IPC mechanisms on side for mq");
			if(mq_unlink(mq_name.c_str()) < 0 && errno != ENOENT) {
				threadsafe_console_output.perror("MESSAGE_QUEUE_BOUNDED_BUFFER:" + mq_name + ": failed to unlink mq");
			}
		}
	}
}

std::string MessageQueueBoundedBuffer::push_back(std::string req) {
	if(mq_send(mq_descriptor, req.c_str(), req.length(), 0) < 0) {
		return "ERROR_MQ_SEND_FAILED";
	}
	
	return req;
}

std::string MessageQueueBoundedBuffer::retrieve_front() {
	char buf[msg_size];
	memset(buf, '\0', msg_size);
	std::string read_result;
	
	if(mq_receive(mq_descriptor, buf, msg_size, NULL) < 0) {
		return "ERROR_MQ_RECEIVE_FAILED";
	}
	
	read_result = buf;
	
	return read_result;
}

int MessageQueueBoundedBuffer::size() {
	struct mq_attr * attr;
	int error = mq_getattr(mq_descriptor, attr);
	if(error < 0) {
		return error;
	}
	return attr->mq_curmsgs;
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
