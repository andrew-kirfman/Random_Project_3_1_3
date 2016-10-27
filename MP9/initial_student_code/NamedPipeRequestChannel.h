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
	int wfd = -1;
	int rfd = -1;
	
	/*  Generates a name for a pipe                             */
	char * pipe_name(Mode _mode);
	
	/*  Open named pipes for reading and writing                */
	void open_read_pipe(char * _pipe_name);
	void open_write_pipe(char * _pipe_name);
	bool read_pipe_opened = false;
	bool write_pipe_opened = false;
	
	/*  Return the read file descriptor and
	 write file descriptor respectively                      */
	int read_fd();
	int write_fd();
public:
	/* Class constructor, and overrides of virtual functions    */
	NamedPipeRequestChannel(const std::string _name, const Side _side);
	virtual ~NamedPipeRequestChannel();
	virtual std::string send_request(std::string _request);
	virtual std::string cread();
	virtual int cwrite(std::string _msg);
	virtual std::string name();
	
};

#endif /* NamedPipeRequestChannel_h */
