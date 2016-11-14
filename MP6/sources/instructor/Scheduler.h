/* --------------------------------------------------------------------------- */
/* Project: CSCE-313 Machine Problem #6                                        */
/* Developer: Andrew Kirfman												   */
/*                                                                             */
/* File: ./MP6/Scheduler.h                                                     */
/* --------------------------------------------------------------------------- */

#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

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
#include "Running_process.h"
#include "shell.h"


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
    void schedule_process(pid_t process_pid);
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

#endif
