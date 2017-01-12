/* ------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman                                                 */
/* Project: CSCE-313 Machine Problem #4 Part 2                               */
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
/* User Defined                                                              */
/* ------------------------------------------------------------------------- */

#include "proctest.h"


/* ------------------------------------------------------------------------- */
/* Proctest Private Member Functions                                         */
/* ------------------------------------------------------------------------- */

std::string Proctest::stringify_buffer(char *buffer)
{
    std::string buffer_string = "";
    int pos = 0;

    while(true)
    {
        if(buffer[pos] == '\0')
        {
            break;
        }
        else
        {
            buffer_string += buffer[pos];
            pos++;
        }
    }

    return buffer_string;
}


std::vector<std::string> Proctest::split_on_spaces(std::string input)
{
    std::vector<std::string> components;
    std::string token = "";

    for(unsigned short int i=0; i<input.length(); i++)
    {
        if(input[i] != ' ')
        {
            token += input[i];
        }
        else
        {
            components.push_back(token);
            token = "";
        }
    }

    return components;
}

/* ------------------------------------------------------------------------- */
/* Proctest Public Member Functions                                          */
/* ------------------------------------------------------------------------- */

Proctest::Proctest(int process_id)
{
    /* Record process ID */
    m_process_id = process_id;

    /* Initialize the read buffer */
    const int BUFFER_SIZE = 16394;
    m_read_buffer = new char[BUFFER_SIZE];

    /* Read from the stat file */
    int stat_pipe[2];
    pipe(stat_pipe);

    pid_t stat_pid = fork();

    if(stat_pid == 0)
    {
        close(stat_pipe[0]);
        close(STDOUT_FILENO);
        dup(stat_pipe[1]);

        std::string fname = "/proc/" + std::to_string(m_process_id) + "/stat";
        execlp("cat", "cat", fname.c_str(), NULL);
    }
    else
    {
        close(stat_pipe[1]);
        waitpid(stat_pid, NULL, 0);   
    }

    int read_size = read(stat_pipe[0], m_read_buffer, BUFFER_SIZE);

    if(read_size == -1)
    {
        std::cout << "[ERROR]: Unable to read stat file." << std::endl;
        _exit(1);
    }

    std::string string_stat = stringify_buffer(m_read_buffer);
 
    /* Clean the read buffer for use again later */
    memset(m_read_buffer, '\0', BUFFER_SIZE);

    /* Assign result to stat_array member function */
    m_stat_array = split_on_spaces(string_stat);

    /* Read from the status file */
    std::string fname = "/proc/" + std::to_string(m_process_id) + "/status";

    int status_pipe[2];
    pipe(status_pipe);

    pid_t status_pid = fork();

    if(status_pid == 0)
    {
        close(status_pipe[0]);
        close(STDOUT_FILENO);
        dup(status_pipe[1]);

        execlp("cat", "cat", fname.c_str(), NULL);
    }
    else
    {
        close(status_pipe[1]);
        waitpid(status_pid, NULL, 0);
    }

    read_size = read(status_pipe[0], m_read_buffer, BUFFER_SIZE);
    if(read_size == -1)
    {
        std::cout << "[ERROR]: Unable to read status file." << std::endl;
        _exit(1);
    }

    std::string string_status = stringify_buffer(m_read_buffer);

    /* Clean Read Buffer */
    memset(m_read_buffer, '\0', BUFFER_SIZE);

    /* Turn status array into a vector<vector<string> > */
    std::vector<std::vector<std::string> > status_data;
    std::string token1 = "";
    std::string token2 = "";
    for(unsigned short int i=0; i<string_status.length(); i++)
    {
        if(string_status[i] == '\n')
        {
            status_data.push_back(std::vector<std::string>());

            /* Now separate the string on tabs */
            for(unsigned short int j=0; j<token1.length(); j++)
            {
                if(token1[j] == '\t')
                {
                    status_data[status_data.size() - 1].push_back(token2);

                    token2 = "";
                }
                else if(j == token1.length() - 1)
                {
                    token2 += token1[j];

                    status_data[status_data.size() - 1].push_back(token2);

                    token2 = "";
                }
                else
                {
                    token2 += token1[j];
                }
            }
            token1 = "";
        }
        else
        {
            token1 += string_status[i];
        }
    }

    /* Assign the found data into the status array */
    m_status_array = status_data;

    /* Find the number of open file descriptors  */
    int fd_pipe_1[2];
    pipe(fd_pipe_1);

    pid_t fd_pid_1 = fork();

    if(fd_pid_1 == 0)
    {
        close(fd_pipe_1[0]);
        close(STDOUT_FILENO);
        dup(fd_pipe_1[1]);
        close(STDERR_FILENO);
        dup(fd_pipe_1[1]);

        std::string fd_file = "/proc/" + std::to_string(m_process_id) + "/fd";
        execlp("ls", "ls", "-la", fd_file.c_str(), NULL);
    }
    else
    {
        close(fd_pipe_1[1]);
        waitpid(fd_pid_1, NULL, 0);
    }

    read(fd_pipe_1[0], m_read_buffer, BUFFER_SIZE);
    std::string fd_data = stringify_buffer(m_read_buffer);

    close(fd_pipe_1[0]);

    int failure_test = fd_data.find("Permission denied");
    if(failure_test != -1)
    {
        m_num_fds = "permission denied";
    }
    else
    {
        int fd_pipe_2[2];
        pipe(fd_pipe_2);

        int fd_pipe_3[2];
        pipe(fd_pipe_3);
        
        write(fd_pipe_3[1], m_read_buffer, BUFFER_SIZE);

        pid_t fd_pid_2 = fork();

        if(fd_pid_2 == 0)
        {
            close(fd_pipe_2[0]);
            
            close(STDOUT_FILENO);
            dup(fd_pipe_2[1]);

            close(fd_pipe_3[1]);

            close(STDIN_FILENO);
            dup(fd_pipe_3[0]);

            execlp("wc", "wc", "-l", NULL);
        }
        else
        {
            close(fd_pipe_2[1]);
            close(fd_pipe_3[1]);
            waitpid(fd_pid_2, NULL, 0);
        }
        read(fd_pipe_2[0], m_read_buffer, 16392);
        fd_data = stringify_buffer(m_read_buffer);
        
        close(fd_pipe_2[0]);
        
        m_num_fds = std::to_string(atoi(fd_data.c_str()) - 3);
    }

    /* Clean read buffer */
    memset(m_read_buffer, '\0', BUFFER_SIZE);

    /* Populate Memory Map */
    std::string mem_file = "/proc/" + std::to_string(process_id) + "/smaps";
    int mem_fd = open(mem_file.c_str(), O_RDONLY);
    std::string mem_data = "";

    while(read(mem_fd, m_read_buffer, BUFFER_SIZE) > 0)
    {
        mem_data += stringify_buffer(m_read_buffer);
        memset(m_read_buffer, '\0', BUFFER_SIZE);
    }

    if(mem_data == "")
    {
        m_mem_array.push_back("permission denied");
    }
    else
    {
        std::vector<std::string> mem_lines;
        std::string mem_line = "";
        for(unsigned int i=0; i<mem_data.length(); i++)
        {
            if(mem_data[i] == '\n' || i == mem_data.length() - 1)
            {
                mem_lines.push_back(mem_line);

                mem_line = "";
            }
            else
            {
                mem_line += mem_data[i];
            }
        }

        for(unsigned short int i=0; i<mem_lines.size(); i++)
        {
            if(!isupper(mem_lines[i][0]))
            {
                m_mem_array.push_back(mem_lines[i]);
            }
        }
    }
}


Proctest::~Proctest()
{
    // Free buffer memory
    delete m_read_buffer;

    // Clear all vectors
    m_stat_array.clear();
    m_status_array.clear();
    m_mem_array.clear();
}


std::string Proctest::getpid()
{
    if(m_stat_array[0] != "")
    {
        return m_stat_array[0];
    }
    else
    {
        return "ERROR";
    }

    return "";
}


std::string Proctest::getppid()
{
    if(m_stat_array[3] != "")
    {
        return m_stat_array[3];
    }
    else
    {
        return "ERROR";
    }

    return "";
}

std::string Proctest::geteuid()
{
    int location = -1;
    for(unsigned short int i=0; i<m_status_array.size(); i++)
    {
        location = m_status_array[i][0].find("Uid:");
        if(location != -1)
        {
            if(m_status_array[i][2] != "")
            {
                return m_status_array[i][2];
            }
            else
            {
                return "ERROR";
            }
        }
    }

    return "";
}

std::string Proctest::getegid()
{
    int location = -1;
    for(unsigned short int i=0; i<m_status_array.size(); i++)
    {
        location = m_status_array[i][0].find("Gid:");
        if(location != -1)
        {
            if(m_status_array[i][2] != "")
            {
                return m_status_array[i][2];
            }
            else
            {
                return "ERROR";
            }
        }
    }

    return "";
}

std::string Proctest::getruid()
{
    int location = -1;
    for(unsigned short int i=0; i<m_status_array.size(); i++)
    {
        location = m_status_array[i][0].find("Uid:");
        if(location != -1)
        {
            if(m_status_array[i][1] != "")
            {
                return m_status_array[i][1];
            }
            else
            {
                return "ERROR";
            }
        }
    }

    return "";
}

std::string Proctest::getrgid()
{
    int location = -1;
    for(unsigned short int i=0; i<m_status_array.size(); i++)
    {
        location = m_status_array[i][0].find("Gid:");
        if(location != -1)
        {
            if(m_status_array[i][1] != "")
            {
                return m_status_array[i][1];
            }
            else
            {
                return "ERROR";
            }
        }
    }

    return "";
}

std::string Proctest::getfsuid()
{
    int location = -1;
    for(unsigned short int i=0; i<m_status_array.size(); i++)
    {
        location = m_status_array[i][0].find("Uid:");
        if(location != -1)
        {
            if(m_status_array[i][4] != "")
            {
                return m_status_array[i][4];
            }
            else
            {
                return "ERROR";
            }
        }
    }
    
    return "";
}

std::string Proctest::getfsgid()
{
    int location = -1;
    for(unsigned short int i=0; i<m_status_array.size(); i++)
    {
        location = m_status_array[i][0].find("Gid:");
        if(location != -1)
        {
            if(m_status_array[i][4] != "")
            {
                return m_status_array[i][4];
            }
            else
            {
                return "ERROR";
            }
        }
    }

    return "";
}

std::string Proctest::getstate()
{
    if(m_stat_array[2] != "")
    {
        return m_stat_array[2];
    }
    else
    {
        return "ERROR";
    }

    return "";
}

std::string Proctest::getthread_count()
{
    if(m_stat_array[19] != "")
    {
        return m_stat_array[19];
    }
    else
    {
        return "ERROR";
    }

    return "";
}

std::string Proctest::getpriority()
{
    if(m_stat_array[17] != "")
    {
        return m_stat_array[17];
    }
    else
    {
        return "ERROR";
    }

    return "";
}

std::string Proctest::getniceness()
{
    if(m_stat_array[18] != "")
    {
        return m_stat_array[18];
    }
    else
    {
        return "ERROR";
    }

    return "";
}

std::string Proctest::getstime()
{
    if(m_stat_array[14] != "")
    {
        return m_stat_array[14];
    }
    else
    {
        return "ERROR";
    }

    return "";
}

std::string Proctest::getutime()
{
    if(m_stat_array[13] != "")
    {
        return m_stat_array[13];
    }
    else
    {
        return "ERROR";
    }

    return "";
}

std::string Proctest::getcstime()
{
    if(m_stat_array[16] != "")
    {
        return m_stat_array[16];
    }
    else
    {
        return "ERROR";
    }

    return "";
}

std::string Proctest::getcutime()
{
    if(m_stat_array[15] != "")
    {
        return m_stat_array[15];
    }
    else
    {
        return "ERROR";
    }

    return "";
}

std::string Proctest::getstartcode()
{
    if(m_stat_array[25] != "")
    {
        return m_stat_array[25];
    }
    else
    {
        return "ERROR";
    }

    return "";
}

std::string Proctest::getendcode()
{
    if(m_stat_array[26] != "")
    {
        return m_stat_array[26];
    }
    else
    {
        return "ERROR";
    }

    return "";
}

std::string Proctest::getesp()
{
    if(m_stat_array[28] != "")
    {
        return m_stat_array[28];
    }
    else
    {
        return "ERROR";
    }

    return "";
}

std::string Proctest::geteip()
{
    if(m_stat_array[29] != "")
    {
        return m_stat_array[29];
    }
    else
    {
        return "ERROR";
    }

    return "";
}

std::string Proctest::getfiles()
{
    if(m_num_fds != "")
    {
        return m_num_fds;
    }
    else
    {
        return "ERROR";
    }

    return "";
}

std::string Proctest::getvoluntary_context_switches()
{
    int location_1 = -1;
    int location_2 = -1;
    for(unsigned short int i=0; i<m_status_array.size(); i++)
    {
        location_1 = m_status_array[i][0].find("voluntary");
        location_2 = m_status_array[i][0].find("nonvoluntary");
        if(location_1 >= 0 && location_2 == -1)
        {
            if(m_status_array[i][1] != "")
            {
                return m_status_array[i][1];
            }
            else
            {
                return "ERROR";
            }
        }
    }

    return "";
}

std::string Proctest::getnonvoluntary_context_switches()
{
    int location = -1;
    for(unsigned short int i=0; i<m_status_array.size(); i++)
    {
        location = m_status_array[i][0].find("nonvoluntary");
        if(location >= 0)
        {
            if(m_status_array[i][1] != "")
            {
                return m_status_array[i][1];
            }
            else
            {
                return "ERROR";
            }
        }
    }

    return "";
}

std::string Proctest::getlast_cpu()
{
    if(m_stat_array[38] != "")
    {
        return m_stat_array[38];   
    }
    else
    {
        return "ERROR";
    }

    return "";
}

std::string Proctest::getallowed_cpus()
{
    int location = -1;
    for(unsigned short int i=0; i<m_status_array.size(); i++)
    {
        location = m_status_array[i][0].find("Cpus_allowed_list");
        if(location != -1)
        {
            if(m_status_array[i][1] != "")
            {
                return m_status_array[i][1];
            }
            else
            {
                return "ERROR";
            }
        }
    }

    return "";
}

std::vector<std::string> Proctest::getmemory_map()
{
    if(!m_mem_array.empty())
    {
        return m_mem_array;
    }
    else
    {
        std::vector<std::string> error_return;
        error_return.push_back("ERROR");
        return error_return;
    }

    return std::vector<std::string>();
}
