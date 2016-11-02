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


int main()
{
    raise(SIGSTOP);

    std::cout << "STARTING TIGHT LOOP (3)" << std::endl;
    int input_size = 2 << 9;
    tight_loop(input_size);

    exit(0);
}




