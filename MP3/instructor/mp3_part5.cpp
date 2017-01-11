/* -------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman                                                  */
/* Project: CSCE-313 Machine Problem #3                                       */
/*                                                                            */
/* File: ./mp3_part5.cpp                                                      */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* Standard Library Includes                                                  */
/* -------------------------------------------------------------------------- */

#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>


int main()
{
    pid_t pid = fork();

    if(pid == -1)
    {
        std::cerr << "Fork Failed!" << std::endl;

        // Terminate with failure
        exit(255);
    }
    else if(pid == 0)
    {
        std::cout << "Hello";   
    }
    else
    {
        // Ensure that the child executes befor ethe parent
        waitpid(pid, NULL, 0);
        std::cout << "World!" << std::endl;
    }

    exit(0);
}

