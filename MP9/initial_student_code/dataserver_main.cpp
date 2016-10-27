/*
	dataserver_main.cpp

	Created by Joshua Higginbotham on 6/22/16.

	Allows the dataserver to be run separately
	from the client, i.e. without using fork.
 */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "Dataserver.h"
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

/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/

int main(int argc, char** argv) {
	
	int IPC_MECHANISM = NAMED_PIPE; //Results in default execution of NAMED_PIPE code
	if(argc == 2) {
		if(std::string(argv[1]) == "NAMED_PIPE") {
			IPC_MECHANISM = NAMED_PIPE;
		}
		else if(std::string(argv[1]) == "UNNAMED_PIPE") {
			IPC_MECHANISM = UNNAMED_PIPE;
		}
		else if(std::string(argv[1]) == "MESSAGE_QUEUE") {
			IPC_MECHANISM = MESSAGE_QUEUE;
		}
		else if(std::string(argv[1]) == "SHARED_MEMORY") {
			IPC_MECHANISM = SHARED_MEMORY;
		}
		else if(std::string(argv[1]) == "SIGNAL") {
			IPC_MECHANISM = SIGNAL;
		}
		else {
			IPC_MECHANISM = ERR_MODE_UNKNOWN;
		}
	}
	else if(argc > 2) {
		IPC_MECHANISM = ERR_MODE_UNKNOWN;
	}
	
	if(IPC_MECHANISM != ERR_MODE_UNKNOWN) {
		try {
			if(IPC_MECHANISM == NAMED_PIPE) {
				threadsafe_console_output.println("DATASERVER: Establishing NAMED PIPE control channel... ");
				NamedPipeRequestChannel control_channel(control_channel_name, RequestChannel::SERVER_SIDE);
				threadsafe_console_output.println("DATASERVER: Finished establishing NAMED PIPE control channel.");
				Dataserver::handle_process_loop(control_channel);
			}
			else if(IPC_MECHANISM == UNNAMED_PIPE) {
				threadsafe_console_output.println("DATASERVER: Establishing UNNAMED PIPE control channel... ");
				UnnamedPipeRequestChannel control_channel(control_channel_name, RequestChannel::SERVER_SIDE);
				threadsafe_console_output.println("DATASERVER: Finished establishing UNNAMED PIPE control channel.");
				Dataserver::handle_process_loop(control_channel);
			}
			else if(IPC_MECHANISM == MESSAGE_QUEUE) {
				threadsafe_console_output.println("DATASERVER: Establishing MESSAGE QUEUE control channel... ");
				MessageQueueRequestChannel control_channel(control_channel_name, RequestChannel::SERVER_SIDE);
				threadsafe_console_output.println("DATASERVER: Finished establishing MESSAGE QUEUE control channel.");
				Dataserver::handle_process_loop(control_channel);
			}
			else if(IPC_MECHANISM == SHARED_MEMORY) {
				threadsafe_console_output.println("DATASERVER: Establishing SHARED MEMORY control channel... ");
				SharedMemoryRequestChannel control_channel(control_channel_name, RequestChannel::SERVER_SIDE);
				threadsafe_console_output.println("DATASERVER: Finished establishing SHARED MEMORY control channel.");
				Dataserver::handle_process_loop(control_channel);
			}
			else if(IPC_MECHANISM == SIGNAL) {
				threadsafe_console_output.println("DATASERVER: Establishing SIGNAL control channel... ");
				SignalRequestChannel control_channel(control_channel_name, RequestChannel::SERVER_SIDE);
				threadsafe_console_output.println("DATASERVER: Finished establishing SIGNAL control channel.");
				Dataserver::handle_process_loop(control_channel);
			}

		}
		catch (sync_lib_exception sle) {
			threadsafe_console_output.perror("DATASERVER: control channel not created: " + std::string(sle.what()));
			throw;
		}
	}
	else {
		threadsafe_console_output.println("Apologies, one of 3 things has happened:");
		threadsafe_console_output.println("\t1) The mode-name argument was for a valid mode, but it was mistyped");
		threadsafe_console_output.println("\t2) There is no mode corresponding to the name argument (i.e. the argument was invalid)");
		threadsafe_console_output.println("\t3) More than two arguments were supplied, and the program didn't know which one was the mode name");
	}
}

