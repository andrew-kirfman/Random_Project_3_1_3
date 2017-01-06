/* -------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman                                                  */
/* Project: CSCE-313 Machine Problem #3                                       */
/*                                                                            */
/* File: ./mp3_part7.cpp                                                      */
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

    // Set up a UNIX pipe so that the commands can communicate back with
    // the parent.  
    int command_pipe[2];
    pipe(command_pipe);

    // Fork the parent so that the child can execute the two commands
    pid_t pid = fork();

    if(pid == -1)
    {

    }
    else if(pid == 0)
    {
        // Set up a UNIX pipe to communicate between two commands
        int command_pipe2[2];
        pipe(command_pipe2);

        pid = fork();
    
        if(pid == -1)
        {
            std::cerr << "Fork Failed!" << std::endl;
        }
        else if(pid == 0)
        {
            // Child closes the read end of the pipe
            close(command_pipe2[0]);
            
            // Redirect the command's output to the write end of the pipe
            close(STDOUT_FILENO);
            dup(command_pipe2[1]);

            ret_val = execlp("ls", "ls", "-la", NULL);

            if(ret_val == -1)
            {
                std::cerr << "Exec Failed!" << std::endl;
            }
        }
        else
        {
            // Parent closes the write end of the pipe
            close(command_pipe2[1]);

            // Redirect the output of the previous command to the input of the pipe
            close(STDIN_FILENO);
            dup(command_pipe2[0]);

            // Redirect the output to the write end of the other pipe
            close(STDOUT_FILENO);
            dup(command_pipe[1]);

            ret_val = execlp("tr", "tr", "[a-zA-Z0-9]", "a", NULL);

            if(ret_val == -1)
            {
                std::cerr << "Exec Failed!" << std::endl;
            }
        }
    }
    else
    {
        // Parent closes the write end of the pipe
        close(command_pipe[1]);

        int bytes_read = 0;

        // I'm making a guess of a potentially beneficial block size
        int BUFF_SIZE = 8196;
        char *buff = new char[BUFF_SIZE];
        while((bytes_read = read(command_pipe[0], buff, BUFF_SIZE)) > 0)
        {
            write(STDOUT_FILENO, buff, bytes_read);
        }
    }

    exit(0);
}

