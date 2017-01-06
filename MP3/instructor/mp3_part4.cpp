/* -------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman                                                  */
/* Project: CSCE-313 Machine Problem #3                                       */
/*                                                                            */
/* File: ./mp3_part4.cpp                                                      */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* Standard Library Includes                                                  */
/* -------------------------------------------------------------------------- */

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>


int main()
{
    int ret_val = 0;
    
    // execv expects the arguments to be stored in a NULL terminated vector
    char** argv = new char*[3];

    argv[0] = (char*) malloc(strlen("ls"));
    strcpy(argv[0], "ls");

    argv[1] = (char*) malloc(strlen("-la"));
    strcpy(argv[1], "-la");

    argv[2] = NULL;

    ret_val == execvp(argv[0], argv);

    // execv won't return if it's successfull
    if(ret_val == -1)
    {
        std::cout << "Execv Failed!" << std::endl;
    }

    // Terminate Unsuccessfully
    exit(255);
}

