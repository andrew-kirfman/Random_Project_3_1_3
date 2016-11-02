/*
    File: client_E7.cpp

    Author: J. Higginbotham
    Department of Computer Science
    Texas A&M University
    Date  : 2016/05/20
    Last Modified : 2016/05/21


    Based on original code by: Dr. R. Bettati, PhD
    Department of Computer Science
    Texas A&M University
    Date  : 2013/01/31

    MP7 for Dr. //Tyagi's
    Ahmed's sections of CSCE 313.
 */

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*
    This assignment does not require any additional includes
 	besides un-commenting SafeBuffer.h, nor will you probably use
 	all the provided includes, but you are free to add any that
 	you find helpful.
*/
/*--------------------------------------------------------------------------*/

#include <cassert>
#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include "reqchannel.h"
#include <pthread.h>
#include <string>
#include <sstream>
#include <sys/time.h>
#include <assert.h>
#include <cmath>
#include <fstream>
#include <numeric>
#include <list>
#include <vector>

//#include "SafeBuffer.h"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/

struct PARAMS_REQUEST {
	/*
		This structure will be used as a "shipping container"
		for everything that will be USED in the request thread
		function but has to be INITIALIZED or DEFINED in main.
		These will be mostly pointers to data structures, such as
	 	the request buffer, or function parameters such as
	 	which person the request thread is for, as well as the
	 	number of requests to make.
	 */
};

struct PARAMS_WORKER {
    /*
	 	This structure has the same general purpose as PARAMS_REQUEST
	 	but is used in the worker thread function, and thus requires
	 	different fields and a different constructor based on what
	 	the worker thread function needs to use.
     */
};

/*
    The class below allows any thread in this program to use
    the variable "atomic_standard_output" down below to write output to the
    console without it getting jumbled together with the output
    from other threads.
    For example:
    thread A {
        atomic_standard_output.print("Hello ");
    }
    thread B {
        atomic_standard_output.print("World!\n");
    }
    The output could be:
    Hello World!
    or...
    World!
    Hello 
    ...but it will NOT be something like:
    HelWorllo d!
 */

class atomic_standard_output {
    /*
         Note that this class provides an example
         of the usage of mutexes, which are a crucial
         synchronization type. You will probably not
         be able to write the worker thread function
         without using these.
     */
    pthread_mutex_t console_lock;
public:
    atomic_standard_output() { pthread_mutex_init(&console_lock, NULL); }
    void print(std::string s){
        pthread_mutex_lock(&console_lock);
        std::cout << s << std::endl;
        pthread_mutex_unlock(&console_lock);
    }
};

atomic_standard_output threadsafe_standard_output;

/*--------------------------------------------------------------------------*/
/* HELPER FUNCTIONS */
/*--------------------------------------------------------------------------*/

std::string make_histogram(std::string name, std::vector<int> *data) {
    std::string results = "Frequency count for " + name + ":\n";
    for(int i = 0; i < data->size(); ++i) {
        results += std::to_string(i * 10) + "-" + std::to_string((i * 10) + 9) + ": " + std::to_string(data->at(i)) + "\n";
    }
    return results;
}

void* request_thread_function(void* arg) {
	/*
		Fill in the loop conditions and body.
		If you use PARAMS_REQUEST well, the body
		should require only a single line of code.
		The conditions should be somewhat intuitive.
	 */
	
	PARAMS_REQUEST rp = *(PARAMS_REQUEST*)arg;
	
	for(;;) {
		
	}
}

void* worker_thread_function(void* arg) {
    /*
        Fill in the loop body. Make sure
        it terminates only when, and not before,
        all the requests have been processed.
	 
        Each thread must have its own dedicated
        RequestChannel. Make sure that if you
        construct a RequestChannel (or any object)
        using "new" that you "delete" it properly,
        and that you send a "quit" request for every
        RequestChannel you construct regardless of
        whether you used "new" for it.
     */
    
    PARAMS_WORKER wp = *(PARAMS_WORKER*)arg;
	
    while(true) {
        
    }
}

/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/

int main(int argc, char * argv[]) {
    /*
        Note that the assignment calls for n = 10000.
        That is far too large for a single thread to accomplish quickly,
        but you will still have to test it for w = 1 once you finish
        filling in the worker thread function.
     */
    int n = 100; //default number of requests per "patient"
    int w = 1; //default number of worker threads
    int opt = 0;
    while ((opt = getopt(argc, argv, "n:w:h")) != -1) {
        switch (opt) {
            case 'n':
                n = atoi(optarg);
                break;
            case 'w':
                //This won't do a whole lot until you fill in the worker thread function
                w = atoi(optarg);
                break;
            case 'h':
            default:
                std::cout << "This program can be invoked with the following flags:" << std::endl;
                std::cout << "-n [int]: number of requests per patient" << std::endl;
                std::cout << "-w [int]: number of worker threads" << std::endl;
                std::cout << "-h: print this message and quit" << std::endl;
                std::cout << "Example: ./client_solution -n 10000 -w 120 -v 1" << std::endl;
                std::cout << "If a given flag is not used, or given an invalid value," << std::endl;
                std::cout << "a default value will be given to the corresponding variable." << std::endl;
                std::cout << "If an illegal option is detected, behavior is the same as using the -h flag." << std::endl;
                exit(0);
        }
    }
    
    int pid = fork();
    if(pid == 0){
        
        struct timeval start_time;
        struct timeval finish_time;
        int64_t start_usecs;
        int64_t finish_usecs;
        
        std::cout << "n == " << n << std::endl;
        std::cout << "w == " << w << std::endl;
        
        std::cout << "CLIENT STARTED:" << std::endl;
        std::cout << "Establishing control channel... " << std::flush;
        RequestChannel *chan = new RequestChannel("control", RequestChannel::CLIENT_SIDE);
        std::cout << "done." << std::endl;
        
        /*
            All worker threads will use these,
            but the current implementation puts
            them on the stack in main. You can change the code
		 	to use the heap if you'd like, or even a
		 	container other than std::vector. However,
		 	in the end you MUST use SafeBuffer instead
		 	of std::list for request_buffer.
         */
		
		//SafeBuffer request_buffer;
		std::list<std::string> request_buffer;
        std::vector<int> john_frequency_count(10, 0);
        std::vector<int> jane_frequency_count(10, 0);
        std::vector<int> joe_frequency_count(10, 0);
        
        /*-------------------------------------------*/
        /* START TIMER HERE */
        /*-------------------------------------------*/
		
/*--------------------------------------------------------------------------*/
/*  BEGIN CRITICAL SECTION  */
/*
	You will modify the program so that client.cpp
 	populates the request buffer using 3 request threads
 	in parallel instead of sequentially in a loop, and likewise
 	to process the request using w worker threads instead of
 	sequentially in a loop.
 
 	Note that in the finished product, as in this initial code,
 	all the requests will be pushed to the request buffer
 	BEFORE the worker threads begin processing them. This means
 	that you will have to ensure that all 3 request threads
 	have terminated before kicking off any worker threads.
 	In the next machine problem, we'll deal with the 
 	synchronization problems that arise from allowing the
 	request threads and worker threads to run in parallel
 	with each other. 
 
 	Just to be clear: for this machine problem, request
	threads will run concurrently with request threads, and worker
 	threads will run concurrently with worker threads, but request
	threads will NOT run concurrently with worker threads.
	
	Please remember to comment out the output statements
	while gathering data for your report. They throw off the timer.
*/
/*--------------------------------------------------------------------------*/
        
        std::cout << "Populating request buffer... ";
        fflush(NULL);
        for(int i = 0; i < n; ++i) {
            request_buffer.push_back("data John Smith");
            request_buffer.push_back("data Jane Smith");
            request_buffer.push_back("data Joe Smith");
        }
        std::cout << "done." << std::endl;
        
        std::cout << "Pushing quit requests... ";
        fflush(NULL);
        for(int i = 0; i < w; ++i) {
            request_buffer.push_back("quit");
        }
        std::cout << "done." << std::endl;
		
        std::string s = chan->send_request("newthread");
        RequestChannel *workerChannel = new RequestChannel(s, RequestChannel::CLIENT_SIDE);
        
        while(true) {
            std::string request = request_buffer.front();
            request_buffer.pop_front();
            std::string response = workerChannel->send_request(request);
            
            if(request == "data John Smith") {
                john_frequency_count.at(stoi(response) / 10) += 1;
            }
            else if(request == "data Jane Smith") {
                jane_frequency_count.at(stoi(response) / 10) += 1;
            }
            else if(request == "data Joe Smith") {
                joe_frequency_count.at(stoi(response) / 10) += 1;
            }
            else if(request == "quit") {
                delete workerChannel;
                break;
            }
        }
/*--------------------------------------------------------------------------*/
/*  END CRITICAL SECTION    */
/*--------------------------------------------------------------------------*/
        
        /*
            By the point at which you end the timer,
            all worker threads should have terminated.
            Note that the containers from earlier
            are still in scope, so threads can use them
            if they have a pointer to them.
         */
        
        /*-------------------------------------------*/
        /* END TIMER HERE   */
        /*-------------------------------------------*/
        
        /*
            You may want to eventually add file output
            to this section of the code to make it easier
            to assemble the timing data from different iterations
            of the program.
         */
        start_usecs = (start_time.tv_sec * 1e6) + start_time.tv_usec;
        finish_usecs = (finish_time.tv_sec * 1e6) + finish_time.tv_usec;
        std::cout << "Finished!" << std::endl;
        
        std::string john_results = make_histogram("John Smith", &john_frequency_count);
        std::string jane_results = make_histogram("Jane Smith Smith", &jane_frequency_count);
        std::string joe_results = make_histogram("Joe Smith", &joe_frequency_count);
        
        std::cout << "Results for n == " << n << ", w == " << w << std::endl;
        std::cout << "Time to completion: " << std::to_string(finish_usecs - start_usecs) << " usecs" << std::endl;
        std::cout << "John Smith total: " << accumulate(john_frequency_count.begin(), john_frequency_count.end(), 0) << std::endl;
        std::cout << john_results << std::endl;
        std::cout << "Jane Smith total: " << accumulate(jane_frequency_count.begin(), jane_frequency_count.end(), 0) << std::endl;
        std::cout << jane_results << std::endl;
        std::cout << "Joe Smith total: " << accumulate(joe_frequency_count.begin(), joe_frequency_count.end(), 0) << std::endl;
        std::cout << joe_results << std::endl;
        
        std::cout << "Sleeping..." << std::endl;
        usleep(10000);
        
        /*
            EVERY RequestChannel must send a "quit"
            request before program termination, and
            the destructor for each RequestChannel must be
            called somehow.
         */
        std::string finale = chan->send_request("quit");
        delete chan;
        std::cout << "Finale: " << finale << std::endl; //However, this line is optional.
    }
    else if(pid != 0) execl("dataserver", NULL);
}
