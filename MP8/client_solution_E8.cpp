/*
    File: client_solution_E8.cpp

    Author: J. Higginbotham
    Department of Computer Science
    Texas A&M University
    Date  : 2016/05/21

    Based on original code by: Dr. R. Bettati, PhD
    Department of Computer Science
    Texas A&M University
    Date  : 2013/01/31

    MP8 for Dr. //Tyagi's
    Ahmed's sections of CSCE 313.
 */

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

#define PATIENT_RESPONSE_BUF_SIZE       100    //Arbitrary reasonable-looking number
#define VERBOSITY_HELP_ONLY             0
#define VERBOSITY_DEFAULT               1
#define VERBOSITY_CHECK_CORRECTNESS     2
#define VERBOSITY_DEBUG                 3

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <cassert>
#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include "reqchannel.h"
#include "bounded_buffer.h"
#include <pthread.h>
#include <string>
#include <sys/time.h>
#include <assert.h>
#include <fstream>
#include <numeric>

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/

struct PARAMS_REQUEST {
    int n = 10;
    bounded_buffer* request_buffer;
    std::string request = "hello";
    PARAMS_REQUEST(int num_requests, bounded_buffer* req_buf, std::string req) {
        n = num_requests;
        request_buffer = req_buf;
        request = req;
    }
};

struct PARAMS_WORKER {
    RequestChannel *workerChannel = nullptr;
	bool failed = false;
    bounded_buffer *request_buffer;
    bounded_buffer *john_smith;
    bounded_buffer *jane_smith;
    bounded_buffer *joe_smith;
    PARAMS_WORKER(RequestChannel *wc, bounded_buffer *reqb, bounded_buffer *john, bounded_buffer *jane, bounded_buffer *joe) {
        workerChannel = wc;
        request_buffer = reqb;
        john_smith = john;
        jane_smith = jane;
        joe_smith = joe;
    }
};

struct PARAMS_STAT {
    int n = 10;
    bounded_buffer *patient_response_buf;
    std::vector<int> *patient_frequency_count;
    PARAMS_STAT(int num_requests, bounded_buffer *bbuf, std::vector<int> *freq) {
        n = num_requests;
        patient_response_buf = bbuf;
        patient_frequency_count = freq;
    }
};

/*
     This class can be used to write to standard output
     in a multithreaded environment. It's primary purpose
     is printing debug messages while multiple threads
     are in execution.
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
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

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
    PARAMS_REQUEST rtp = *(PARAMS_REQUEST*) arg;
    int n = rtp.n;
    bounded_buffer *request_buffer = rtp.request_buffer;
    std::string request = rtp.request;
    for(int i = 0; i < n; ++i) {
        request_buffer->push_back(request);
    }
}

void* worker_thread_function(void* arg) {
    PARAMS_WORKER wtp = *(PARAMS_WORKER*)arg;
    
    RequestChannel *workerChannel = wtp.workerChannel;
    bounded_buffer *request_buffer = wtp.request_buffer;
    bounded_buffer *john_smith = wtp.john_smith;
    bounded_buffer *jane_smith = wtp.jane_smith;
    bounded_buffer *joe_smith = wtp.joe_smith;
    
    while(true) {
        std::string request = request_buffer->retrieve_front();
        std::string response = workerChannel->send_request(request);
        if(request == "data John Smith") {
            john_smith->push_back(response);
        }
        else if(request == "data Jane Smith") {
            jane_smith->push_back(response);
        }
        else if(request == "data Joe Smith") {
            joe_smith->push_back(response);
        }
        else if(request == "quit") {
            delete workerChannel;
            break;
        }
    }
}

void* stat_thread_function(void* arg) {
    PARAMS_STAT stp = *(PARAMS_STAT*)arg;
    int n = stp.n;
    bounded_buffer *patient_response_buf = stp.patient_response_buf;
    std::vector<int> *patient_frequency_count = stp.patient_frequency_count;
    int count = 0;
    while(count < n) {
        patient_frequency_count->at(stoi(patient_response_buf->retrieve_front()) / 10) += 1; //Thread assumes bin size of 10
        ++count;
    }
}

/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/

int main(int argc, char * argv[]) {
    int n = 10; //default number of requests per "patient"
    int b = 50; //default size of request_buffer
    int w = 10; //default number of worker threads
    int v = VERBOSITY_DEFAULT;
    bool USE_ALTERNATE_FILE_OUTPUT = false;
    int opt = 0;
	int THREADS_FAILED = 0;
    while ((opt = getopt(argc, argv, "n:b:w:m:v:h")) != -1) {
        switch (opt) {
            case 'n':
                n = atoi(optarg);
                if(n < 0) n = 10;
                break;
            case 'b':
                b = atoi(optarg);
                if(b < 0) b = 50;
                break;
            case 'w':
                w = atoi(optarg);
                if(w < 0) w = 10;
                break;
            case 'm':
                if(atoi(optarg) == 2) USE_ALTERNATE_FILE_OUTPUT = true;
                break;
            case 'v':
                v = atoi(optarg);
                if(v < 0) v = VERBOSITY_HELP_ONLY;
                break;
            case 'h':
            default:
                if(v >= VERBOSITY_HELP_ONLY) std::cout << "This program can be invoked with the following flags:" << std::endl;
                if(v >= VERBOSITY_HELP_ONLY) std::cout << "-n [int]: number of requests per patient" << std::endl;
                if(v >= VERBOSITY_HELP_ONLY) std::cout << "-b [int]: size of request buffer" << std::endl;
                if(v >= VERBOSITY_HELP_ONLY) std::cout << "-w [int]: number of worker threads" << std::endl;
                if(v >= VERBOSITY_HELP_ONLY) std::cout << "-m 2: use output2.txt instead of output.txt for all file output" << std::endl;
                if(v >= VERBOSITY_HELP_ONLY) std::cout << "-v [0..3]: verbosity - controls how many output messages are displayed" << std::endl;
                if(v >= VERBOSITY_HELP_ONLY) std::cout << "Options:" << std::endl;
                if(v >= VERBOSITY_HELP_ONLY) std::cout << "\t0: only this help message will be displayed, if asked for" << std::endl;
                if(v >= VERBOSITY_HELP_ONLY) std::cout << "\t1: basic execution info and time will be displayed" << std::endl;
                if(v >= VERBOSITY_HELP_ONLY) std::cout << "\t2: final histogram totals (not individual bin values) will be displayed" << std::endl;
                if(v >= VERBOSITY_HELP_ONLY) std::cout << "\t3: debug: trace and full histograms will be displayed, in addition to all above info" << std::endl;
                if(v >= VERBOSITY_HELP_ONLY) std::cout << "-h: print this message and quit" << std::endl;
                if(v >= VERBOSITY_HELP_ONLY) std::cout << "Example: ./client_solution -n 10000 -b 50 -w 120 -m 2 -v 1" << std::endl;
                if(v >= VERBOSITY_HELP_ONLY) std::cout << "If a given flag is not used, or given an invalid value," << std::endl;
                if(v >= VERBOSITY_HELP_ONLY) std::cout << "a default value will be given to the corresponding variable." << std::endl;
                if(v >= VERBOSITY_HELP_ONLY) std::cout << "If an illegal option is detected, behavior is the same as using the -h flag." << std::endl;
                exit(0);
        }
    }
    
    int pid = fork();
    if(pid == 0){
        struct timeval start_time;
        struct timeval finish_time;
        int64_t start_usecs;
        int64_t finish_usecs;
        std::ofstream ofs;
        if(USE_ALTERNATE_FILE_OUTPUT) ofs.open("output2.txt", std::ios::out | std::ios::app);
        else ofs.open("output.txt", std::ios::out | std::ios::app);
        
        if(v >= VERBOSITY_DEFAULT) std::cout << "n == " << n << std::endl;
        if(v >= VERBOSITY_DEFAULT) std::cout << "b == " << b << std::endl;
        if(v >= VERBOSITY_DEFAULT) std::cout << "w == " << w << std::endl;
        
        if(v >= VERBOSITY_DEFAULT) std::cout << "CLIENT STARTED:" << std::endl;
        if(v >= VERBOSITY_DEFAULT) std::cout << "Establishing control channel... " << std::flush;
        RequestChannel *chan = new RequestChannel("control", RequestChannel::CLIENT_SIDE);
        if(v >= VERBOSITY_DEFAULT) std::cout << "done." << std::endl;
        
        bounded_buffer request_buffer = bounded_buffer(b);
        bounded_buffer john_smith = bounded_buffer(PATIENT_RESPONSE_BUF_SIZE);
        bounded_buffer jane_smith = bounded_buffer(PATIENT_RESPONSE_BUF_SIZE);
        bounded_buffer joe_smith = bounded_buffer(PATIENT_RESPONSE_BUF_SIZE);
        std::vector<int> john_frequency_count(10,0);
        std::vector<int> jane_frequency_count(10,0);
        std::vector<int> joe_frequency_count(10,0);
        
        PARAMS_REQUEST rtp0 = PARAMS_REQUEST(n, &request_buffer, "data John Smith");
        PARAMS_REQUEST rtp1 = PARAMS_REQUEST(n, &request_buffer, "data Jane Smith");
        PARAMS_REQUEST rtp2 = PARAMS_REQUEST(n, &request_buffer, "data Joe Smith");
        PARAMS_STAT stp0 = PARAMS_STAT(n, &john_smith, &john_frequency_count);
        PARAMS_STAT stp1 = PARAMS_STAT(n, &jane_smith, &jane_frequency_count);
        PARAMS_STAT stp2 = PARAMS_STAT(n, &joe_smith, &joe_frequency_count);
        
        std::vector<PARAMS_WORKER> wtps = std::vector<PARAMS_WORKER>(
        w, PARAMS_WORKER(nullptr, &request_buffer, &john_smith, &jane_smith, &joe_smith));
        
        pthread_t tid1, tid2, tid3, tid4, tid5, tid6;
		
		assert(gettimeofday(&start_time, 0) == 0);
		
        pthread_create(&tid4, NULL, stat_thread_function, &stp0);
        pthread_create(&tid5, NULL, stat_thread_function, &stp1);
        pthread_create(&tid6, NULL, stat_thread_function, &stp2);
        
        pthread_create(&tid1, NULL, request_thread_function, &rtp0);
        pthread_create(&tid2, NULL, request_thread_function, &rtp1);
        pthread_create(&tid3, NULL, request_thread_function, &rtp2);
        
        std::vector<pthread_t> wtids;
        for(int i = 0; i < w; ++i) {
			try {
				std::string s = chan->send_request("newthread");
				wtps[i].workerChannel = new RequestChannel(s, RequestChannel::CLIENT_SIDE);
				wtids.push_back(0);
				if((errno = pthread_create(&wtids[i], NULL, worker_thread_function, (void *) &wtps[i])) != 0) {
					threadsafe_standard_output.println("MAIN: pthread_create failure for " + wtps[i].workerChannel->name() + ": " + strerror(errno));
					delete wtps[i].workerChannel;
					wtps[i].failed = true;
					++THREADS_FAILED;
				}
			}
			catch (sync_lib_exception sle) {
				threadsafe_standard_output.println("MAIN: new client-side channel not created: " + std::string(sle.what()) + ": " + strerror(errno));
				wtps[i].failed = true;
				++THREADS_FAILED;
			}
			catch (std::bad_alloc ba) {
				threadsafe_standard_output.println("MAIN: caught std:bad_alloc in worker thread loop");
				throw;
			}
        }
        
        pthread_join(tid1, NULL);
        pthread_join(tid2, NULL);
        pthread_join(tid3, NULL);
        
        for(int i = 0; i < w - THREADS_FAILED; ++i) {
            request_buffer.push_back("quit");
        }
        
		for(int i = 0; i < w; ++i) {
			if(!wtps[i].failed && (errno = pthread_join(wtids[i], NULL)) != 0) {
				perror(std::string("MAIN: failed on pthread_join for [" + std::to_string(i) + "]").c_str());
			}
		}
		
        assert(gettimeofday(&finish_time, 0) == 0);
        start_usecs = (start_time.tv_sec * 1e6) + start_time.tv_usec;
        finish_usecs = (finish_time.tv_sec * 1e6) + finish_time.tv_usec;
        
        if(v >= VERBOSITY_DEBUG) std::cout << "All requests processed!" << std::endl;
        
        pthread_join(tid4, NULL);
        pthread_join(tid5, NULL);
        pthread_join(tid6, NULL);
        
        if(v >= VERBOSITY_DEBUG) std::cout << "Stat threads finished!" << std::endl;
        
        std::string john_results = make_histogram("John Smith", &john_frequency_count);
        std::string jane_results = make_histogram("Jane Smith Smith", &jane_frequency_count);
        std::string joe_results = make_histogram("Joe Smith", &joe_frequency_count);
        
        if(v >= VERBOSITY_CHECK_CORRECTNESS) std::cout << "Results for n == " << n << ", b == " << b << ", w == " << w << std::endl;
        if(v >= VERBOSITY_DEFAULT) std::cout << "Time to completion: " << std::to_string(finish_usecs - start_usecs) << " usecs" << std::endl;
        if(v >= VERBOSITY_CHECK_CORRECTNESS) std::cout << "John Smith total: " << accumulate(john_frequency_count.begin(), john_frequency_count.end(), 0) << std::endl;
        if(v >= VERBOSITY_DEBUG) std::cout << john_results << std::endl;
        if(v >= VERBOSITY_CHECK_CORRECTNESS) std::cout << "Jane Smith total: " << accumulate(jane_frequency_count.begin(), jane_frequency_count.end(), 0) << std::endl;
        if(v >= VERBOSITY_DEBUG) std::cout << jane_results << std::endl;
        if(v >= VERBOSITY_CHECK_CORRECTNESS) std::cout << "Joe Smith total: " << accumulate(joe_frequency_count.begin(), joe_frequency_count.end(), 0) << std::endl;
        if(v >= VERBOSITY_DEBUG) std::cout << joe_results << std::endl;
        if(v >= VERBOSITY_CHECK_CORRECTNESS) ofs << "Results for n == " << n << ", b == " << b << ", w == " << w << std::endl;
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
        std::string finale = chan->send_request("quit");
        if(v >= VERBOSITY_DEFAULT) std::cout << "Finale: " << finale << std::endl;
        delete chan;
    }
    else if(pid != 0) execl("dataserver", NULL);
}
