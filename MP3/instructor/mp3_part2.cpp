/* -------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman                                                  */
/* Project: CSCE-313 Machine Problem #3                                       */
/*                                                                            */
/* File: ./mp3_part2.cpp                                                      */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* Standard Library Includes                                                  */
/* -------------------------------------------------------------------------- */

#include <stdlib.h>
#include <unistd.h>
#include <iostream>


int main()
{
    int ret_val = 0;

    // execlp won't return if it is successful
    ret_val = execlp("ls", "ls", "-la", NULL);

    if(ret_val == -1)
    {
        std::cerr << "Execlp Failed!" << std::endl;
    }

    // Terminate unsuccessfully
    exit(255);
}

