/*
	sync_lib.h


	Created by Joshua Higginbotham on 7/27/16.

*/

#ifndef sync_lib_h_
#define sync_lib_h_
/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*
	These should be sorted in a manner that indicates where
 	they are used.
*/
/*--------------------------------------------------------------------------*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <list>
#include <assert.h>
#include <assert.h>
#include <semaphore.h>
#include <iostream>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <string.h>
#include <string>

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

const int MY_CREATION_FLAGS = S_IRWXU;

/*--------------------------------------------------------------------------*/
/* INTERFACE   S e m a p h o r e  */
/*--------------------------------------------------------------------------*/

class Semaphore {
	
public:
	
	/* -- CONSTRUCTOR/DESTRUCTOR */
	
	virtual ~Semaphore() {};
	
	/* -- SEMAPHORE OPERATIONS */
	
	//P and V functions should return error codes
	virtual int P() = 0;
	virtual int V() = 0;
};

/*--------------------------------------------------------------------------*/
/* CLASS   P t h r e a d S e m a p h o r e  */
/*--------------------------------------------------------------------------*/

class PthreadSemaphore : public Semaphore {
private:
	/* -- INTERNAL DATA STRUCTURES */
	
	int             value = 0;
	pthread_mutex_t value_access_lock;
	pthread_cond_t  wait_queue;
	
public:
	
	/* -- CONSTRUCTOR/DESTRUCTOR */
	
	PthreadSemaphore(int _val);
	virtual ~PthreadSemaphore();
	
	/* -- SEMAPHORE OPERATIONS */
	
	virtual int P();
	virtual int V();
};

/*--------------------------------------------------------------------------*/
/* CLASS   K e r n e l S e m a p h o r e  */
/*--------------------------------------------------------------------------*/

class KernelSemaphore : public Semaphore {
private:
	/* -- INTERNAL DATA STRUCTURES	*/
	
	int			max = 0;
	std::string	sema_name = "";
	sem_t* 		sema = SEM_FAILED;
	bool		unlink_on_destruction = true;
	bool		empty_constructor_used = false;
	
public:
	
	/* -- CONSTRUCTOR/DESTRUCTOR	*/
	
	KernelSemaphore(std::string _name, int _val);
	KernelSemaphore() { empty_constructor_used = true; }
	virtual ~KernelSemaphore();
	
	/* -- SEMAPHORE OPERATIONS		*/
	
	virtual int P();
	virtual int V();
	
	/* -- HELPER FUNCTIONS			*/
	
	bool		initial_construction_failed();
	std::string get_name();
	void		set_unlink_on_destruction(bool _unlink_afterwards);
};

/*
	This class can be used to write to standard output
	in a multithreaded environment. Its primary purpose
	is printing debug messages while multiple threads
	are in execution.
*/

class atomic_console_output {
	//PthreadSemaphore console_lock = PthreadSemaphore(1);
	KernelSemaphore console_lock = KernelSemaphore("/jhigginbotham_step2_console_lock", 1);
public:
	atomic_console_output() {}
	~atomic_console_output() {}
	void print(std::string s) {
		console_lock.P();
		std::cout << s;
		console_lock.V();
	}
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


#endif /* sync_lib_h_ */
