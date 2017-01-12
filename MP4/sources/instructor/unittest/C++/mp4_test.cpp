/* ------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman                                                 */
/* Project: CSCE-313 Machine Problem 4                                       */
/*                                                                           */
/* File: ./instructor/mp4_test.sh                                            */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/* Standard Library Includes                                                 */
/* ------------------------------------------------------------------------- */

#include<iostream>
#include<vector>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<signal.h>

// Attempt to include student's executable
// CHANGE THIS CHANGE THIS CHANGE THIS CHANGE THIS CHANGE THIS CHANGE THIS CHANGE THIS
// CHANGE THIS CHANGE THIS CHANGE THIS CHANGE THIS CHANGE THIS CHANGE THIS CHANGE THIS
#include "../../solution/proctest.h"
#include "./utility.h"

/* ------------------------------------------------------------------------- */
/* User Defined Includes                                                     */
/* ------------------------------------------------------------------------- */

// Add include for proctest here!!!

class MP4_Test
{
public:
    /* Constructor */
    MP4_Test();

    /* Destructor */
    ~MP4_Test();

    /* Data Getter Functions */
    int getprocess_id();
    int getsuccessful_tests();
    int getfailed_tests();
    int gettotal_tests();

    /* Data Setter Functions */
    void setprocess_id(int new_process_id);
    void setsuccessful_tests(int new_successful_tests);
    void setfailed_tests(int new_failed_tests);
    void settotal_tests(int new_total_tests);

    /* Proctest Testing Functions  */
    /* Identifiers */
    bool testpid();
    bool testppid();
    bool testeuid();
    bool testegid();
    bool testruid();
    bool testrgid();
    bool testfsuid();
    bool testfsgid();

    /* State */
    bool teststate();
    
    /* Thread Information */
    bool testthread_count();

    /* Priority */
    bool testpriority();
    bool testniceness();

    /* Time Information */
    bool teststime();
    bool testutime();
    bool testcstime();
    bool testcutime();

    /* Address Space */
    bool teststartcode();
    bool testendcode();
    bool testesp();
    bool testeip();

    /* Resources */
    bool testfiles();
    bool testvoluntary_context_switches();
    bool testnonvoluntary_context_switches();

    /* Processors */
    bool testlast_cpu();
    bool testallowed_cpus();

    /* Memory Map */
    bool testmemory_map();

    /* Primary Testing Function */
    void testall();

private:
    /* Corect Process Data  */
    int m_process_id;
    char *m_read_buffer;
    std::string m_num_fds;
    std::vector<std::string> m_stat_array;
    std::vector<std::vector<std::string> > m_status_array;
    std::vector<std::string> m_mem_array;

    std::string stringify_buffer(char *buffer);
    std::vector<std::string> split_on_spaces(std::string input);

    /* Student's Executable Data */
    Proctest *student_executable;


    /* Testing Data */
    int m_successful_tests;
    int m_failed_tests;
    int m_total_tests;
};


/* Supporting Functions */
std::string MP4_Test::stringify_buffer(char *buffer)
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

std::vector<std::string> MP4_Test::split_on_spaces(std::string input)
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


/* Constructor */
MP4_Test::MP4_Test()
{
    m_successful_tests = 0;
    m_failed_tests = 0;
    m_total_tests = 26;
    
    pid_t process_id = fork();
    
    if(process_id == 0)
    {
        execlp("sleep", "sleep", "1000", NULL);
    }
    else
    {
        // Parent doesn't do anything
    }
    

    /* Record process ID */
    m_process_id = process_id;


    /* Initialize Student Data */
    student_executable = new Proctest(m_process_id);

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

MP4_Test::~MP4_Test()
{
    /* Wipe out the process that was created */
    kill(m_process_id, SIGKILL);    

    /* Clear all other data */
    delete m_read_buffer;
    m_stat_array.clear();
    m_status_array.clear();
    m_mem_array.clear();
    delete student_executable;
}


/* Data Getter Functions */
int MP4_Test::getprocess_id()
{
    return m_process_id;
}

int MP4_Test::getsuccessful_tests()
{
    return m_successful_tests;
}

int MP4_Test::getfailed_tests()
{
    return m_failed_tests;
}

int MP4_Test::gettotal_tests()
{
    return m_total_tests;
}


/* Data Setter Functions */
void MP4_Test::setprocess_id(int new_process_id)
{
    m_process_id = new_process_id;

    return;
}

void MP4_Test::setsuccessful_tests(int new_successful_tests)
{
    m_successful_tests = new_successful_tests;

    return;
}

void MP4_Test::setfailed_tests(int new_failed_tests)
{
    m_failed_tests = new_failed_tests;

    return;
}

void MP4_Test::settotal_tests(int new_total_tests)
{
    m_total_tests = new_total_tests;

    return;
}


/* Proctest Testing Functions */
/* Identifiers */
bool MP4_Test::testpid()
{
    std::string student_pid = student_executable->getpid();
    std::string test_pid = m_stat_array[0];
    
    return (student_pid == test_pid);
}

bool MP4_Test::testppid()
{
    std::string student_ppid = student_executable->getppid();
    std::string test_ppid = m_stat_array[3];

    return (student_ppid == test_ppid);
}

bool MP4_Test::testeuid()
{
    std::string student_euid = student_executable->geteuid();
    std::string test_euid = "";
    
    int location = -1;
    for(unsigned short int i=0; i<m_status_array.size(); i++)
    {
        location = m_status_array[i][0].find("Uid:");
        if(location != -1)
        {
            if(m_status_array[i][2] != "")
            {
                test_euid = m_status_array[i][2];
            }
        }
    }

    return (student_euid == test_euid);
}

bool MP4_Test::testegid()
{
    std::string student_egid = student_executable->getegid();
    std::string test_egid = "";

    int location = -1;
    for(unsigned short int i=0; i<m_status_array.size(); i++)
    {
        location = m_status_array[i][0].find("Gid:");
        if(location != -1)
        {
            if(m_status_array[i][2] != "")
            {
                test_egid = m_status_array[i][2];
            }
        }
    }

    return (student_egid == test_egid);
}

bool MP4_Test::testruid()
{
    std::string student_ruid = student_executable->getruid();
    std::string test_ruid = "";

    int location = -1;
    for(unsigned short int i=0; i<m_status_array.size(); i++)
    {
        location = m_status_array[i][0].find("Uid:");
        if(location != -1)
        {
            if(m_status_array[i][1] != "")
            {
                test_ruid = m_status_array[i][1];
            }
        }
    }


    return (student_ruid == test_ruid);
}

bool MP4_Test::testrgid()
{
    std::string student_rgid = student_executable->getrgid();
    std::string test_rgid = "";

    int location = -1;
    for(unsigned short int i=0; i<m_status_array.size(); i++)
    {
        location = m_status_array[i][0].find("Gid:");
        if(location != -1)
        {
            if(m_status_array[i][1] != "")
            {
                test_rgid = m_status_array[i][1];
            }
        }
    }

    return (student_rgid == test_rgid);
}

bool MP4_Test::testfsuid()
{
    std::string student_fsuid = student_executable->getfsuid();
    std::string test_fsuid = "";

    int location = -1;
    for(unsigned short int i=0; i<m_status_array.size(); i++)
    {
        location = m_status_array[i][0].find("Uid:");
        if(location != -1)
        {
            if(m_status_array[i][4] != "")
            {
                test_fsuid = m_status_array[i][4];
            }
        }
    }

    return (student_fsuid == test_fsuid);
}

bool MP4_Test::testfsgid()
{
    std::string student_fsgid = student_executable->getfsgid();
    std::string test_fsgid = "";

    int location = -1;
    for(unsigned short int i=0; i<m_status_array.size(); i++)
    {
        location = m_status_array[i][0].find("Gid:");
        if(location != -1)
        {
            if(m_status_array[i][4] != "")
            {
                test_fsgid = m_status_array[i][4];
            }
        }
    }

    return (student_fsgid == test_fsgid);
}


/* State */
bool MP4_Test::teststate()
{
    std::string student_state = student_executable->getstate();
    std::string test_state = m_stat_array[2];

    return (student_state == test_state);
}


/* Thread Information */
bool MP4_Test::testthread_count()
{
    std::string student_thread_count = student_executable->getthread_count();
    std::string test_thread_count = m_stat_array[19];

    return (student_thread_count == test_thread_count);
}


/* Priority */
bool MP4_Test::testpriority()
{
    std::string student_priority = student_executable->getpriority();
    std::string test_priority = m_stat_array[17];

    return (student_priority == test_priority);
}

bool MP4_Test::testniceness()
{
    std::string student_niceness = student_executable->getniceness();
    std::string test_niceness = m_stat_array[18];

    return (student_niceness == test_niceness);
}


/* Time Information */
bool MP4_Test::teststime()
{
    std::string student_stime = student_executable->getstime();
    std::string test_stime = m_stat_array[14];

    return (student_stime == test_stime);
}

bool MP4_Test::testutime()
{
    std::string student_utime = student_executable->getutime();
    std::string test_utime = m_stat_array[13];

    return (student_utime == test_utime); 
}

bool MP4_Test::testcstime()
{
    std::string student_cstime = student_executable->getcstime();
    std::string test_cstime = m_stat_array[16];

    return (student_cstime == test_cstime);
}

bool MP4_Test::testcutime()
{
    std::string student_cutime = student_executable->getcutime();
    std::string test_cutime = m_stat_array[15];

    return (student_cutime == test_cutime);
}


/* Address Space */
bool MP4_Test::teststartcode()
{
    std::string student_startcode = student_executable->getstartcode();
    std::string test_startcode = m_stat_array[25];

    return (student_startcode == test_startcode);
}

bool MP4_Test::testendcode()
{
    std::string student_endcode = student_executable->getendcode();
    std::string test_endcode = m_stat_array[26];

    return (student_endcode == test_endcode);
}

bool MP4_Test::testesp()
{
    std::string student_esp = student_executable->getesp();
    std::string test_esp = m_stat_array[28];

    return (student_esp == test_esp);
}

bool MP4_Test::testeip()
{
    std::string student_eip = student_executable->geteip();
    std::string test_eip = m_stat_array[29];

    return (student_eip == test_eip);
}


/* Resources */
bool MP4_Test::testfiles()
{
    std::string student_files = student_executable->getfiles();
    std::string test_files = m_num_fds;

    return (student_files == test_files);
}

bool MP4_Test::testvoluntary_context_switches()
{
    std::string student_voluntary_context_switches =
        student_executable->getnonvoluntary_context_switches();
    std::string test_voluntary_context_switches = "";

    int location1 = -1;
    int location2 = -1;
    for(unsigned short int i=0; i<m_status_array.size(); i++)
    {
        location1 = m_status_array[i][0].find("voluntary");
        location2 = m_status_array[i][0].find("nonvoluntary");
        if(location1 != -1 && location2 == -1)
        {
            test_voluntary_context_switches = m_status_array[i][1];
        }
    }

    return (student_voluntary_context_switches == test_voluntary_context_switches);
}

bool MP4_Test::testnonvoluntary_context_switches()
{
    std::string student_nonvoluntary_context_switches = 
        student_executable->getnonvoluntary_context_switches();
    std::string test_nonvoluntary_context_switches = "";

    int location = -1;
    for(unsigned short int i=0; i<m_status_array.size(); i++)
    {
        location = m_status_array[i][0].find("nonvoluntary");
        if(location != -1)
        {
            test_nonvoluntary_context_switches = m_status_array[i][1];
        }
    }

    return (student_nonvoluntary_context_switches == test_nonvoluntary_context_switches);
}



/* Processors */
bool MP4_Test::testlast_cpu()
{
    std::string student_last_cpu = student_executable->getlast_cpu();
    std::string test_last_cpu = m_stat_array[38];

    return (student_last_cpu == test_last_cpu);
}

bool MP4_Test::testallowed_cpus()
{
    std::string student_allowed_cpus = student_executable->getallowed_cpus();
    std::string test_allowed_cpus = "";

    int location = -1;
    for(unsigned short int i=0; i<m_status_array.size(); i++)
    {
        location = m_status_array[i][0].find("Cpus_allowed_list");
        if(location != -1)
        {
            test_allowed_cpus = m_status_array[i][1];   
        }
    }

    return (student_allowed_cpus == test_allowed_cpus);
}


/* Memory Map */
bool MP4_Test::testmemory_map()
{
    std::vector<std::string> student_memory_map = student_executable->getmemory_map();
    std::vector<std::string> test_memory_map = m_mem_array;

    bool is_equal = false;
    for(unsigned int i=0; i<test_memory_map.size(); i++)
    {
        for(unsigned int j=0; j<student_memory_map.size(); j++)
        {
            if(test_memory_map[i] == student_memory_map[j])
            {
                is_equal = true;
            }
        }

        if(is_equal == false)
        {
            return false;
        }
    }

    return true;
}


/* Primary Testing Function */
void MP4_Test::testall()
{
    /* Start of testing */
    std::cout << BOLDCYAN << "# --------------------------------------------------------------------------- #" << RESET << std::endl;
    std::cout << BOLDCYAN << "# Correctness Tests                                                           #" << RESET << std::endl;
    std::cout << BOLDCYAN << "# --------------------------------------------------------------------------- #" << RESET << std::endl;
    std::cout << std::endl;

    /* getpid */
    std::cout << BOLDGREEN <<  "1.  Testing PID..." << RESET << std::endl;
    if(testpid())
    {
        std::cout << BOLDCYAN << "    PID" << RESET << " test"
            << WHITE << " succeeded!" << RESET << std::endl;
        m_successful_tests += 1;
    }
    else
    {
        std::cout << BOLDCYAN << "    PID" << RESET << " test"
            << RED << " failed!" << RESET << std::endl;
        m_failed_tests += 1;
    }
    std::cout << std::endl;

    /* getppid */
    std::cout << BOLDGREEN << "2.  Testing PPID..." << RESET << std::endl;
    if(testppid())
    {
        std::cout << BOLDCYAN << "    PPID" << RESET << " test"
            << WHITE << " succeeded!" << RESET << std::endl;
        m_successful_tests += 1;
    }
    else
    {
        std::cout << BOLDCYAN << "    PPID" << RESET << " test"
            << RED << " failed!" << RESET << std::endl;
        m_failed_tests += 1;
    }
    std::cout << std::endl;
    
    /* geteuid */
    std::cout << BOLDGREEN << "3.  Testing EUID..." << RESET << std::endl;
    if(testeuid())
    {
        std::cout << BOLDCYAN << "    EUID" << RESET << " test"
            << WHITE << " succeeded!" << RESET << std::endl;
        m_successful_tests += 1;
    }
    else
    {
        std::cout << BOLDCYAN << "    EUID" << RESET << " test"
            << RED << " faled!" << RESET << std::endl;
        m_failed_tests += 1;
    }
    std::cout << std::endl;

    /* getegid */
    std::cout << BOLDGREEN << "4.  Testing EGID..." << RESET << std::endl;
    if(testegid())
    {
        std::cout << BOLDCYAN << "    EGID" << RESET << " test"
            << WHITE << " succeeded!" << RESET << std::endl;
        m_successful_tests += 1;
    }
    else
    {
        std::cout << BOLDCYAN << "    EGID" << RESET << " test"
            << RED << " faled!" << RESET << std::endl;
        m_failed_tests += 1;
    }
    std::cout << std::endl;

    /* getruid */
    std::cout << BOLDGREEN << "5.  Testing RUID..." << RESET << std::endl;
    if(testruid())
    {
        std::cout << BOLDCYAN << "    RUID" << RESET << " test"
            << WHITE << " succeeded!" << RESET << std::endl;
        m_successful_tests += 1;
    }
    else
    {
        std::cout << BOLDCYAN << "    RUID" << RESET << " test"
            << RED << " faled!" << RESET << std::endl;
        m_failed_tests += 1;
    }
    std::cout << std::endl;

    /* getrgid */
    std::cout << BOLDGREEN << "6.  Testing RGID..." << RESET << std::endl;
    if(testrgid())
    {
        std::cout << BOLDCYAN << "    RGID" << RESET << " test"
            << WHITE << " succeeded!" << RESET << std::endl;
        m_successful_tests += 1;
    }
    else
    {
        std::cout << BOLDCYAN << "    RGID" << RESET << " test"
            << RED << " failed!" << RESET << std::endl;
        m_failed_tests += 1;
    }
    std::cout << std::endl;

    /* getfsuid */
    std::cout << BOLDGREEN << "7.  Testing FSUID..." << RESET << std::endl;
    if(testfsuid())
    {
        std::cout << BOLDCYAN << "    FSUID" << RESET << " test"
            << WHITE << " succeeded!" << RESET << std::endl;
        m_successful_tests += 1;
    }
    else
    {
        std::cout << BOLDCYAN << "    FSUID" << RESET << " test"
            << RED << " faled!" << RESET << std::endl;
        m_failed_tests += 1;
    }
    std::cout << std::endl;

    /* getfsgid */
    std::cout << BOLDGREEN << "8.  Testing FSGID..." << RESET << std::endl;
    if(testfsgid())
    {
        std::cout << BOLDCYAN << "    FSGID" << RESET << " test"
            << WHITE << " succeeded!" << RESET << std::endl;
        m_successful_tests += 1;
    }
    else
    {
        std::cout << BOLDCYAN << "    FSGID" << RESET << " test"
            << RED << " faled!" << RESET << std::endl;
        m_failed_tests += 1;
    }
    std::cout << std::endl;

    /* getstate */
    std::cout << BOLDGREEN << "9.  Testing State..." << RESET << std::endl;
    if(teststate())
    {
        std::cout << BOLDCYAN << "    State" << RESET << " test"
            << WHITE << " succeeded!" << RESET << std::endl;
        m_successful_tests += 1;
    }
    else
    {
        std::cout << BOLDCYAN << "    State" << RESET << " test"
            << RED << " faled!" << RESET << std::endl;
        m_failed_tests += 1;
    }
    std::cout << std::endl;

    /* getthread_count */
    std::cout << BOLDGREEN << "10. Testing Thread_count..." << RESET << std::endl;
    if(testthread_count())
    {
        std::cout << BOLDCYAN << "    Thread_count" << RESET << " test"
            << WHITE << " succeeded!" << RESET << std::endl;
        m_successful_tests += 1;
    }
    else
    {
        std::cout << BOLDCYAN << "    Thread_count" << RESET << " test"
            << RED << " faled!" << RESET << std::endl;
        m_failed_tests += 1;
    }
    std::cout << std::endl;

    /* getpriority */
    std::cout << BOLDGREEN << "11. Testing Priority..." << RESET << std::endl;
    if(testpriority())
    {
        std::cout << BOLDCYAN << "    Priority" << RESET << " test"
            << WHITE << " succeeded!" << RESET << std::endl;
        m_successful_tests += 1;
    }
    else
    {
        std::cout << BOLDCYAN << "    Priority" << RESET << " test"
            << RED << " faled!" << RESET << std::endl;
        m_failed_tests += 1;
    }
    std::cout << std::endl;

    /* getniceness */
    std::cout << BOLDGREEN << "12. Testing Niceness..." << RESET << std::endl;
    if(testniceness())
    {
        std::cout << BOLDCYAN << "    Niceness" << RESET << " test"
            << WHITE << "succeeded!" << RESET << std::endl;
        m_successful_tests += 1;
    }
    else
    {
        std::cout << BOLDCYAN << "    Niceness" << RESET << " test"
            << RED << " faled!" << RESET << std::endl;
        m_failed_tests += 1;
    }
    std::cout << std::endl;

    /* getstime */
    std::cout << BOLDGREEN << "13. Testing stime..." << RESET << std::endl;
    if(teststime())
    {
        std::cout << BOLDCYAN << "    stime" << RESET << " test"
            << WHITE << " succeeded!" << RESET << std::endl;
        m_successful_tests += 1;
    }
    else
    {
        std::cout << BOLDCYAN << "    stime" << RESET << " test"
            << RED << " faled!" << RESET << std::endl;
        m_failed_tests += 1;
    }
    std::cout << std::endl;

    /* getutime */
    std::cout << BOLDGREEN << "14. Testing utime..." << RESET << std::endl;
    if(testutime())
    {
        std::cout << BOLDCYAN << "    utime" << RESET << " test"
            << WHITE << " succeeded!" << RESET << std::endl;
        m_successful_tests += 1;
    }
    else
    {
        std::cout << BOLDCYAN << "    utime" << RESET << " test"
            << RED << " faled!" << RESET << std::endl;
        m_failed_tests += 1;
    }
    std::cout << std::endl;

    /* getcstime */
    std::cout << BOLDGREEN << "15. Testing cstime..." << RESET << std::endl;
    if(testcstime())
    {
        std::cout << BOLDCYAN << "    cstime" << RESET << " test"
            << WHITE << " succeeded!" << RESET << std::endl;
        m_successful_tests += 1;
    }
    else
    {
        std::cout << BOLDCYAN << "    cstime" << RESET << " test"
            << RED << " faled!" << RESET << std::endl;
        m_failed_tests += 1;
    }
    std::cout << std::endl;

    /* getcutime */
    std::cout << BOLDGREEN << "16. Testing cutime..." << RESET << std::endl;
    if(testcutime())
    {
        std::cout << BOLDCYAN << "    cutime" << RESET << " test"
            << WHITE << " succeeded!" << RESET << std::endl;
        m_successful_tests += 1;
    }
    else
    {
        std::cout << BOLDCYAN << "    cutime" << RESET << " test"
            << RED << " faled!" << RESET << std::endl;
        m_failed_tests += 1;
    }
    std::cout << std::endl;

    /* getstartcode */
    std::cout << BOLDGREEN << "17. Testing Startcode..." << RESET << std::endl;
    if(teststartcode())
    {
        std::cout << BOLDCYAN << "    Startcode" << RESET << " test"
            << WHITE << " succeeded!" << RESET << std::endl;
        m_successful_tests += 1;
    }
    else
    {
        std::cout << BOLDCYAN << "    Startcode" << RESET << " test"
            << RED << " faled!" << RESET << std::endl;
        m_failed_tests += 1;
    }
    std::cout << std::endl;

    /* getendcode */
    std::cout << BOLDGREEN << "18. Testing Endcode..." << RESET << std::endl;
    if(testendcode())
    {
        std::cout << BOLDCYAN << "    Endcode" << RESET << " test"
            << WHITE << " succeeded!" << RESET << std::endl;
        m_successful_tests += 1;
    }
    else
    {
        std::cout << BOLDCYAN << "    Endcode" << RESET << " test"
            << RED << " faled!" << RESET << std::endl;
        m_failed_tests += 1;
    }
    std::cout << std::endl;

    /* getesp */
    std::cout << BOLDGREEN << "19. Testing ESP..." << RESET << std::endl;
    if(testesp())
    {
        std::cout << BOLDCYAN << "    ESP" << RESET << " test"
            << WHITE << " succeeded!" << RESET << std::endl;
        m_successful_tests += 1;
    }
    else
    {
        std::cout << BOLDCYAN << "    ESP" << RESET << " test"
            << RED << " faled!" << RESET << std::endl;
        m_failed_tests += 1;
    }
    std::cout << std::endl;

    /* geteip */
    std::cout << BOLDGREEN << "20. Testing EIP..." << RESET << std::endl;
    if(testeip())
    {
        std::cout << BOLDCYAN << "    EIP" << RESET << " test"
            << WHITE << " succeeded!" << RESET << std::endl;
        m_successful_tests += 1;
    }
    else
    {
        std::cout << BOLDCYAN << "    EIP" << RESET << " test"
            << RED << " faled!" << RESET << std::endl;
        m_failed_tests += 1;
    }
    std::cout << std::endl;

    /* getfiles */
    std::cout << BOLDGREEN << "21. Testing Files..." << RESET << std::endl;
    if(testfiles())
    {
        std::cout << BOLDCYAN << "    Files" << RESET << " test"
            << WHITE << " succeeded!" << RESET << std::endl;
        m_successful_tests += 1;
    }
    else
    {
        std::cout << BOLDCYAN << "    Files" << RESET << " test"
            << RED << " faled!" << RESET << std::endl;
        m_failed_tests += 1;
    }
    std::cout << std::endl;

    /* getvoluntary_context_switches */
    std::cout << BOLDGREEN << "22. Testing Voluntary Context Switches..." << RESET << std::endl;
    if(testvoluntary_context_switches())
    {
        std::cout << BOLDCYAN << "    Voluntary context switches" << RESET << " test"
            << WHITE << " succeeded!" << RESET << std::endl;
        m_successful_tests += 1;
    }
    else
    {
        std::cout << BOLDCYAN << "    Voluntary context switches" << RESET << " test"
            << RED << " faled!" << RESET << std::endl;
        m_failed_tests += 1;
    }
    std::cout << std::endl;

    /* getnonvoluntary_context_switches */
    std::cout << BOLDGREEN << "23. Testing Nonvoluntary Context Switches..." << RESET << std::endl;
    if(testnonvoluntary_context_switches())
    {
        std::cout << BOLDCYAN << "    Nonvoluntary context switches" << RESET << " test"
            << WHITE << " succeeded!" << RESET << std::endl;
        m_successful_tests += 1;
    }
    else
    {
        std::cout << BOLDCYAN << "    Nonvoluntary context switches" << RESET << " test"
            << RED << " faled!" << RESET << std::endl;
        m_failed_tests += 1;
    }
    std::cout << std::endl;

    /* getlast_cpu */
    std::cout << BOLDGREEN << "24. Testing Last_cpu..." << RESET << std::endl;
    if(testlast_cpu())
    {
        std::cout << BOLDCYAN << "    Last_CPU" << RESET << " test"
            << WHITE << " succeeded!" << RESET << std::endl;
        m_successful_tests += 1;
    }
    else
    {
        std::cout << BOLDCYAN << "    Last_CPU" << RESET << " test"
            << RED << " succeeded!" << RESET << std::endl;
        m_failed_tests += 1;
    }
    std::cout << std::endl;

    /* getallowed_cpus */
    std::cout << BOLDGREEN << "25. Testing Allowed_CPUs..." << RESET << std::endl;
    if(testallowed_cpus())
    {
        std::cout << BOLDCYAN << "    Allowed_CPUs" << RESET << " test"
            << WHITE << " succeeded!" << RESET << std::endl;
        m_successful_tests += 1;
    }
    else
    {
        std::cout << BOLDCYAN << "    Allowed_CPUs" << RESET << " test"
            << RED << " failed!" << RESET << std::endl;
        m_failed_tests += 1;
    }
    std::cout << std::endl;

    /* getmemory_map */
    std::cout << BOLDGREEN << "26. Testing Memory_Map..." << RESET << std::endl;
    if(testmemory_map())
    {
        std::cout << BOLDCYAN << "    Memory_Map" << RESET << " test"
            << WHITE << " succeeded!" << RESET << std::endl;
        m_successful_tests += 1;
    }
    else
    {
        std::cout << BOLDCYAN << "    Memory_Map" << RESET << " test"
            << WHITE << " failed!" << RESET << std::endl;
        m_failed_tests += 1;
    }
    std::cout << std::endl;


    /* Ending Statistics */
    std::cout << BOLDCYAN << "# ---------------------------------------------------------------------------- #" << RESET << std::endl;
    std::cout << BOLDCYAN << "# Final Statistics                                                             #" << RESET << std::endl;
    std::cout << BOLDCYAN << "# ---------------------------------------------------------------------------- #" << RESET << std::endl;
    std::cout << std::endl;
    std::cout << BOLDGREEN << "Correctness Tests:" << RESET << std::endl;
    std::cout << "  - Passed Tests: " << m_successful_tests << " / " << m_total_tests << std::endl;
    std::cout << "  - Failed Tests: " << m_failed_tests << " / " << m_total_tests << std::endl;
    std::cout << std::endl;
}


int main()
{
    MP4_Test *test_unit = new MP4_Test;

    test_unit->testall();

    delete test_unit;

    return 0;
}
