/* ------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman                                                 */
/* Project: CSCE-313 Machine Problem #3                                      */
/*                                                                           */
/* File: ./report_ex2.cpp                                                    */
/* ------------------------------------------------------------------------- */

#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>
#include<string.h>
#include<iostream>

int main(int argc, char** argv)
{
    pid_t pid = fork();

    if(pid == -1)
    {
        std::cout << "[ERROR]: Fork failed: " << strerror(errno) << std::endl;
        return 1;
    }

    if(pid == 0)
    {
        execl("/bin/ls", "/bin/ls", "-la", NULL);
    }
    else
    {
        waitpid(pid, NULL, 0);
    }

    return 0;
}
