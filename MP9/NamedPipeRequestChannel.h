/*
	NamedPipeRequestChannel.h

	Created by Joshua Higginbotham on 6/16/16.
 */

#ifndef NamedPipeRequestChannel_h
#define NamedPipeRequestChannel_h

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "sync_lib.h"

/*--------------------------------------------------------------------------*/
/* CLASS   NamedPipeRequestChannel */
/*--------------------------------------------------------------------------*/

class NamedPipeRequestChannel : public RequestChannel {
	
private:
	std::string		my_name = "";
	Side			my_side;
	std::string		side_name = "";
	
	/*  File descriptors of write and read pipes                */
	int wfd = 0;
	int rfd = 0;
	
	/*  Generates a name for a pipe                             */
	std::string pipe_name(Mode _mode);
	
	/*  Open named pipes for reading and writing                */
	void open_read_pipe(const char * _pipe_name);
	void open_write_pipe(const char * _pipe_name);
	bool read_pipe_opened = false;
	bool write_pipe_opened = false;
	
	/*	Locks used to keep the dataserver from dropping requests.	*/
	pthread_mutex_t read_lock = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t write_lock = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutexattr_t srl_attr;
	pthread_mutex_t send_request_lock;
	
	/*  Return the read file descriptor and
	 write file descriptor respectively                      */
	int read_fd();
	int write_fd();
public:
	/* Class constructor, and overrides of virtual functions    */
	NamedPipeRequestChannel(const std::string _name, const Side _side);
	virtual ~NamedPipeRequestChannel() override;
	virtual std::string send_request(std::string _request) override;
	virtual std::string cread() override;
	virtual int cwrite(std::string _msg) override;
	virtual std::string name() override;
	
};

#endif /* NamedPipeRequestChannel_h */
