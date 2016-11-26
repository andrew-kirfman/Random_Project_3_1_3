/*
	File: RequestChannelTest.cpp

	Author: J. Higginbotham
	Department of Computer Science
	Texas A&M University
	Date  : 2016/08/10

	Based on original code by: Dr. R. Bettati, PhD
	Department of Computer Science
	Texas A&M University
	Date  : 2013/01/31

	MP9 for Dr. //Tyagi's
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

#include "sync_lib.h"
#include "Dataserver.h"
#include "NamedPipeRequestChannel.h"
#include "UnnamedPipeRequestChannel.h"
#include "MessageQueueRequestChannel.h"
#include "SharedMemoryRequestChannel.h"
#include "SignalRequestChannel.h"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/

struct request_thread_params {
	int n = 10;
	int v = VERBOSITY_DEFAULT;
	BoundedBuffer * request_buffer;
	std::string request = "hello";
	request_thread_params(int num_requests, int verbosity, BoundedBuffer * req_buf, std::string req) {
		n = num_requests;
		v = verbosity;
		request_buffer = req_buf;
		request = req;
	}
};

struct worker_thread_params {
	pthread_t id;
	int v = VERBOSITY_DEFAULT;
	RequestChannel * workerChannel = nullptr;
	BoundedBuffer * request_buffer;
	BoundedBuffer * john_smith;
	BoundedBuffer * jane_smith;
	BoundedBuffer * joe_smith;
	bool failed = false;
	worker_thread_params(int verbosity, RequestChannel * wc, BoundedBuffer * reqb, BoundedBuffer * john, BoundedBuffer * jane, BoundedBuffer * joe) {
		v = verbosity;
		workerChannel = wc;
		request_buffer = reqb;
		john_smith = john;
		jane_smith = jane;
		joe_smith = joe;
	}
};

struct stat_thread_params {
	int n = 10;
	int v = VERBOSITY_DEFAULT;
	BoundedBuffer * patient_response_buf;
		std::vector<int> * patient_frequency_count;
		pthread_mutex_t *patient_m;
		stat_thread_params(int num_requests, int verbosity, BoundedBuffer * bbuf, std::vector<int> *freq, pthread_mutex_t * _patient_m) {
		n = num_requests;
		v = verbosity;
		patient_response_buf = bbuf;
		patient_frequency_count = freq;
			patient_m = _patient_m;
	}
};

struct display_histogram_params {
		int v = VERBOSITY_DEFAULT;
		std::vector<int> * john_frequency_count;
		std::vector<int> * jane_frequency_count;
		std::vector<int> * joe_frequency_count;
		pthread_mutex_t * john_m;
		pthread_mutex_t * jane_m;
		pthread_mutex_t * joe_m;
		display_histogram_params(int verbosity, std::vector<int> * john_fc, std::vector<int> * jane_fc,
		        std::vector<int> * joe_fc, pthread_mutex_t * _john_m, pthread_mutex_t * _jane_m,
		        pthread_mutex_t * _joe_m) :
				v(verbosity), john_frequency_count(john_fc), jane_frequency_count(jane_fc),
				        joe_frequency_count(joe_fc), john_m(_john_m), jane_m(_jane_m), joe_m(_joe_m) {
		}

};

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

const int ERR_MODE_UNKNOWN 		= -1;
const int NAMED_PIPE 			= 0;
const int UNNAMED_PIPE			= 1;
const int MESSAGE_QUEUE 		= 2;
const int SHARED_MEMORY 		= 3;
const int SIGNAL 				= 4;

const int PTHREAD_BUFFER		= 0;
const int MESSAGE_QUEUE_BUFFER	= 1;
const int SHARED_MEMORY_BUFFER	= 2;

const std::string control_channel_name	= "control";
const std::string request_buffer_name = "request";
const std::string john_smith_buffer_name = "john_smith";
const std::string jane_smith_buffer_name = "jane_smith";
const std::string joe_smith_buffer_name = "joe_smith";

const unsigned int SIGRT_RANGE_LENGTH = SIGRTMAX - SIGRTMIN + 1;

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

std::string make_histogram_table(std::string name1, std::string name2,
        std::string name3, std::vector<int> *data1, std::vector<int> *data2,
        std::vector<int> *data3) {
	std::stringstream tablebuilder;
	tablebuilder << std::setw(25) << std::right << name1;
	tablebuilder << std::setw(15) << std::right << name2;
	tablebuilder << std::setw(15) << std::right << name3 << std::endl;
	for (int i = 0; i < data1->size(); ++i) {
		tablebuilder << std::setw(10) << std::left
		             << std::string(
		                     std::to_string(i * 10) + "-"
		                             + std::to_string((i * 10) + 9));
		tablebuilder << std::setw(15) << std::right
		        << std::to_string(data1->at(i));
		tablebuilder << std::setw(15) << std::right
		        << std::to_string(data2->at(i));
		tablebuilder << std::setw(15) << std::right
		        << std::to_string(data3->at(i)) << std::endl;
	}
	tablebuilder << std::setw(10) << std::left << "Total";
	tablebuilder << std::setw(15) << std::right
	             << accumulate(data1->begin(), data1->end(), 0);
	tablebuilder << std::setw(15) << std::right
	             << accumulate(data2->begin(), data2->end(), 0);
	tablebuilder << std::setw(15) << std::right
	             << accumulate(data3->begin(), data3->end(), 0)
	             << std::endl;

	return tablebuilder.str();
}

void display_histograms(int sig, siginfo_t * si, void * unused) {
	display_histogram_params dhp = *(display_histogram_params*) si->si_value.sival_ptr;

	pthread_mutex_lock(dhp.john_m);
	pthread_mutex_lock(dhp.jane_m);
	pthread_mutex_lock(dhp.joe_m);
	std::string histogram_table = make_histogram_table("John Smith",
	        "Jane Smith", "Joe Smith", dhp.john_frequency_count,
	        dhp.jane_frequency_count, dhp.joe_frequency_count);
	pthread_mutex_unlock(dhp.john_m);
	pthread_mutex_unlock(dhp.jane_m);
	pthread_mutex_unlock(dhp.joe_m);

	system("clear");
	threadsafe_console_output.println(histogram_table);
}


void* request_thread_function(void* arg) {
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

	request_thread_params rtp = *(request_thread_params*) arg;

	for(int i = 0; i < rtp.n; ++i) {
		if(rtp.v == VERBOSITY_HYPER) threadsafe_console_output.println("REQUEST: pushing request [" + rtp.request + "] to request buffer");
		rtp.request_buffer->push_back(rtp.request);
	}

	return nullptr;
}

void* worker_thread_function(void* arg) {



	worker_thread_params wtp = *(worker_thread_params*)arg;

	if(wtp.v == VERBOSITY_HYPER) {
		threadsafe_console_output.println("WORKER:" + wtp.workerChannel->name() + ": entered thread function");
	}

	while(true) {
		std::string request = "";
		request = wtp.request_buffer->retrieve_front();
		if(wtp.v == VERBOSITY_HYPER) {
			threadsafe_console_output.println("WORKER:" + wtp.workerChannel->name() + ": sending request [" + request + "]");
		}
		std::string response = wtp.workerChannel->send_request(request);
		if(request.substr(0,15) == "data John Smith") {
			if(wtp.v == VERBOSITY_HYPER) {
				threadsafe_console_output.println("WORKER:" + wtp.workerChannel->name() + ": pushing [" + response + "] to john smith response buffer");
			}
			wtp.john_smith->push_back(response);
		}
		else if(request.substr(0,15) == "data Jane Smith") {
			if(wtp.v == VERBOSITY_HYPER) {
				threadsafe_console_output.println("WORKER:" + wtp.workerChannel->name() + ": pushing [" + response + "] to jane smith response buffer");
			}
			wtp.jane_smith->push_back(response);
		}
		else if(request.substr(0,14) == "data Joe Smith") {
			if(wtp.v == VERBOSITY_HYPER) {
				threadsafe_console_output.println("WORKER:" + wtp.workerChannel->name() + ": pushing [" + response + "] to joe smith response buffer");
			}
			wtp.joe_smith->push_back(response);
		}
		else if(request.substr(0,4) == "quit") {
			if(wtp.v == VERBOSITY_HYPER) {
				threadsafe_console_output.println("WORKER:" + wtp.workerChannel->name() + ": destructing");
			}

			delete wtp.workerChannel;

			break;
		}
	}

	return nullptr;
}

void* stat_thread_function(void* arg) {
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

	stat_thread_params stp = *(stat_thread_params*)arg;

	int count = 0;
	while(count < stp.n) {
		std::string patient_response(stp.patient_response_buf->retrieve_front());
		if(stp.v == VERBOSITY_HYPER) {
			threadsafe_console_output.println("STAT: retrieved [" + patient_response + "] from response buffer");
		}
		pthread_mutex_lock(stp.patient_m);
		stp.patient_frequency_count->at(stoi(patient_response) / 10) += 1;
		pthread_mutex_unlock(stp.patient_m);
		++count;
	}

	return nullptr;
}

/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/

int main(int argc, char * argv[]) {
	int n = 10; //default number of requests per "patient"
	int s = 10; //default size of request_buffer.
	//Assigned value of 10 because that is the default msg_max value on Linux.
	//Trying to create message queues with more messages than msg_max results in mq_open failing with EINVAL.
	int w = 10; //default number of worker threads
	int v = VERBOSITY_DEFAULT;
	int r = NAMED_PIPE;
	int b = PTHREAD_BUFFER;
	int m = 15; //max request size for MessageQueueBoundedBuffer and SharedMemoryBoundedBuffer to use
	int THREADS_NOT_CREATED = 0;
	bool CHANNEL_TYPE_IMPLEMENTED = true;
	bool SEPARATE_WINDOWS = false;
	bool REAL_TIME_HIST_DISP = false;
	bool USE_ALTERNATE_FILE_OUTPUT = false;
	std::string channel_type = "";
	int opt = 0;
	while ((opt = getopt(argc, argv, "n:s:w:ov:m:r:b:xdh")) != -1) {
		switch (opt) {
			case 'n':
				n = atoi(optarg);
				if(n <= 0) n = 10;
				break;
			case 's':
				s = atoi(optarg);
				if(s <= 0) s = 50;
				break;
			case 'w':
				w = atoi(optarg);
				if(w <= 0) w = 10;
				break;
			case 'o':
				USE_ALTERNATE_FILE_OUTPUT = true;
				break;
			case 'v':
				v = std::max(atoi(optarg), VERBOSITY_HELP_ONLY);
				break;
			case 'r':
				r = atoi(optarg);
				if(r == NAMED_PIPE) {
					channel_type = "NAMED_PIPE";
				}
				else if(r == UNNAMED_PIPE) {
					channel_type = "UNNAMED_PIPE";
				}
				else if(r == MESSAGE_QUEUE) {
					channel_type = "MESSAGE_QUEUE";
				}
				else if(r == SHARED_MEMORY) {
					channel_type = "SHARED_MEMORY";
				}
				else if(r == SIGNAL) {
					channel_type = "SIGNAL";
				}
				else {
					CHANNEL_TYPE_IMPLEMENTED = false;
				}
				break;
			case 'b':
				b = atoi(optarg);
				if(b != PTHREAD_BUFFER &&
				   b != MESSAGE_QUEUE_BUFFER &&
				   b != SHARED_MEMORY_BUFFER) {
					b = PTHREAD_BUFFER;
				}
				break;
			case 'm':
				m = std::max(atoi(optarg), 15);
				break;
			case 'x':
				SEPARATE_WINDOWS = true;
				break;
			case 'd':
				REAL_TIME_HIST_DISP = true;
				break;
			case 'h':
			default:
				if(v >= VERBOSITY_HELP_ONLY) threadsafe_console_output.println("This program can be invoked with the following flags:");
				if(v >= VERBOSITY_HELP_ONLY) threadsafe_console_output.println("-n [int]: number of requests per patient");
				if(v >= VERBOSITY_HELP_ONLY) threadsafe_console_output.println("-s [int]: size of request buffer and patient response buffers");
				if(v >= VERBOSITY_HELP_ONLY) threadsafe_console_output.println("\tNOTE: for b == 2, max value is 10 on Raspbian");
				if(v >= VERBOSITY_HELP_ONLY) threadsafe_console_output.println("-w [int]: number of worker threads");
				if(v >= VERBOSITY_HELP_ONLY) threadsafe_console_output.println("\tNOTE: for r == 4 (SIGNAL), limited by the number of real-time signal values available to an application on a given operating system (for this OS: " + std::to_string(SIGRT_RANGE_LENGTH - 1) + ")");
				if(v >= VERBOSITY_HELP_ONLY) threadsafe_console_output.println("-o: use output2.txt instead of output.txt for all file output");
				if(v >= VERBOSITY_HELP_ONLY) threadsafe_console_output.println("-v [0..3]: verbosity - controls how many output messages are displayed");
				if(v >= VERBOSITY_HELP_ONLY) threadsafe_console_output.println("\tOptions:");
				if(v >= VERBOSITY_HELP_ONLY) threadsafe_console_output.println("\t\t0: only this help message will be displayed, if asked for");
				if(v >= VERBOSITY_HELP_ONLY) threadsafe_console_output.println("\t\t1: basic execution info and time will be displayed");
				if(v >= VERBOSITY_HELP_ONLY) threadsafe_console_output.println("\t\t2: final histogram totals (not individual bin values) will be displayed");
				if(v >= VERBOSITY_HELP_ONLY) threadsafe_console_output.println("\t\t3: debug: trace and full histograms will be displayed, in addition to all above info");
				if(v >= VERBOSITY_HELP_ONLY) threadsafe_console_output.println("\t\t4: hyper: thread creation and execution also displayed (WARNING: prepare for A LOT of scrolling)");
				if(v >= VERBOSITY_HELP_ONLY) threadsafe_console_output.println("-r [0..4]: specify the IPC mechanism for RequestChannel to use");
				if(v >= VERBOSITY_HELP_ONLY) threadsafe_console_output.println("\tOptions:");
				if(v >= VERBOSITY_HELP_ONLY) threadsafe_console_output.println("\t\t0: named pipes, a.k.a. FIFOs (default)");
				if(v >= VERBOSITY_HELP_ONLY) threadsafe_console_output.println("\t\t1: unnamed pipes, via UNIX domain sockets");
				if(v >= VERBOSITY_HELP_ONLY) threadsafe_console_output.println("\t\t2: POSIX message queues");
				if(v >= VERBOSITY_HELP_ONLY) threadsafe_console_output.println("\t\t3: shared memory");
				if(v >= VERBOSITY_HELP_ONLY) threadsafe_console_output.println("\t\t4: signals (only works on operating systems that implement real-time signals)");
				if(v >= VERBOSITY_HELP_ONLY) threadsafe_console_output.println("-b [0..1]: specify the bounded buffer implementation to use");
				if(v >= VERBOSITY_HELP_ONLY) threadsafe_console_output.println("\tOptions:");
				if(v >= VERBOSITY_HELP_ONLY) threadsafe_console_output.println("\t\t0: std::queue and pthread_* objects");
				if(v >= VERBOSITY_HELP_ONLY) threadsafe_console_output.println("\t\t1: POSIX message queues");
				if(v >= VERBOSITY_HELP_ONLY) threadsafe_console_output.println("\t\t2: shared memory and POSIX semaphores");
				if(v >= VERBOSITY_HELP_ONLY) threadsafe_console_output.println("-m [int]: max size of bounded buffer slots (only for -b 1 and -b 2)");
				if(v >= VERBOSITY_HELP_ONLY) threadsafe_console_output.println("\tNOTE: lower bound of 15, which is the length of the longest request for this program, but larger values may be used for testing");
				if(v >= VERBOSITY_HELP_ONLY) threadsafe_console_output.println("-x: do not call fork, but wait for the dataserver to be invoked separately");
				if (v >= VERBOSITY_HELP_ONLY)
					threadsafe_console_output.println("-d: display histograms in real time using a SIGALRM handler (if implemented)");
				if(v >= VERBOSITY_HELP_ONLY) threadsafe_console_output.println("-h: print this message and quit");
				if(v >= VERBOSITY_HELP_ONLY) threadsafe_console_output.println("Example: ./client_solution -n 10000 -b 1 -s 50 -w 120 -m 2 -v 1 -r 3 -m 2 -x");
				if(v >= VERBOSITY_HELP_ONLY) threadsafe_console_output.println("If a given flag is not used, or given an invalid value,");
				if(v >= VERBOSITY_HELP_ONLY) threadsafe_console_output.println("a default value will be given to the corresponding variable.");
				if(v >= VERBOSITY_HELP_ONLY) threadsafe_console_output.println("If an illegal option is detected, behavior is the same as using the -h flag.");
				exit(EXIT_SUCCESS);
		}
	}

	if(!CHANNEL_TYPE_IMPLEMENTED) {
		threadsafe_console_output.println("Apologies, but there is no IPC mechanism corresponding to mode " + std::to_string(r) + " for this program.");
		threadsafe_console_output.println("Invoke program with -h to see full help message.");
		exit(EXIT_FAILURE);
	}
	else if(r == ERR_MODE_UNKNOWN) {
		threadsafe_console_output.println("Mode " + std::to_string(r) + " is not supported and probably will never be.");
		threadsafe_console_output.println("Invoke the program with -h for a list of valid mode numbers.");
		exit(EXIT_FAILURE);
	}

	pid_t pid;
	if(!SEPARATE_WINDOWS) pid = fork();
	if((!SEPARATE_WINDOWS && pid > 0) ||
	   SEPARATE_WINDOWS){
		struct timeval start_time;
		struct timeval finish_time;
		int64_t start_usecs;
		int64_t finish_usecs;
		RequestChannel * control_channel = nullptr;

		/*
			Using more SignalRequestChannels than can
		 	be immediately accommodated by the available
		 	signal numbers introduces some complicated
		 	synchronization problems.
		 */

		if(r == SIGNAL) w = std::min(w, NUM_SYSTEM_RT_SIGVALUES - 1);

		if(v >= VERBOSITY_DEFAULT) threadsafe_console_output.println("n == " + std::to_string(n));
		if(v >= VERBOSITY_DEFAULT) threadsafe_console_output.println("s == " + std::to_string(s));
		if(v >= VERBOSITY_DEFAULT) threadsafe_console_output.println("w == " + std::to_string(w));
		if(v >= VERBOSITY_DEFAULT) threadsafe_console_output.println("v == " + std::to_string(v));
		if(v >= VERBOSITY_DEFAULT) threadsafe_console_output.println("r == " + std::to_string(r));
		if(v >= VERBOSITY_DEFAULT) threadsafe_console_output.println("b == " + std::to_string(b));
		if(v >= VERBOSITY_DEFAULT) threadsafe_console_output.println("m == " + std::to_string(m));

		if(v >= VERBOSITY_DEFAULT) threadsafe_console_output.println("MAIN: Establishing " + channel_type + " control channel... ");

		try {
			if(r == NAMED_PIPE) {
				control_channel = (RequestChannel*) new NamedPipeRequestChannel(control_channel_name, RequestChannel::CLIENT_SIDE);
			}
			else if(r == UNNAMED_PIPE) {
				control_channel = (RequestChannel*) new UnnamedPipeRequestChannel(control_channel_name, RequestChannel::CLIENT_SIDE);
			}
			else if(r == MESSAGE_QUEUE) {
				control_channel = (RequestChannel*) new MessageQueueRequestChannel(control_channel_name, RequestChannel::CLIENT_SIDE);
			}
			else if(r == SHARED_MEMORY) {
				control_channel = (RequestChannel*) new SharedMemoryRequestChannel(control_channel_name, RequestChannel::CLIENT_SIDE);
			}
			else if(r == SIGNAL) {
				control_channel = (RequestChannel*) new SignalRequestChannel(control_channel_name, RequestChannel::CLIENT_SIDE);
			}
		}
		catch (sync_lib_exception sle) {
			threadsafe_console_output.perror("MAIN: control channel not created: " + std::string(sle.what()));
			throw;
		}


		if(v >= VERBOSITY_DEFAULT) threadsafe_console_output.println("MAIN: Finished establishing " + channel_type + " control channel.");

		BoundedBuffer * request_buffer = nullptr;
		BoundedBuffer * john_smith = nullptr;
		BoundedBuffer * jane_smith = nullptr;
		BoundedBuffer * joe_smith = nullptr;

		try {
			if(b == PTHREAD_BUFFER) {
				if(v >= VERBOSITY_DEBUG) threadsafe_console_output.println("MAIN: created PTHREAD BOUNDED BUFFERS for use by threads");
				request_buffer = (BoundedBuffer*) new ThreadSharedBoundedBuffer(s);
				john_smith = (BoundedBuffer*) new ThreadSharedBoundedBuffer(s);
				jane_smith = (BoundedBuffer*) new ThreadSharedBoundedBuffer(s);
				joe_smith = (BoundedBuffer*) new ThreadSharedBoundedBuffer(s);
			}
			else if(b == MESSAGE_QUEUE_BUFFER) {
				if(v >= VERBOSITY_DEBUG) threadsafe_console_output.println("MAIN: created MESSAGE QUEUE BOUNDED BUFFERS for use by threads");
				request_buffer = (BoundedBuffer*) new MessageQueueBoundedBuffer(request_buffer_name, s, m);
				john_smith = (BoundedBuffer*) new MessageQueueBoundedBuffer(john_smith_buffer_name, s, m);
				jane_smith = (BoundedBuffer*) new MessageQueueBoundedBuffer(jane_smith_buffer_name, s, m);
				joe_smith = (BoundedBuffer*) new MessageQueueBoundedBuffer(joe_smith_buffer_name, s, m);
			}
			else if(b == SHARED_MEMORY_BUFFER) {
				if(v >= VERBOSITY_DEBUG) threadsafe_console_output.println("MAIN: created SHARED MEMORY BOUNDED BUFFERS for use by threads");
				request_buffer = (BoundedBuffer*) new SharedMemoryBoundedBuffer(request_buffer_name, s, m);
				john_smith = (BoundedBuffer*) new SharedMemoryBoundedBuffer(john_smith_buffer_name, s, m);
				jane_smith = (BoundedBuffer*) new SharedMemoryBoundedBuffer(jane_smith_buffer_name, s, m);
				joe_smith = (BoundedBuffer*) new SharedMemoryBoundedBuffer(joe_smith_buffer_name, s, m);
			}
		}
		catch (sync_lib_exception sle) {
			threadsafe_console_output.perror("MAIN: bounded buffers not created: " + std::string(sle.what()));
			control_channel->send_request("quit");
			delete control_channel;
			if(request_buffer != nullptr) delete request_buffer;
			if(john_smith != nullptr) delete john_smith;
			if(jane_smith != nullptr) delete jane_smith;
			if(joe_smith != nullptr) delete joe_smith;
			throw sync_lib_exception("MAIN: bounded buffers not created, no point in continuing.");
		}

		std::vector<int> john_frequency_count(10,0);
		std::vector<int> jane_frequency_count(10,0);
		std::vector<int> joe_frequency_count(10,0);
		pthread_mutex_t john_m, jane_m, joe_m;
		pthread_mutex_init(&john_m, NULL);
		pthread_mutex_init(&jane_m, NULL);
		pthread_mutex_init(&joe_m, NULL);

		display_histogram_params dhp(v, &john_frequency_count, &jane_frequency_count, &joe_frequency_count, &john_m, &jane_m, &joe_m);
		struct sigaction sa;
		sigevent sevp;
		timer_t timer;
		itimerspec timer_value;
		timespec initial_timervalue, timer_interval;

		request_thread_params rtp0 = request_thread_params(n, v, request_buffer, "data John Smith");
		request_thread_params rtp1 = request_thread_params(n, v, request_buffer, "data Jane Smith");
		request_thread_params rtp2 = request_thread_params(n, v, request_buffer, "data Joe Smith");
		stat_thread_params stp0 = stat_thread_params(n, v, john_smith, &john_frequency_count, &john_m);
		stat_thread_params stp1 = stat_thread_params(n, v, jane_smith, &jane_frequency_count, &jane_m);
		stat_thread_params stp2 = stat_thread_params(n, v, joe_smith, &joe_frequency_count, &joe_m);
		std::vector<worker_thread_params> wtps = std::vector<worker_thread_params>(
																				   w, worker_thread_params(v, nullptr, request_buffer, john_smith, jane_smith, joe_smith));

		pthread_t tid1, tid2, tid3, tid4, tid5, tid6;
		pthread_create(&tid4, NULL, stat_thread_function, &stp0);
		pthread_create(&tid5, NULL, stat_thread_function, &stp1);
		pthread_create(&tid6, NULL, stat_thread_function, &stp2);

		if (REAL_TIME_HIST_DISP) {
			sigemptyset(&sa.sa_mask);
			sa.sa_flags = SA_SIGINFO;
			sa.sa_sigaction = display_histograms;
			sigaction(SIGALRM, &sa, NULL);

			sevp.sigev_notify = SIGEV_SIGNAL;
			sevp.sigev_signo = SIGALRM;
			sevp.sigev_value.sival_ptr = (void*) &dhp;
			timer_create(CLOCK_REALTIME, &sevp, &timer);

			initial_timervalue.tv_nsec = 0;
			initial_timervalue.tv_sec = 2;
			timer_interval.tv_nsec = 0;
			timer_interval.tv_sec = 2;
			timer_value.it_value = initial_timervalue;
			timer_value.it_interval = timer_interval;
		}

		assert(gettimeofday(&start_time, 0) == 0);

		if (REAL_TIME_HIST_DISP) {
			timer_settime(timer, TIMER_ABSTIME, &timer_value, NULL);
		}

		pthread_create(&tid1, NULL, request_thread_function, &rtp0);
		pthread_create(&tid2, NULL, request_thread_function, &rtp1);
		pthread_create(&tid3, NULL, request_thread_function, &rtp2);

		for(int i = 0; i < w; ++i) {
			try {
				std::string new_channel_name = "";
				if(v == VERBOSITY_HYPER) {
					threadsafe_console_output.println("MAIN: i == " + std::to_string(i) + ", sending request [newthread_" + channel_type + "] over channel [" + control_channel_name + "]...");
				}
				new_channel_name = control_channel->send_request("newthread_" + channel_type);
				if(v == VERBOSITY_HYPER) {
					threadsafe_console_output.println("MAIN: received new channel name [" + new_channel_name + "], creating new " + channel_type + " request channel...");
				}

				if(r == NAMED_PIPE) {
					wtps[i].workerChannel = (RequestChannel*) new NamedPipeRequestChannel(new_channel_name, RequestChannel::CLIENT_SIDE);
				}
				else if(r == UNNAMED_PIPE) {
					wtps[i].workerChannel = (RequestChannel*) new UnnamedPipeRequestChannel(new_channel_name, RequestChannel::CLIENT_SIDE);
				}
				else if(r == MESSAGE_QUEUE) {
					wtps[i].workerChannel = (RequestChannel*) new MessageQueueRequestChannel(new_channel_name, RequestChannel::CLIENT_SIDE);
				}
				else if(r == SHARED_MEMORY) {
					wtps[i].workerChannel = (RequestChannel*) new SharedMemoryRequestChannel(new_channel_name, RequestChannel::CLIENT_SIDE);
				}
				else if(r == SIGNAL) {
					wtps[i].workerChannel = (RequestChannel*) new SignalRequestChannel(new_channel_name, RequestChannel::CLIENT_SIDE);
				}

				if(v == VERBOSITY_HYPER) {
					threadsafe_console_output.println("MAIN: created new " + channel_type + " request channel [" + new_channel_name + "], starting worker thread [" + std::to_string(i) + "]");
				}

				if((errno = pthread_create(&wtps[i].id, NULL, worker_thread_function, (void *) &wtps[i])) != 0) {
					threadsafe_console_output.perror("MAIN: pthread_create failed for [" + new_channel_name + "], new " + channel_type + " thread not created");
					delete wtps[i].workerChannel;
					wtps[i].failed = true;
					++THREADS_NOT_CREATED;
				}
			}
			catch (sync_lib_exception sle) {
				threadsafe_console_output.perror("MAIN: new " + channel_type + " channel not created: " + std::string(sle.what()));
				++THREADS_NOT_CREATED;
				wtps[i].failed = true;
				/*
				 	Clean up everything if no worker threads
				 	have been created.
				 */
				if(THREADS_NOT_CREATED == w) {
					pthread_cancel(tid1);
					pthread_cancel(tid2);
					pthread_cancel(tid3);
					pthread_cancel(tid4);
					pthread_cancel(tid5);
					pthread_cancel(tid6);
					pthread_join(tid1, NULL);
					pthread_join(tid2, NULL);
					pthread_join(tid3, NULL);
					pthread_join(tid4, NULL);
					pthread_join(tid5, NULL);
					pthread_join(tid6, NULL);
					control_channel->send_request("quit");
					delete control_channel;
					delete request_buffer;
					delete john_smith;
					delete jane_smith;
					delete joe_smith;
					throw sync_lib_exception("MAIN: all worker threads failed, no point in continuing.");
				}
			}
			catch (std::bad_alloc ba) {
				threadsafe_console_output.println("MAIN: caught std::bad_alloc in worker thread loop");
				throw;
			}
		}

		if(v >= VERBOSITY_DEBUG) threadsafe_console_output.println("MAIN: Joining request threads...");

		pthread_join(tid1, NULL);
		pthread_join(tid2, NULL);
		pthread_join(tid3, NULL);

		if(v >= VERBOSITY_DEBUG) threadsafe_console_output.println("MAIN: Pushing quit requests...");

		for(int i = 0; i < w - THREADS_NOT_CREATED; ++i) {
			std::string quit_request = "quit" + std::to_string(i + 1);
			if(v >= VERBOSITY_HYPER) threadsafe_console_output.println("MAIN: Pushing request [" + quit_request + "]");
			request_buffer->push_back(quit_request);
		}

		if(v >= VERBOSITY_DEBUG) threadsafe_console_output.println("MAIN: Joining worker threads...");

		for(int i = 0; i < w; ++i) {
			if(!wtps[i].failed) {
				if(!wtps[i].failed && (errno = pthread_join(wtps[i].id, NULL)) != 0) {
					threadsafe_console_output.perror("MAIN: Failed on pthread_join for [" + std::to_string(i) + "]");
				}
			}
		}

		assert(gettimeofday(&finish_time, 0) == 0);
		start_usecs = (start_time.tv_sec * 1e6) + start_time.tv_usec;
		finish_usecs = (finish_time.tv_sec * 1e6) + finish_time.tv_usec;

		if(v >= VERBOSITY_DEBUG) threadsafe_console_output.println("MAIN: Joining stat threads...");

		pthread_join(tid4, NULL);
		pthread_join(tid5, NULL);
		pthread_join(tid6, NULL);

		if (REAL_TIME_HIST_DISP) {
			sleep(2);
			timer_delete(timer);
			signal(SIGALRM, SIG_IGN);
		}

		if(v >= VERBOSITY_DEBUG) threadsafe_console_output.println("MAIN: Stat threads finished!");

		delete request_buffer;
		delete john_smith;
		delete jane_smith;
		delete joe_smith;

		std::string histogram_table = make_histogram_table("John Smith", "Jane Smith", "Joe Smith",
		        &john_frequency_count, &jane_frequency_count, &joe_frequency_count);

		if(v >= VERBOSITY_CHECK_CORRECTNESS) threadsafe_console_output.println("Results for n == " + std::to_string(n) + ", s == " + std::to_string(s) + ", w == " + std::to_string(w) + ", r == " + channel_type + ", b == " + std::to_string(b) + ", m == " + std::to_string(m));

		if(v >= VERBOSITY_DEFAULT) threadsafe_console_output.println("Time to completion: " + std::to_string(finish_usecs - start_usecs) + " usecs");
		if (v >= VERBOSITY_DEFAULT)
			threadsafe_console_output.println(histogram_table);

		if(v >= VERBOSITY_DEFAULT) threadsafe_console_output.println("MAIN: Sleeping...");
		usleep(10000);
		std::string finale = control_channel->send_request("quit_control");
		delete control_channel;

		std::ofstream ofs;
		if(USE_ALTERNATE_FILE_OUTPUT) ofs.open("output2.txt", std::ios::out | std::ios::app);
		else ofs.open("output.txt", std::ios::out | std::ios::app);

		if(v >= VERBOSITY_CHECK_CORRECTNESS) ofs << "Results for n == " << n << ", s == " << s << ", w == " << w << ", r == " << channel_type << ", b == " << b << ", m == " << m << std::endl;
		if(v >= VERBOSITY_DEFAULT) ofs << "Time to completion: " << std::to_string(finish_usecs - start_usecs) << " usecs" << std::endl;
		if (v >= VERBOSITY_CHECK_CORRECTNESS)
			ofs << histogram_table;
		ofs.close();

		if(v >= VERBOSITY_DEFAULT) threadsafe_console_output.println("MAIN: Finale: " + finale);
	}
	else if(!SEPARATE_WINDOWS && pid == 0) {
		try {
			if(r == NAMED_PIPE) {
				threadsafe_console_output.println("SERVER: Establishing NAMED PIPE control channel... ");
				NamedPipeRequestChannel control_channel(control_channel_name, RequestChannel::SERVER_SIDE);
				threadsafe_console_output.println("SERVER: Finished establishing NAMED PIPE control channel.");
				Dataserver::handle_process_loop(control_channel);
				threadsafe_console_output.println("SERVER: Exiting...");
			}
			else if(r == UNNAMED_PIPE) {
				threadsafe_console_output.println("SERVER: Establishing UNNAMED PIPE control channel... ");
				UnnamedPipeRequestChannel control_channel(control_channel_name, RequestChannel::SERVER_SIDE);
				threadsafe_console_output.println("SERVER: Finished establishing UNNAMED PIPE control channel.");
				Dataserver::handle_process_loop(control_channel);
				threadsafe_console_output.println("SERVER: Exiting...");
			}
			else if(r == MESSAGE_QUEUE) {
				threadsafe_console_output.println("SERVER: Establishing MESSAGE QUEUE control channel... ");
				MessageQueueRequestChannel control_channel(control_channel_name, RequestChannel::SERVER_SIDE);
				threadsafe_console_output.println("SERVER: Finished establishing MESSAGE QUEUE control channel.");
				Dataserver::handle_process_loop(control_channel);
				threadsafe_console_output.println("SERVER: Exiting...");
			}
			else if(r == SHARED_MEMORY) {
				threadsafe_console_output.println("SERVER: Establishing SHARED MEMORY control channel... ");
				SharedMemoryRequestChannel control_channel(control_channel_name, RequestChannel::SERVER_SIDE);
				threadsafe_console_output.println("SERVER: Finished establishing SHARED MEMORY control channel.");
				Dataserver::handle_process_loop(control_channel);
				threadsafe_console_output.println("SERVER: Exiting...");
			}
			else if(r == SIGNAL) {
				threadsafe_console_output.println("SERVER: Establishing SIGNAL control channel... ");
				SignalRequestChannel control_channel(control_channel_name, RequestChannel::SERVER_SIDE);
				threadsafe_console_output.println("SERVER: Finished establishing SIGNAL control channel.");
				Dataserver::handle_process_loop(control_channel);
				threadsafe_console_output.println("SERVER: Exiting...");
			}
		}
		catch (sync_lib_exception sle) {
			threadsafe_console_output.perror("DATASERVER: control channel not created: " + std::string(sle.what()));
		}
	}
}
