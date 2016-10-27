/*
	UnnamedPipeRequestChannel.h
 
	Created by Joshua Higginbotham on 6/16/16.
 */

#ifndef UnnamedPipeRequestChannel_h
#define UnnamedPipeRequestChannel_h

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "sync_lib.h"

/*--------------------------------------------------------------------------*/
/* CLASS   UnnamedPipeRequestChannel */
/*--------------------------------------------------------------------------*/

class UnnamedPipeRequestChannel : public RequestChannel {
	
private:
	std::string		my_name;
	Side			my_side;
	std::string		side_name;
	std::string		sock_pathname;
	
	/*  File descriptors of write and read pipes.
	 Copies of all file descriptors are kept intially,
	 so that in case it takes a while for the other
	 end of the channel to respond proper file
	 description closure can still take place. */
	int server_wfd = -1;
	int server_rfd = -1;
	int client_wfd = -1;
	int client_rfd = -1;
	int wfd = -1;
	int rfd = -1;
	
	ProcessSharedSemaphore sock_listening;
	ProcessSharedSemaphore fds_received;
	
public:
	/* Class constructor, and overrides of virtual functions    */
	UnnamedPipeRequestChannel(const std::string _name, const Side _side);
	virtual ~UnnamedPipeRequestChannel() override;
	virtual std::string send_request(std::string _request) override;
	virtual std::string cread() override;
	virtual int cwrite(std::string _msg) override;
	virtual std::string name() override;
	
	/* Other helper functions */
	/*  Return the read file descriptor and
	 write file descriptor respectively                      */
	int read_fd();
	int write_fd();
	std::string format_file_descriptor_string();
};

#endif /* UnnamedPipeRequestChannel_h */
