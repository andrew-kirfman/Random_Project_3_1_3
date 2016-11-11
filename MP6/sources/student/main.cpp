/* Project: CSCE-313 Machine Problem #1                                        */
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

// This library includes several macros that allow you to color the output
// of your program in the terminal.  They are provided to you in the event that
// you would like to spice up your program and make it look more interesting.  
#include "utility.h"

// ADD ANY FURTHER USER DEFINED INCLUDES HERE!

/* --------------------------------------------------------------------------- */
/* Global Variables                                                            */
/* --------------------------------------------------------------------------- */

// ADD ANY FURTHER GLOAL VARIABLES HERE!


// This method finds the difference of two timespec structures.  A timespec is defined as follows:
// 
// struct timespec
// {
//     time_t tv_sec;
//     long tv_nsec;
// }
// 
// You may use this function as needed throughout your program.   
timespec diff(timespec start, timespec end)
{
    timespec temp;
    if ((end.tv_nsec-start.tv_nsec)<0) 
    {
        temp.tv_sec = end.tv_sec-start.tv_sec-1;
        temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
    } 
    else 
    {
        temp.tv_sec = end.tv_sec-start.tv_sec;
        temp.tv_nsec = end.tv_nsec-start.tv_nsec;
    }
    
    return temp;
}

void cleanup()
{
    /* CODE REQUIRED HERE!!! */

    /* This function should be called whenever your program would terminate (either normally
     * or abnormally) and perform all clean up duties.  The program may have created processes
     * that are still running.  If there are any, you should clean them up as leaving orphaned
     * processes running is bad for the system.  
     *
     * In order to call this method during abnormal termination, look into which signals you
     * account for and write a signal handler (i.e. if the process receives a SIGINT, you 
     * should have a handler to call this cleanup function.  
     *
     * IMPORTANT NOTE:
     * This assignment will require a number of signal handlers which are not included in this
     * pre-provided code.  You will have to write them in addition to everything provided here.  
     */
}


/* This class serves as a data container for child processes that your program starts.  Read
 * through the member functions listed here and try to understand what they do.  If you don't
 * understand anything, be sure to ask your TA as soon as possible.  
 *
 * Member variables:
 *   - process_pid: The pid of the process that this class is representing.  This pid is
 *     recovered when you call fork to create a new process.  
 *   - remaining_execution_time: This variable is used for shortest job first scheduling.  
 *     In a SJF system, each process has an approximate execution time (approximate since
 *     you can never know how long it will actually take to run).  This variable keeps
 *     track of how much time is probably left before the process will terminate.  
 *   - running: Is the process currently executing (i.e. it is not stopped by the SIGSTOP signa
 *   - initialized: keeps track of whether or not the class actually contains a real instance
 *     of a running process.  
 *   - last_start_time: Used for the SJF scheduler.  Should keep track of the last time that 
 *     the process was started (the start time and the eventual stop time (if the process
 *     is ever preempted to be replaced by a faster process) can be used to calculate the 
 *     time that the process was running.  This running time can be deducted from the 
 *     remaining_execution_time in order to update it.  
 *
 * You are free to changet the member functions as you wish.  The ones provided are simply those
 * that your instructors used to implement the assignment in the first place.  
 */
class Running_process
{
    public:
    /* Constructor */
    Running_process();
    Running_process(pid_t pid, double remaining_exec_time, bool active);

    /* Getter Methods */
    pid_t get_process_pid();
    double get_remaining_execution_time();
    bool is_running();
    bool is_initialized();
    
    /* Setter Methods */
    void set_process_pid(pid_t new_pid, bool is_initialized, double remaining_exec_time,
        bool is_active);
    void set_remaining_execution_time(double new_exec_time);
    void set_initialized(bool is_initialized);
    void set_running(bool is_running);
    void set_time_structure(struct timespec t1);

    /* Methods for SJF  */
    void recalculate_remaining_time();

private:
    pid_t process_pid;
    double remaining_execution_time;
    bool running;
    bool initialized;
    struct timespec last_start_time;
};

/* Constructors */
Running_process::Running_process()
{
    process_pid = 0;   
    remaining_execution_time = 0.0;
    running = false;
    initialized = false;

    last_start_time.tv_sec  = 0;
    last_start_time.tv_nsec = 0;
}

Running_process::Running_process(pid_t pid, double remaining_exec_time, bool active = false)
{
    process_pid = pid;
    remaining_execution_time = remaining_exec_time;
    running = active;
    initialized = true;

    last_start_time.tv_sec  = 0;
    last_start_time.tv_nsec = 0;
}


/* Getter Methods */
pid_t Running_process::get_process_pid()
{

}

double Running_process::get_remaining_execution_time()
{

}

bool Running_process::is_running()
{

}

bool Running_process::is_initialized()
{

}


/* Setter Methods */
void Running_process::set_process_pid(pid_t new_pid, bool is_initialized = false,
    double remaining_exec_time = 0.0, bool active = false)
{

}

void Running_process::set_remaining_execution_time(double new_exec_time)
{

}

void Running_process::set_initialized(bool is_initialized)
{

}

void Running_process::set_running(bool is_running)
{

}

/* Methods for SJF  */
void Running_process::recalculate_remaining_time()
{

}

void Running_process::set_time_structure(struct timespec t1)
{

}

// Defines to specify the type of scheduling policy to be used
#define NO_POLICY 0
#define ROUND_ROBIN 1
#define FIFO 2
#define SJF 3
#define INTERACTIVE_ROUND_ROBIN 4
#define INTERACTIVE_FIFO 5
#define INTERACTIVE_SJF 6

class Scheduler
{
public:

    /* Constructor */
    Scheduler();

    /* Utility methods */
    void schedule_process(pid_t process_pid, double expected_run_time);
    
    /* Setter Methods */
    void set_policy(int policy);
    void set_time_quantum(int time_secs);

    /* Public Scheduling Routines */
    void schedule_all();

private:
    int scheduling_policy;
    int processes_scheduled;
    
    int time_quantum;
    std::vector<std::pair<int, Running_process*>> scheduleable_processes;

    /* Private Scheduling Routines */
    void schedule_RR();
    void schedule_FIFO();
    void schedule_SJF();
    
    void schedule_interactive_RR();
    void schedule_interactive_FIFO();
    void schedule_interactive_SJF();
};

/* Constructor */
Scheduler::Scheduler()
{
    processes_scheduled = 0;
    scheduling_policy = NO_POLICY;

    // Set a default time quantum (10 ms is default)
    time_quantum = 10000;
}


/* Utility Methods */
void Scheduler::schedule_process(pid_t process_pid, double expected_run_time = 0.0)
{
    Running_process *process = new Running_process(process_pid, expected_run_time, false);
    
    std::pair<int, Running_process*> new_process(processes_scheduled, process);
    processes_scheduled += 1;

    scheduleable_processes.push_back(new_process);
}


/* Setter Methods */
void Scheduler::set_policy(int policy)
{
    if(policy == NO_POLICY || policy == ROUND_ROBIN
        || policy == FIFO || policy == SJF
        || policy == INTERACTIVE_ROUND_ROBIN
        || policy == INTERACTIVE_FIFO
        || policy == INTERACTIVE_SJF)
    {
        scheduling_policy = policy;
    }
    else
    {
        std::cout << "[ERROR]: Unknown scheduling policy." << std::endl;
    }

    // Also reset the number of processes scheduled
    processes_scheduled = 0;
}

void Scheduler::set_time_quantum(int time_nsecs)
{
    if(time_nsecs >= 10000000)
    {
        std::cout << "  [" << BOLDYELLOW << "WARNING" << RESET "]: Long time quantum. Short processes may be starved." 
            << std::endl;
    }

    time_quantum = time_nsecs;   
}


/* Public Scheduling Routines */
void Scheduler::schedule_all()
{
    if(scheduling_policy == ROUND_ROBIN)
    {
        schedule_RR();
    }
    else if(scheduling_policy == FIFO)
    {
        schedule_FIFO();
    }
    else if(scheduling_policy == SJF)
    {
        schedule_SJF();
    }
    else if(scheduling_policy == INTERACTIVE_ROUND_ROBIN)
    {
        schedule_interactive_RR();
    }
    else if(scheduling_policy == INTERACTIVE_FIFO)
    {
        schedule_interactive_FIFO();
    }
    else if(scheduling_policy == INTERACTIVE_SJF)
    {
        schedule_interactive_SJF();
    }
    else if(scheduling_policy == NO_POLICY)
    {
        std::cout << "  [" << BOLDRED << "ERROR" << "]: No scheduling policy chosen." << std::endl;
    }

    return;
}

/* Global Variables */
// IMPORTANT NOTE: Whenever your program sets calls a signal handler, it needs some way of communicating 
//sig_atomic_t alarm_flag = 0;



/* Private Scheduling Routines */
void Scheduler::schedule_RR()
{   
    /* IMPORTANT NOTE:
     *
     *
     *
     * CONTINUE HERE!!!!!!!
     *
     *
     *
     */ 

    /* Handler for time slicing */
    struct sigaction signal_struct_1;
    signal_struct_1.sa_handler = handle_RR;
    sigaction(SIGALRM, &signal_struct_1, NULL);
    
    /* Handler for process termination */
    struct sigaction signal_struct_2;
    signal_struct_2.sa_handler = handle_term;
    signal_struct_2.sa_flags = SA_NOCLDSTOP;
    sigaction(SIGCHLD, &signal_struct_2, NULL);

    // Set the initial alarm plus some time for this process to do management tasks
    struct itimerval timer_int;
    struct timeval timer_interval;
    timer_interval.tv_usec = time_quantum;
    timer_interval.tv_sec = 0;
    timer_int.it_interval = timer_interval;

    struct timeval timer_value;
    timer_value.tv_usec = time_quantum;
    timer_value.tv_sec = 0;
    timer_int.it_value = timer_value;

    setitimer(ITIMER_REAL, &timer_int, NULL);

    // Keep track of currently running process
    int process_pointer = 0;
    bool term = false;

    // Start the first process!
    kill(std::get<1>(scheduleable_processes[0])->get_process_pid(), SIGCONT);

    while(true)
    {
        if(terminated_pid != -1 && terminated_pid >= 0)
        {
            // Deal with terminated process
            bool found = false;
            for(unsigned short int i=0; i<scheduleable_processes.size(); i++)
            {
                if(std::get<1>(scheduleable_processes[i])->get_process_pid() == terminated_pid)
                {
                    std::cout << "  [" << BOLDBLUE << "SUCCESS" << RESET << "]: Process_" << 
                        std::to_string(std::get<0>(scheduleable_processes[i]) + 1) << " terminated successfully." << std::endl;

                    // Set appropriate flags
                    term = true;
                    found = true;
                    terminated_pid = -1;

                    scheduleable_processes.erase(scheduleable_processes.begin() + i);
                    break;
                }
            }

            if(found == false)
            {
                std::cout << "  [ERROR]: Cound not terminate finished process successfully." << std::endl;
                break;
            }

            // Set process_pointer properly
            if((unsigned int)process_pointer >= scheduleable_processes.size())
            {
                process_pointer = scheduleable_processes.size() - 1;
            }
        }
 
        /* If the process queue is empty, exit! */
        if(scheduleable_processes.size() == 0)
        {
            std::cout << "  [" << BOLDWHITE << "INFO" << RESET << "]: All processes terminated.  Exiting Round Robin scheduler"
                << std::endl;
            break;
        }


        if(alarm_flag == 1)
        {
            if(term == true)
            {
                term = false;

                if((unsigned int)process_pointer >= scheduleable_processes.size() - 1)
                {
                    process_pointer = 0;
                }

                pid_t pid_to_start = std::get<1>(scheduleable_processes[process_pointer])->get_process_pid();
                kill(pid_to_start, SIGCONT);
            }
            else
            {   
                // Stop currently running process
                pid_t pid_to_stop = std::get<1>(scheduleable_processes[process_pointer])->get_process_pid();
                kill(pid_to_stop, SIGSTOP);

                // Adjust process pointer
                if((unsigned int)process_pointer >= scheduleable_processes.size() - 1)
                {
                    process_pointer = 0;
                }
                else
                {
                    process_pointer += 1;
                }

                // Start the next process
                pid_t pid_to_start = std::get<1>(scheduleable_processes[process_pointer])->get_process_pid();
                kill(pid_to_start, SIGCONT);
            }

            alarm_flag = 0;
        }

    }

    return;
}


void Scheduler::schedule_FIFO()
{
    if(scheduleable_processes.size() == 0)
    {
        std::cout << "[" << BOLDRED << "ERROR" << RESET << "]: No processes to schedule!" << std::endl;
        return;
    }

    std::cout << "  [" << BOLDWHITE << "INFO" << RESET << "]: Starting FIFO processes in order from 1 to " 
        << std::to_string(scheduleable_processes.size()) << "." << std::endl;

    /* Handler for SIGCHILD */
    struct sigaction signal_struct;
    signal_struct.sa_handler = handle_term;

    // Don't do anything if the sigchild is sent on a stop or resume
    signal_struct.sa_flags = SA_NOCLDSTOP;
    sigaction(SIGCHLD, &signal_struct, NULL);

    while(true)
    {
        if(terminated_pid != -1 && terminated_pid >= 0)
        {
            bool found = false;
            for(unsigned short int i=0; i<scheduleable_processes.size(); i++)
            {
                if(std::get<1>(scheduleable_processes[i])->get_process_pid() == terminated_pid)
                {
                    scheduleable_processes.erase(scheduleable_processes.begin() + i);
                    found = true;

                    terminated_pid = -1;

                    std::cout << "  [" << BOLDBLUE << "SUCCESS" << RESET << "]: Process_" << 
                        std::to_string(std::get<0>(scheduleable_processes[i])) << " terminated successfully." << std::endl;
                    break;
                }
            }

            if(found == false)
            {
                std::cout << "[ERROR]: Cound not terminate finished process successfully." << std::endl;
            }
        }

        /* If the process queue is empty, exit! */
        if(scheduleable_processes.size() == 0)
        {
            std::cout << "  [" << BOLDWHITE << "INFO" << RESET << "]: All processes terminated.  Exiting FIFO scheduler"
                << std::endl;
            break;
        }

        /* Find the next process */
        Running_process *current_process = std::get<1>(scheduleable_processes[0]);

        // Currently waiting for the process to finish
        if(current_process->is_running())
        {
            usleep(10000);
            continue;
        }
        // The process has not been started.  Start it.  
        else
        {
            current_process->set_running(true);
            kill(current_process->get_process_pid(), SIGCONT);
            sleep(1);
            kill(current_process->get_process_pid(), SIGCONT);
        }
    }

    return;
}

void Scheduler::schedule_SJF()
{
    if(scheduleable_processes.size() == 0)
    {
        std::cout << "[" << BOLDRED << "ERROR" << RESET << "]: No processes to schedule!" << std::endl;
        return;
    }

    std::cout << "  [" << BOLDWHITE << "INFO" << RESET << "]: Starting SJF processes fastest first." << "." << std::endl;

    /* Handler for SIGCHILD */
    struct sigaction signal_struct;
    signal_struct.sa_handler = handle_term;

    // Don't do anything if the sigchild is sent on a stop or resume
    signal_struct.sa_flags = SA_NOCLDSTOP;
    sigaction(SIGCHLD, &signal_struct, NULL);

    while(true)
    {
        if(terminated_pid != -1 && terminated_pid >= 0)
        {
            bool found = false;
            for(unsigned short int i=0; i<scheduleable_processes.size(); i++)
            {
                if(std::get<1>(scheduleable_processes[i])->get_process_pid() == terminated_pid)
                {
                    scheduleable_processes.erase(scheduleable_processes.begin() + i);
                    found = true;

                    terminated_pid = -1;

                    std::cout << "  [" << BOLDBLUE << "SUCCESS" << RESET << "]: Process_" << 
                        std::to_string(std::get<0>(scheduleable_processes[i])) << " terminated successfully." << std::endl;
                    break;
                }
            }

            if(found == false)
            {
                std::cout << "[ERROR]: Cound not terminate finished process successfully." << std::endl;
            }
        }

        /* If the process queue is empty, exit! */
        if(scheduleable_processes.size() == 0)
        {
            std::cout << "  [" << BOLDWHITE << "INFO" << RESET << "]: All processes terminated.  Exiting SJF scheduler"
                << std::endl;
            break;
        }

        /* Find the next process */
        double min_remaining_execution_time = DBL_MAX;
        int shortest_position = -1;
        for(unsigned short int i=0; i<scheduleable_processes.size(); i++)
        {
            if(std::get<1>(scheduleable_processes[i])->get_remaining_execution_time() < min_remaining_execution_time)
            {
                min_remaining_execution_time = std::get<1>(scheduleable_processes[i])->get_remaining_execution_time();
                shortest_position = i;
            }
        }

        Running_process *current_process = std::get<1>(scheduleable_processes[shortest_position]);

        // Currently waiting for the process to finish
        if(current_process->is_running())
        {
            usleep(10000);
            continue;
        }
        // The process has not been started.  Start it.  
        else
        {
            current_process->set_running(true);
            kill(current_process->get_process_pid(), SIGCONT);
            sleep(1);
            kill(current_process->get_process_pid(), SIGCONT);
        }
    }

    return;
}

void Scheduler::schedule_interactive_RR()
{
    // Your interactive algorithm will need to perform non-blocking reads in order to keep running in the background.  
    // (i.e. it needs to be able to wait for user input without blocking for a command like std::cin)
    // Below is an example of how to do it for this assignment.  
    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds);

    struct timespec timeout;
    timeout.tv_sec = 0;
    timeout.tv_nsec = 0;

    sigset_t new_sigmask;
    sigemptyset(&new_sigmask);
    sigaddset(&new_sigmask, SIGALRM);
    sigaddset(&new_sigmask, SIGINT);

    int result = pselect(STDIN_FILENO + 1, &read_fds, NULL, NULL, &timeout, &new_sigmask);
}

void Scheduler::schedule_interactive_FIFO()
{

}

void Scheduler::schedule_interactive_SJF()
{

}

int main(int arcg, char **argv)
{
    /* Define signal handlers to clean up from exceptions */
    /* 
     * Note: It would disastrous for any of the cpu intensive processes to 
     * continue running in the event that something goes wrong.  Make absolutely
     * sure that they will be cleaned up by the machine.  
     * 
     * Note to students: Do not use the signal() function to define your
     * signal handler.  The signal function is deprecated and newer tools are
     * available to you that are better and more reliable.  Specifically,
     * look into the function sigaction() and the struct sigaction.  Your
     * system should contain all relevant manpages necessary to use these
     * methods.  
     */

    // The following code is provided to assist you in testing your methods
    // Use it to only pick the scheduling algorithms that you specifically wish to test
    //
    // Note: If you choose not to do the bonus, delete anything relevant to the interactive
    // scheduling algorithms.  

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

    // Short demo of how to color your console output using the utility.h header macros
    std::cout << BOLDWHITE << "Hello World!" << RESET << std::endl;
    std::cout << BOLDBLUE << "Hello World!" << RESET << std::endl;
    std::cout << BOLDGREEN << "Hello World!" << RESET << std::endl;

    /* ------------------------------------------------------------------------- */
    /* First In First Out                                                        */
    /* ------------------------------------------------------------------------- */

    if(fifo == true)
    {

    }

    /* ------------------------------------------------------------------------- */
    /* Round Robbin                                                              */
    /* ------------------------------------------------------------------------- */
    
    if(round_robin == true)
    {

    }

    /* ------------------------------------------------------------------------- */
    /* Shortest Job First                                                        */
    /* ------------------------------------------------------------------------- */
    
    if(sjf == true)
    {

    }

    /* ------------------------------------------------------------------------- */
    /* Interactive First In First Out                                            */
    /* ------------------------------------------------------------------------- */
 
    if(interactive_fifo == true)
    {

    }

    /* ------------------------------------------------------------------------- */
    /* Interactive Round Robin                                                   */
    /* ------------------------------------------------------------------------- */
 
    if(interactive_round_robin == true)
    {
        
    }

    /* ------------------------------------------------------------------------- */
    /* Interactive Shortest Job First                                            */
    /* ------------------------------------------------------------------------- */
    
    if(interactive_sjf == true)
    {

    }

    // Remember to clean everything up at the very end
    cleanup();
    return 0;
}
