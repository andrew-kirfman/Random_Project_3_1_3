/* --------------------------------------------------------------------------- */
/* Project: CSCE-313 Machine Problem #6                                        */
/* Developer: Andrew Kirfman												   */
/*                                                                             */
/* File: ./MP6/Running_process.h                                               */
/* --------------------------------------------------------------------------- */

#ifndef _RUNNING_PROCESS_H_
#define _RUNNING_PROCESS_H_

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

#endif
