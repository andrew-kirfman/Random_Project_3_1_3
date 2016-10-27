/*
	File: client.cpp
 
	Author: J. Higginbotham
	Department of Computer Science
	Texas A&M University
	Date  : 2016/08/10

	Based on original code by : Dr. R. Bettati, PhD
	Department of Computer Science
	Texas A&M University
	Date  : 2012/07/11

	Helps with testing the different RequestChannel
 	classes in MP9.
 */

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* --- none --- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "Dataserver.h"
#include "sync_lib.h"
#include "NamedPipeRequestChannel.h"
#include "UnnamedPipeRequestChannel.h"
#include "MessageQueueRequestChannel.h"
#include "SharedMemoryRequestChannel.h"
#include "SignalRequestChannel.h"

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

const int ERR_MODE_UNKNOWN 	= -1;
const int NAMED_PIPE 		= 0;
const int UNNAMED_PIPE		= 1;
const int MESSAGE_QUEUE 	= 2;
const int SHARED_MEMORY 	= 3;
const int SIGNAL 			= 4;

const std::string control_channel_name	= "control";
bool VERBOSE = false;

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

/* --- none --- */

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/

/* --- none --- */

/*--------------------------------------------------------------------------*/
/* THREAD FUNCTIONS */
/*--------------------------------------------------------------------------*/

/* --- none --- */

/*--------------------------------------------------------------------------*/
/* HELPER FUNCTIONS */
/*--------------------------------------------------------------------------*/

void time_data_requests(RequestChannel & control_channel, std::vector<int64_t> * times) {
	struct timeval start_time;
	struct timeval finish_time;
	
	for(int i = 0; i < 10000; i++) {
		std::string request_string("data Andrew Kirfman " + std::to_string(i));
		assert(gettimeofday(&start_time, 0) == 0);
		std::string reply_string =  control_channel.send_request(request_string);
		assert(gettimeofday(&finish_time, 0) == 0);
		if(VERBOSE) threadsafe_console_output.println("MAIN: reply to request [" + request_string + "]:" + reply_string);
		
		int64_t start_usecs = (start_time.tv_sec * pow(10,6)) + start_time.tv_usec;
		int64_t finish_usecs = (finish_time.tv_sec * pow(10,6)) + finish_time.tv_usec;
		times->push_back(finish_usecs - start_usecs);
	}
}

void process_timing_data(std::vector<int64_t> * times) {
	std::ofstream ofs;
	ofs.open("output.txt", std::ios::out | std::ios::app);
	
	//Find the maximum time and the minimum time
	int64_t MAX_TIME = *(max_element(times->begin(), times->end()));
	int64_t MIN_TIME = *(min_element(times->begin(), times->end()));
	
	//Calculate average of processing times times: sum all times and divide by number of times sampled
	int64_t accumulator = 0;
	for(int i = 0; i < times->size(); ++i) {
		accumulator += times->at(i);
	}
	long double AVERAGE = accumulator / (double) times->size();
	
	//Calculate standard deviation of processing times
	//Step 1: square of the difference between each processing time and the average processing time
	std::vector<int64_t> squared_differences;
	for(int i = 0; i < times->size(); ++i) {
		long long difference = times->at(i) - AVERAGE;
		squared_differences.push_back(pow(difference,2));
	}
	
	//Step 2: calculate variance of processing times: average of squared differences
	accumulator = 0;
	for(int i = 0; i < squared_differences.size(); ++i) {
		accumulator += squared_differences[i];
	}
	long double variance = accumulator / (double) squared_differences.size();
	
	//Step 3: standard deviation is the square root of the variance
	long double STD_DEVIATION = sqrt(variance);
	assert(STD_DEVIATION <= MAX_TIME - MIN_TIME);
	
	/*
		Output results and clean up
	 */
	threadsafe_console_output.println("Max value == " + std::to_string(MAX_TIME));
	threadsafe_console_output.println("Min value == " + std::to_string(MIN_TIME));
	threadsafe_console_output.println("Average of all propagation times: " + std::to_string(AVERAGE) + " usec");
	threadsafe_console_output.println("Standard deviation: " + std::to_string(STD_DEVIATION) + " usec");
	ofs << "Max value == " << MAX_TIME << std::endl;
	ofs << "Min value == " << MIN_TIME << std::endl;
	ofs << "Average of all propagation times: " << AVERAGE << " usec" << std::endl;
	ofs << "Standard deviation: " << STD_DEVIATION << " usec" << std::endl << std::endl;
	
	ofs.close();
	
	usleep(1000000);
}

/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/

int main(int argc, char * argv[]) {
	int IPC_MECHANISM = NAMED_PIPE; //Results in default execution of NAMED_PIPE code
	bool SEPARATE_WINDOWS = false;
	pid_t client_pid = 1;
	int r = 0;
	int opt = 0;
	while ((opt = getopt(argc, argv, "vr:xh")) != -1) {
		switch (opt) {
			case 'v':
				VERBOSE = true;
				break;
			case 'r':
				r = atoi(optarg);
				if(r == NAMED_PIPE) {
					IPC_MECHANISM = NAMED_PIPE;
				}
				else if(r == UNNAMED_PIPE) {
					IPC_MECHANISM = UNNAMED_PIPE;
				}
				else if(r == MESSAGE_QUEUE) {
					IPC_MECHANISM = MESSAGE_QUEUE;
				}
				else if(r == SHARED_MEMORY) {
					IPC_MECHANISM = SHARED_MEMORY;
				}
				else if(r == SIGNAL) {
					IPC_MECHANISM = SIGNAL;
				}
				else {
					IPC_MECHANISM = ERR_MODE_UNKNOWN;
				}
				break;
			case 'x':
				SEPARATE_WINDOWS = true;
				break;
			case 'h':
			default:
				threadsafe_console_output.println("This program can be invoked with the following flags:");
				threadsafe_console_output.println("-v: enable debug output");
				threadsafe_console_output.println("-x: do not call fork, but wait for the dataserver to be invoked separately");
				threadsafe_console_output.println("-r [0..4]: specify the IPC mechanism for RequestChannel to use");
				threadsafe_console_output.println("\tOptions:");
				threadsafe_console_output.println("\t\t0: named pipes, a.k.a. FIFOs (default)");
				threadsafe_console_output.println("\t\t1: unnamed pipes, via UNIX domain sockets");
				threadsafe_console_output.println("\t\t2: POSIX message queues");
				threadsafe_console_output.println("\t\t3: shared memory");
				threadsafe_console_output.println("\t\t4: signals (only works on operating systems that implement real-time signals)");
				threadsafe_console_output.println("-h: print this message and quit");
				threadsafe_console_output.println("If a given flag is not used, or given an invalid value,");
				threadsafe_console_output.println("a default value will be given to the corresponding variable.");
				threadsafe_console_output.println("If an illegal option is detected, behavior is the same as using the -h flag.");
				exit(EXIT_SUCCESS);
		}
	}
	
	if(IPC_MECHANISM != ERR_MODE_UNKNOWN) {
		
		std::vector<int64_t> request_processing_times;
		RequestChannel * control_channel = nullptr;
		
		if(IPC_MECHANISM == NAMED_PIPE) {
			
			if(!SEPARATE_WINDOWS) client_pid = fork();
			try {
				if((!SEPARATE_WINDOWS && client_pid > 0) ||
				   (SEPARATE_WINDOWS))
				{
					threadsafe_console_output.println("MAIN: Establishing NAMED PIPE control channel... ");
					control_channel = (RequestChannel*) new NamedPipeRequestChannel(control_channel_name, RequestChannel::CLIENT_SIDE);
					threadsafe_console_output.println("MAIN: Finished establishing NAMED PIPE control channel.");
					
					time_data_requests(*control_channel, &request_processing_times);
					
					std::string reply = control_channel->send_request("quit");
					threadsafe_console_output.println("MAIN: reply to request 'quit' is '" + reply);
				}
				else if(!SEPARATE_WINDOWS && client_pid == 0) {
					threadsafe_console_output.println("DATASERVER: Establishing NAMED PIPE control channel... ");
					control_channel = (RequestChannel*) new NamedPipeRequestChannel(control_channel_name, RequestChannel::SERVER_SIDE);
					threadsafe_console_output.println("DATASERVER: Finished establishing NAMED PIPE control channel.");
					Dataserver::handle_process_loop(*control_channel);
				}
			}
			catch (sync_lib_exception sle) {
				threadsafe_console_output.perror(((client_pid > 0) ? "MAIN: " : "DATASERVER: ") + std::string(sle.what()));
				throw;
			}
		}
		else if(IPC_MECHANISM == UNNAMED_PIPE) {
			pid_t client_pid;
			if(!SEPARATE_WINDOWS) client_pid = fork();
			try {
				if((!SEPARATE_WINDOWS && client_pid > 0) ||
				   (SEPARATE_WINDOWS))
				{
					threadsafe_console_output.println("MAIN: Establishing UNNAMED PIPE control channel... ");
					control_channel = (RequestChannel*)new UnnamedPipeRequestChannel(control_channel_name, RequestChannel::CLIENT_SIDE);
					threadsafe_console_output.println("MAIN: Finished establishing UNNAMED PIPE control channel.");
					
					time_data_requests(*control_channel, &request_processing_times);
					
					std::string reply = control_channel->send_request("quit");
					threadsafe_console_output.println("MAIN: reply to request 'quit' is '" + reply);
				}
				else if(!SEPARATE_WINDOWS && client_pid == 0) {
					threadsafe_console_output.println("DATASERVER: Establishing UNNAMED PIPE control channel... ");
					control_channel = (RequestChannel*)new UnnamedPipeRequestChannel(control_channel_name, RequestChannel::SERVER_SIDE);
					threadsafe_console_output.println("DATASERVER: Finished UNNAMED PIPE establishing control channel.");
					Dataserver::handle_process_loop(*control_channel);
				}
			}
			catch (sync_lib_exception sle) {
				threadsafe_console_output.perror(((client_pid > 0) ? "MAIN: " : "DATASERVER: ") + std::string(sle.what()));
				throw;
			}
			
		}
		else if(IPC_MECHANISM == MESSAGE_QUEUE) {
			pid_t client_pid;
			if(!SEPARATE_WINDOWS) client_pid = fork();
			try {
				if((!SEPARATE_WINDOWS && client_pid > 0) ||
				   (SEPARATE_WINDOWS))
				{
					threadsafe_console_output.println("MAIN: Establishing MESSAGE QUEUE control channel... ");
					control_channel = (RequestChannel*) new MessageQueueRequestChannel(control_channel_name, RequestChannel::CLIENT_SIDE);
					threadsafe_console_output.println("MAIN: Finished establishing MESSAGE QUEUE control channel.");
					
					time_data_requests(*control_channel, &request_processing_times);
					
					std::string reply = control_channel->send_request("quit");
					threadsafe_console_output.println("MAIN: reply to request 'quit' is '" + reply);
				}
				else if(!SEPARATE_WINDOWS && client_pid == 0) {
					threadsafe_console_output.println("DATASERVER: Establishing MESSAGE QUEUE control channel... ");
					control_channel = (RequestChannel*) new MessageQueueRequestChannel(control_channel_name, RequestChannel::SERVER_SIDE);
					threadsafe_console_output.println("DATASERVER: Finished establishing MESSAGE QUEUE control channel.");
					Dataserver::handle_process_loop(*control_channel);
				}
			}
			catch (sync_lib_exception sle) {
				threadsafe_console_output.perror(((client_pid > 0) ? "MAIN: " : "DATASERVER: ") + std::string(sle.what()));
				throw;
			}
		}
		else if(IPC_MECHANISM == SHARED_MEMORY) {
			pid_t client_pid;
			if(!SEPARATE_WINDOWS) client_pid = fork();
			try {
				if((!SEPARATE_WINDOWS && client_pid > 0) ||
				   (SEPARATE_WINDOWS))
				{
					threadsafe_console_output.println("MAIN: Establishing SHARED MEMORY control channel... ");
					control_channel = (RequestChannel*) new SharedMemoryRequestChannel(control_channel_name, RequestChannel::CLIENT_SIDE);
					threadsafe_console_output.println("MAIN: Finished establishing SHARED MEMORY control channel.");
					
					time_data_requests(*control_channel, &request_processing_times);
					
					std::string reply = control_channel->send_request("quit");
					threadsafe_console_output.println("MAIN: reply to request 'quit' is '" + reply);
				}
				else if(!SEPARATE_WINDOWS && client_pid == 0) {
					threadsafe_console_output.println("DATASERVER: Establishing SHARED MEMORY control channel... ");
					control_channel = (RequestChannel*) new SharedMemoryRequestChannel(control_channel_name, RequestChannel::SERVER_SIDE);
					threadsafe_console_output.println("DATASERVER: Finished establishing SHARED MEMORY control channel.");
					Dataserver::handle_process_loop(*control_channel);
				}
			}
			catch (sync_lib_exception sle) {
				threadsafe_console_output.perror(((client_pid > 0) ? "MAIN: " : "DATASERVER: ") + std::string(sle.what()));
				throw;
			}
		}
		else if(IPC_MECHANISM == SIGNAL) {
			pid_t client_pid;
			if(!SEPARATE_WINDOWS) client_pid = fork();
			try {
				if((!SEPARATE_WINDOWS && client_pid > 0) ||
				   (SEPARATE_WINDOWS))
				{
					threadsafe_console_output.println("MAIN: Establishing SIGNAL control channel... ");
					control_channel = (RequestChannel*) new SignalRequestChannel(control_channel_name, RequestChannel::CLIENT_SIDE);
					threadsafe_console_output.println("MAIN: Finished establishing SIGNAL control channel.");
					
					time_data_requests(*control_channel, &request_processing_times);
					
					std::string reply = control_channel->send_request("quit");
					threadsafe_console_output.println("MAIN: reply to request 'quit' is '" + reply);
				}
				else if(!SEPARATE_WINDOWS && client_pid == 0) {
					threadsafe_console_output.println("DATASERVER: Establishing SIGNAL control channel... ");
					control_channel = (RequestChannel*) new SignalRequestChannel(control_channel_name, RequestChannel::SERVER_SIDE);
					threadsafe_console_output.println("DATASERVER: Finished establishing SIGNAL control channel.");
					Dataserver::handle_process_loop(*control_channel);
				}
			}
			catch (sync_lib_exception sle) {
				threadsafe_console_output.perror(((client_pid > 0) ? "MAIN: " : "DATASERVER: ") + std::string(sle.what()));
				throw;
			}
		}
		
		delete control_channel;
		
		if(!SEPARATE_WINDOWS && client_pid == 0) exit(EXIT_SUCCESS);
		
		/*
			Crunch all the data that we've just gathered
		 */
		process_timing_data(&request_processing_times);
	}
	else {
		threadsafe_console_output.println("Invalid mode argument (value of -r flag). Invoke program with -h to view usage information.");
	}
}