/*
	MessageQueueRequestChannel.h
 
	Created by Joshua Higginbotham on 6/16/16.
 */

#ifndef MessageQueueRequestChannel_h
#define MessageQueueRequestChannel_h

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "sync_lib.h"

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

/* --- none --- */

/*--------------------------------------------------------------------------*/
/* CLASS   MessageQueueRequestChannel */
/*--------------------------------------------------------------------------*/

class MessageQueueRequestChannel : public RequestChannel {
	
private:
	
	std::string					my_name;
	Side						my_side;
	std::string					side_name;
	
//	MessageQueueBoundedBuffer	write_buf;
//	MessageQueueBoundedBuffer	read_buf;
	
public:
	/* Class constructor, and overrides of virtual functions    */
	MessageQueueRequestChannel(const std::string _name, const Side _side);
	virtual ~MessageQueueRequestChannel();
	virtual std::string send_request(std::string _request);
	virtual std::string cread();
	virtual int cwrite(std::string _msg);
	virtual std::string name();
	
};

#endif /* MessageQueueRequestChannel_h */
