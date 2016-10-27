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

/* --- none --- */

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

SignalRequestChannel::SignalRequestChannel(const std::string _name, const Side _side)
{
	throw sync_lib_exception("SignalRequestChannel not yet implemented");
}

SignalRequestChannel::~SignalRequestChannel() {
	
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
	return "";
}

int SignalRequestChannel::cwrite(std::string _msg) {
	return -1;
}

/*--------------------------------------------------------------------------*/
/* ACCESS THE NAME OF REQUEST CHANNEL  */
/*--------------------------------------------------------------------------*/

std::string SignalRequestChannel::name() {
	return my_name;
}
