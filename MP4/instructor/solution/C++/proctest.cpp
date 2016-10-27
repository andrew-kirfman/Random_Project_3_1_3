/* ------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman                                                 */
/* Project: CSCE-313 Episode #4 Part 2                                       */
/*                                                                           */
/* File: ./proctest.cpp                                                      */
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
/* Standard Library Includes                                                 */
/* ------------------------------------------------------------------------- */

#include<iostream>
#include<vector>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<string>
#include<regex>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>

/* ------------------------------------------------------------------------- */
/* Standard Library Includes                                                 */
/* ------------------------------------------------------------------------- */

#include "proctest.h"


int main()
{
    /* Read in PID */
    int pid;
    std::cout << "Enter the pid of a process: ";
    std::cin >> pid;

    while(std::cin.fail())
    {
        std::cout << "[ERROR]: PID is not an integer.  Try again: ";
        std::cin >> pid;
    }
    
    /* Check a process with that pid actually exists in the system */
    std::string fname = "";
    while(true)
    {
        fname = "/proc/" + std::to_string(pid) + "/stat";

        struct stat test_buffer;
        if(stat(fname.c_str(), &test_buffer) != 0)
        {
            std::cout << "Process does not exist.  Try again: ";
            std::cin >> pid;

            while(std::cin.fail())
            {
                std::cout << "[ERROR]: PID is not an integer.  Try again: ";
                std::cin >> pid;
            }
        }
        else
        {
            break;
        }
    }

    Proctest *process_data = new Proctest(pid);

    /* Print everything */
    std::cout << "Process Information: " << std::endl;
    std::cout << "  1) Identifiers" << std::endl;
    std::cout << "     PID: " << process_data->getpid() << std::endl;
    std::cout << "     PPID: " << process_data->getppid() << std::endl;
    std::cout << "     EUID: " << process_data->geteuid() << std::endl;
    std::cout << "     EGID: " << process_data->getegid() << std::endl;
    std::cout << "     RUID: " << process_data->getruid() << std::endl;
    std::cout << "     RGID: " << process_data->getrgid() << std::endl;
    std::cout << "     FSUID: " << process_data->getfsuid() << std::endl;
    std::cout << "     FSGID: " << process_data->getfsgid() << std::endl;
    std::cout << std::endl;
    std::cout << "  2) State" << std::endl;
    std::cout << "     State: " << process_data->getstate() << std::endl;
    std::cout << std::endl;
    std::cout << "  3) Thread Information" << std::endl;
    std::cout << "     Thread Count: " << process_data->getthread_count() << std::endl;
    std::cout << std::endl;
    std::cout << "  4) Priority" << std::endl;
    std::cout << "     Priority Number: " << process_data->getpriority() << std::endl;
    std::cout << "     Niceness Value: " << process_data->getniceness() << std::endl;
    std::cout << std::endl;
    std::cout << "  5) Time Information" << std::endl;
    std::cout << "     stime: " << process_data->getstime() << std::endl;
    std::cout << "     utime: " << process_data->getutime() << std::endl;
    std::cout << "     cstime: " << process_data->getcstime() << std::endl;
    std::cout << "     cutime: " << process_data->getcutime() << std::endl;
    std::cout << std::endl;
    std::cout << "  6) Address Space" << std::endl;
    std::cout << "     Startcode: " << process_data->getstartcode() << std::endl;
    std::cout << "     Endcode: " << process_data->getendcode() << std::endl;
    std::cout << "     ESP: " << process_data->getesp() << std::endl;
    std::cout << "     EIP: " << process_data->geteip() << std::endl;
    std::cout << std::endl;
    std::cout << "  7) Resources" << std::endl;
    std::cout << "     File Handles: " << process_data->getfiles() << std::endl;
    std::cout << "     Voluntary Context Switches: " << process_data->getvoluntary_context_switches() << std::endl;
    std::cout << "     Nonvoluntary Context Switches: " << process_data->getnonvoluntary_context_switches() << std::endl;
    std::cout << std::endl;
    std::cout << "  8) Processor" << std::endl;
    std::cout << "     Last Processor: " << process_data->getlast_cpu() << std::endl;
    std::cout << "     Allowed Cores: " << process_data->getallowed_cpus() << std::endl;
    std::cout << std::endl;
    std::cout << "  9) Memory" << std::endl;
    std::vector<std::string> temp_mem_array = process_data->getmemory_map();
    for(unsigned short int i=0; i<temp_mem_array.size(); i++)
    {
        std::cout << "     " << temp_mem_array[i] << std::endl;
    }

    return 0;
}
