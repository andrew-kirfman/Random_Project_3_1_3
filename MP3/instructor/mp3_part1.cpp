/* -------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman                                                  */
/* Project: CSCE-313 Machine Problem #3                                       */
/*                                                                            */
/* File: ./mp3_part1.cpp                                                      */
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

    // execl won't return if it is successful
    ret_val = execl("/bin/ls", "/bin/ls", "-la", NULL);

    if(ret_val == -1)
    {
        std::cerr << "Execl Failed!" << std::endl;
    }

    // Terminate unsuccessfully
    exit(255);
}
