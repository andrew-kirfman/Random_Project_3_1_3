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

const int MAX_MESSAGE = NAME_MAX;
const bool VERBOSE = false;
const bool VERBOSE_DEBUG = false;

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
	std::string pname = "fifo_" + my_name;

	if (my_side == CLIENT_SIDE) {
		if (_mode == READ_MODE)
			pname += "a";
		else
			pname += "b";
	} else {
	/* SERVER_SIDE */
	if (_mode == READ_MODE) 
		pname += "b";
	else 
		pname += "a";
	}
	char * result = new char[pname.size()+1];
	strncpy(result, pname.c_str(), pname.size()+1);
	return result;
}

void NamedPipeRequestChannel::open_write_pipe(char * _pipe_name) {

	if(VERBOSE_DEBUG) threadsafe_console_output.println("NAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": mkfifo write pipe");
	if (mkfifo(_pipe_name, 0600) < 0) {
		if (errno != EEXIST) {
			if(read_pipe_opened) close(rfd);
			if(write_pipe_opened) close(wfd);
			remove(pipe_name(READ_MODE));
			remove(pipe_name(WRITE_MODE));
			throw sync_lib_exception("NAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": error creating pipe for writing");
		}
	}
	if(VERBOSE_DEBUG) threadsafe_console_output.println("NAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": open write pipe");
	wfd = open(_pipe_name, O_WRONLY);
	if (wfd < 0) {
		if(read_pipe_opened) close(rfd);
		remove(pipe_name(READ_MODE));
		remove(pipe_name(WRITE_MODE));
		throw sync_lib_exception("NAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": error opening pipe for writing");
	}
	if(VERBOSE) threadsafe_console_output.println("NAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": done opening write pipe");
	write_pipe_opened = true;
}

void NamedPipeRequestChannel::open_read_pipe(char * _pipe_name) {

	if(VERBOSE_DEBUG) threadsafe_console_output.println("NAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": mkfifo read pipe");
	if (mkfifo(_pipe_name, 0600) < 0) {
		if (errno != EEXIST) {
			if(read_pipe_opened) close(rfd);
			if(write_pipe_opened) close(wfd);
			remove(pipe_name(READ_MODE));
			remove(pipe_name(WRITE_MODE));
			throw sync_lib_exception("NAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": error creating pipe for reading");
		}
	}
	if(VERBOSE_DEBUG) threadsafe_console_output.println("NAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": open read pipe");
	rfd = open(_pipe_name, O_RDONLY);
	if (rfd < 0) {
		if(write_pipe_opened) close(wfd);
		remove(pipe_name(READ_MODE));
		remove(pipe_name(WRITE_MODE));
		throw sync_lib_exception("NAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": error opening pipe for reading");
	}
	if(VERBOSE) threadsafe_console_output.println("NAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": done opening read pipe");
	read_pipe_opened = true;
}

/*--------------------------------------------------------------------------*/
/* CONSTRUCTOR/DESTRUCTOR FOR CLASS   NamedPipeRequestChannel  */
/*--------------------------------------------------------------------------*/

NamedPipeRequestChannel::NamedPipeRequestChannel(const std::string _name, const Side _side) :
my_name(_name),
my_side(_side),
side_name((my_side == RequestChannel::SERVER_SIDE) ? "SERVER" : "CLIENT")
{
	//Necessary for proper error handling
	sigset_t sigpipe_set;
	sigemptyset(&sigpipe_set);
	if(sigaddset(&sigpipe_set, SIGPIPE) < 0) {
		throw sync_lib_exception("NAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": failed on sigaddset(&sigpipe_set, SIGPIPE)");
	}
	
	if((errno = pthread_sigmask(SIG_SETMASK, &sigpipe_set, NULL)) != 0) {
		throw sync_lib_exception("NAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": failed on pthread_sigmas(SIG_SETMASK, &sigpipe_set, NULL)");
	}
	
	if (_side == SERVER_SIDE) {
		open_write_pipe(pipe_name(WRITE_MODE));
		open_read_pipe(pipe_name(READ_MODE));
	}
	else {
		open_read_pipe(pipe_name(READ_MODE));
		open_write_pipe(pipe_name(WRITE_MODE));
	}
}

NamedPipeRequestChannel::~NamedPipeRequestChannel() {
	if(VERBOSE_DEBUG) threadsafe_console_output.println("NAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": destructing");
	close(wfd);
	close(rfd);
	
	if(VERBOSE) threadsafe_console_output.println("NAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": close IPC mechanisms for channel " + my_name);
	
	/* Destruct the underlying IPC mechanisms. */
	if (remove(pipe_name(READ_MODE)) < 0 && errno != ENOENT) {
		threadsafe_console_output.perror("NAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": error deleting pipe for reading");
	}
	
	if (remove(pipe_name(WRITE_MODE)) < 0 && errno != ENOENT) {
		threadsafe_console_output.perror("NAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": error deleting pipe for writing");
	}
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
	
	char read_buf[MAX_MESSAGE];
	memset(read_buf, '\0', MAX_MESSAGE);

	int read_return_value = read(rfd, read_buf, MAX_MESSAGE);
	
	if(VERBOSE_DEBUG) threadsafe_console_output.println("NAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": reads [" + std::string(read_buf) + "], read_return_value == " + std::to_string(read_return_value));
	
	if (read_return_value <= 0) {
		if(read_return_value < 0) {
			threadsafe_console_output.perror("NAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": error reading from pipe");
			return "ERROR";
		}
		else {
			threadsafe_console_output.println("NAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": broken/closed pipe detected, exiting worker thread...");
		}
		close(rfd);
		close(wfd);
		remove(pipe_name(READ_MODE));
		remove(pipe_name(WRITE_MODE));
		pthread_exit(NULL);
	}

	std::string read_result = read_buf;

	return read_result;

}

int NamedPipeRequestChannel::cwrite(std::string _msg) {
	
	if (_msg.length() >= MAX_MESSAGE) {
		std::cerr << "NAMED_PIPE:" + my_name + ":" + side_name + ": message too long for channel" << std::endl << std::flush;
		return -1;
	}

	if(VERBOSE) threadsafe_console_output.println("NAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": writing [" + _msg + "]");
	
	const char * write_buf = _msg.c_str();
	
	int write_return_value = write(wfd, write_buf, strlen(write_buf)+1);
	
	if(VERBOSE_DEBUG) threadsafe_console_output.println("NAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": done writing [" + std::string(write_buf) + "], write_return_value == " + std::to_string(write_return_value));
	
	if (write_return_value < 0) {
		//EPIPE is part of the normal error-handling process for
		//NamedPipeRequestChannels that are successfully constructed,
		//but that don't have threads enough threads created for them.
		
		if(errno != EPIPE) {
			threadsafe_console_output.perror("NAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": error writing to pipe");
			return -1;
		}
		else {
			threadsafe_console_output.println("NAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": Broken/closed pipe detected, exiting worker thread...");
		}
		close(rfd);
		close(wfd);
		remove(pipe_name(READ_MODE));
		remove(pipe_name(WRITE_MODE));
		pthread_exit(NULL);
	}

	return write_return_value;
}

/*--------------------------------------------------------------------------*/
/* ACCESS THE NAME OF REQUEST CHANNEL  */
/*--------------------------------------------------------------------------*/

std::string NamedPipeRequestChannel::name() {
  return my_name;
}



