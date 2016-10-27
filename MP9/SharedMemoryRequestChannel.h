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

const int SHM_MAX_REQUEST = NAME_MAX;
const int SHM_BUF_SIZE = 1;

/*--------------------------------------------------------------------------*/
/* CLASS   SharedMemoryRequestChannel */
/*--------------------------------------------------------------------------*/

class SharedMemoryRequestChannel : public RequestChannel {
	
private:
	
	std::string					my_name;
	Side						my_side;
	std::string					side_name;
	
	SharedMemoryBoundedBuffer	write_buf;
	SharedMemoryBoundedBuffer	read_buf;
	
public:
	/* Class constructor, and overrides of virtual functions    */
	SharedMemoryRequestChannel(const std::string _name, const Side _side);
	virtual ~SharedMemoryRequestChannel() override;
	virtual std::string send_request(std::string _request) override;
	virtual std::string cread() override;
	virtual int cwrite(std::string _msg) override;
	virtual std::string name() override;
	
};

#endif /* SharedMemoryRequestChannel_h */
