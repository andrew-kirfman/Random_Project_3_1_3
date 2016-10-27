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

/* --- none --- */

/*--------------------------------------------------------------------------*/
/* PRIVATE METHODS FOR CLASS   MessageQueueRequestChannel  */
/*--------------------------------------------------------------------------*/

/* --- none --- */

/*--------------------------------------------------------------------------*/
/* CONSTRUCTOR/DESTRUCTOR FOR CLASS   MessageQueueRequestChannel  */
/*--------------------------------------------------------------------------*/

MessageQueueRequestChannel::MessageQueueRequestChannel(const std::string _name, const Side _side)
{
	throw sync_lib_exception("MessageQueueRequestChannel not yet implemented");
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
	return "";
}

int MessageQueueRequestChannel::cwrite(std::string _msg) {
	return -1;
}

/*--------------------------------------------------------------------------*/
/* ACCESS THE NAME OF REQUEST CHANNEL  */
/*--------------------------------------------------------------------------*/

std::string MessageQueueRequestChannel::name() {
	return my_name;
}