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
#include<time.h>
#include<vector>
#include<climits>

void sort_list(std::vector<int>& list)
{
    std::cout << "Got Here!" << std::endl;

    int num_swaps = 0;
    int placeholder = 0;
    int num_iterations = 0;
    while(true)
    {
        if(num_iterations == list.size() - 1)
        {
            break;
        }

        for(int i=0; i<(list.size() - num_iterations - 1); i++)
        {
            std::cout << "Flag1" << std::endl;
            if(list[i] > list[i + 1])
            {
                placeholder = list[i + 1];
                list[i + 1] = list[i];
                list[i] = placeholder;

                num_swaps += 1;
            }
        }

        if(num_swaps == 0)
        {
            break;
        }

        num_iterations += 1;
    }

}

std::vector<int> generate_list(int size)
{
    std::vector<int> list;
    for(int i=0; i<size; i++)
    {
        list.push_back(rand() % INT_MAX);
    }
}


void handle_start(int sig_num)
{
    int list_size = 2 << 10;
    std::vector<int> list_to_sort = generate_list(list_size);
    sort_list(list_to_sort);
}

int main()
{
    int list_size = 2 << 20;
    std::vector<int> list_to_sort = generate_list(list_size);
    sort_list(list_to_sort);
    
    exit(0);

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




