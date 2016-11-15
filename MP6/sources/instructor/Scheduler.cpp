/* --------------------------------------------------------------------------- */
/* Project: CSCE-313 Machine Problem #6                                        */
/* Developer: Andrew Kirfman												   */
/*                                                                             */
/* File: ./MP6/Scheduler.cpp                                                   */
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

#include "Scheduler.h"

/* Constructor */
Scheduler::Scheduler()
{
    processes_scheduled = 0;
    scheduling_policy = NO_POLICY;

    // Set a default time quantum (10 ms is default)
    time_quantum = 10000;
}


/* Utility Methods */
void Scheduler::schedule_process(pid_t process_pid)
{
    Running_process *process = new Running_process(process_pid, 0.0, false);
    
    std::pair<int, Running_process*> new_process(processes_scheduled, process);
    processes_scheduled += 1;

    scheduleable_processes.push_back(new_process);
}


void Scheduler::schedule_process(pid_t process_pid, double expected_run_time)
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
sig_atomic_t alarm_flag = 0;
sig_atomic_t terminated_pid = -1;

void handle_RR(int signum)
{	
    struct sigaction signal_struct_1;
    signal_struct_1.sa_handler = handle_RR;
    sigaction(SIGALRM, &signal_struct_1, NULL);
	
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
					int terminated_number = std::get<0>(scheduleable_processes[i]);
                    scheduleable_processes.erase(scheduleable_processes.begin() + i);
                    found = true;

                    terminated_pid = -1;

                    std::cout << "  [" << BOLDBLUE << "SUCCESS" << RESET << "]: Process_" << 
                        std::to_string(terminated_number + 1) << " terminated successfully." << std::endl;
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
					int terminated_number = std::get<0>(scheduleable_processes[i]);
                    scheduleable_processes.erase(scheduleable_processes.begin() + i);
                    found = true;

                    terminated_pid = -1;

                    std::cout << "  [" << BOLDBLUE << "SUCCESS" << RESET << "]: Process_" << 
                        std::to_string(terminated_number + 1) << " terminated successfully." << std::endl;
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

    bool first = true;
    std::cout << "  --> ";
    std::cout.flush();

    bool hang_prompt = true;

    pid_t current_pid = -1;

    // get starting time
    struct timespec clock_time;
    clock_gettime(CLOCK_MONOTONIC, &clock_time);

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
                        current_pid = -1;
                    }

                    std::cout << "\b\b\b\b\b\b";
                    std::cout << "  [" << BOLDBLUE << "SUCCESS" << RESET << "]: Process_" << 
                        std::to_string(std::get<0>(scheduleable_processes[i]) + 1) << " terminated successfully." << std::endl;
                    hang_prompt = false;

                    // Set appropriate flags
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

                if(command_to_execute == "exit")
                {
                    return;
                }
                else if(command_to_execute == "" || command_to_execute == "\n")
                {
                    hang_prompt = false;
                    continue;
                }


                int space_pos = 0;
                for(int i=command_to_execute.length()-1; i>=0; i--)
                {
                    if(command_to_execute[i] == ' ')
                    {
                        space_pos = i;
                        break;
                    }
                }
                
                double entered_time = 0.0;
                try
                {
                    std::string time_string = command_to_execute.substr(space_pos + 1, (command_to_execute.length() - (space_pos + 1)));
                    command_to_execute = command_to_execute.substr(0, space_pos);
                    entered_time = stod(time_string);
                }
                catch(std::exception &e)
                {
                    continue;
                }

                hang_prompt = false;

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

        if(scheduleable_processes.size() == 1 && current_pid == -1)
        {
            pid_t process_pid = std::get<1>(scheduleable_processes[0])->get_process_pid();

            struct timespec start_time;
            clock_gettime(CLOCK_MONOTONIC, &start_time);
            std::get<1>(scheduleable_processes[0])->set_time_structure(start_time);

            kill(process_pid, SIGCONT);
            current_pid = process_pid;

            continue;
        }
        else if(scheduleable_processes.size() == 1 && current_pid != std::get<1>(scheduleable_processes[0])->get_process_pid())
        {
            pid_t process_pid = std::get<1>(scheduleable_processes[0])->get_process_pid();

            struct timespec start_time;
            clock_gettime(CLOCK_MONOTONIC, &start_time);
            std::get<1>(scheduleable_processes[0])->set_time_structure(start_time);

            kill(process_pid, SIGCONT);
            current_pid = process_pid;

            continue;
        }

        int min_pos = 0;
        double min_expected_time = 999999.99;

        for(unsigned short int i=0; i<scheduleable_processes.size(); i++)
        {
            Running_process *p1 = std::get<1>(scheduleable_processes[i]);

            if(p1->get_process_pid() == current_pid)
            {
                p1->recalculate_remaining_time();
                struct timespec t1;
                clock_gettime(CLOCK_MONOTONIC, &t1);
                p1->set_time_structure(t1);
            }

            double r_execution = std::get<1>(scheduleable_processes[i])->get_remaining_execution_time();

            if(r_execution < min_expected_time)
            {
                min_expected_time = r_execution;
                min_pos = i;
            }
        }

        if(min_expected_time == 0)
        {
            return;
        }

        pid_t to_start = std::get<1>(scheduleable_processes[min_pos])->get_process_pid();
        if(to_start != current_pid)
        {
            std::cout << "  [" << BOLDWHITE << "INFO" << RESET << "]: Preempting a process to start another that is faster" << std::endl;
            kill(current_pid, SIGSTOP);

            struct timespec current_time;
            clock_gettime(CLOCK_MONOTONIC, &current_time);
            std::get<1>(scheduleable_processes[min_pos])->set_time_structure(current_time);

            kill(to_start, SIGCONT);

            current_pid = to_start;
        }
        else
        {
            continue;
        }
    }

    return;
}
