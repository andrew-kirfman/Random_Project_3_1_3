/*
	File: NamedPipeRequestChannel.cpp

	Author: J. Higginbotham
	Department of Computer Science
	Texas A&M University
	Date  : 2016/06/16

	Based on original code by: Dr. R. Bettati, PhD
	Department of Computer Science
	Texas A&M University
	Date  : 2012/07/11
 */

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* --- none --- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "NamedPipeRequestChannel.h"

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

/* --- none --- */

/*--------------------------------------------------------------------------*/
/* PRIVATE METHODS FOR CLASS   NamedPipeRequestChannel  */
/*--------------------------------------------------------------------------*/

int NamedPipeRequestChannel::read_fd() {
	return rfd;
}

int NamedPipeRequestChannel::write_fd() {
	return wfd;
}

char * NamedPipeRequestChannel::pipe_name(Mode _mode) {
	return nullptr;
}

void NamedPipeRequestChannel::open_write_pipe(char * _pipe_name) {

}

void NamedPipeRequestChannel::open_read_pipe(char * _pipe_name) {

}

/*--------------------------------------------------------------------------*/
/* CONSTRUCTOR/DESTRUCTOR FOR CLASS   NamedPipeRequestChannel  */
/*--------------------------------------------------------------------------*/

NamedPipeRequestChannel::NamedPipeRequestChannel(const std::string _name, const Side _side) :
my_name(_name),
my_side(_side),
side_name((my_side == RequestChannel::SERVER_SIDE) ? "SERVER" : "CLIENT")
{
	throw sync_lib_exception("NamedPipeRequestChannel not yet implemented");
}

NamedPipeRequestChannel::~NamedPipeRequestChannel() {
	
}

/*--------------------------------------------------------------------------*/
/* READ/WRITE FROM/TO REQUEST CHANNELS  */
/*--------------------------------------------------------------------------*/

std::string NamedPipeRequestChannel::send_request(std::string _request) {
  cwrite(_request);
  std::string s = cread();
  return s;
}

std::string NamedPipeRequestChannel::cread() {
	return "";
}

int NamedPipeRequestChannel::cwrite(std::string _msg) {
	return -1;
}

/*--------------------------------------------------------------------------*/
/* ACCESS THE NAME OF REQUEST CHANNEL  */
/*--------------------------------------------------------------------------*/

std::string NamedPipeRequestChannel::name() {
  return my_name;
}



