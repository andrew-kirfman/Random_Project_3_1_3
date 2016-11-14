/* --------------------------------------------------------------------------- */
/* Project: CSCE-313 Machine Problem #6                                        */
/* Developer: Andrew Kirfman												   */
/*                                                                             */
/* File: ./MP6/Running_process.h                                               */
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
