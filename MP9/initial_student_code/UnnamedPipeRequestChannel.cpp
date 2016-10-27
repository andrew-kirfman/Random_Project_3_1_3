/*
	UnnamedPipeRequestChannel.cpp
 
	Created by Joshua Higginbotham on 6/16/16.
 */

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* --- none --- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "UnnamedPipeRequestChannel.h"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/

/* --- none --- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

/* --- none --- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

/* --- none --- */

/*--------------------------------------------------------------------------*/
/* HELPER FUNCTIONS */
/*--------------------------------------------------------------------------*/

/* --- none --- */

/*--------------------------------------------------------------------------*/
/* PRIVATE METHODS FOR CLASS   UnnamedPipeRequestChannel  */
/*--------------------------------------------------------------------------*/

int UnnamedPipeRequestChannel::write_fd() {
	return wfd;
}

int UnnamedPipeRequestChannel::read_fd() {
	return rfd;
}

/*--------------------------------------------------------------------------*/
/* CONSTRUCTOR/DESTRUCTOR FOR CLASS   UnnamedPipeRequestChannel  */
/*--------------------------------------------------------------------------*/

UnnamedPipeRequestChannel::UnnamedPipeRequestChannel(const std::string _name, const Side _side)
{
	throw sync_lib_exception("UnnamedPipeRequestChannel not yet implemented");
}

UnnamedPipeRequestChannel::~UnnamedPipeRequestChannel() {
	
}

/*--------------------------------------------------------------------------*/
/* READ/WRITE FROM/TO REQUEST CHANNELS  */
/*--------------------------------------------------------------------------*/

std::string UnnamedPipeRequestChannel::send_request(std::string _request) {
	cwrite(_request);
	std::string s = cread();
	return s;
}

std::string UnnamedPipeRequestChannel::cread() {
	return "";
}

int UnnamedPipeRequestChannel::cwrite(std::string _msg) {
	return -1;
}

/*--------------------------------------------------------------------------*/
/* ACCESS THE NAME OF REQUEST CHANNEL  */
/*--------------------------------------------------------------------------*/

std::string UnnamedPipeRequestChannel::name() {
	return my_name;
}


