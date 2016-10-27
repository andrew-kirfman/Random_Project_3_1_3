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
#include<math.h>
#include<stdio.h>
#include<string.h>
#include<string>
#include<cstdlib>
#include<vector>

/* Sieve of Erastosthenes is a fast algorithm for calculating prime numbers */
void runEratosthenesSieve(int upperBound) 
{
    int count = 1;
    std::vector<bool> arr;

    for(int i=0; i<upperBound; i++)
    {
        arr.push_back(true);
    }

    for(int i=2; i<upperBound - 1; i++)
    {
        if(count == upperBound)
        {
            break;
        }

        // Mark all multiples
        for(int j=2; (j*i) < upperBound-1; j++)
        {
            arr[i*j] = false;
        }

        // Find next prime
        for(int k=(i + 1); k<upperBound-1; k++)
        {
            if(arr[k])
            {
                i=k;
                ++count;
                break;
            }
        }
    }

}


void handle_start(int sig_num)
{
    int prime_max = 2 << 24;
    runEratosthenesSieve(prime_max);
}


int main()
{
    struct sigaction start_action;
    start_action.sa_handler = handle_start;
    sigaction(SIGCONT, &start_action, NULL);

    raise(SIGSTOP);

    exit(0);
}
