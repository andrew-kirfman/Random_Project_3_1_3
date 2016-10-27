/*
	SharedMemoryRequestChannel.h
 
	Created by Joshua Higginbotham on 6/16/16.
 */

#ifndef SharedMemoryRequestChannel_h
#define SharedMemoryRequestChannel_h

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "sync_lib.h"

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

/* --- none --- */

/*--------------------------------------------------------------------------*/
/* CLASS   SharedMemoryRequestChannel */
/*--------------------------------------------------------------------------*/

class SharedMemoryRequestChannel : public RequestChannel {
	
private:
	
	std::string					my_name;
	Side						my_side;
	std::string					side_name;
	
//	SharedMemoryBoundedBuffer	write_buf;
//	SharedMemoryBoundedBuffer	read_buf;
	
public:
	/* Class constructor, and overrides of virtual functions    */
	SharedMemoryRequestChannel(const std::string _name, const Side _side);
	virtual ~SharedMemoryRequestChannel();
	virtual std::string send_request(std::string _request);
	virtual std::string cread();
	virtual int cwrite(std::string _msg);
	virtual std::string name();
	
};

#endif /* SharedMemoryRequestChannel_h */
