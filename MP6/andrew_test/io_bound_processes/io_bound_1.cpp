/* --------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman, Margaret Baxter                                  */
/* Project: CSCE-313 Machine Problem #1                                        */
/*                                                                             */
/* File: ./MP6/cpu_bound_processes/cpu_bound_2.cpp                             */
/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/* Standard Library Includes                                                   */
/* --------------------------------------------------------------------------- */

#include<iostream>
#include<sys/types.h>
#include<signal.h>
#include<unistd.h>
#include<stdio.h>

void handle_start(int sig_num)
{
    _exit(255);   
}

int main()
{
    struct sigaction start_action;
    start_action.sa_handler = handle_start;
    sigaction(SIGCONT, &start_action, NULL);

    raise(SIGSTOP);

    while(true)
    {
        usleep(100000);
    }

    exit(0);
}




