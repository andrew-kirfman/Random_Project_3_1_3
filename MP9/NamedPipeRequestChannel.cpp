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

std::string NamedPipeRequestChannel::pipe_name(Mode _mode) {
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
	
	return pname;
}

void NamedPipeRequestChannel::open_write_pipe(const char * _pipe_name) {

	if(VERBOSE_DEBUG) threadsafe_console_output.println("NAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": mkfifo write pipe");
	if (mkfifo(_pipe_name, 0600) < 0) {
		if (errno != EEXIST) {
			int prev_errno = errno;
			if(read_pipe_opened) close(rfd);
			if(write_pipe_opened) close(wfd);
			remove(pipe_name(READ_MODE).c_str());
			remove(pipe_name(WRITE_MODE).c_str());
			errno = prev_errno;
			throw sync_lib_exception("NAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": error creating pipe for writing");
		}
	}
	if(VERBOSE_DEBUG) threadsafe_console_output.println("NAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": open write pipe");
	wfd = open(_pipe_name, O_WRONLY);
	if (wfd < 0) {
		int prev_errno = errno;
		if(read_pipe_opened) close(rfd);
		remove(pipe_name(READ_MODE).c_str());
		remove(pipe_name(WRITE_MODE).c_str());
		errno = prev_errno;
		throw sync_lib_exception("NAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": error opening pipe for writing");
	}
	if(VERBOSE) threadsafe_console_output.println("NAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": done opening write pipe");
	write_pipe_opened = true;
}

void NamedPipeRequestChannel::open_read_pipe(const char * _pipe_name) {

	if(VERBOSE_DEBUG) threadsafe_console_output.println("NAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": mkfifo read pipe");
	if (mkfifo(_pipe_name, 0600) < 0) {
		if (errno != EEXIST) {
			int prev_errno = errno;
			if(read_pipe_opened) close(rfd);
			if(write_pipe_opened) close(wfd);
			remove(pipe_name(READ_MODE).c_str());
			remove(pipe_name(WRITE_MODE).c_str());
			errno = prev_errno;
			throw sync_lib_exception("NAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": error creating pipe for reading");
		}
	}
	if(VERBOSE_DEBUG) threadsafe_console_output.println("NAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": open read pipe");
	rfd = open(_pipe_name, O_RDONLY);
	if (rfd < 0) {
		int prev_errno = errno;
		if(write_pipe_opened) close(wfd);
		remove(pipe_name(READ_MODE).c_str());
		remove(pipe_name(WRITE_MODE).c_str());
		errno = prev_errno;
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
	
	if((errno = pthread_mutexattr_init(&srl_attr)) != 0) {
		throw sync_lib_exception(my_name + ":" + side_name + ": failed on pthread_mutexattr_init");
	}
	if((errno = pthread_mutexattr_setrobust(&srl_attr, PTHREAD_MUTEX_ROBUST)) != 0) {
		throw sync_lib_exception(my_name + ":" + side_name + ": failed on pthread_mutexattr_setrobust");
	}
	if((errno = pthread_mutexattr_setpshared(&srl_attr, PTHREAD_PROCESS_SHARED)) != 0) {
		throw sync_lib_exception(my_name + ":" + side_name + ": failed on pthread_mutexattr_setpshared");
	}
	if((errno = pthread_mutex_init(&send_request_lock, &srl_attr)) != 0) {
		throw sync_lib_exception(my_name + ":" + side_name + ": failed on pthread_mutex_init");
	}
	
	if (_side == SERVER_SIDE) {
		open_write_pipe(pipe_name(WRITE_MODE).c_str());
		open_read_pipe(pipe_name(READ_MODE).c_str());
	}
	else {
		open_read_pipe(pipe_name(READ_MODE).c_str());
		open_write_pipe(pipe_name(WRITE_MODE).c_str());
	}
}

NamedPipeRequestChannel::~NamedPipeRequestChannel() {
	if(VERBOSE_DEBUG) threadsafe_console_output.println("NAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": destructing");
	pthread_mutexattr_destroy(&srl_attr);
	pthread_mutex_destroy(&read_lock);
	pthread_mutex_destroy(&write_lock);
	pthread_mutex_destory(&send_request_lock);
	close(wfd);
	close(rfd);
	if(my_side == RequestChannel::SERVER_SIDE) {
		if(VERBOSE) threadsafe_console_output.println("NAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": close IPC mechanisms for channel " + my_name);
		
		/* Destruct the underlying IPC mechanisms. */
		if (remove(pipe_name(READ_MODE).c_str()) < 0 && errno != ENOENT) {
			threadsafe_console_output.perror("NAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": error deleting pipe for reading");
		}
		
		if (remove(pipe_name(WRITE_MODE).c_str()) < 0 && errno != ENOENT) {
			threadsafe_console_output.perror("NAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": error deleting pipe for writing");
		}
	}
}

/*--------------------------------------------------------------------------*/
/* READ/WRITE FROM/TO REQUEST CHANNELS  */
/*--------------------------------------------------------------------------*/

std::string NamedPipeRequestChannel::send_request(std::string _request) {
	pthread_mutex_lock(&send_request_lock);
	if(cwrite(_request) < 0) {
		pthread_mutex_unlock(&send_request_lock);
		return "ERROR";
	}
	std::string s = cread();
	pthread_mutex_unlock(&send_request_lock);
	return s;
}

std::string NamedPipeRequestChannel::cread() {
	
	pthread_mutex_lock(&read_lock);
	
	char read_buf[MAX_MESSAGE];
	memset(read_buf, '\0', MAX_MESSAGE);

	int read_return_value = read(rfd, read_buf, MAX_MESSAGE);
	
	if(VERBOSE_DEBUG) threadsafe_console_output.println("NAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": reads [" + std::string(read_buf) + "], read_return_value == " + std::to_string(read_return_value));
	
	if (read_return_value <= 0) {
		if(read_return_value < 0) {
			threadsafe_console_output.perror("NAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": error reading from pipe");
			pthread_mutex_unlock(&read_lock);
			return "ERROR";
		}
		else {
			threadsafe_console_output.println("NAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": broken/closed pipe detected, exiting worker thread...");
		}
		close(rfd);
		close(wfd);
		remove(pipe_name(READ_MODE).c_str());
		remove(pipe_name(WRITE_MODE).c_str());
		pthread_mutex_unlock(&send_request_lock);
		pthread_mutex_unlock(&read_lock);
		pthread_exit(NULL);
	}
	pthread_mutex_unlock(&read_lock);

	std::string read_result = read_buf;

	return read_result;

}

int NamedPipeRequestChannel::cwrite(std::string _msg) {
	
	if (_msg.length() >= MAX_MESSAGE) {
		std::cerr << "NAMED_PIPE:" + my_name + ":" + side_name + ": message too long for channel" << std::endl << std::flush;
		return -1;
	}

	pthread_mutex_lock(&write_lock);
	
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
			pthread_mutex_unlock(&write_lock);
			return -1;
		}
		else {
			threadsafe_console_output.println("NAMED_PIPE_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": Broken/closed pipe detected, exiting worker thread...");
		}
		close(rfd);
		close(wfd);
		remove(pipe_name(READ_MODE).c_str());
		remove(pipe_name(WRITE_MODE).c_str());
		pthread_mutex_unlock(&send_request_lock);
		pthread_mutex_unlock(&write_lock);
		pthread_exit(NULL);
	}
	pthread_mutex_unlock(&write_lock);

	return write_return_value;
}

/*--------------------------------------------------------------------------*/
/* ACCESS THE NAME OF REQUEST CHANNEL  */
/*--------------------------------------------------------------------------*/

std::string NamedPipeRequestChannel::name() {
  return my_name;
}



