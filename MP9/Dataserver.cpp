/* 
    File: Dataserver.cpp

	Author: J. Higginbotham
	Department of Computer Science
	Texas A&M University
	Date  : 2016/08/10
 
    Based on original code by: Dr. R. Bettati, PhD
	Department of Computer Science
	Texas A&M University
    Date  : 2013/01/30

    Dataserver method implementations for MP9.
 */

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* --- none --- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "Dataserver.h"

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

const std::string data_channel_name = "data_";
const bool VERBOSE = false;
const bool VERBOSE_DEBUG = false;

/*--------------------------------------------------------------------------*/
/* VARIABLES */
/*--------------------------------------------------------------------------*/

int Dataserver::nthreads = 0;
int Dataserver::nadditionalprocesses = 0;

/*--------------------------------------------------------------------------*/
/* SUPPORT FUNCTIONS */
/*--------------------------------------------------------------------------*/

/* --- none --- */

/*--------------------------------------------------------------------------*/
/* THREAD FUNCTIONS */
/*--------------------------------------------------------------------------*/

void * Dataserver::handle_data_requests(void * args) {

	RequestChannel * data_channel =  (RequestChannel*)args;

	//Handle client requests on this channel. 

	handle_process_loop(*data_channel);

	delete data_channel;
	
	//Client has quit. We remove channel.
	
	return nullptr;
}

/*--------------------------------------------------------------------------*/
/* HANDLE INDIVIDUAL REQUESTS */
/*--------------------------------------------------------------------------*/

void Dataserver::process_hello(RequestChannel & _channel, const std::string & _request) {
  _channel.cwrite("hello to you too");
}

void Dataserver::process_data(RequestChannel & _channel, const std::string &  _request) {
    _channel.cwrite(std::to_string(rand() % 100));
}

void Dataserver::process_newthread(RequestChannel & _channel, const std::string & _request) {
	pthread_t thread_id = 0;
	nthreads ++;

	std::string new_channel_name = data_channel_name + std::to_string(nthreads);
	_channel.cwrite(new_channel_name);
	
	std::string channel_type = "";
	if(_request.length() > 11) channel_type = _request.substr(10,std::string::npos);
	else channel_type = "NAMED_PIPE";
	
	RequestChannel * data_channel;
	
	try {
		if(_request == "newthread_NAMED_PIPE") {
			data_channel = new NamedPipeRequestChannel(new_channel_name, RequestChannel::SERVER_SIDE);
		}
		else if(_request == "newthread_UNNAMED_PIPE") {
			data_channel = new UnnamedPipeRequestChannel(new_channel_name, RequestChannel::SERVER_SIDE);
		}
		else if(_request == "newthread_SHARED_MEMORY") {
			data_channel = new SharedMemoryRequestChannel(new_channel_name, RequestChannel::SERVER_SIDE);
		}
		else if(_request == "newthread_MESSAGE_QUEUE") {
			data_channel = new MessageQueueRequestChannel(new_channel_name, RequestChannel::SERVER_SIDE);
		}
		else if(_request == "newthread_SIGNAL") {
			data_channel = new SignalRequestChannel(new_channel_name, RequestChannel::SERVER_SIDE);
		}
		else {
			/*	Default RequestChannel implementation
			 is NamedPipeRequestChannel	*/
			data_channel =  new NamedPipeRequestChannel(new_channel_name, RequestChannel::SERVER_SIDE);
		}
		
		if(VERBOSE) threadsafe_console_output.println("DATASERVER:" + _channel.name() + ": starting new " + channel_type + " thread " + std::to_string(nthreads));
		if ((errno = pthread_create(& thread_id, NULL, handle_data_requests, data_channel)) != 0) {
			threadsafe_console_output.perror("DATASERVER:" + _channel.name() + ": pthread_create failure, new " + channel_type + " thread not created");
			delete data_channel;
		}
	}
	catch (sync_lib_exception sle) {
		threadsafe_console_output.perror("DATASERVER:" + std::string(sle.what()));
	}
	
}

/*--------------------------------------------------------------------------*/
/* THE PROCESS REQUEST LOOP */
/*--------------------------------------------------------------------------*/

void Dataserver::process_request(RequestChannel & _channel, const std::string & _request) {

	if (_request.compare(0, 5, "hello") == 0) {
		process_hello(_channel, _request);
	}
	else if (_request.compare(0, 5, "data ") == 0) {
		process_data(_channel, _request);
	}
	else if (_request.compare(0, 9, "newthread") == 0) {
		process_newthread(_channel, _request);
	}
	else {
		_channel.cwrite("unknown request");
	}

}

void Dataserver::handle_process_loop(RequestChannel & _channel) {
	for(;;) {
		std::string request = "";
		if(VERBOSE) threadsafe_console_output.println("DATASERVER:" + _channel.name() + ": Reading next request from channel...");
		request = _channel.cread();
		if(VERBOSE) threadsafe_console_output.println("DATASERVER:" + _channel.name() + ": Finished reading from channel, new request is: [" + request + "]");

		if(VERBOSE_DEBUG) threadsafe_console_output.println("DATASERVER:" + _channel.name() + ": comparing new request [" + request + "] to [quit]: " + ((request.substr(0,4) == "quit") ? "TRUE" : "FALSE"));

		if (request.substr(0,4) == "quit") {
			if(VERBOSE_DEBUG) threadsafe_console_output.println("DATASERVER:" + _channel.name() + ": Writing final response 'bye'...");
			if(request.length() <= 4)
				_channel.cwrite("bye");
			else
				_channel.cwrite("bye" + request.substr(4, std::string::npos));
			if(VERBOSE_DEBUG) threadsafe_console_output.println("DATASERVER:" + _channel.name() + ": Sleeping...");
			usleep(10000); 			// give the other end a bit of time.
			if(VERBOSE_DEBUG) threadsafe_console_output.println("DATASERVER:" + _channel.name() + ": Exiting...");
			break;					// break out of the loop;
		}

		process_request(_channel, request);
	}

}
