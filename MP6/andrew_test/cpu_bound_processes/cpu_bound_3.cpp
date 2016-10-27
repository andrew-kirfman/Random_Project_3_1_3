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

/* O(n^3) loop that iterates a large number of times */
void tight_loop(int loop_size)
{
    int64_t increment;   
    for(int i=0; i<loop_size; i++)
    {
        for(int j=0; j<loop_size; j++)
        {
            for(int k=0; k<loop_size; k++)
            {
                increment++;
            }
        }
    }
}

void handle_start(int sig_num)
{
    int input_size = 2 << 9;
    tight_loop(input_size);
}

int main()
{
    struct sigaction start_action;
    start_action.sa_handler = handle_start;
    sigaction(SIGCONT, &start_action, NULL);

    raise(SIGSTOP);

    exit(0);
}




