/*
	SignalRequestChannel.h

	Created by Joshua Higginbotham on 6/16/16.
 */

#ifndef SignalRequestChannel_h
#define SignalRequestChannel_h

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "sync_lib.h"

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

const int SIG_MAX_REQUEST = NAME_MAX;

/*--------------------------------------------------------------------------*/
/* CLASS   SignalRequestChannel */
/*--------------------------------------------------------------------------*/

class SignalRequestChannel : public RequestChannel {
	
private:
	/*
	 	Strings can't be sent between processes
	 	by signal, but integers (including file descriptors)
	 	can be. This solution is admittedly a bit hacked,
	 	but should be fine within the context of this machine problem.
	 
	 	Would this be better done with an enum class?
	 */
	const static int HELLO = -1;
	const static int HELLO_TO_YOU_TOO = -2;
	const static int QUIT = -3;
	const static int BYE = -4;
	const static int DATA = -5;
	const static int NEWTHREAD = -6;
	const static int INVALID_REQUEST = -7;
	const static int UNKNOWN_REQUEST = INT_MIN;
	static bool stoi_throws_on_input(std::string test_string);
	static int string_to_sig_msg_value(std::string str);
	static std::string sig_msg_value_to_string(int sig_msg_value);
	
	std::string my_name = "";
	Side my_side;
	int channel_signum = -1;
	pid_t my_pid = -1; //Channel's own pid
	pid_t partner_pid = -1;//Pid of channel's "partner"
	sigset_t rt_signals;
	std::string side_name = "";
	
	/* Used only by SERVER_SIDE */
	const std::string sig_num_buf_name = "available_rt_sig_nums_";
	SharedMemoryBoundedBuffer available_rt_sig_nums;
	
	
public:
	/* Constructor */
	SignalRequestChannel(const std::string _name, const Side _side);
	
	/* Overrides of virtual functions from RequestChannel */
	virtual ~SignalRequestChannel() override;
	virtual std::string send_request(std::string _request) override;
	virtual std::string cread() override;
	virtual int cwrite(std::string _msg) override;
	virtual std::string name() override;
};

#endif /* SignalRequestChannel_h */
