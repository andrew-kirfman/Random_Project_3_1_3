/*
	sync_lib.h

	Created by Joshua Higginbotham on 11/4/15.
 */

#ifndef sync_lib_h
#define sync_lib_h
/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* --- none --- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <stdio.h>
#include <queue>
#include <string>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <errno.h>
#include <semaphore.h>
#include <iostream>
#include <fstream>
#include <stddef.h>
#include <cassert>
#include <cstring>
#include <sys/un.h>
#include <sys/socket.h>
#include <mqueue.h>
#include <limits.h>
#include <signal.h>
#include <cmath>
#include <stdexcept>
#include <sys/time.h>
#include <numeric>
#include <exception>
#include <list>
#include <iterator>
#include <assert.h>
#include <vector>
#include <algorithm>
#include <memory>
#include <sstream>
#include <iomanip>

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

/* --- none --- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

const int MY_CREATION_FLAGS = S_IRWXU | S_IRWXG | S_IRWXO;
const int NUM_SYSTEM_RT_SIGVALUES = SIGRTMAX - SIGRTMIN + 1;

/*--------------------------------------------------------------------------*/
/* INTERFACE   S e m a p h o r e  */
/*--------------------------------------------------------------------------*/

class Semaphore {

public:

	/* CONSTRUCTOR/DESTRUCTOR */

	virtual ~Semaphore() {};

	/* SEMAPHORE OPERATIONS */

	//The int return value for P() and V() can be used
	//as part of error checking.
	virtual int P() = 0;
	virtual int V() = 0;
};

/*--------------------------------------------------------------------------*/
/* CLASS   T h r e a d S h a r e d S e m a p h o r e  */
/*--------------------------------------------------------------------------*/

class ThreadSharedSemaphore : public Semaphore {
private:
	/* INTERNAL DATA STRUCTURES */

	int             value = 0;
	pthread_mutex_t value_access_lock;
	pthread_cond_t  wait_queue;

public:

	/* CONSTRUCTOR/DESTRUCTOR */

	ThreadSharedSemaphore(int _val);
	virtual ~ThreadSharedSemaphore() override;

	/* SEMAPHORE OPERATIONS */

	virtual int P() override;
	virtual int V() override;
};

/*--------------------------------------------------------------------------*/
/* CLASS   P r o c e s s S h a r e d S e m a p h o r e  */
/*--------------------------------------------------------------------------*/

class ProcessSharedSemaphore : public Semaphore {
private:
	/* INTERNAL DATA STRUCTURES	*/

	int			max = 0;
	std::string	sema_name = "";
	sem_t* 		sema = SEM_FAILED;
	bool		unlink_on_destruction = true;
	bool		empty_constructor_used = false;

public:

	/* CONSTRUCTOR/DESTRUCTOR	*/

	ProcessSharedSemaphore(std::string _name, int _val);
	ProcessSharedSemaphore() { empty_constructor_used = true; }
	virtual ~ProcessSharedSemaphore() override;

	/* SEMAPHORE OPERATIONS		*/

	virtual int P() override;
	virtual int V() override;

	/*HELPER FUNCTIONS			*/

	bool		initial_construction_failed();
	std::string get_name();
	void		set_unlink_on_destruction(bool _unlink_afterwards);
};

/*--------------------------------------------------------------------------*/
/* INTERFACE   B o u n d e d B u f f e r */
/*--------------------------------------------------------------------------*/

class BoundedBuffer {
public:
	virtual ~BoundedBuffer() {};
	virtual std::string push_back(std::string str) = 0;
	virtual std::string retrieve_front() = 0;
	virtual int size() = 0;
};

/*--------------------------------------------------------------------------*/
/* CLASS   T h r e a d S h a r e d B o u n d e d B u f f e r */
/*--------------------------------------------------------------------------*/

class ThreadSharedBoundedBuffer : public BoundedBuffer {
	int max_items;
	ThreadSharedSemaphore full; //No overflow
	ThreadSharedSemaphore empty; //No underflow
	pthread_mutex_t item_access_lock; //Thread-safe: only allows one at a time
	std::queue<std::string> items;

public:
	ThreadSharedBoundedBuffer(int _size);
	virtual ~ThreadSharedBoundedBuffer() override;
	virtual std::string push_back(std::string str) override;
	virtual std::string retrieve_front() override;
	virtual int size() override;
};

/*--------------------------------------------------------------------------*/
/* CLASS   M e s s a g e Q u e u e B o u n d e d B u f f e r */
/*--------------------------------------------------------------------------*/

class MessageQueueBoundedBuffer : public BoundedBuffer {

	int max_msgs = 0;
	std::string my_name = "";
	std::string mq_name = "";
	int msg_size = 0;
	bool unlink_on_destruction = true;
	bool empty_constructur_used = false;

	mqd_t mq_descriptor = (mqd_t) -1;

public:
	MessageQueueBoundedBuffer(std::string _name, int _num_msgs, int _msg_size);
	MessageQueueBoundedBuffer() { empty_constructur_used = true; }
	virtual ~MessageQueueBoundedBuffer() override;
	virtual std::string push_back(std::string str) override;
	virtual std::string retrieve_front() override;
	virtual int size() override;

	std::string get_name();
	int 		get_msg_size();
	void 		set_unlink_on_destruction(bool _unlink_afterwards);
};

/*--------------------------------------------------------------------------*/
/* CLASS   S h a r e d M e m o r y B o u n d e d B u f f e r */
/*--------------------------------------------------------------------------*/

class SharedMemoryBoundedBuffer : public BoundedBuffer {

	int max_items = 0;
	int block_size = 0;
	int shm_total_bytes = 0;
	int blocks_taken = 0;
	std::string my_name = "";
	std::string seg_name = "";
	ProcessSharedSemaphore full = ProcessSharedSemaphore(); //No overflow
	ProcessSharedSemaphore empty = ProcessSharedSemaphore(); //No underflow
	ProcessSharedSemaphore seg_access_lock = ProcessSharedSemaphore(); //Thread-safe: only allows one at a time
	bool unlink_on_destruction = true;
	bool already_existed = false;
	bool empty_constructor_used = false;

	char* shm_seg = (char*) MAP_FAILED;
	char* shm_seg_blocks_start = (char*) MAP_FAILED;
	int read_index = 0; //in blocks
	int write_index = 0;

public:
	SharedMemoryBoundedBuffer(std::string _name, int _num_items, int _block_size);
	SharedMemoryBoundedBuffer() : empty_constructor_used(true) {}
	virtual ~SharedMemoryBoundedBuffer() override;
	virtual std::string push_back(std::string str) override;
	virtual std::string retrieve_front() override;
	virtual int size() override;

	std::string get_name();
	int 		get_blocks_taken();
	int 		get_block_size();
	void		set_unlink_on_destruction(bool _unlink_afterwards);
	bool		get_already_existed();
};

/*--------------------------------------------------------------------------*/
/* INTERFACE   R e q u e s t C h a n n e l */
/*--------------------------------------------------------------------------*/

class RequestChannel {

public:
	typedef enum {SERVER_SIDE, CLIENT_SIDE} Side;
	typedef enum {READ_MODE, WRITE_MODE} Mode;

	/* CONSTRUCTOR/DESTRUCTOR */

	//virtual RequestChannel(const std::string _name, const Side _side) = 0;
	/*
	 EXPECTED BEHAVIOR OF CONSTRUCTORS THAT USE THIS INTERFACE:
	 Creates a "local copy" of the channel specified by the given name.
	 If the channel does not exist, the associated IPC mechanisms are
	 created. If the channel exists already, this object is associated with the channel.
	 The channel has two ends, which are conveniently called "SERVER_SIDE" and "CLIENT_SIDE".
	 If two processes connect through a channel, one has to connect on the server side
	 and the other on the client side. Otherwise the results are unpredictable.

	 NOTE: If the creation of the request channel fails (typically happens when too many
	 request channels are being created) a sync_lib_exception can be thrown.

	 NOTE: It is easy to open too many request channels in parallel. In some systems,
	 limits on the number of open files per process limit the number of established
	 request channels to 125 or less.
	 */

	virtual ~RequestChannel() {};
	/*
	 EXPECTED BEHAVIOR OF DESTRUCTORS THAT USE THIS INTERFACE:
	 Destructor of the local copy of the bus. By default, the Server Side deletes any IPC
	 mechanisms associated with the channel.
	 */

	virtual std::string send_request(std::string _request) = 0;
	/* Send a string over the channel and wait for a reply. */

	virtual std::string cread() = 0;
	/* Blocking read of data from the channel. Returns a string of characters
	 read from the channel. Returns NULL if read failed. */

	virtual int cwrite(std::string _msg) = 0;
	/* Write the data to the channel. The function returns the number of characters written
	 to the channel. */

	virtual std::string name() = 0;
	/* Returns the name of the request channel. */
};

/*--------------------------------------------------------------------------*/
/* HELPER FUNCTIONS */
/*--------------------------------------------------------------------------*/

//Taken from Stack Overflow: http://stackoverflow.com/questions/3407012/c-rounding-up-to-the-nearest-multiple-of-a-number
static int roundUp (size_t numToRound, size_t multiple) {
	if(multiple == 0) {
		return numToRound;
	}

	int remainder = numToRound % multiple;

	if (remainder == 0) {
		return numToRound;
	}

	return numToRound + multiple - remainder;
}

/*
	This class can be used to write to standard output
	in a multithreaded environment. Its primary purpose
	is printing debug messages while multiple threads
	are executing.
*/

class atomic_console_output {
	ProcessSharedSemaphore console_lock = ProcessSharedSemaphore("/console_output_lock", 1);;
public:
	atomic_console_output() {}
	~atomic_console_output() {}
	void println(std::string s) {
		console_lock.P();
		std::cout << s << std::endl;
		console_lock.V();
	}
	void perror(std::string s) {
		console_lock.P();
		std::cerr << s << ", errno == " << strerror(errno) << std::endl;
		console_lock.V();
	}
};

static atomic_console_output threadsafe_console_output;

class sync_lib_exception : public std::exception {
	std::string err = "failure in sync library";

public:
	sync_lib_exception() {}
	sync_lib_exception(std::string msg) : err(msg) {}
	virtual const char* what() const throw() {
		return err.c_str();
	}
};


#endif /* sync_lib_h */
