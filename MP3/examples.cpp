/* ------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman                                                 */
/* Project: CSCE-313 Machine Problem #3                                      */
/*                                                                           */
/* File: ./examples.cpp                                                      */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/* Standard Library Includes                                                 */
/* ------------------------------------------------------------------------- */

#include<unistd.h>
#include<string>
#include<cstring>
#include<errno.h>
#include<time.h>
#include<stdio.h>
#include<iostream>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<fcntl.h>
using namespace std;


/* ------------------------------------------------------------------------ */
/* Error Codes                                                              */
/* ------------------------------------------------------------------------ */

// Fork failure: 1
// Exec failure: 2
// Pipe failure: 3
// Read/Write failure: 4
// Clock failure: 5
// Other: 6


/* ------------------------------------------------------------------------- */
/* Assignment Description                                                    */
/* ------------------------------------------------------------------------- */

/* 
 * MP3 will be a qualitative assignment capturing the essence of system calls,
 * fork, exec, and wait.  The code portion will consist of a number of small 
 * segments illustrating a particular concept.  Afterwards, students will pen
 * a detailed report which captures the essence of what they did by answering
 * a series of simple questions.  
 *
 * Segments of code are as follows
 *   - Use of the 4 important exec functions: execl, execlp, execv, and execvp.  
 *     The command/commands will need to be predetermined for the assignment 
 *     as we need to be able to create a unittest framework to test student's
 *     work.  Along with each exec, students must use both wait functions at
 *     least once each.  Make students test result from wait to see how the 
 *     process terminated.  Also, maybe even test for abnormal terminations
 *   - Creation of a unix pipe between an execed command and its parent. 
 *   - Piping data between two commands.  
 *   - Examine the cost of system calls by running several calls 1000 times
 *     and examine their cost.  
 *       - do a test involving open and fopen.  Tell the students to do just
 *         this and then ask them to explain why
 */


string check_status(int return_status)
{
    if(WIFEXITED(return_status))
    {
        return "Process exited normally";
    }
    else if(WIFSIGNALED(return_status))
    {
        return "Process killed by signal";
    }
    else if(WIFSTOPPED(return_status))
    {
        return "Process stopped by a signal";
    }
    else if(WIFCONTINUED(return_status))
    {
        return "Process continued";
    }
    
    return "Unknown Status";
}


int main()
{
    /* Reused program variables */
    pid_t pid;
    int return_status = 0;
    int status = 0;
    int BUFFER_SIZE = 16392;
    char *buffer = new char[BUFFER_SIZE];

    /* Use of the 4 important exec functions to execute ls -la */
    /*  - execl, execlp, execv, execvp                         */
    
    // execl
    //   - Variable number of input arguments given in 
    //     list format
    //   - Uses the full absolute pathname to the program
    //   - Must be terminated by a NULL argument (i.e. 
    //     NULL or a (char*) 0
    
    /* execl */
    cout << "Test of execl: " << endl;
    pid = fork();
    
    if(pid == -1)
    {
        cout << "[ERROR]: Fork failed: " << strerror(errno) << endl;
        _exit(1);
    }   

    if(pid == 0)
    {
        status = execl("/bin/ls", "/bin/ls", "-la", NULL);

        if(status == -1)
        {
            cout << "[ERROR]: Execl failed: " << strerror(errno) << endl;
            _exit(2);
        }
    }
    else
    {
        wait(&return_status);
        cout << "1. execl: " << check_status(return_status) << endl << endl;
    }

    // execlp
    //   - Same argument structure as execl
    //   - Instead of taking an absolute pathname, execlp takes
    //     a executable name and then uses the PATH environment
    //     variable to search for the executable.  If the executable
    //     is not found in a directory specified by PATH, 
    //     execlp returns with an error.  

    /* execlp */
    cout << "Test of execlp" << endl;
    pid = fork();
    
    if(pid == -1)
    {
        cout << "[ERROR]: Fork failed: " << strerror(errno) << endl;
        _exit(1);
    }

    if(pid == 0)
    {
        status = execlp("ls", "ls", "-la", NULL);
        
        if(status == -1)
        {
            cout << "[ERROR]: Execlp failed: " << strerror(errno) << endl;
            _exit(2);
        }
    }
    else
    {
        waitpid(pid, &return_status, 0);
        cout << "2. execlp: " << check_status(return_status) << endl << endl;
    }

    // execv
    //   - Requires that arguments be formatted into a char** 
    //     array (an array of pointers to null terminated strings).  
    //   - The first argument must be the absolute pathname of
    //     the program to execute.  
    //   - The second argument must be the entire array.  
    //   - The array must be terminated with a NULL element

    /* execv */
    const char **argv1 = new const char*[3];
    argv1[0] = "/bin/ls";
    argv1[1] = "-la";
    argv1[2] = NULL;
    
    cout << "Test of execv" << endl;
    pid = fork();

    if(pid == -1)
    {
        cout << "[ERROR]: Fork failed: " << strerror(errno) << endl;
        _exit(1);
    }

    if(pid == 0)
    {
        status = execv(argv1[0], (char**)argv1);
        
        if(status == -1)
        {
            cout << "[ERROR]: Execv failed: " << strerror(errno) << endl;
            _exit(2);
        }
    }
    else
    {
        wait(&return_status);
        cout << "3. execv: " << check_status(return_status) << endl << endl;
    }

    // execvp
    //   - Same argument structure as execv
    //   - Similar to execlp, takes the filename instead
    //     of absolute pathname.  

    /* execvp */
    const char **argv2 = new const char*[3];
    argv2[0] = "ls";
    argv2[1] = "-la";
    argv2[2] = NULL;

    cout << "Test of execvp" << endl;
    pid = fork();

    if(pid == -1)
    {
        cout << "[ERROR]: Fork failed: " << strerror(errno) << endl;
        _exit(1);
    }

    if(pid == 0)
    {
        status = execvp(argv2[0], (char**)argv2);

        if(status == -1)
        {
            cout << "[ERROR]: Execvp failed: " << strerror(errno) << endl;
            _exit(1);
        }
    }
    else
    {
        waitpid(pid, &return_status, 0);
        cout << "4. execvp: " << check_status(return_status) << endl << endl;
    }


    /* Reading data from a pipe */
    cout << "Test of UNIX pipe" << endl;

    // Unix Pipe:
    //   - pipe1[1] is the write end of the pipe
    //   - pipe1[0] is the read end of the pipe
    int pipe1[2];
    
    // The pipe system call turns the int array into
    // a set of two initialized file descriptors.  
    status = pipe(pipe1);

    if(status == -1)
    {
        cout << "[ERROR]: Creation of UNIX pipe failed: " << strerror(errno) << endl;
        _exit(3);
    }

    pid = fork();

    if(pid == -1)
    {
        cout << "[ERROR]: Fork failed: " << strerror(errno) << endl;
        _exit(1);
    }

    if(pid == 0)
    {
        // Close the end of the pipe that is not needed
        close(pipe1[0]);
        
        // Close standard output and redirect
        close(STDOUT_FILENO);
        dup(pipe1[1]);

        execlp("ls", "ls", "-la", NULL);
    }
    else
    {
        // Close the end of the pipe that is not needed
        close(pipe1[1]);

        waitpid(pid, &return_status, 0);
        status = read(pipe1[0], buffer, BUFFER_SIZE);

        if(status == -1)
        {
            cout << "[ERROR]: Could not read from pipe";
        }
        else
        {
            cout << buffer;
            free(buffer);
            buffer = new char[BUFFER_SIZE];
            cout << "5. simple pipe: " << check_status(return_status) << endl << endl;
        }
    }

    
    /* Piping between two commands */
    cout << "Test of UNIX pipe between two commands" << endl;

    int pipe2[2];

    status = pipe(pipe2);
    
    if(status == -1)
    {
        cout << "[ERROR]: Creation of UNIX pipe failed: " << strerror(errno) << endl;
        _exit(3);
    }

    pid = fork();

    if(pid == -1)
    {
        cout << "[ERROR]: Fork failed: " << strerror(errno) << endl;
        _exit(1);
    }

    if(pid == 0)
    {
        close(pipe2[0]);
            
        close(STDOUT_FILENO);
        dup(pipe2[1]);

        status = execlp("ls", "ls", "-la", NULL);

        if(status == -1)
        {
            cout << "[ERROR]: Execlp failed: " << strerror(errno) << endl;
            _exit(1);                
        }
    }
    else
    {
        close(pipe2[1]);

        waitpid(pid, &return_status, 0);
    }

    pid = fork();

    if(pid == -1)
    {
        cout << "[ERROR]: Fork failed: " << strerror(errno) << endl;
        _exit(1);
    }

    if(pid == 0)
    {
        close(STDIN_FILENO);
        dup(pipe2[0]);

        status = execlp("wc", "wc", "-l", NULL);

        if(status == -1)
        {
            cout << "[ERROR]: Execlp failed: " << strerror(errno) << endl;
            _exit(1);
        }
    }
    else
    {
        waitpid(pid, &return_status, 0);
    }
    cout << "6. Pipe between commands: " << check_status(return_status) << endl << endl;

    /* Performance analysis of open/close vs. fopen/fclose */
    int unix_open_sum = 0;
    int unix_close_sum = 0;
    int fd = 0;
    FILE *file;
    int std_open_sum = 0;
    int std_close_sum = 0;
    struct timespec time1, time2;
    string filename = "./open_test";

    // Create a temporary file to open and close
    status = creat(filename.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);

    if(status == -1)
    {
        cout << "[ERROR]: Could not create file, " << filename << ": " << strerror(errno) << endl;
        _exit(6);
    }

    // UNIX open/close
    for(int i=0; i<1000; i++)
    {
        // Open
        status = clock_gettime(CLOCK_MONOTONIC, &time1);

        if(status == -1)
        {
            cout << "[ERROR]: Could not get clock time: " << strerror(errno) << endl;
            _exit(5);
        }

        fd = open(filename.c_str(), O_RDWR);

        status = clock_gettime(CLOCK_MONOTONIC, &time2);
        
        if(status == -1)
        {
            cout << "[ERROR]: Counld not get clock time: " << strerror(errno) << endl;
            _exit(5);
        }

        unix_open_sum += (time2.tv_nsec - time1.tv_nsec);

        // Close
        status = clock_gettime(CLOCK_MONOTONIC, &time1);

        if(status == -1)
        {
            cout << "[ERROR]: Could not get clock time: " << strerror(errno) << endl;
            _exit(5);
        }

        close(fd);

        status = clock_gettime(CLOCK_MONOTONIC, &time2);

        if(status == -1)
        {
            cout << "[ERROR]: Could not get clock time: " << strerror(errno) << endl;
            _exit(5);
        }
    
        unix_close_sum += (time2.tv_nsec - time1.tv_nsec);
    }
    
    cout << "Average time to call open: " << unix_open_sum / 1000 << endl;
    cout << "Average time to call close: " << unix_close_sum / 1000 << endl;
    cout << endl;

    // stdlib fopen/fclose
    for(int i=0; i<1000; i++)
    {
        // Fopen
        status = clock_gettime(CLOCK_MONOTONIC, &time1);

        if(status == -1)
        {
            cout << "[ERROR]: Could not get clock time: " << strerror(errno) << endl;
            _exit(5);
        }
        
        file = fopen(filename.c_str(), "w+");

        status = clock_gettime(CLOCK_MONOTONIC, &time2);

        if(status == -1)
        {
            cout << "[ERROR: Could not get clock time: " << strerror(errno) << endl;
            _exit(5);
        }

        std_open_sum += (time2.tv_nsec - time1.tv_nsec);

        // Fclose
        status = clock_gettime(CLOCK_MONOTONIC, &time1);

        if(status == -1)
        {
            cout << "[ERROR]: Could not get clock time: " << strerror(errno) << endl;
            _exit(5);
        }

        fclose(file);

        status = clock_gettime(CLOCK_MONOTONIC, &time2);

        if(status == -1)
        {
            cout << "[ERROR]: Could not get clock time: " << strerror(errno) << endl;
            _exit(5);
        }

        std_close_sum += (time2.tv_nsec - time1.tv_nsec);
    }

    cout << "Average time to call fopen: " << std_open_sum / 1000 << endl;
    cout << "Average time to call fclose: " << std_close_sum / 1000 << endl;

    unlink(filename.c_str());

    return 0;
}
