/*
    File: client_solution_E7.cpp

    Author: J. Higginbotham
    Department of Computer Science
    Texas A&M University
    Date  : 2016/05/20
    Last Modified : 2016/05/21



    Based on original code by: Dr. R. Bettati, PhD
    Department of Computer Science
    Texas A&M University
    Date  : 2013/01/31

    Solution code to MP7 for Dr. //Tyagi's
    Ahmed's sections of CSCE 313.
 */

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

#define VERBOSITY_HELP_ONLY             0
#define VERBOSITY_DEFAULT               1
#define VERBOSITY_CHECK_CORRECTNESS     2
#define VERBOSITY_DEBUG                 3
#define VERBOSITY_HYPER					4

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <cassert>
#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include "reqchannel.h"
#include <pthread.h>
#include <string>
#include <sys/time.h>
#include <assert.h>
#include <cmath>
#include <fstream>
#include <numeric>
#include <list>
#include <vector>
#include <exception>
#include <errno.h>

#include "SafeBuffer.h"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/

//DEATH TO GLOBAL VARIABLES!
struct PARAMS_REQUEST {
	SafeBuffer *request_buffer;
	std::string name;
	int n;
	int v = 0;
	PARAMS_REQUEST(SafeBuffer *req_buf, std::string _name, int _n, int verbosity) :
		request_buffer(req_buf), name(_name), n(_n), v(verbosity) {};
};

struct PARAMS_WORKER {
	bool failed = false;
    RequestChannel* workerChannel = nullptr;
	SafeBuffer *request_buffer = nullptr;
    std::vector<int> *john_frequency_count;
    std::vector<int> *jane_frequency_count;
    std::vector<int> *joe_frequency_count;
    pthread_mutex_t *john_m;
    pthread_mutex_t *jane_m;
    pthread_mutex_t *joe_m;
	int v = 0;
    PARAMS_WORKER(RequestChannel *wc, SafeBuffer *req_buf, std::vector<int> *john_fc,
				  std::vector<int> *jane_fc, std::vector<int> *joe_fc, pthread_mutex_t *_john_m,
				  pthread_mutex_t *_jane_m, pthread_mutex_t *_joe_m, int verbosity) :
	workerChannel(wc), request_buffer(req_buf), john_frequency_count(john_fc), jane_frequency_count(jane_fc),
	joe_frequency_count(joe_fc), john_m(_john_m), jane_m(_jane_m), joe_m(_joe_m), v(verbosity) {};
};

/*
    This class helps by allowing one to print
    debug messages from inside threads without them getting
    jumbled together, as well as by providing students
    with an example of mutex usage.
 */
class atomic_standard_output {
    pthread_mutex_t console_lock;
public:
    atomic_standard_output() { pthread_mutex_init(&console_lock, NULL); }
    ~atomic_standard_output() { pthread_mutex_destroy(&console_lock); }
    void println(std::string s){
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
	PARAMS_REQUEST rp = *(PARAMS_REQUEST*)arg;
	if(rp.v >= VERBOSITY_HYPER) {
		threadsafe_standard_output.println("Inside request thread function for " + rp.name + "...");
	}
	for(int i = 0; i < rp.n; ++i) {
		rp.request_buffer->push_back(std::string("data " + rp.name));
	}
	if(rp.v >= VERBOSITY_HYPER) {
		threadsafe_standard_output.println("Exiting request thread function for " + rp.name + "...");
	}
}

void* worker_thread_function(void* arg) {
    PARAMS_WORKER wp = *(PARAMS_WORKER*)arg;
    while(true) {
		if(wp.v >= VERBOSITY_HYPER) {
			threadsafe_standard_output.println("wthread[" + wp.workerChannel->name() + " retrieving request...");
		}
		std::string request = wp.request_buffer->retrieve_front();
		if(wp.v >= VERBOSITY_HYPER) {
			threadsafe_standard_output.println("wthread[" + wp.workerChannel->name() + " sending request " + request + "...");
		}
        std::string response = wp.workerChannel->send_request(request);
		if(wp.v >= VERBOSITY_HYPER) {
			threadsafe_standard_output.println("wthread[" + wp.workerChannel->name() + " received response " + response + " to request " + request + "...");
		}
		
        if(request == "data John Smith") {
            pthread_mutex_lock(wp.john_m);
            wp.john_frequency_count->at(stoi(response) / 10) += 1;
            pthread_mutex_unlock(wp.john_m);
        }
        else if(request == "data Jane Smith") {
            pthread_mutex_lock(wp.jane_m);
            wp.jane_frequency_count->at(stoi(response) / 10) += 1;
            pthread_mutex_unlock(wp.jane_m);
        }
        else if(request == "data Joe Smith") {
            pthread_mutex_lock(wp.joe_m);
            wp.joe_frequency_count->at(stoi(response) / 10) += 1;
            pthread_mutex_unlock(wp.joe_m);
        }
        else if(request == "quit") {
            delete wp.workerChannel;
            break;
        }
    }
}

/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/

int main(int argc, char * argv[]) {
    
    int n = 10000; //default number of requests per "patient"
    int w = 10; //default number of worker threads
    int v = VERBOSITY_DEFAULT;
    int opt = 0;
	int THREADS_FAILED = 0;
    
    while ((opt = getopt(argc, argv, "n:w:v:h")) != -1) {
        switch (opt) {
            case 'n':
                n = atoi(optarg);
                break;
            case 'w':
                w = atoi(optarg);
                break;
            case 'v':
                v = atoi(optarg);
                if(v < 0) v = VERBOSITY_HELP_ONLY;
                break;
            case 'h':
            default:
                if(v >= VERBOSITY_HELP_ONLY) std::cout << "This program can be invoked with the following flags:" << std::endl;
                if(v >= VERBOSITY_HELP_ONLY) std::cout << "-n [int]: number of requests per patient" << std::endl;
                if(v >= VERBOSITY_HELP_ONLY) std::cout << "-w [int]: number of worker threads" << std::endl;
                if(v >= VERBOSITY_HELP_ONLY) std::cout << "-v [0..3]: verbosity - controls how many output messages are displayed" << std::endl;
                if(v >= VERBOSITY_HELP_ONLY) std::cout << "Options:" << std::endl;
                if(v >= VERBOSITY_HELP_ONLY) std::cout << "\t0: only this help message will be displayed, if asked for" << std::endl;
                if(v >= VERBOSITY_HELP_ONLY) std::cout << "\t1: basic execution info and time will be displayed" << std::endl;
                if(v >= VERBOSITY_HELP_ONLY) std::cout << "\t2: final histogram totals (not individual bin values) will be displayed" << std::endl;
                if(v >= VERBOSITY_HELP_ONLY) std::cout << "\t3: debug: trace and full histograms will be displayed, in addition to all above info" << std::endl;
                if(v >= VERBOSITY_HELP_ONLY) std::cout << "-h: print this message and quit" << std::endl;
                if(v >= VERBOSITY_HELP_ONLY) std::cout << "Example: ./client_solution -n 10000 -b 50 -w 120 -v 1" << std::endl;
                if(v >= VERBOSITY_HELP_ONLY) std::cout << "If a given flag is not used, or given an invalid value," << std::endl;
                if(v >= VERBOSITY_HELP_ONLY) std::cout << "a default value will be given to the corresponding variable." << std::endl;
                if(v >= VERBOSITY_HELP_ONLY) std::cout << "If an illegal option is detected, behavior is the same as using the -h flag." << std::endl;
                exit(0);
        }
    }
    
    pid_t pid = fork();
    if(pid == 0){
        struct timeval start_time;
        struct timeval finish_time;
        int64_t start_usecs;
        int64_t finish_usecs;
        
		std::ofstream ofs;
		ofs.open("output.txt", std::ios::out | std::ios::app);
        
        if(v >= VERBOSITY_DEFAULT) std::cout << "n == " << n << std::endl;
        if(v >= VERBOSITY_DEFAULT) std::cout << "w == " << w << std::endl;
        
        if(v >= VERBOSITY_DEFAULT) std::cout << "CLIENT STARTED:" << std::endl;
        if(v >= VERBOSITY_DEFAULT) std::cout << "Establishing control channel... " << std::flush;
        RequestChannel *chan = new RequestChannel("control", RequestChannel::CLIENT_SIDE);
        if(v >= VERBOSITY_DEFAULT) std::cout << "done." << std::endl;
        
        /*
            All worker and request threads will use,
            these but they are kept on the stack in
            main and NOT in global scope.
         */

		SafeBuffer request_buffer;
        std::vector<int> john_frequency_count(10, 0);
        std::vector<int> jane_frequency_count(10, 0);
        std::vector<int> joe_frequency_count(10, 0);
        pthread_mutex_t john_m, jane_m, joe_m;
        pthread_mutex_init(&john_m, NULL);
        pthread_mutex_init(&jane_m, NULL);
        pthread_mutex_init(&joe_m, NULL);

		PARAMS_REQUEST john_req_params(&request_buffer, "John Smith", n, v);
		PARAMS_REQUEST jane_req_params(&request_buffer, "Jane Smith", n, v);
		PARAMS_REQUEST joe_req_params(&request_buffer, "Joe Smith", n, v);
		
        /*
            Failing to initialize these properly can
            leads to some pretty nasty bugs.
         */
        std::vector<PARAMS_WORKER> params(w, PARAMS_WORKER(nullptr, &request_buffer, &john_frequency_count,
                                             &jane_frequency_count, &joe_frequency_count,
                                             &john_m, &jane_m, &joe_m, v));
        
        /*
            Request buffer is populated sequentially,
            as opposed to using request threads as in
            the original MP4. "quit" requests are pushed
            in advance as well, since there's no bounded
            buffer to manage the synchronization.
         */
		
		/*
			TANZIR-REQUESTED-REVISION-RELATED QUESTION:
		 		Should the request thread execution be timed?
		 		If so, how? Most importantly, should it be combined
		 		with the worker thread timing as it is now?
		 */
        assert(gettimeofday(&start_time, 0) == 0);

        if(v >= VERBOSITY_DEBUG) {
            std::cout << "Populating request buffer; starting request threads... ";
            fflush(NULL);
        }
		
		pthread_t john_req_tid, jane_req_tid, joe_req_tid;
		
		pthread_create(&john_req_tid, NULL, request_thread_function, (void*) &john_req_params);
		pthread_create(&jane_req_tid, NULL, request_thread_function, (void*) &jane_req_params);
		pthread_create(&joe_req_tid, NULL, request_thread_function, (void*) &joe_req_params);
		
		pthread_join(john_req_tid, NULL);
		pthread_join(jane_req_tid, NULL);
		pthread_join(joe_req_tid, NULL);
		
        if(v >= VERBOSITY_DEBUG) std::cout << "done." << std::endl;
        
        if(v >= VERBOSITY_DEBUG) {
            std::cout << "Pushing quit requests pre-emptively... ";
            fflush(NULL);
        }
        
        for(int i = 0; i < w; ++i) {
            request_buffer.push_back("quit");
        }
        if(v >= VERBOSITY_DEBUG) std::cout << "done." << std::endl;
        
        std::vector<pthread_t> wtids;
        for(int i = 0; i < w; ++i) {
			try {
				std::string s = chan->send_request("newthread");
				params[i].workerChannel = new RequestChannel(s, RequestChannel::CLIENT_SIDE);
				wtids.push_back(0);
				if((errno = pthread_create(&wtids[i], NULL, worker_thread_function, (void *) &params[i])) != 0) {
					threadsafe_standard_output.println("MAIN: pthread_create failure for " + params[i].workerChannel->name() + ": " + strerror(errno));
					delete params[i].workerChannel;
					params[i].failed = true;
					++THREADS_FAILED;
				}
			}
			catch (sync_lib_exception sle) {
				threadsafe_standard_output.println("MAIN: new client-side channel not created: " + std::string(sle.what()) + ": " + strerror(errno));
				params[i].failed = true;
				++THREADS_FAILED;
			}
			catch (std::bad_alloc ba) {
				threadsafe_standard_output.println("MAIN: caught std:bad_alloc in worker thread loop");
				throw;
			}
        }
        
        for(int i = 0; i < w; ++i) {
			if(!params[i].failed && (errno = pthread_join(wtids[i], NULL)) != 0) {
				perror(std::string("MAIN: failed on pthread_join for [" + std::to_string(i) + "]").c_str());
			}
        }
        
        assert(gettimeofday(&finish_time, 0) == 0);
        
        /*
            All the data is assembled and presented,
            as well as output to a file if necessary.
         */
        start_usecs = (start_time.tv_sec * 1e6) + start_time.tv_usec;
        finish_usecs = (finish_time.tv_sec * 1e6) + finish_time.tv_usec;
        if(v >= VERBOSITY_DEBUG) std::cout << "Finished!" << std::endl;
        
        std::string john_results = make_histogram("John Smith", &john_frequency_count);
        std::string jane_results = make_histogram("Jane Smith Smith", &jane_frequency_count);
        std::string joe_results = make_histogram("Joe Smith", &joe_frequency_count);
        
        if(v >= VERBOSITY_CHECK_CORRECTNESS) std::cout << "Results for n == " << n << ", w == " << w << ", v == " << v << std::endl;
        if(v >= VERBOSITY_DEFAULT) std::cout << "Time to completion: " << std::to_string(finish_usecs - start_usecs) << " usecs" << std::endl;
        if(v >= VERBOSITY_CHECK_CORRECTNESS) std::cout << "John Smith total: " << accumulate(john_frequency_count.begin(), john_frequency_count.end(), 0) << std::endl;
        if(v >= VERBOSITY_DEBUG) std::cout << john_results << std::endl;
        if(v >= VERBOSITY_CHECK_CORRECTNESS) std::cout << "Jane Smith total: " << accumulate(jane_frequency_count.begin(), jane_frequency_count.end(), 0) << std::endl;
        if(v >= VERBOSITY_DEBUG) std::cout << jane_results << std::endl;
        if(v >= VERBOSITY_CHECK_CORRECTNESS) std::cout << "Joe Smith total: " << accumulate(joe_frequency_count.begin(), joe_frequency_count.end(), 0) << std::endl;
        if(v >= VERBOSITY_DEBUG) std::cout << joe_results << std::endl;
        if(v >= VERBOSITY_CHECK_CORRECTNESS) ofs << "Results for n == " << n << ", w == " << w << ", v == " << v << std::endl;
        if(v >= VERBOSITY_DEFAULT) ofs << "Time to completion: " << std::to_string(finish_usecs - start_usecs) << " usecs" << std::endl;
        if(v >= VERBOSITY_CHECK_CORRECTNESS) ofs << "John Smith total: " << accumulate(john_frequency_count.begin(), john_frequency_count.end(), 0) << std::endl;
        if(v >= VERBOSITY_DEBUG) ofs << john_results << std::endl;
        if(v >= VERBOSITY_CHECK_CORRECTNESS) ofs << "Jane Smith total: " << accumulate(jane_frequency_count.begin(), jane_frequency_count.end(), 0) << std::endl;
        if(v >= VERBOSITY_DEBUG) ofs << jane_results << std::endl;
        if(v >= VERBOSITY_CHECK_CORRECTNESS) ofs << "Joe Smith total: " << accumulate(joe_frequency_count.begin(), joe_frequency_count.end(), 0) << std::endl;
        if(v >= VERBOSITY_DEBUG) ofs << joe_results << std::endl;
        ofs.close();
        
        if(v >= VERBOSITY_DEFAULT) std::cout << "Sleeping..." << std::endl;
        usleep(10000);
        /*
            EVERY RequestChannel must send a "quit"
            request before program termination, and
            the destructor for each RequestChannel
		 	must be called somehow.
         */
        std::string finale = chan->send_request("quit");
        if(v >= VERBOSITY_DEFAULT) std::cout << "Finale: " << finale << std::endl;
        delete chan;
    }
    else if(pid != 0) execl("dataserver", NULL);
}
