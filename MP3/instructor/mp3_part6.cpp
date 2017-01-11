/* -------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman                                                  */
/* Project: CSCE-313 Machine Problem #3                                       */
/*                                                                            */
/* File: ./mp3_part6.cpp                                                      */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* Standard Library Includes                                                  */
/* -------------------------------------------------------------------------- */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>


int main()
{
    // Create a pipe to be shared between the parent and child
    int communication_pipe[2];
    pipe(communication_pipe);

    pid_t pid = fork();

    if(pid == -1)
    {
        std::cerr << "Fork Failed!" << std::endl;

        // Terminate with failure
        exit(255);
    }
    else if(pid == 0)
    {
        // Close the read end of the pipe.  The child has no need to 
        // interact with this side of the pipe.  
        close(communication_pipe[0]);

        // Close standard output because the child shouldn't write 
        // to the console.  
        close(STDOUT_FILENO);

        // Make standard output point to the write end of the pipe.  
        dup(communication_pipe[1]);

        int ret_val = 0;

        ret_val = execlp("ls", "ls", "-la", NULL);

        if(ret_val == -1)
        {
            // Standard output is closed.  Write to standard error instead.
            std::cerr << "Exec Failed" << std::endl;
        
            // Closing the pipe should cause an EOF to be written to it.  
            // The parent process will read that back, and it should terminate
            // the program.  
            close(communication_pipe[1]);

            exit(255);
        }
    }
    else
    {
        // Close the write end of the pipe since the parent doesn't need
        // to write any data.  
        close(communication_pipe[1]);  

        char *buf = new char[8196];
        int bytes_read = 0;

        while((bytes_read = read(communication_pipe[0], buf, 8196)) > 0)
        {
            write(STDOUT_FILENO, buf, bytes_read);
        }

        delete buf;
    }



    exit(0);
}

