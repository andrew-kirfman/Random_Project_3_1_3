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
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<time.h>
#include<math.h>
#include<utility>

/* --------------------------------------------------------------------------- */
/* User Defined Includes                                                       */
/* --------------------------------------------------------------------------- */

#include "utility.h"

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

class Scheduler
{
public:

    /* Constructor */
    Scheduler();

    /* Utility methods */
    void schedule_process(pid_t process_pid, double expected_run_time);
    
    /* Setter Methods */
    void set_policy(int policy);

    /* Public Scheduling Routines */
    void schedule_all();

private:
    int scheduling_policy;
    int processes_scheduled;
    std::vector<std::pair<int, Running_process*>> scheduleable_processes;

    /* Private Scheduling Routines */
    void schedule_RR();
    void schedule_FIFO();
    void schedule_SJF();
};

/* Constructor */
Scheduler::Scheduler()
{
    processes_scheduled = 0;
    scheduling_policy = NO_POLICY;
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
        || policy == FIFO || policy == SJF)
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
    else if(scheduling_policy == NO_POLICY)
    {

    }
}

/* Global Variables */
sig_atomic_t signal_flag = 0;
sig_atomic_t terminated_pid = -1;

void handle_RR(int signum)
{
    signal_flag = 1;
}

void handle_term(int signum)
{
    terminated_pid = waitpid(-1, NULL, 0);
}


/* Private Scheduling Routines */
void Scheduler::schedule_RR()
{
    std::cout << "[INFO]: Starting RR processes." << std::endl;



    return;
}


void Scheduler::schedule_FIFO()
{
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
                    /* Note:
                     * The program does not work without these next three lines of code.  
                     * I have no idea why it makes a difference.  Spent hours trying to figure it out.
                     * Someone should probably figure out the problem someday.  
                     */
                    //std::cout.setstate(std::ios_base::failbit);
                    //std::cout << "Flag3 " << std::to_string(terminated_pid) << std::endl;
                    //std::cout.clear();

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
        }
    }

    return;
}

void Scheduler::schedule_SJF()
{
    
    return;
}


int main()
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

    std::cout << std::endl;
    std::cout << BOLDGREEN << "/* -------------------------------------------------------------------------- */" << RESET << std::endl;
    std::cout << BOLDGREEN << "/* Setup                                                                      */" << RESET << std::endl;
    std::cout << BOLDGREEN << "/* -------------------------------------------------------------------------- */" << RESET << std::endl;
    std::cout << std::endl;

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

    /* Start CPU bound processes */
    std::cout << "Starting processes." << std::endl;
    
    for(unsigned short int i=1; i<=5; i++)
    {
        // RESTART HERE!!!
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
                return_val = execlp(system_string.c_str(), system_string.c_str(), NULL);
            }
            else if(pid == -1)
            {
                std::cout << "  [" << BOLDRED << "ERROR" << RESET << "]: Could not create Process_" << std::to_string(i) << std::endl
                    << "    " << strerror(errno) << std::endl;
                cleanup();
            }
            else
            {
                sleep(1);
                running_processes.push_back(pid);
                std::cout << "  [" << BOLDBLUE << "SUCCESS" << RESET << "]: Created Process_" << std::to_string(i) << std::endl;
            }
        }
    }

    /* ------------------------------------------------------------------------- */
    /* First In First Out                                                        */
    /* ------------------------------------------------------------------------- */
 
    std::cout << std::endl;
    std::cout << BOLDGREEN << "/* -------------------------------------------------------------------------- */" << RESET << std::endl;
    std::cout << BOLDGREEN << "/* First In First Out                                                         */" << RESET << std::endl;
    std::cout << BOLDGREEN << "/* -------------------------------------------------------------------------- */" << RESET << std::endl;
    std::cout << std::endl;

    /* Instantiate the scheduler class and add the processes to it */
    Scheduler *process_scheduler = new Scheduler();
    for(unsigned short int i=0; i<running_processes.size(); i++)
    {
        process_scheduler->schedule_process(running_processes[i]);
    }
    process_scheduler->set_policy(FIFO);

    /* Create CPU Bound Processes */
    std::cout << "Starting CPU bound scheduler." << std::endl;
    process_scheduler->schedule_all();


    return 0;
}



int thing1()
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

    std::cout << std::endl;
    std::cout << BOLDGREEN << "/* -------------------------------------------------------------------------- */" << RESET << std::endl;
    std::cout << BOLDGREEN << "/* Setup                                                                      */" << RESET << std::endl;
    std::cout << BOLDGREEN << "/* -------------------------------------------------------------------------- */" << RESET << std::endl;
    std::cout << std::endl;

    /* Reused Variables */
    pid_t pid = 0;
    int return_val = 0;
    struct sched_param scheduling_struct;
    std::string system_string = "";

    /* Clear old occurrences of the temp directories */
    return_val = system("rm -rf ./running_processes 2>&1 > /dev/null");
    if(return_val == -1)
    {
        std::cout << "[ERROR]: Cound not remove old running_processes directory." << std::endl;
    }

    return_val = system("rm -rf ./running_processes 2>&1 > /dev/null");
    if(return_val == -1)
    {
        std::cout << "[ERROR]: Cound not remove old running_processes directory." << std::endl;
    }

    /* Make a directory to write executable files to */
    mkdir("./running_processes", S_IRWXU | S_IRWXG | S_IRWXO);
    mkdir("./io_bound", S_IRWXU | S_IRWXG | S_IRWXO);

    /* Start CPU bound processes */
    std::cout << "Starting CPU bound processes." << std::endl;
    
    for(unsigned short int i=1; i<=5; i++)
    {
        system_string = "g++ -std=c++11 ./running_processes_processes/running_processes_" + std::to_string(i) + ".cpp -o ./running_processes/cpu_" + std::to_string(i);
        return_val = system(system_string.c_str());
        if(return_val == -1)
        {
            std::cout << "  [" << BOLDRED << "ERROR" << RESET << "]: Could not create CPU_" << std::to_string(i) << std::endl
                << "    " << strerror(errno) << std::endl;
            cleanup();
        }
        else
        {
            pid = fork();

            if(pid == 0)
            {
                system_string = "./running_processes/cpu_" + std::to_string(i);
                return_val = execlp(system_string.c_str(), system_string.c_str(), NULL);
            }
            else if(pid == -1)
            {
                std::cout << "  [" << BOLDRED << "ERROR" << RESET << "]: Could not create CPU_" << std::to_string(i) << std::endl
                    << "    " << strerror(errno) << std::endl;
                cleanup();
            }
            else
            {
                running_processes.push_back(pid);
                std::cout << "  [" << BOLDBLUE << "SUCCESS" << RESET << "]: Created CPU_" << std::to_string(i) << std::endl;
            }
        }
    }
    std::cout << std::endl;

    /* ------------------------------------------------------------------------- */
    /* Round Robbin                                                              */
    /* ------------------------------------------------------------------------- */
    
    std::cout << std::endl;
    std::cout << BOLDGREEN << "/* -------------------------------------------------------------------------- */" << RESET << std::endl;
    std::cout << BOLDGREEN << "/* Round Robin                                                                */" << RESET << std::endl;
    std::cout << BOLDGREEN << "/* -------------------------------------------------------------------------- */" << RESET << std::endl;
    std::cout << std::endl;

    /* Set scheduling priority for cpu-bound processes */
    int max_priority = sched_get_priority_max(SCHED_RR);

    // Make sure that the parent process is at the highest priority!
    scheduling_struct.sched_priority = max_priority;
    return_val = sched_setscheduler(getpid(), SCHED_RR, &scheduling_struct);
    if(return_val == -1)
    {
        std::cout << "[" << BOLDRED << "ERROR" << RESET << "]: Could not properly set scheduling priority for parent process." << std::endl
            << "    " << strerror(errno) << std::endl;
        cleanup();
    }
    else
    {
        std::cout << "[" << BOLDBLUE << "SUCCESS" << RESET << "]: Set the parent's scheduling process to Round Robin." << std::endl;
    }
    std::cout << std::endl;

    // Now also set the children
    scheduling_struct.sched_priority = max_priority - 1;

    std::cout << "Setting Child scheduling policies." << std::endl;
    for(unsigned short int i=1; i<=5; i++)
    {
        return_val = sched_setscheduler(running_processes[i-1], SCHED_RR, &scheduling_struct);
        if(return_val == -1)
        {
            std::cout << "  [" << BOLDRED << "ERROR" << RESET << "]: Could not set scheduling priority for cpu_" << std::to_string(i) << "." << std::endl
                << "      " << strerror(errno) << std::endl;
            cleanup();
        }
        else
        {
            std::cout << "  [" << BOLDBLUE << "SUCCESS" << RESET << "]: Set CPU_" << std::to_string(i) << " scheduling policy to round robin." << std::endl;     
        }
    }
    std::cout << std::endl;

    /* Keep time during program runs */
    struct timespec run_time;
    struct timespec snapshot_time;
    clock_gettime(CLOCK_MONOTONIC, &run_time);
    time_t seconds;
    long nanoseconds;

    /* Start the CPU bound processes */
    struct timespec rr_time;
    return_val = sched_rr_get_interval(running_processes[0], &rr_time);

    std::cout << "Starting cpu bound processes in order from 1 through 5." << std::endl;
    if(return_val != -1)
    {
        std::cout << "  Round robin time quantum: " << std::endl
            << "    - seconds: " << rr_time.tv_sec << std::endl
            << "    - nanoseconds: " << rr_time.tv_nsec << std::endl
            << std::endl;
    }
    else
    {
        std::cout << "[ERROR]: Could not determine round robin time quantum." << std::endl
            << "    " << strerror(errno) << std::endl;
    }
    for(unsigned short int i=0; i<running_processes.size(); i++)
    {
        return_val = kill(running_processes[i], SIGCONT);

        if(return_val == -1)
        {
            std::cout << "  [" << BOLDRED << "ERROR" << RESET << "]: Could not wake up CPU_" << std::to_string(i + 1) << "." << std::endl
                << "    " << strerror(errno) << std::endl;
            cleanup();
        }

        // Ensure that the processes are scheduled in the right order 
        // The sleep is a slightly flawed way of making sure that all processes
        // receive their designated signals in the right order
        usleep(10000);
    }

    unsigned short int wait_count = 0;
    while(wait_count < running_processes.size())
    {
        for(unsigned short int i=0; i<running_processes.size(); i++)
        {
            return_val = waitpid(running_processes[i], NULL, WNOHANG);
            if(return_val != 0 && return_val != -1)
            {
                std::cout << "  [" << BOLDBLUE << "SUCCESS" << RESET << "]: Process " << std::to_string(i + 1) << " completed!  ";
                clock_gettime(CLOCK_MONOTONIC, &snapshot_time);
                seconds = snapshot_time.tv_sec - run_time.tv_sec;
                if(run_time.tv_nsec > snapshot_time.tv_nsec)
                {
                    nanoseconds = snapshot_time.tv_nsec + (1*pow(10, 9) - run_time.tv_nsec);
                    seconds -= 1;
                }
                else
                {
                    nanoseconds = snapshot_time.tv_nsec - run_time.tv_nsec;
                }
                std::cout << " Finish Time: " << seconds << " seconds, " << nanoseconds << " nanoseconds." << std::endl;
                wait_count += 1;
            }
            usleep(100000);
        }
    }
    std::cout << std::endl;
 
    /* Clean up at the very end of round robin */
    running_processes.clear();

    /* ------------------------------------------------------------------------- */
    /* First In First Out                                                        */
    /* ------------------------------------------------------------------------- */
 
    std::cout << std::endl;
    std::cout << BOLDGREEN << "/* -------------------------------------------------------------------------- */" << RESET << std::endl;
    std::cout << BOLDGREEN << "/* First In First Out                                                         */" << RESET << std::endl;
    std::cout << BOLDGREEN << "/* -------------------------------------------------------------------------- */" << RESET << std::endl;
    std::cout << std::endl;

    /* Create CPU Bound Processes */
    std::cout << "Starting CPU bound proccesses." << std::endl;
    for(unsigned short int i=1; i<=5; i++)
    {
        pid = fork();

        if(pid == 0)
        {
            system_string = "./running_processes/cpu_" + std::to_string(i);
            execlp(system_string.c_str(), system_string.c_str(), NULL);
        }
        else if(pid == -1)
        {
            std::cout << "  [" << BOLDRED << "ERROR" << RESET << "]: Could not create CPU_" << std::to_string(i) << std::endl
                << "    " << strerror(errno) << std::endl;
            cleanup();
        }
        else
        {
            std::cout << "  [" << BOLDBLUE << "SUCCESS" << RESET << "]: Created CPU_" << std::to_string(i) << std::endl;
            running_processes.push_back(pid);
        }
    }
    std::cout << std::endl;


    /* Set scheduling priority to fifo for CPU bound processes */
    
    // Note: There is no need to worry about a priority or niceness value here.  
    // First in first out only cares about the order in which the processes arrive.  
    scheduling_struct.sched_priority = sched_get_priority_max(SCHED_FIFO);
    
    return_val = sched_setscheduler(getpid(), SCHED_FIFO, &scheduling_struct);
    if(return_val == -1)
    {
        std::cout << "[" << BOLDRED << "ERROR" << RESET << "]: Could not properly set scheduling priority for parent process." << std::endl
            << "    " << strerror(errno) << std::endl;
        cleanup();
    }
    else
    {
        std::cout << "[" << BOLDBLUE << "SUCCESS" << RESET << "]: Set the parent's scheduling policy to FIFO." << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Setting child scheduling policies." << std::endl;
    scheduling_struct.sched_priority = scheduling_struct.sched_priority - 1;
    for(unsigned short int i=0; i<running_processes.size(); i++)
    {
        return_val = sched_setscheduler(running_processes[i], SCHED_FIFO, &scheduling_struct);

        if(return_val == -1)
        {
            std::cout << "  [" << BOLDRED << "ERROR" << RESET << "]: Could not set scheduling policy for CPU_" << std::to_string(i) << "." << std::endl
                << "    " << strerror(errno) << std::endl;
            cleanup();
        }
        else
        {
            std::cout << "  [" << BOLDBLUE << "SUCCESS" << RESET << "]: Set CPU_" << std::to_string(i) << " scheduling policy to fifo." << std::endl;
        }
    }   
    std::cout << std::endl;

    /* Start CPU bound processes */
    std::cout << "Starting cpu bound processes in order from 1 through 5." << std::endl;
    clock_gettime(CLOCK_MONOTONIC, &run_time);

    for(unsigned short int i=0; i<running_processes.size(); i++)
    {
        return_val = kill(running_processes[i], SIGCONT);

        if(return_val == -1)
        {
            std::cout << "  [" << BOLDRED << "ERROR" << RESET << "]: Could not wake up CPU_" << std::to_string(i + 1) << "." << std::endl
                << "    " << strerror(errno) << std::endl;
            cleanup();
        }

        usleep(10000);
    }

    // Remember Definition: unsigned short int wait_count = 0
    wait_count = 0;
    while(wait_count < 5)
    {
        pid_t returned_pid = wait(NULL);

        for(unsigned short int i=0; i<running_processes.size(); i++)
        {
            if(returned_pid == running_processes[i])
            {
                std::cout << "  [" << BOLDBLUE << "SUCCESS" << RESET << "]: Process " << std::to_string(i + 1) << " completed!";
                clock_gettime(CLOCK_MONOTONIC, &snapshot_time);
                seconds = snapshot_time.tv_sec - run_time.tv_sec;
                if(run_time.tv_nsec > snapshot_time.tv_nsec)
                {
                    nanoseconds = snapshot_time.tv_nsec + (1*pow(10, 9) - run_time.tv_nsec);
                    seconds -= 1;
                }
                else
                {
                    nanoseconds = snapshot_time.tv_nsec - run_time.tv_nsec;
                }
                std::cout << "   Finish Time: " << seconds << " seconds, " << nanoseconds << " nanoseconds." << std::endl;
                
                wait_count += 1;
            }
        }
    }


    std::cout << std::endl;
    cleanup();
    return 0;
}
