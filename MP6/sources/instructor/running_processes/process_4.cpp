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

    return;
}

std::vector<int>* generate_list(int size)
{
    std::vector<int>* list = new std::vector<int>();
    list->reserve(size);
    for(int i=0; i<size; i++)
    {
        list->push_back(rand() % INT_MAX);
    }

    return list;
}


int main()
{
    kill(getpid(), SIGSTOP);

    int list_size = 2 << 15;
    std::vector<int> list_to_sort = *generate_list(list_size);
    sort_list(list_to_sort);
    list_to_sort.clear();

    return 0;
}




