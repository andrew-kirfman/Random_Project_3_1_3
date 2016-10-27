/*
	Dataserver.h
 
	Created by Joshua Higginbotham on 6/18/16.
*/

#ifndef dataserver_h
#define dataserver_h
#include "sync_lib.h"
#include "NamedPipeRequestChannel.h"
#include "UnnamedPipeRequestChannel.h"
#include "MessageQueueRequestChannel.h"
#include "SharedMemoryRequestChannel.h"
#include "SignalRequestChannel.h"

class Dataserver {
	/*
	 	State information
	 */
	static int nthreads;				//Initialized to 0 inside Dataserver.cpp
	static int nadditionalprocesses;	//Initialized to 0 inside Dataserver.cpp
	
	/*
	 	Request processing algorithms
	 */
	static void process_request(RequestChannel & _channel, const std::string & _request);
	static void process_hello(RequestChannel & _channel, const std::string & _request);
	static void process_data(RequestChannel & _channel, const std::string & _request);
	static void process_newthread(RequestChannel & _channel, const std::string & _request);
	
	
public:
	/*
	 	Thread function
	 */
	static void * handle_data_requests(void * args);
	/*
	 	Kick off main loop
	 */
	static void handle_process_loop(RequestChannel & _channel);

};

#endif /* dataserver_h */
