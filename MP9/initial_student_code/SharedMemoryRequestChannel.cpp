/*
	SharedMemoryRequestChannel.cpp
 
	Created by Joshua Higginbotham on 6/16/16.
 */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "SharedMemoryRequestChannel.h"

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

/* --- none --- */

/*--------------------------------------------------------------------------*/
/* PRIVATE METHODS FOR CLASS   SharedMemoryRequestChannel  */
/*--------------------------------------------------------------------------*/

/* --- none --- */

/*--------------------------------------------------------------------------*/
/* CONSTRUCTOR/DESTRUCTOR FOR CLASS   SharedMemoryRequestChannel  */
/*--------------------------------------------------------------------------*/

SharedMemoryRequestChannel::SharedMemoryRequestChannel(const std::string _name, const Side _side)
{
	throw sync_lib_exception("SharedMemoryRequestChannel not yet implemented");
}

SharedMemoryRequestChannel::~SharedMemoryRequestChannel() {
	
}

/*--------------------------------------------------------------------------*/
/* READ/WRITE FROM/TO REQUEST CHANNELS  */
/*--------------------------------------------------------------------------*/

std::string SharedMemoryRequestChannel::send_request(std::string _request) {
	cwrite(_request);
	std::string read_result = cread();
	return read_result;
}

std::string SharedMemoryRequestChannel::cread() {
	return "";
}

int SharedMemoryRequestChannel::cwrite(std::string _msg) {
	return -1;
}

/*--------------------------------------------------------------------------*/
/* ACCESS THE NAME OF REQUEST CHANNEL  */
/*--------------------------------------------------------------------------*/

std::string SharedMemoryRequestChannel::name() {
	return my_name;
}



