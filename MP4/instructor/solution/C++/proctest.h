/* ------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman                                                 */
/* Project: CSCE-313 Machine Problem #4 Part 2                               */
/*                                                                           */
/* File: ./proctest.h                                                        */
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


class Proctest
{
private:
    int m_process_id;
    char *m_read_buffer;
    std::string m_num_fds;
    std::vector<std::string> m_stat_array;
    std::vector<std::vector<std::string> > m_status_array;
    std::vector<std::string> m_mem_array;

    std::string stringify_buffer(char *buffer);
    std::vector<std::string> split_on_spaces(std::string input);

public:
    /* Constructor */
    Proctest(int process_id);
    ~Proctest();

    /* Process IDs */
    std::string getpid();
    std::string getppid();
    std::string geteuid();
    std::string getegid();
    std::string getruid();
    std::string getrgid();
    std::string getfsuid();
    std::string getfsgid();

    /* State */
    std::string getstate();


    /* Thread Information */
    std::string getthread_count();

    /* Scheduling Information */
    std::string getpriority();
    std::string getniceness();

    /* Time Information */
    std::string getstime();
    std::string getutime();
    std::string getcstime();
    std::string getcutime();

    /* Address Space */
    std::string getstartcode();
    std::string getendcode();
    std::string getesp();
    std::string geteip();
    
    /* Resources */
    std::string getfiles();
    std::string getvoluntary_context_switches();
    std::string getnonvoluntary_context_switches();

    /* Processor Information */
    std::string getlast_cpu();
    std::string getallowed_cpus();

    /* Memory */
    std::vector<std::string> getmemory_map();
};
