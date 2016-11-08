/* --------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman                                                   */
/* Project: CSCE-313 Machine Problem #1                                        */
/*                                                                             */
/* File: ./MP6/main.cpp                                                        */
/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/* Standard Library Includes                                                   */
/* --------------------------------------------------------------------------- */

#include<iostream>
#include<sched.h>
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

#include<curses.h>

/* --------------------------------------------------------------------------- */
/* User Defined Includes                                                       */
/* --------------------------------------------------------------------------- */

#include "utility.h"
#include "shell.h"

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



sig_atomic_t time_quantum_expired = 0;
void sigalarm_handler(int signum)
{
    time_quantum_expired = 1;
}


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

    /* Control Methods */
    void start();
    void stop();

private:
    pid_t process_pid;
    double remaining_execution_time;
    bool running;
    bool initialized;
};

/* Constructors */
Running_process::Running_process()
{
    process_pid = 0;   
    remaining_execution_time = 0.0;
    running = false;
    initialized = false;
}

Running_process::Running_process(pid_t pid, double remaining_exec_time, bool active = false)
{
    process_pid = pid;
    remaining_execution_time = remaining_exec_time;
    running = active;
    initialized = true;
}


/* Getter Methods */
pid_t Running_process::get_process_pid()
{
    return process_pid;
}

double Running_process::get_remaining_execution_time()
{
    return remaining_execution_time;
}

bool Running_process::is_running()
{
    return running;
}

bool Running_process::is_initialized()
{
    return initialized;
}


/* Setter Methods */
void Running_process::set_process_pid(pid_t new_pid, bool is_initialized = false,
    double remaining_exec_time = 0.0, bool active = false)
{
    process_pid = new_pid;
    initialized = is_initialized;
    if(initialized == true)
    {
        running = active;
        remaining_execution_time = remaining_exec_time;
    }
    else
    {
        running = false;
        remaining_execution_time = 0.0;
    }
}

void Running_process::set_remaining_execution_time(double new_exec_time)
{
    remaining_execution_time = new_exec_time;
}

void Running_process::set_initialized(bool is_initialized)
{
    initialized = is_initialized;
}

void Running_process::set_running(bool is_running)
{
    running = is_running;
}

/* Control Methods */
void Running_process::start()
{
    running = true;
    kill(process_pid, SIGCONT);
}

void Running_process::stop()
{
    running = false;
    kill(process_pid, SIGSTOP);
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
    
    
    // CHANGE THIS TO A STRUCT TIMEVAL ... timeval.tv_nsec & tv_sec.  
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
}

/* Global Variables */
sig_atomic_t alarm_flag = 0;
sig_atomic_t terminated_pid = -1;

void handle_RR(int signum)
{
    alarm_flag = 1;
}

void handle_term(int signum)
{
    terminated_pid = waitpid(-1, NULL, 0);
}


/* Private Scheduling Routines */
void Scheduler::schedule_RR()
{
    if(scheduleable_processes.size() == 0)
    {
        std::cout << "[" << BOLDRED << "ERROR" << RESET << "]: No processes to schedule!" << std::endl;
        return;
    }

    std::cout << "  [" << BOLDWHITE << "INFO" << RESET << "]: Starting RR processes." << std::endl;

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
    std::cout << "  [" << BOLDWHITE << "INFO" << RESET << "]: Enter commands to schedule them." << std::endl;

    fd_set read_fds;

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

    bool first = true;
    std::cout << "  --> ";
    std::cout.flush();

    bool hang_prompt = true;

    while(true)
    {
        /* Redefine the signal handler each iteration */
        // Don't know if this is a necessary action.  However, I did have issues
        // with the handler being "forgotten"
        sigaction(SIGALRM, &signal_struct_1, NULL);

        std::string command_to_execute = "";
        if(first == true)
        {
            first = false;
        }
        else if(hang_prompt == false)
        {
            std::cout << "  --> ";
            std::cout.flush();
            hang_prompt = true;
        }

        if(terminated_pid != -1 && terminated_pid >= 0)
        {
            // Deal with terminated process
            bool found = false;
            for(unsigned short int i=0; i<scheduleable_processes.size(); i++)
            {
                if(std::get<1>(scheduleable_processes[i])->get_process_pid() == terminated_pid)
                {
                    std::cout << "\b\b\b\b\b\b";
                    std::cout << "  [" << BOLDBLUE << "SUCCESS" << RESET << "]: Process_" << 
                        std::to_string(std::get<0>(scheduleable_processes[i]) + 1) << " terminated successfully." << std::endl;
                    hang_prompt = false;

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
            }

            // Set process_pointer properly
            if((unsigned int)process_pointer >= scheduleable_processes.size())
            {
                process_pointer = scheduleable_processes.size() - 1;
            }

            terminated_pid = -1;
        }

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
        if(result == -1 && errno != EINTR)
        {
            // Problems.  Maybe should do something
        }
        else if(result == -1 && errno == EINTR)
        {
            // Received an interrupt.  Do something about this.  
        }
        else
        {
            if(FD_ISSET(STDIN_FILENO, &read_fds))
            {
                std::getline(std::cin, command_to_execute, '\n');
                hang_prompt = false;

                if(command_to_execute == "exit")
                {
                    // First stop the alarm timer
                    timer_int.it_interval.tv_usec = 0;
                    timer_int.it_interval.tv_sec  = 0;
                    timer_int.it_value.tv_usec = 0;
                    timer_int.it_value.tv_sec  = 0;

                    setitimer(ITIMER_REAL, &timer_int, NULL);

                    return;                   
                }
                else if(command_to_execute == "" || command_to_execute == "\n")
                {
                    continue;
                }

                int return_val = process_shell(command_to_execute);
                if(return_val != -1)
                {
                    std::cout << "  [" << BOLDWHITE << "INFO" << RESET << "]: Process " << return_val << " started." << std::endl;
                    schedule_process(return_val);
                    kill(return_val, SIGCONT);
                    sleep(1);
                }
                else
                {      
                    std::cout << "  [" << BOLDRED << "ERROR" << RESET << "]: Could not execute command." << std::endl;
                }  
            }
        }

        // Keep going if there's nothing to schedule
        if(scheduleable_processes.size() == 0)
        {
            continue;
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
                else if(scheduleable_processes.size() > 1)
                {
                    process_pointer += 1;
                }
                else
                {
                    process_pointer = 0;
                }

                // Start the next process
                pid_t pid_to_start = std::get<1>(scheduleable_processes[process_pointer])->get_process_pid();
                kill(pid_to_start, SIGCONT);
            }
            alarm_flag = 0;
        }
    }
}

void Scheduler::schedule_interactive_FIFO()
{
    std::cout << "  [" << BOLDWHITE << "INFO" << RESET << "]: Enter commands to schedule them." << std::endl;

    fd_set read_fds;

    /* Handler for process termination */
    struct sigaction signal_struct_2;
    signal_struct_2.sa_handler = handle_term;
    signal_struct_2.sa_flags = SA_NOCLDSTOP;
    sigaction(SIGCHLD, &signal_struct_2, NULL);

    // Keep track of currently running process
    int process_pointer = 0;
    bool term = false;

    bool first = true;
    std::cout << "  --> ";
    std::cout.flush();

    bool hang_prompt = true;

    while(true)
    {
        std::string command_to_execute = "";

        if(first == true)
        {
            first = false;
        }
        else if(hang_prompt == false)
        {
            std::cout << "  --> ";
            std::cout.flush();
            hang_prompt = true;
        }

        if(terminated_pid != -1 && terminated_pid >= 0)
        {
            // Deal with terminated process
            bool found = false;
            for(unsigned short int i=0; i<scheduleable_processes.size(); i++)
            {
                if(std::get<1>(scheduleable_processes[i])->get_process_pid() == terminated_pid)
                {
                    std::cout << "\b\b\b\b\b\b";
                    std::cout << "  [" << BOLDBLUE << "SUCCESS" << RESET << "]: Process_" << 
                        std::to_string(std::get<0>(scheduleable_processes[i]) + 1) << " terminated successfully." << std::endl;
                    hang_prompt = false;

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
            }

            // Set process_pointer properly
            if((unsigned int)process_pointer >= scheduleable_processes.size())
            {
                process_pointer = scheduleable_processes.size() - 1;
            }

            terminated_pid = -1;
        }

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
        if(result == -1 && errno != EINTR)
        {
            // Problems.  Maybe should do something
        }
        else if(result == -1 && errno == EINTR)
        {
            // Received an interrupt.  Do something about this.  
        }
        else
        {
            if(FD_ISSET(STDIN_FILENO, &read_fds))
            {
                std::getline(std::cin, command_to_execute, '\n');
                hang_prompt = false;

                if(command_to_execute == "exit")
                {
                    return;
                }
                else if(command_to_execute == "" || command_to_execute == "\n")
                {
                    continue;
                }

                int return_val = process_shell(command_to_execute);
                if(return_val != -1)
                {
                    std::cout << "  [" << BOLDWHITE << "INFO" << RESET << "]: Process " << return_val << " started." << std::endl;
                    schedule_process(return_val);
                    kill(return_val, SIGCONT);
                    sleep(1);
                }
                else
                {      
                    std::cout << "  [" << BOLDRED << "ERROR" << RESET << "]: Could not execute command." << std::endl;
                }  
            }
        }

        // Keep going if there's nothing to schedule
        if(scheduleable_processes.size() == 0)
        {
            continue;
        }


        /* Find the next process */
        Running_process *current_process = std::get<1>(scheduleable_processes[0]);

        // Currently waiting for the process to finish
        if(current_process->is_running())
        {
            usleep(100);
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
}

void Scheduler::schedule_interactive_SJF()
{
    std::cout << "  [" << BOLDWHITE << "INFO" << RESET << "]: Enter commands followed by an execution time to schedule them." \
        << std::endl;

    fd_set read_fds;

    /* Handler for process termination */
    struct sigaction signal_struct_2;
    signal_struct_2.sa_handler = handle_term;
    signal_struct_2.sa_flags = SA_NOCLDSTOP;
    sigaction(SIGCHLD, &signal_struct_2, NULL);

    // Keep track of currently running process
    int process_pointer = 0;
    bool term = false;

    bool first = true;
    std::cout << "  --> ";
    std::cout.flush();

    bool hang_prompt = true;

    pid_t current_pid = -1;

    while(true)
    {
        std::string command_to_execute = "";

        if(first == true)
        {
            first = false;
        }
        else if(hang_prompt == false)
        {
            std::cout << "  --> ";
            std::cout.flush();
            hang_prompt = true;
        }

        if(terminated_pid != -1 && terminated_pid >= 0)
        {
            // Deal with terminated process
            bool found = false;
            for(unsigned short int i=0; i<scheduleable_processes.size(); i++)
            {
                if(std::get<1>(scheduleable_processes[i])->get_process_pid() == terminated_pid)
                {
                    if(current_pid == terminated_pid)
                    {
                        current_pid == -1;
                    }


                    std::cout << "\b\b\b\b\b\b";
                    std::cout << "  [" << BOLDBLUE << "SUCCESS" << RESET << "]: Process_" << 
                        std::to_string(std::get<0>(scheduleable_processes[i]) + 1) << " terminated successfully." << std::endl;
                    hang_prompt = false;

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
            }

            // Set process_pointer properly
            if((unsigned int)process_pointer >= scheduleable_processes.size())
            {
                process_pointer = scheduleable_processes.size() - 1;
            }

            terminated_pid = -1;
        }

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
        if(result == -1 && errno != EINTR)
        {
            // Problems.  Maybe should do something
        }
        else if(result == -1 && errno == EINTR)
        {
            // Received an interrupt.  Do something about this.  
        }
        else
        {
            if(FD_ISSET(STDIN_FILENO, &read_fds))
            {
                std::getline(std::cin, command_to_execute, '\n');
                
                int space_pos = 0;
                for(int i=command_to_execute.length()-1; i>=0; i--)
                {
                    if(command_to_execute[i] == ' ')
                    {
                        space_pos = i;
                        break;
                    }
                }

                string time_string = command_to_execute.substr(i + 1, (command_to_execute.length() - (i + 1)));
                double entered_time = stod(time_string);

                hang_prompt = false;

                if(command_to_execute == "exit")
                {
                    return;
                }
                else if(command_to_execute == "" || command_to_execute == "\n")
                {
                    continue;
                }

                int return_val = process_shell(command_to_execute);
                if(return_val != -1)
                {
                    std::cout << "  [" << BOLDWHITE << "INFO" << RESET << "]: Process " << return_val << " started." << std::endl;

                    schedule_process(return_val, entered_time);
                }
                else
                {      
                    std::cout << "  [" << BOLDRED << "ERROR" << RESET << "]: Could not execute command." << std::endl;
                }  
            }
        }

        // Keep going if there's nothing to schedule
        if(scheduleable_processes.size() == 0)
        {
            continue;
        }


        // CODE TO CHOOSE NEXT PROCESS HERE!!!
        int min_pos = 0;
        double min_expected_time = 999999.99;

        for(unsigned short int i=0; i<schdeuleable_processes.size(); i++)
        {
            double r_execution = std::get<1>(scheduleable_processes[i])->get_remaining_execution_time();
            
            if(r_execution < min_expected_time)
            {
                min_expected_time = r_execution;
                min_pos = i;
            }
        }

        pid_t to_start = std::get<1>(scheduleable_processes[min_pos])->get_process_pid();



    }

    return;

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





    /* ------------------------------------------------------------------------- */
    /* First In First Out                                                        */
    /* ------------------------------------------------------------------------- */
 
    std::cout << std::endl;
    std::cout << BOLDGREEN << "/* -------------------------------------------------------------------------- */" << RESET << std::endl;
    std::cout << BOLDGREEN << "/* First In First Out                                                         */" << RESET << std::endl;
    std::cout << BOLDGREEN << "/* -------------------------------------------------------------------------- */" << RESET << std::endl;
    std::cout << std::endl;

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
        else
        {
            pid = fork();

            if(pid == 0)
            {
                system_string = "./active/process_" + std::to_string(i);
//                return_val = execlp(system_string.c_str(), system_string.c_str(), NULL);
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
                wait(NULL);
                running_processes.push_back(pid);
                std::cout << "  [" << BOLDBLUE << "SUCCESS" << RESET << "]: Created Process_" << std::to_string(i) << std::endl;
            }
        }
    }

    /* Instantiate the scheduler class and add the processes to it */
    Scheduler *fifo_scheduler = new Scheduler();
    for(unsigned short int i=0; i<running_processes.size(); i++)
    {
        fifo_scheduler->schedule_process(running_processes[i]);
    }
    fifo_scheduler->set_policy(FIFO);

    std::cout << "Starting CPU bound scheduler." << std::endl;
//    fifo_scheduler->schedule_all();
    std::cout << std::endl;

    /* Clear running processes vector */
    running_processes.clear();

    /* ------------------------------------------------------------------------- */
    /* Round Robbin                                                              */
    /* ------------------------------------------------------------------------- */
    
    std::cout << std::endl;
    std::cout << BOLDGREEN << "/* -------------------------------------------------------------------------- */" << RESET << std::endl;
    std::cout << BOLDGREEN << "/* Round Robin                                                                */" << RESET << std::endl;
    std::cout << BOLDGREEN << "/* -------------------------------------------------------------------------- */" << RESET << std::endl;
    std::cout << std::endl;

    /* Reinitialize the 5 processes */
    std::cout << "Restarting processes." << std::endl;

    for(unsigned short int i=1; i<=5; i++)
    {
        pid = fork();

        if(pid == 0)
        {
            system_string = "./active/process_" + std::to_string(i);
//            return_val = execlp(system_string.c_str(), system_string.c_str(), NULL);
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
            wait(NULL);
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
//    rr_scheduler->schedule_all();
    std::cout << std::endl;

    /* Clear running processes vector */
    running_processes.clear();

    /* ------------------------------------------------------------------------- */
    /* Shortest Job First                                                        */
    /* ------------------------------------------------------------------------- */
    
    std::cout << std::endl;
    std::cout << BOLDGREEN << "/* -------------------------------------------------------------------------- */" << RESET << std::endl;
    std::cout << BOLDGREEN << "/* Shortest Job First                                                         */" << RESET << std::endl;
    std::cout << BOLDGREEN << "/* -------------------------------------------------------------------------- */" << RESET << std::endl;
    std::cout << std::endl;

    /* Reinitialize the 5 processes */
    std::cout << "Restarting processes." << std::endl;

    for(unsigned short int i=1; i<=5; i++)
    {
        pid = fork();

        if(pid == 0)
        {
            system_string = "./active/process_" + std::to_string(i);
//            return_val = execlp(system_string.c_str(), system_string.c_str(), NULL);
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
            wait(NULL);
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
//    sjf_scheduler->schedule_all();
    std::cout << std::endl;

    /* Clear the running process vector */
    running_processes.clear();

    /* ------------------------------------------------------------------------- */
    /* Interactive Round Robin                                                   */
    /* ------------------------------------------------------------------------- */
 
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

    /* ------------------------------------------------------------------------- */
    /* Interactive First In First Out                                            */
    /* ------------------------------------------------------------------------- */
 
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





    cleanup();
    return 0;
}
