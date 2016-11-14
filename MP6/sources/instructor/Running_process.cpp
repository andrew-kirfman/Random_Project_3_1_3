/* --------------------------------------------------------------------------- */
/* Project: CSCE-313 Machine Problem #6                                        */
/* Developer: Andrew Kirfman												   */
/*                                                                             */
/* File: ./MP6/Running_process.cpp                                             */
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

#include "Running_process.h"

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
    return process_pid;
}

double Running_process::get_remaining_execution_time()
{
    return (double) remaining_execution_time;
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

/* Methods for SJF  */
void Running_process::recalculate_remaining_time()
{
    if(last_start_time.tv_sec == 0 && last_start_time.tv_nsec == 0)
    {
        return;
    }

    struct timespec curr_time;
    clock_gettime(CLOCK_MONOTONIC, &curr_time);

    struct timespec time_difference = diff(last_start_time, curr_time);

    double passed_time = double(time_difference.tv_sec) + double(time_difference.tv_nsec)/1000000000.00;

    if(double(remaining_execution_time) - double(passed_time) < 0.0)
    {
        remaining_execution_time = 0.0;
    }
    else
    {
        remaining_execution_time = double(remaining_execution_time) - double(passed_time);
    }

}

void Running_process::set_time_structure(struct timespec t1)
{
    last_start_time.tv_sec = t1.tv_sec;
    last_start_time.tv_nsec = t1.tv_nsec;
}
