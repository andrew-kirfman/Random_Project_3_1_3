/*
	MessageQueueRequestChannel.cpp

	Created by Joshua Higginbotham on 6/16/16.
 */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "MessageQueueRequestChannel.h"

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

const bool VERBOSE = false;

/*--------------------------------------------------------------------------*/
/* PRIVATE METHODS FOR CLASS   MessageQueueRequestChannel  */
/*--------------------------------------------------------------------------*/

/* --- none --- */

/*--------------------------------------------------------------------------*/
/* CONSTRUCTOR/DESTRUCTOR FOR CLASS   MessageQueueRequestChannel  */
/*--------------------------------------------------------------------------*/

MessageQueueRequestChannel::MessageQueueRequestChannel(const std::string _name, const Side _side) :
my_name(_name),
my_side(_side),
side_name((my_side == RequestChannel::SERVER_SIDE) ? "SERVER" : "CLIENT"),
write_buf((my_side == RequestChannel::SERVER_SIDE) ? (my_name + "a") : (my_name + "b"),
		  MQ_NUM_MESSAGES, MQ_MAX_REQUEST),
read_buf((my_side == RequestChannel::SERVER_SIDE) ? (my_name + "b") : (my_name + "a"),
		 MQ_NUM_MESSAGES, MQ_MAX_REQUEST)
{
	
}

MessageQueueRequestChannel::~MessageQueueRequestChannel() {
	
}

/*--------------------------------------------------------------------------*/
/* READ/WRITE FROM/TO REQUEST CHANNELS  */
/*--------------------------------------------------------------------------*/

std::string MessageQueueRequestChannel::send_request(std::string _request) {
	cwrite(_request);
	std::string read_result = cread();
	return read_result;
}

std::string MessageQueueRequestChannel::cread() {
	
	if(VERBOSE) {
		threadsafe_console_output.println("MESSAGE_QUEUE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": reading...");
	}
	
	std::string read_result = read_buf.retrieve_front();
	if(read_result.substr(0, 5) == "ERROR") {
		threadsafe_console_output.perror("MESSAGE_QUEUE_REQUEST_CHANNEL:" + my_name + ": " + side_name + ":cread: " + read_result);
		return read_result;
	}
	
	if(VERBOSE) {
		threadsafe_console_output.println("MESSAGE_QUEUE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": reads [" + read_result + "]");
	}
	
	return read_result;
	
}

int MessageQueueRequestChannel::cwrite(std::string _msg) {
	
	if (_msg.length() + 1 > MQ_MAX_REQUEST) {
		threadsafe_console_output.println("MESSAGE_QUEUE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ":cwrite: message too long for channel");
		return -1;
	}
	
	if(VERBOSE) {
		threadsafe_console_output.println("MESSAGE_QUEUE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": writing [" + _msg + "]");
	}
	
	std::string write_result = write_buf.push_back(_msg);
	if(write_result.substr(0, 5) == "ERROR") {
		threadsafe_console_output.perror("MESSAGE_QUEUE_REQUEST_CHANNEL:" + my_name + ": " + side_name + ":cwrite: " + write_result);
		return -1;
	}
	
	if(VERBOSE) {
		threadsafe_console_output.println("MESSAGE_QUEUE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": done writing.");
	}
	
	return _msg.length() + 1;
	
}

/*--------------------------------------------------------------------------*/
/* ACCESS THE NAME OF REQUEST CHANNEL  */
/*--------------------------------------------------------------------------*/

std::string MessageQueueRequestChannel::name() {
	return my_name;
}