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

const int MQ_MAX_REQUEST	= NAME_MAX;
const int MQ_NUM_MESSAGES	= 1;

/*--------------------------------------------------------------------------*/
/* CLASS   MessageQueueRequestChannel */
/*--------------------------------------------------------------------------*/

class MessageQueueRequestChannel : public RequestChannel {
	
private:
	
	std::string					my_name;
	Side						my_side;
	std::string					side_name;
	
	MessageQueueBoundedBuffer	write_buf;
	MessageQueueBoundedBuffer	read_buf;
	
public:
	/* Class constructor, and overrides of virtual functions    */
	MessageQueueRequestChannel(const std::string _name, const Side _side);
	virtual ~MessageQueueRequestChannel() override;
	virtual std::string send_request(std::string _request) override;
	virtual std::string cread() override;
	virtual int cwrite(std::string _msg) override;
	virtual std::string name() override;
	
};

#endif /* MessageQueueRequestChannel_h */
