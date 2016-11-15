/* --------------------------------------------------------------------------- */
/* Project: CSCE-313 Machine Problem #6                                        */
/* Developer: Andrew Kirfman												   */
/*                                                                             */
/* File: ./MP6/main.cpp                                                        */
/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/* Standard Library Includes                                                   */
/* --------------------------------------------------------------------------- */

#include<iostream>
#include<vector>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<sys/time.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<time.h>
#include<math.h>
#include<utility>
#include<float.h>

/* --------------------------------------------------------------------------- */
/* User Defined Includes                                                       */
/* --------------------------------------------------------------------------- */

#include "utility.h"
#include "Scheduler.h"

/* --------------------------------------------------------------------------- */
/* Global Variables                                                            */
/* --------------------------------------------------------------------------- */

std::vector<pid_t> running_processes;

/* 
 * This machine problem will introduce you to the function of the operating system's scheduler and how different
 * processes are affected by various scheduling policies.  Additionally, it will also serve as a practical visualization
 * of the scheduling policies covered in lecture and those policies that are actually implemented in real UNIX/UNIX Like systems.  
 *
 * Two fundamentally different process types will be used to demonstrate the relative strengths and weaknesses inherrent in
 * the variety of different scheduling algorithms persented here: cpu bound and i/o bound processes.  CPU bound processes perform
 * tasks that require, almost exclusively, the use of the CPU/main memory and no other components of the system (hard disk, network,
 * etc..).  As such, whenever a CPU bound process is running, the CPU itself is, in general, completing an instruction every clock
 * cycle.  (not counting unavoidable stalls for pipeline hazards, stalls from cache misses, or other data hazards)  This means that 
 * the CPU never wastes time and is always busy performing useful work.  
 *
 * Examples of both CPU and I/O bound processes are provided below.  For this assignment, you must implement 5 CPU bound functions and
 * 5 I/O bound functions all of your choice.  The input parameters for these programs must be chosen beforehand such that each process
 * will run for a reasonable, finite amount of time.  Also, you should stagger the expected runtimes of each function.  
 *
 * CPU Bound Processes:
 *  - Satisfiability problem
 *  - Cryptarithm Solver
 *  - Breadth-First Search/Depth-First Search for something
 *  - Seive of (whatever his name is)
 *  - Matrix multiplication
 *  - A for loop that increments a number (tight loop)
 */

void cleanup()
{
    /* Clean up all running processes */
    for(unsigned short int i=0; i<running_processes.size(); i++)
    {
        kill(running_processes[i], SIGKILL);
    }

    /* Clean up temporary directories */
    system("rm -rf ./active");

    exit(255);
}

void handle_signals(int signum)
{
    cleanup();
}

int main(int arcg, char **argv)
{
    /* Define signal handlers to clean up from exceptions */
    /* 
     * Note: It would disastrous for any of the cpu intensive processes to 
     * continue running in the event that something goes wrong.  Make absolutely
     * sure that they will be cleaned up by the machine.  
     */
    struct sigaction signal_struct;
    signal_struct.sa_handler = handle_signals;
    sigaction(SIGINT, &signal_struct, NULL);
    sigaction(SIGQUIT, &signal_struct, NULL);
    sigaction(SIGILL, &signal_struct, NULL);
    sigaction(SIGABRT, &signal_struct, NULL);
    sigaction(SIGFPE, &signal_struct, NULL);
    sigaction(SIGSEGV, &signal_struct, NULL);
    sigaction(SIGTERM, &signal_struct, NULL);

    /* Reused Variables */
    pid_t pid = 0;
    int return_val = 0;
    std::string system_string = "";

    /* Clear old occurrences of the temp directories */
    return_val = system("rm -rf ./active 2>&1 > /dev/null");
    if(return_val == -1)
    {
        std::cout << "[" << BOLDRED << "ERROR" << RESET << "]: Cound not remove old running_processes directory." << std::endl;
    }

    /* Make a directory to write executable files to */
    mkdir("./active", S_IRWXU | S_IRWXG | S_IRWXO);

    /* Policy selection menu */
    bool fifo = false;
    bool round_robin = false;
    bool sjf = false;
    bool interactive_fifo = false;
    bool interactive_round_robin = false;
    bool interactive_sjf = false;

    system("Clear");
    std::cout << "Choose a series of policies to test by entering numbers separated by spaces on the following line." << std::endl;
    std::cout << "  - 1 for first in first out." << std::endl;
    std::cout << "  - 2 for round robin." << std::endl;
    std::cout << "  - 3 for shortest job first." << std::endl;
    std::cout << "  - 4 for interactive first in first out." << std::endl;
    std::cout << "  - 5 for interactive round robin." << std::endl;
    std::cout << "  - 6 for interactive shortest job first." << std::endl;
    std::cout << BOLDGREEN <<  "  --> " << RESET;
    std::string choice = "";
    getline(std::cin, choice);

    for(unsigned short int i=0; i<choice.length(); i++)
    {
        if(choice[i] == ' ')
        {
            continue;
        }
        else
        {
            switch(choice[i])
            {
                case '1': { fifo = true; break; }
                case '2': { round_robin = true; break; }
                case '3': { sjf = true; break; }
                case '4': { interactive_fifo = true; break; }
                case '5': { interactive_round_robin = true; break; }
                case '6': { interactive_sjf = true; break; }
                default: { break; }
            }
        }
    }

	for(unsigned short int i=1; i<=5; i++)
	{
		system_string = "g++ -std=c++11 ./running_processes/process_" + std::to_string(i) + ".cpp -o ./active/process_" 
			+ std::to_string(i);
		return_val = system(system_string.c_str());
		if(return_val == -1)
		{
			std::cout << "  [" << BOLDRED << "ERROR" << RESET << "]: Could not create Process_" << std::to_string(i) << std::endl
				<< "    " << strerror(errno) << std::endl;
			cleanup();
		}
	}

    /* ------------------------------------------------------------------------- */
    /* First In First Out                                                        */
    /* ------------------------------------------------------------------------- */

    if(fifo == true)
    {
        std::cout << std::endl;
        std::cout << BOLDGREEN << "/* -------------------------------------------------------------------------- */" << RESET << std::endl;
        std::cout << BOLDGREEN << "/* First In First Out                                                         */" << RESET << std::endl;
        std::cout << BOLDGREEN << "/* -------------------------------------------------------------------------- */" << RESET << std::endl;
        std::cout << std::endl;

        std::cout << "Starting processes." << std::endl;
        for(unsigned short int i=1; i<=5; i++)
        {
			pid = fork();

			if(pid == 0)
			{
				system_string = "./active/process_" + std::to_string(i);
				return_val = execlp(system_string.c_str(), system_string.c_str(), NULL);
				exit(0);
			}
			else if(pid == -1)
			{
				std::cout << "  [" << BOLDRED << "ERROR" << RESET << "]: Could not create Process_" << std::to_string(i) << std::endl
					<< "    " << strerror(errno) << std::endl;
				cleanup();
			}
			else
			{
				running_processes.push_back(pid);
				std::cout << "  [" << BOLDBLUE << "SUCCESS" << RESET << "]: Created Process_" << std::to_string(i) << std::endl;
			}
        }
        std::cout << std::endl;

        /* Instantiate the scheduler class and add the processes to it */
        Scheduler *fifo_scheduler = new Scheduler();
        for(unsigned short int i=0; i<running_processes.size(); i++)
        {
            fifo_scheduler->schedule_process(running_processes[i]);
        }
        fifo_scheduler->set_policy(FIFO);

        std::cout << "Starting CPU bound scheduler." << std::endl;
        fifo_scheduler->schedule_all();
        std::cout << std::endl;

        /* Clear running processes vector */
        running_processes.clear();

        delete fifo_scheduler;
    }

    /* ------------------------------------------------------------------------- */
    /* Round Robbin                                                              */
    /* ------------------------------------------------------------------------- */
    
    if(round_robin == true)
    {
        std::cout << std::endl;
        std::cout << BOLDGREEN << "/* -------------------------------------------------------------------------- */" << RESET << std::endl;
        std::cout << BOLDGREEN << "/* Round Robin                                                                */" << RESET << std::endl;
        std::cout << BOLDGREEN << "/* -------------------------------------------------------------------------- */" << RESET << std::endl;
        std::cout << std::endl;

        /* Reinitialize the 5 processes */
        std::cout << "Starting processes." << std::endl;

        for(unsigned short int i=1; i<=5; i++)
        {
            pid = fork();
    
            if(pid == 0)
            {
				system_string = "./active/process_" + std::to_string(i);
				return_val = execlp(system_string.c_str(), system_string.c_str(), NULL);
				exit(0);
            }
            else if(pid == -1)
            {
                std::cout << "  [" << BOLDRED << "ERROR" << RESET << "]: Could not create Process_" << std::to_string(i) << std::endl
                    << "    " << strerror(errno) << std::endl;
                cleanup();
            }
            else
            {
                running_processes.push_back(pid);
                std::cout << "  [" << BOLDBLUE << "SUCCESS" << RESET << "]: Created Process_" << std::to_string(i) << std::endl;
            }
        }
        std::cout << std::endl;

        /* Instantiate the scheduler class and add the processes to it */
        Scheduler *rr_scheduler = new Scheduler();
        for(unsigned short int i=0; i<running_processes.size(); i++)
        {
            rr_scheduler->schedule_process(running_processes[i]);
        }
        rr_scheduler->set_policy(ROUND_ROBIN);

        std::cout << "Starting Round Robin scheduler." << std::endl; 
        rr_scheduler->schedule_all();
        std::cout << std::endl;

        /* Clear running processes vector */
        running_processes.clear();
        delete rr_scheduler;
    }

    /* ------------------------------------------------------------------------- */
    /* Shortest Job First                                                        */
    /* ------------------------------------------------------------------------- */
    
    if(sjf == true)
    {
        std::cout << std::endl;
        std::cout << BOLDGREEN << "/* -------------------------------------------------------------------------- */" << RESET << std::endl;
        std::cout << BOLDGREEN << "/* Shortest Job First                                                         */" << RESET << std::endl;
        std::cout << BOLDGREEN << "/* -------------------------------------------------------------------------- */" << RESET << std::endl;
        std::cout << std::endl;

        /* Reinitialize the 5 processes */
        std::cout << "Starting processes." << std::endl;

        for(unsigned short int i=1; i<=5; i++)
        {
            pid = fork();

            if(pid == 0)
            {
                system_string = "./active/process_" + std::to_string(i);
                return_val = execlp(system_string.c_str(), system_string.c_str(), NULL);
                exit(0);
            }
            else if(pid == -1)
            {
                std::cout << "  [" << BOLDRED << "ERROR" << RESET << "]: Could not create Process_" << std::to_string(i) << std::endl
                    << "    " << strerror(errno) << std::endl;
                cleanup();
            }
            else
            {
                running_processes.push_back(pid);
                std::cout << "  [" << BOLDBLUE << "SUCCESS" << RESET << "]: Created Process_" << std::to_string(i) << std::endl;
            }
        }
        std::cout << std::endl;

        /* Instantiate the scheduler class and add the processes to it */
        Scheduler *sjf_scheduler = new Scheduler();

        /* Note:
         * The shortest job first sheduler takes an approximate completion time of each process.  
         * This may need to be recalculated for each machine that this is run on.  It should be
         * good enough to simply have a relative comparison of completion times (i.e. which 
         * processes will probably finish before others), but better safe than sorry.  
         */
        sjf_scheduler->schedule_process(running_processes[0], 8.3);
        sjf_scheduler->schedule_process(running_processes[1], 10.9);
        sjf_scheduler->schedule_process(running_processes[2], 1.92);
        sjf_scheduler->schedule_process(running_processes[3], 33.2);
        sjf_scheduler->schedule_process(running_processes[4], 41.7);
        std::cout << "Starting Shortest Job First scheduler." << std::endl;
        std::cout << "  [" << BOLDWHITE << "INFO" << RESET << "]: Process_1 (Sieve of Eratosthenes): Expected run time: 8.3 seconds." << std::endl;
        std::cout << "  [" << BOLDWHITE << "INFO" << RESET << "]: Process_2 (Matrix Multiplication): Expected run time: 10.9 seconds." << std::endl;
        std::cout << "  [" << BOLDWHITE << "INFO" << RESET << "]: Process_3 (Tight Loop): Expected run time: 1.92 seconds." << std::endl;
        std::cout << "  [" << BOLDWHITE << "INFO" << RESET << "]: Process_4 (Bubble Sort): Expected run time: 33.2 seconds." << std::endl;
        std::cout << "  [" << BOLDWHITE << "INFO" << RESET << "]: Process_5 (Cryptarithm Solver): Expected run time: 41.7 seconds." << std::endl;

        sjf_scheduler->set_policy(SJF);
        sjf_scheduler->schedule_all();
        std::cout << std::endl;

        /* Clear the running process vector */
        running_processes.clear();
        delete sjf_scheduler;
    }

    /* ------------------------------------------------------------------------- */
    /* Interactive First In First Out                                            */
    /* ------------------------------------------------------------------------- */
 
    if(interactive_fifo == true)
    {
        std::cout << std::endl;
        std::cout << BOLDGREEN << "/* -------------------------------------------------------------------------- */" << RESET << std::endl;
        std::cout << BOLDGREEN << "/* Interactive First In First Out                                             */" << RESET << std::endl;
        std::cout << BOLDGREEN << "/* -------------------------------------------------------------------------- */" << RESET << std::endl;
        std::cout << std::endl;

        /* Instantiate the scheduler class */
        Scheduler *interactive_fifo_scheduler = new Scheduler();
    
        /* Start scheduler running */
        interactive_fifo_scheduler->set_policy(INTERACTIVE_FIFO);
        interactive_fifo_scheduler->schedule_all();
    }

    /* ------------------------------------------------------------------------- */
    /* Interactive Round Robin                                                   */
    /* ------------------------------------------------------------------------- */
 
    if(interactive_round_robin == true)
    {
        std::cout << std::endl;
        std::cout << BOLDGREEN << "/* -------------------------------------------------------------------------- */" << RESET << std::endl;
        std::cout << BOLDGREEN << "/* Interactive Round Robin                                                    */" << RESET << std::endl;
        std::cout << BOLDGREEN << "/* -------------------------------------------------------------------------- */" << RESET << std::endl;
        std::cout << std::endl;

        /* Instantiate the scheduler class */
        Scheduler *interactive_rr_scheduler = new Scheduler();
        
        /* Start scheduler running */
        interactive_rr_scheduler->set_policy(INTERACTIVE_ROUND_ROBIN);
        interactive_rr_scheduler->schedule_all();
    }

    /* ------------------------------------------------------------------------- */
    /* Interactive Shortest Job First                                            */
    /* ------------------------------------------------------------------------- */
    
    if(interactive_sjf == true)
    {
        std::cout << std::endl;
        std::cout << BOLDGREEN << "/* -------------------------------------------------------------------------- */" << RESET << std::endl;
        std::cout << BOLDGREEN << "/* Interactive Shortest Job First                                             */" << RESET << std::endl;
        std::cout << BOLDGREEN << "/* -------------------------------------------------------------------------- */" << RESET << std::endl;
        std::cout << std::endl;

        /* Instantiate the scheduler class */
        Scheduler *interactive_sjf_scheduler = new Scheduler();
    
        /* Start scheduler running */
        interactive_sjf_scheduler->set_policy(INTERACTIVE_SJF);
        interactive_sjf_scheduler->schedule_all();
    }

    cleanup();
    return 0;
}
