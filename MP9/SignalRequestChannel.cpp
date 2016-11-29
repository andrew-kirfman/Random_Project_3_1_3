/*
	SignalRequestChannel.cpp

	Created by Joshua Higginbotham on 6/16/16.
 */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "SignalRequestChannel.h"
#include "MessageQueueRequestChannel.h"

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

const bool VERBOSE = false;
const bool VERBOSE_DEBUG = false;

/*--------------------------------------------------------------------------*/
/* PRIVATE STATIC METHODS FOR CLASS   SignalRequestChannel  */
/*--------------------------------------------------------------------------*/

bool SignalRequestChannel::stoi_throws_on_input(std::string input_string) {
	try {
		int exception_test = std::stoi(input_string);
		return false;
	}
	catch(...) {
		return true;
	}
}

int SignalRequestChannel::string_to_sig_msg_value(std::string str) {
	if(str.substr(0,5) == "hello") return HELLO;
	else if(str == "hello to you too") return HELLO_TO_YOU_TOO;
	else if(str.substr(0,4) == "quit") return QUIT;
	else if(str.substr(0,3) == "bye") return BYE;
	else if(str.substr(0,5) == "data ") return DATA;
	else if(str.substr(0,9) == "newthread") return NEWTHREAD;
	else if(str.substr(0,15) == "unknown request") return UNKNOWN_REQUEST;
	else if(str.substr(0,5) == "data_") return (std::stoi(str.substr(5,std::string::npos)) + 100);
	else if(!stoi_throws_on_input(str)) return std::stoi(str);
	else return INVALID_REQUEST;
}

std::string SignalRequestChannel::sig_msg_value_to_string(int sig_msg_value) {
	if(sig_msg_value == HELLO) return "hello";
	else if(sig_msg_value == HELLO_TO_YOU_TOO) return "hello to you too";
	else if(sig_msg_value == QUIT) return "quit";
	else if(sig_msg_value == BYE) return "bye";
	else if(sig_msg_value == DATA) return "data Dr. Ahmed";
	else if(sig_msg_value == NEWTHREAD) return "newthread_SIGNAL";
	else if(sig_msg_value == UNKNOWN_REQUEST) return "unknown request";
	else if(sig_msg_value >= 100) return "data_" + std::to_string(sig_msg_value - 100);
	else if(sig_msg_value < 100 && sig_msg_value >= 0) return std::to_string(sig_msg_value);
	else return "*****"; //Includes case for sig_msg_value == INVALID_REQUEST

}

/*--------------------------------------------------------------------------*/
/* CONSTRUCTOR/DESTRUCTOR FOR CLASS   SignalRequestChannel  */
/*--------------------------------------------------------------------------*/

SignalRequestChannel::SignalRequestChannel(const std::string _name, const Side _side) :
my_name(_name),
my_side(_side),
my_pid(getpid()),
side_name((my_side == SERVER_SIDE) ? "SERVER" : "CLIENT"),
available_rt_sig_nums(
						SharedMemoryBoundedBuffer(sig_num_buf_name + std::to_string(my_pid), NUM_SYSTEM_RT_SIGVALUES, std::to_string(SIGRTMAX).length() + 1)
					  )
{
	/*
	 	Set signal mask for process to block all real-time signals
	 */
	if(VERBOSE) {
		threadsafe_console_output.println("SIGNAL_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": initializing signal mask...");
	}
	if(sigemptyset(&rt_signals) < 0) {
		throw sync_lib_exception("SIGNAL_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": failed on sigemptyset(&rt_signals)");
	}
	for(int i = 0; i < NUM_SYSTEM_RT_SIGVALUES; ++i) {
		if(sigaddset(&rt_signals, SIGRTMIN + i) < 0) {
			throw sync_lib_exception("SIGNAL_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": failed on sigaddset(&rt_signals, " + std::to_string(SIGRTMIN + i) + ")");
		}
	}

	if(VERBOSE) {
		threadsafe_console_output.println("SIGNAL_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": setting signal mask");
	}
	if ((errno = pthread_sigmask(SIG_BLOCK, &rt_signals, NULL)) != 0) {
		throw sync_lib_exception("SIGNAL_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": failed on pthread_sigmask(SIG_SETMASK, &rt_signals, NULL)");
	}

	if(my_side == SERVER_SIDE) {

		if(available_rt_sig_nums.get_already_existed() == false) {
			if(VERBOSE) {
				threadsafe_console_output.println("SIGNAL_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": initializing sig num buffer...");
			}
			for(int i = 0; i < NUM_SYSTEM_RT_SIGVALUES; ++i) {
				std::string push_result = available_rt_sig_nums.push_back(std::to_string(SIGRTMIN + i));
				if(push_result.substr(0,5) == "ERROR") {
					throw sync_lib_exception("SIGNAL_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": failed on push_back to available_rt_sig_nums: " + push_result);
				}
			}
			if(VERBOSE) {
				threadsafe_console_output.println("SIGNAL_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": finished initializing sig num buffer");
			}
		}

		if(VERBOSE) {
			threadsafe_console_output.println("SIGNAL_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": retrieving signum from buffer...");
 		}
		std::string signum_retrieval_result = available_rt_sig_nums.retrieve_front();

		if(signum_retrieval_result.substr(0,5) == "ERROR") {
			throw sync_lib_exception("SIGNAL_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": failed on retrieve_front from available_rt_sig_nums: " + signum_retrieval_result);
		}
		channel_signum = stoi(signum_retrieval_result);
		if(VERBOSE) {
			threadsafe_console_output.println("SIGNAL_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": retreived channel_signum == " + std::to_string(channel_signum));
 		}
		int estimated_max_pid_val = (int) pow(2, 7 + (8 * (sizeof(pid_t) - 1))) - 1;
		if(VERBOSE) {
			threadsafe_console_output.println("SIGNAL_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": constructing MessageQueueRequestChannel for sync messages...");
		}
		MessageQueueRequestChannel exchange_channel(my_name, RequestChannel::SERVER_SIDE);
		std::string server_sync_request = "SERVER:" + std::to_string(my_pid) + ":" + std::to_string(channel_signum);

		if(VERBOSE) {
			threadsafe_console_output.println("SIGNAL_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": sending sync request to client...");
		}
		std::string client_response = exchange_channel.send_request(server_sync_request);
		if(client_response.substr(0,7) != "CLIENT:") {
			errno = 0;
			throw sync_lib_exception("SIGNAL_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": failed to send pid/signum request to client [" + server_sync_request + "]");
		}

		if(VERBOSE) {
			threadsafe_console_output.println("SIGNAL_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": parsing client response...");
		}
		size_t index_of_first_colon = client_response.find_first_of(":");
		size_t index_of_last_colon = client_response.find_last_of(":");
		std::string client_pid_string = client_response.substr(index_of_first_colon + 1, index_of_last_colon - index_of_first_colon - 1);
		std::string client_signum_string = client_response.substr(index_of_last_colon + 1, std::string::npos);
		partner_pid = stoi(client_pid_string);
		if(channel_signum != stoi(client_signum_string)) {
			errno = 0;
			throw sync_lib_exception("SIGNAL_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": failed to synchronize client/server signal numbers");
		}
	}
	else {
		if(VERBOSE) {
			threadsafe_console_output.println("SIGNAL_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": constructing MessageQueueRequestChannel for sync messages...");
		}
		MessageQueueRequestChannel exchange_channel(my_name, RequestChannel::CLIENT_SIDE);
		if(VERBOSE) {
			threadsafe_console_output.println("SIGNAL_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": reading server sync request...");
		}
		std::string server_sync_request = exchange_channel.cread();
		if(server_sync_request.substr(0,5) == "ERROR") {
			errno = 0;
			throw sync_lib_exception("SIGNAL_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": failed to read server sync request from message queue");
		}
		else if(server_sync_request.substr(0,7) != "SERVER:") {
			errno = 0;
			throw sync_lib_exception("SIGNAL_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": received invalid sync request from server [" + server_sync_request + "]");
		}

		if(VERBOSE) {
			threadsafe_console_output.println("SIGNAL_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": parsing server sync request...");
		}
		size_t index_of_first_colon = server_sync_request.find_first_of(":");
		size_t index_of_last_colon = server_sync_request.find_last_of(":");
		std::string server_pid_string = server_sync_request.substr(index_of_first_colon + 1, index_of_last_colon - index_of_first_colon - 1);
		std::string server_signum_string = server_sync_request.substr(index_of_last_colon + 1, std::string::npos);
		partner_pid = stoi(server_pid_string);
		channel_signum = stoi(server_signum_string);

		std::string client_response = "CLIENT:" + std::to_string(my_pid) + ":" + std::to_string(channel_signum);
		if(VERBOSE) {
			threadsafe_console_output.println("SIGNAL_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": sending response to server...");
		}
		int write_result = exchange_channel.cwrite(client_response);
		if(write_result < 0) {
			errno = 0;
			throw sync_lib_exception("SIGNAL_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": failed to write client response back to server [" + client_response + "]");
		}
	}

	if(VERBOSE) {
		threadsafe_console_output.println("SIGNAL_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": SignalRequestChannel constructed [pid == " + std::to_string(my_pid) + ", partner_pid == " + std::to_string(partner_pid) + ", channel_signum == " + std::to_string(channel_signum) + "]");
	}
}

SignalRequestChannel::~SignalRequestChannel() {
	if(VERBOSE) {
		threadsafe_console_output.println("SIGNAL_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": destructing...");
	}
	if(VERBOSE && my_side == SERVER_SIDE) {
		threadsafe_console_output.println("SIGNAL_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": returning channel_signum == " + std::to_string(channel_signum) + " to signum buffer, releasing signum buffer semaphore...");
 	}

	if(my_side == SERVER_SIDE) {
		available_rt_sig_nums.push_back(std::to_string(channel_signum));
	}

	if(VERBOSE) {
		threadsafe_console_output.println("SIGNAL_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": finished destructing.");
	}
}

/*--------------------------------------------------------------------------*/
/* READ/WRITE FROM/TO REQUEST CHANNELS  */
/*--------------------------------------------------------------------------*/

std::string SignalRequestChannel::send_request(std::string _request) {
	cwrite(_request);
	std::string read_result = cread();
	return read_result;
}

std::string SignalRequestChannel::cread() {

	sigset_t channel_signum_set;
	sigemptyset(&channel_signum_set);
	if(sigaddset(&channel_signum_set, channel_signum) < 0) {
		threadsafe_console_output.perror("SIGNAL_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": failed on sigaddset(&channel_signum_set, " + std::to_string(channel_signum) + ")");
		return "ERROR";
	}

	if(VERBOSE && VERBOSE_DEBUG) {
		threadsafe_console_output.println("SIGNAL_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": reading...");
	}

	siginfo_t sig_response_buf;

	int signum = sigwaitinfo(&channel_signum_set, &sig_response_buf);
	if(signum < 0) {
		threadsafe_console_output.perror("SIGNAL_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": failed to sigwaitinfo from partner");
		return "ERROR";
	}

	if(VERBOSE && VERBOSE_DEBUG) {
		threadsafe_console_output.println("SIGNAL_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": converting message to string...");
	}
	std::string read_result = sig_msg_value_to_string(sig_response_buf.si_int);

	if(VERBOSE && VERBOSE_DEBUG) {
		int sigismember_result = sigismember(&channel_signum_set, signum);
		if(sigismember_result < 0) {
			threadsafe_console_output.perror("SIGNAL_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": failed to test signum == " + std::to_string(signum) + " with sigismember");
			return "ERROR";
		}
		std::string signum_is_in_set = (sigismember_result == 1) ? "TRUE" : "FALSE";
		threadsafe_console_output.println("SIGNAL_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": finished reading [channel_signum == " + std::to_string(channel_signum) + ", signum_received == " + std::to_string(signum) + ", signum_is_in_set == " + signum_is_in_set + ", msg == " + read_result + "]");
	}

	return read_result;

}

int SignalRequestChannel::cwrite(std::string _msg) {


	if(VERBOSE && VERBOSE_DEBUG) {
		threadsafe_console_output.println("SIGNAL_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": writing [" + _msg + "]");
	}

	sigval sigmsg;
	sigmsg.sival_int = string_to_sig_msg_value(_msg);

	if(sigqueue(partner_pid, channel_signum, sigmsg) < 0) {
		threadsafe_console_output.perror("SIGNAL_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": failed to sigqueue to partner");
		return -1;
	}

	if(VERBOSE && VERBOSE_DEBUG) {
		threadsafe_console_output.println("SIGNAL_REQUEST_CHANNEL:" + my_name + ":" + side_name + ": wrote sig_msg_value [" + std::to_string(sigmsg.sival_int) + "]");
	}

	return sigmsg.sival_int;

}

/*--------------------------------------------------------------------------*/
/* ACCESS THE NAME OF REQUEST CHANNEL  */
/*--------------------------------------------------------------------------*/

std::string SignalRequestChannel::name() {
	return my_name;
}
