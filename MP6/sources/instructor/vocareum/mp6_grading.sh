# ---------------------------------------------------------------------------- #
# Substitute Main File                                                         #
# ---------------------------------------------------------------------------- #

# Replace students main file for a temporary one
mv main.cpp saved_main.cpp

echo "
/* --------------------------------------------------------------------------- */
/* Project: CSCE-313 Machine Problem #6                                        */
/* Developer: Andrew Kirfman												   */
/*                                                                             */
/* File: ./MP6/main.cpp                                                        */
/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/* Standard Library Includes                                                   */
/* --------------------------------------------------------------------------- */

#include<iostream>
#include<vector>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<sys/time.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<time.h>
#include<math.h>
#include<utility>
#include<float.h>

/* --------------------------------------------------------------------------- */
/* User Defined Includes                                                       */
/* --------------------------------------------------------------------------- */

#include \"utility.h\"
#include \"Scheduler.h\"

/* --------------------------------------------------------------------------- */
/* Global Variables                                                            */
/* --------------------------------------------------------------------------- */

std::vector<pid_t> running_processes;

void cleanup()
{
    /* Clean up all running processes */
    for(unsigned short int i=0; i<running_processes.size(); i++)
    {
        kill(running_processes[i], SIGKILL);
    }

    /* Clean up temporary directories */
    system(\"rm -rf ./active\");

    exit(255);
}

void handle_signals(int signum)
{
    cleanup();
}

int main(int argc, char **argv)
{
    bool fifo = false;
    bool round_robin = false;
    bool sjf = false;
    bool interactive_fifo = false;
    bool interactive_round_robin = false;
    bool interactive_sjf = false;	

    int c;

	opterr = 0;
	while ((c = getopt (argc, argv, \"frsFRS\")) != -1)
	{
		switch (c)
		{
			case 'f':
				fifo = true;
				break;
			case 'r':
				round_robin = true;
				break;
			case 's':
				sjf = true;
				break;
			case 'F':
				interactive_fifo = true;
				break;
			case 'R':
				interactive_round_robin = true;
				break;
			case 'S':
				interactive_sjf = true;
				break;
			case '?':
				std::cout << \"[ERROR]: Unknown option.\" << std::endl;
				return 1;
			default:
				std::cout << \"[ERROR]: Unknown option.\" << std::endl;
				return 1;
		}
	}
	
    /* Define signal handlers to clean up from exceptions */
    struct sigaction signal_struct;
    signal_struct.sa_handler = handle_signals;
    sigaction(SIGINT, &signal_struct, NULL);
    sigaction(SIGQUIT, &signal_struct, NULL);
    sigaction(SIGILL, &signal_struct, NULL);
    sigaction(SIGABRT, &signal_struct, NULL);
    sigaction(SIGFPE, &signal_struct, NULL);
    sigaction(SIGSEGV, &signal_struct, NULL);
    sigaction(SIGTERM, &signal_struct, NULL);

    /* Reused Variables */
    pid_t pid = 0;
    int return_val = 0;
    std::string system_string = \"\";

    /* Clear old occurrences of the temp directories */
    return_val = system(\"rm -rf ./active 2>&1 > /dev/null\");
    if(return_val == -1)
    {
        std::cout << \"[\" << BOLDRED << \"ERROR\" << RESET << \"]: Cound not remove old running_processes directory.\" << std::endl;
    }

    /* Make a directory to write executable files to */
    mkdir(\"./active\", S_IRWXU | S_IRWXG | S_IRWXO);

	for(unsigned short int i=1; i<=5; i++)
	{
		system_string = \"g++ -std=c++11 ./running_processes/process_\" + std::to_string(i) + \".cpp -o ./active/process_\" 
			+ std::to_string(i);
		return_val = system(system_string.c_str());
		if(return_val == -1)
		{
			std::cout << \"  [\" << BOLDRED << \"ERROR\" << RESET << \"]: Could not create Process_\" << std::to_string(i) << std::endl
				<< \"    \" << strerror(errno) << std::endl;
			cleanup();
		}
	}
                                                            
    /* ------------------------------------------------------------------------- */
    /* First In First Out                                                        */
    /* ------------------------------------------------------------------------- */

    if(fifo == true)
    {
        std::cout << std::endl;
        std::cout << BOLDGREEN << \"/* -------------------------------------------------------------------------- */\" << RESET << std::endl;
        std::cout << BOLDGREEN << \"/* First In First Out                                                         */\" << RESET << std::endl;
        std::cout << BOLDGREEN << \"/* -------------------------------------------------------------------------- */\" << RESET << std::endl;
        std::cout << std::endl;

        std::cout << \"Starting processes.\" << std::endl;
        for(unsigned short int i=1; i<=5; i++)
        {
			pid = fork();

			if(pid == 0)
			{
				system_string = \"./active/process_\" + std::to_string(i);
				return_val = execlp(system_string.c_str(), system_string.c_str(), NULL);
				exit(0);
			}
			else if(pid == -1)
			{
				std::cout << \"  [\" << BOLDRED << \"ERROR\" << RESET << \"]: Could not create Process_\" << std::to_string(i) << std::endl
					<< \"    \" << strerror(errno) << std::endl;
				cleanup();
			}
			else
			{
				running_processes.push_back(pid);
				std::cout << \"  [\" << BOLDBLUE << \"SUCCESS\" << RESET << \"]: Created Process_\" << std::to_string(i) << std::endl;
			}
        }
        std::cout << std::endl;

        /* Instantiate the scheduler class and add the processes to it */
        Scheduler *fifo_scheduler = new Scheduler();
        for(unsigned short int i=0; i<running_processes.size(); i++)
        {
            fifo_scheduler->schedule_process(running_processes[i]);
        }
        fifo_scheduler->set_policy(FIFO);

        std::cout << \"Starting CPU bound scheduler.\" << std::endl;
        fifo_scheduler->schedule_all();
        std::cout << std::endl;

        /* Clear running processes vector */
        running_processes.clear();

        delete fifo_scheduler;
    }

    /* ------------------------------------------------------------------------- */
    /* Round Robbin                                                              */
    /* ------------------------------------------------------------------------- */
    
    if(round_robin == true)
    {
        std::cout << std::endl;
        std::cout << BOLDGREEN << \"/* -------------------------------------------------------------------------- */\" << RESET << std::endl;
        std::cout << BOLDGREEN << \"/* Round Robin                                                                */\" << RESET << std::endl;
        std::cout << BOLDGREEN << \"/* -------------------------------------------------------------------------- */\" << RESET << std::endl;
        std::cout << std::endl;

        /* Reinitialize the 5 processes */
        std::cout << \"Starting processes.\" << std::endl;

        for(unsigned short int i=1; i<=5; i++)
        {
            pid = fork();
    
            if(pid == 0)
            {
                system_string = \"./active/process_\" + std::to_string(i);
                return_val = execlp(system_string.c_str(), system_string.c_str(), NULL);
                exit(0);
            }
            else if(pid == -1)
            {
                std::cout << \"  [\" << BOLDRED << \"ERROR\" << RESET << \"]: Could not create Process_\" << std::to_string(i) << std::endl
                    << \"    \" << strerror(errno) << std::endl;
                cleanup();
            }
            else
            {
                running_processes.push_back(pid);
                std::cout << \"  [\" << BOLDBLUE << \"SUCCESS\" << RESET << \"]: Created Process_\" << std::to_string(i) << std::endl;
            }
        }
        std::cout << std::endl;

        /* Instantiate the scheduler class and add the processes to it */
        Scheduler *rr_scheduler = new Scheduler();
        for(unsigned short int i=0; i<running_processes.size(); i++)
        {
            rr_scheduler->schedule_process(running_processes[i]);
        }
        rr_scheduler->set_policy(ROUND_ROBIN);

        std::cout << \"Starting Round Robin scheduler.\" << std::endl; 
        rr_scheduler->schedule_all();
        std::cout << std::endl;

        /* Clear running processes vector */
        running_processes.clear();
        delete rr_scheduler;
    }

    /* ------------------------------------------------------------------------- */
    /* Shortest Job First                                                        */
    /* ------------------------------------------------------------------------- */
    
    if(sjf == true)
    {
        std::cout << std::endl;
        std::cout << BOLDGREEN << \"/* -------------------------------------------------------------------------- */\" << RESET << std::endl;
        std::cout << BOLDGREEN << \"/* Shortest Job First                                                         */\" << RESET << std::endl;
        std::cout << BOLDGREEN << \"/* -------------------------------------------------------------------------- */\" << RESET << std::endl;
        std::cout << std::endl;

        /* Reinitialize the 5 processes */
        std::cout << \"Starting processes.\" << std::endl;

        for(unsigned short int i=1; i<=5; i++)
        {
            pid = fork();

            if(pid == 0)
            {
                system_string = \"./active/process_\" + std::to_string(i);
                return_val = execlp(system_string.c_str(), system_string.c_str(), NULL);
                exit(0);
            }
            else if(pid == -1)
            {
                std::cout << \"  [\" << BOLDRED << \"ERROR\" << RESET << \"]: Could not create Process_\" << std::to_string(i) << std::endl
                    << \"    \" << strerror(errno) << std::endl;
                cleanup();
            }
            else
            {
                running_processes.push_back(pid);
                std::cout << \"  [\" << BOLDBLUE << \"SUCCESS\" << RESET << \"]: Created Process_\" << std::to_string(i) << std::endl;
            }
        }
        std::cout << std::endl;

        /* Instantiate the scheduler class and add the processes to it */
        Scheduler *sjf_scheduler = new Scheduler();

        sjf_scheduler->schedule_process(running_processes[0], 8.3);
        sjf_scheduler->schedule_process(running_processes[1], 10.9);
        sjf_scheduler->schedule_process(running_processes[2], 1.92);
        sjf_scheduler->schedule_process(running_processes[3], 33.2);
        sjf_scheduler->schedule_process(running_processes[4], 41.7);
        std::cout << \"Starting Shortest Job First scheduler.\" << std::endl;
        std::cout << \"  [\" << BOLDWHITE << \"INFO\" << RESET << \"]: Process_1 (Sieve of Eratosthenes): Expected run time: 8.3 seconds.\" << std::endl;
        std::cout << \"  [\" << BOLDWHITE << \"INFO\" << RESET << \"]: Process_2 (Matrix Multiplication): Expected run time: 10.9 seconds.\" << std::endl;
        std::cout << \"  [\" << BOLDWHITE << \"INFO\" << RESET << \"]: Process_3 (Tight Loop): Expected run time: 1.92 seconds.\" << std::endl;
        std::cout << \"  [\" << BOLDWHITE << \"INFO\" << RESET << \"]: Process_4 (Bubble Sort): Expected run time: 33.2 seconds.\" << std::endl;
        std::cout << \"  [\" << BOLDWHITE << \"INFO\" << RESET << \"]: Process_5 (Cryptarithm Solver): Expected run time: 41.7 seconds.\" << std::endl;

        sjf_scheduler->set_policy(SJF);
        sjf_scheduler->schedule_all();
        std::cout << std::endl;

        /* Clear the running process vector */
        running_processes.clear();
        delete sjf_scheduler;
    }

    /* ------------------------------------------------------------------------- */
    /* Interactive First In First Out                                            */
    /* ------------------------------------------------------------------------- */
 
    if(interactive_fifo == true)
    {
        std::cout << std::endl;
        std::cout << BOLDGREEN << \"/* -------------------------------------------------------------------------- */\" << RESET << std::endl;
        std::cout << BOLDGREEN << \"/* Interactive First In First Out                                             */\" << RESET << std::endl;
        std::cout << BOLDGREEN << \"/* -------------------------------------------------------------------------- */\" << RESET << std::endl;
        std::cout << std::endl;

        /* Instantiate the scheduler class */
        Scheduler *interactive_fifo_scheduler = new Scheduler();
    
        /* Start scheduler running */
        interactive_fifo_scheduler->set_policy(INTERACTIVE_FIFO);
        interactive_fifo_scheduler->schedule_all();
    }

    /* ------------------------------------------------------------------------- */
    /* Interactive Round Robin                                                   */
    /* ------------------------------------------------------------------------- */
 
    if(interactive_round_robin == true)
    {
        std::cout << std::endl;
        std::cout << BOLDGREEN << \"/* -------------------------------------------------------------------------- */\" << RESET << std::endl;
        std::cout << BOLDGREEN << \"/* Interactive Round Robin                                                    */\" << RESET << std::endl;
        std::cout << BOLDGREEN << \"/* -------------------------------------------------------------------------- */\" << RESET << std::endl;
        std::cout << std::endl;

        /* Instantiate the scheduler class */
        Scheduler *interactive_rr_scheduler = new Scheduler();
        
        /* Start scheduler running */
        interactive_rr_scheduler->set_policy(INTERACTIVE_ROUND_ROBIN);
        interactive_rr_scheduler->schedule_all();
    }

    /* ------------------------------------------------------------------------- */
    /* Interactive Shortest Job First                                            */
    /* ------------------------------------------------------------------------- */
    
    if(interactive_sjf == true)
    {
        std::cout << std::endl;
        std::cout << BOLDGREEN << \"/* -------------------------------------------------------------------------- */\" << RESET << std::endl;
        std::cout << BOLDGREEN << \"/* Interactive Shortest Job First                                             */\" << RESET << std::endl;
        std::cout << BOLDGREEN << \"/* -------------------------------------------------------------------------- */\" << RESET << std::endl;
        std::cout << std::endl;

        /* Instantiate the scheduler class */
        Scheduler *interactive_sjf_scheduler = new Scheduler();
    
        /* Start scheduler running */
        interactive_sjf_scheduler->set_policy(INTERACTIVE_SJF);
        interactive_sjf_scheduler->schedule_all();
    }

    cleanup();
    return 0;                                                          
}
" > main.cpp

# ---------------------------------------------------------------------------- #
# Code Compiles                                                                #
# ---------------------------------------------------------------------------- #

make clean > /dev/null

# Make sure that the code compiles
make > /dev/null
if [ $? == 0 ]; then
	echo "Code Compiles,20" >>  $vocareumGradeFile

	echo 'Code Compiles ... Passed [20/20]' >> $vocareumReportFile
else
	echo "Code Compiles,0" >> $vocareumGradeFile
	echo "Proper Code Design,0" >> $vocareumGradeFile
	echo "Proper Use of Signals,0" >> $vocareumGradeFile
	echo "FIFO Scheduler,0" >> $vocareumGradeFile
	echo "Round Robin Scheduler,0" >> $vocareumGradeFile
	echo "SJF Scheduler,0" >> $vocareumGradeFile
	echo "Interactive FIFO (Bonus),0" >> $vocareumGradeFile
	echo "Interactive RR (Bonus),0" >> $vocareumGradeFile
	echo "Interactive SJF (Bonus),0" >> $vocareumGradeFile

	echo "Code Compiles         ...  Failed [00/20]" >> $vocareumReportFile
	echo "Proper Code Design    ...  Untestable [00/10]" >> $vocareumReportFile
    echo "Proper Use of Signals ...  Untestable [00/10]" >> $vocareumReportFile
	echo "FIFO Scheduler        ...  Untestable [00/20]" >> $vocareumReportFile
	echo "Round Robin Scheduler ...  Untestable [00/20]" >> $vocareumReportFile
	echo "SJF Scheduler         ...  Untestable [00/20]" >> $vocareumReportFile
	echo "Interactive FIFO      ...  Untestable [00/20]" >> $vocareumReportFile
	echo "Interactive RR        ...  Untestable [00/20]" >> $vocareumReportFile
	echo "Interactive SJF       ...  Untestable [00/20]" >> $vocareumReportFile

	exit 0
fi

# Important note: In order to pass these tests, the students will have to print out the string "Process_(X) terminated"
# perfectly in order to be recognized as successful.  This testing script will also check to make sure that the student
# isn't trying to cheat by simply making his code return the strings checked for in the right order.  

# ---------------------------------------------------------------------------- #
# Code Consistency                                                             #
# ---------------------------------------------------------------------------- #

# 1. Test to make sure the student didn't use any gotos
test_1=$(cat Scheduler.h | grep "goto")
test_2=$(cat Scheduler.cpp | grep "goto")
test_3=$(cat Running_process.h | grep "goto")
test_4=$(cat Running_process.cpp | grep "goto")
test_5=$(cat shell.cpp | grep "goto")
test_6=$(cat shell.h | grep "goto")
test_7=$(cat saved_main.cpp | grep "goto")
echo "Testing Code Integrity" >> $vocareumReportFile

FAILED_CHECKS=0

if [ "$test"_1 != "" ]; then
	let "FAILED_CHECKS=$FAILED_CHECKS+1"
	echo "  Code Integrity 1 (Scheduler.h gotos)       ...  Failed [-01/10]" >> $vocareumReportFile
else
	echo "  Code Integrity 1 (Scheduler.h gotos)       ...  Passed [00/10]" >> $vocareumReportFile

fi

if [ "$test_2" != "" ]; then
	let "FAILED_CHECKS=$FAILED_CHECKS+1"
	echo "  Code Integrity 2 (Scheduler.cpp gotos)       ...  Failed [-01/10]" >> $vocareumReportFile
else
	echo "  Code Integrity 2 (Scheduler.cpp gotos)       ...  Passed [00/10]" >> $vocareumReportFile
fi

if [ "$test_3" != "" ]; then
	let "FAILED_CHECKS=$FAILED_CHECKS+1"
	echo "  Code Integrity 3 (Running_process.h gotos)       ...  Failed [-01/10]" >> $vocareumReportFile
else
	echo "  Code Integrity 3 (Running_process.h gotos)       ...  Passed [00/10]" >> $vocareumReportFile
fi

if [ "$test_4" != "" ]; then
	let "FAILED_CHECKS=$FAILED_CHECKS+1"
	echo "  Code Integrity 4 (Running_process.cpp gotos)       ...  Failed [-01/10]" >> $vocareumReportFile
else
	echo "  Code Integrity 4 (Running_process.cpp gotos)       ...  Passed [00/10]" >> $vocareumReportFile

fi

if [ "$test_5" != "" ]; then
	let "FAILED_CHECKS=$FAILED_CHECKS+1"
	echo "  Code Integrity 5 (shell.cpp gotos)       ...  Failed [-01/10]" >> $vocareumReportFile
else
	echo "  Code Integrity 5 (shell.cpp gotos)       ...  Passed [00/10]" >> $vocareumReportFile
fi

if [ "$test_6" != "" ]; then
	let "FAILED_CHECKS=$FAILED_CHECKS+1"
	echo "  Code Integrity 6 (shell.h gotos)       ...  Failed [-01/10]" >> $vocareumReportFile
else
	echo "  Code Integrity 6 (shell.h gotos)       ...  Passed [00/10]" >> $vocareumReportFile
fi

if [ "$test_7" != "" ]; then
	let "FAILED_CHECKS=$FAILED_CHECKS+1"
	echo "  Code Integrity 7 (main.cpp gotos)       ...  Failed [-01/10]" >> $vocareumReportFile
else
	echo "  Code Integrity 7 (main.cpp gotos)       ...  Passed [00/10]" >> $vocareumReportFile

fi

# Test to see if the students are trying to cheat by hard returning
# process numbers
is_cheating=""
is_cheating_1=$(cat Scheduler.h | grep -E "Process_[1-5] terminated")
is_cheating_2=$(cat Scheduler.cpp | grep -E "Process_[1-5] terminated")
is_cheating_3=$(cat Running_process.h | grep -E "Process_[1-5] terminated")
is_cheating_4=$(cat Running_process.cpp | grep -E "Process_[1-5] terminated")
is_cheating_5=$(cat shell.cpp | grep -E "Process_[1-5] terminated")
is_cheating_6=$(cat shell.h | grep -E "Process_[1-5] terminated")
is_cheating_7=$(cat saved_main.cpp | grep -E "Process_[1-5] terminated")

CHEATING_FILES=0

if [ "$is_cheating_1" != "" ]; then
	let "CHEATING_FILES=$CHEATING_FILES+1"
	echo "  Cheating Found in Scheduler.h!" >> $vocareumReportFile
fi

if [ "$is_cheating_2" != "" ]; then
	let "CHEATING_FILES=$CHEATING_FILES+1"
	echo "  Cheating Found in Scheduler.cpp!" >> $vocareumReportFile
fi

if [ "$is_cheating_3" != "" ]; then
	let "CHEATING_FILES=$CHEATING_FILES+1"
	echo "  Cheating Found in Running_process.h!" >> $vocareumReportFile
fi

if [ "$is_cheating_4" != "" ]; then
	let "CHEATING_FILES=$CHEATING_FILES+1"
	echo "  Cheating Found in Running_process.cpp!" >> $vocareumReportFile
fi

if [ "$is_cheating_5" != "" ]; then
	let "CHEATING_FILES=$CHEATING_FILES+1"
	echo "  Cheating Found in shell.cpp!" >> $vocareumReportFile
fi

if [ "$is_cheating_6" != "" ]; then
	let "CHEATING_FILES=$CHEATING_FILES+1"
	echo "  Cheating Found in shell.h!" >> $vocareumReportFile
fi

if [ "$is_cheating_7" != "" ]; then
	let "CHEATING_FILES=$CHEATING_FILES+1"
	echo "  Cheating Found in main.cpp!" >> $vocareumReportFile
fi

if [ "$CHEATING_FILES" -gt 0 ]; then
	echo "STUDENT IS ATTEMPTING TO TRICK THE GRADING SCRIPT BY RETURNING FIXED VALUES!!"

	echo "Code Compiles,0" >> $vocareumGradeFile
	echo "Proper Code Design,0" >> $vocareumGradeFile
	echo "Proper Use of Signals,0" >> $vocareumGradeFile
	echo "FIFO Scheduler,0" >> $vocareumGradeFile
	echo "Round Robin Scheduler,0" >> $vocareumGradeFile
	echo "SJF Scheduler,0" >> $vocareumGradeFile
	echo "Interactive FIFO (Bonus),0" >> $vocareumGradeFile
	echo "Interactive RR (Bonus),0" >> $vocareumGradeFile
	echo "Interactive SJF (Bonus),0" >> $vocareumGradeFile

	echo "Code Compiles         ...  CHEATING [00/20]" >> $vocareumReportFile
	echo "Proper Code Design    ...  CHEATING [00/10]" >> $vocareumReportFile
    echo "Proper Use of Signals ...  CHEATING [00/10]" >> $vocareumReportFile
	echo "FIFO Scheduler        ...  CHEATING [00/20]" >> $vocareumReportFile
	echo "Round Robin Scheduler ...  CHEATING [00/20]" >> $vocareumReportFile
	echo "SJF Scheduler         ...  CHEATING [00/20]" >> $vocareumReportFile
	echo "Interactive FIFO      ...  CHEATING [00/20]" >> $vocareumReportFile
	echo "Interactive RR        ...  CHEATING [00/20]" >> $vocareumReportFile
	echo "Interactive SJF       ...  CHEATING [00/20]" >> $vocareumReportFile

	exit 1
fi

# Count really long lines.  
LONG_LINE_CATEGORY=0
LONG_LINES=0
ALL_CODE=""

# Add all lines into one big file
touch temp_line_count.txt
cat saved_main.cpp >> temp_line_count.txt
cat Scheduler.cpp >> temp_line_count.txt
cat Scheduler.h >> temp_line_count.txt
cat Running_process.cpp >> temp_line_count.txt
cat Running_process.h >> temp_line_count.txt
cat shell.cpp >> temp_line_count.txt
cat shell.h >> temp_line_count.txt

cat temp_line_count.txt | while read line; do
	line_length=${#line}
	
	if [[ "$line_length" -ge 120 ]]; then
		let "LONG_LINES=$LONG_LINES+1"
	fi
done

echo "  Checking for long lines (longer than 120 characters)" >> $vocareumReportFile
if [[ "$LONG_LINES" -gt 0 && "$LONG_LINES" -lt 40 ]]; then
	LONG_LINE_CATEGORY=1
	echo "  Between 1 and 39 long lines found." >> $vocareumReportFile
fi

if [[ "$LONG_LINES" -gt 39 && "$LONG_LINES" -lt 80 ]]; then
	LONG_LINE_CATEGORY=2
	echo "  Between 40 and 79 long lines found." >> $vocareumReportFile
fi

if [[ "$LONG_LINES" -gt 79 && "$LONG_LINES" -lt 120 ]]; then
	LONG_LINE_CATEGORY=3
	echo "  Between 80 and 119 long lines found." >> $vocareumReportFile
fi

if [[ "$LONG_LINES" -gt 119 ]]; then
	LONG_LINE_CATEGORY=4
	echo "  120 or more long lines found." >> $vocareumReportFile
fi

rm temp_line_count.txt

# Make sure the student isn't using the standard library namespace. 
# We really need to stop telling students that it's ok to do this.  
touch is_std.txt
cat saved_main.cpp >> is_std.txt
cat Scheduler.cpp >> is_std.txt
cat Scheduler.h >> is_std.txt
cat Running_process.cpp >> is_std.txt
cat Running_process.h >> is_std.txt
cat shell.cpp >> is_std.txt
cat shell.h >> is_std.txt

RESULT=""
IS_STD=0
RESULT=$(cat is_std.txt | grep "using namespace std;")
if [ "$RESULT" != "" ]; then
	IS_STD=1
fi

rm is_std.txt

CONSISTENCY_SCORE=10
let "CONSISTENCY_SCORE=$CONSISTENCY_SCORE-$FAILED_CHECKS"
let "CONSISTENCY_SCORE=$CONSISTENCY_SCORE-$LONG_LINE_CATEGORY"
let "CONSISTENCY_SCORE=$CONSISTENCY_SCORE-$IS_STD"

if [ $CONSISTENCY_SCORE -lt 0 ]; then
	CONSISTENCY_SCORE=0
fi

if [ $CONSISTENCY_SCORE == 10 ]; then
	echo "Proper Code Design,10" >> $vocareumGradeFile
	echo "Proper Code Design    ...  Passed [10/10]" >> $vocareumReportFile
else
	echo "Proper Code Design,$CONSISTENCY_SCORE" >> $vocareumGradeFile
	echo "Proper Code Design    ...  Partial Pass [$CONSISTENCY_SCORE/10]" >> $vocareumReportFile
fi


# ---------------------------------------------------------------------------- #
# Proper Use of Signals                                                        #
# ---------------------------------------------------------------------------- #

IS_SIGCONT=$(cat Scheduler.cpp | grep -E "kill(.*,.*SIGCONT)")
SIGNAL_POINTS=0

if [ "$IS_SIGCONT" != "" ]; then
	echo "Testing proper use of SIGCONT   ... Passed [5/5]" >> $vocareumReportFile
	let "SIGNAL_POINTS=$SIGNAL_POINTS+5"
else
	echo "Testing proper use of SIGCONT   ... Failed [0/5]" >> $vocareumReportFile
fi

IS_SIGSTOP=$(cat Scheduler.cpp | grep -E "kill(.*,.*SIGSTOP)")

if [ "$IS_SIGSTOP" != "" ]; then
	echo "Testing proper use of SIGSTOP   ... Passed [5/5]" >> $vocareumReportFile
	
else
	echo "Testing proper use of SIGSTOP   ... Failed [0/5]" >> $vocareumReportFile
fi

# ---------------------------------------------------------------------------- #
# FIFO Scheduler                                                               #
# ---------------------------------------------------------------------------- #

command_output=$(./scheduler -f)

# Use grep -E (essentialy egrep) to see if the processes terminated in the correct order
# Note that egrep does not match across newlines, have to combine it all into one string.  
command_output=$(echo $command_output | sed -e ':a' -e 'N' -e '$!ba' -e 's/\n/ /g')
echo $command_output | grep -E 'Process_1 terminated .* Process_2 terminated .* Process_3 terminated .* Process_4 terminated .* Process_5 terminated' > /dev/null

if [ $? == 0 ]; then
	echo "FIFO Scheduler,20" >> $vocareumGradeFile
	echo "FIFO Scheduler        ...  Passed [20/20]" >> $vocareumReportFile

else
	echo "FIFO Scheduler,0" >> $vocareumGradeFile
	echo "FIFO Scheduler        ...  Failed [00/20]" >> $vocareumReportFile
fi

# ---------------------------------------------------------------------------- #
# Round Robin Scheduler                                                        #
# ---------------------------------------------------------------------------- #

command_output=$(./scheduler -r)

command_output=$(echo $command_output | sed -e ':a' -e 'N' -e '$!ba' -e 's/\n/ /g')
echo $command_output | grep -E 'Process_3 terminated .* Process_1 terminated .* Process_2 terminated .* Process_4 terminated .* Process_5 terminated' > /dev/null

if [ $? == 0 ]; then
	echo "Round Robin Scheduler,20" >> $vocareumGradeFile
	echo "Round Robin Scheduler ...  Passed [20/20]" >> $vocareumReportFile

else
	echo "Round Robin Scheduler,0" >> $vocareumGradeFile
	echo "Round Robin Scheduler ...  Failed [00/20]" >> $vocareumReportFile	
fi

# ---------------------------------------------------------------------------- #
# SJF Scheduler                                                                #
# ---------------------------------------------------------------------------- #

command_output=$(./scheduler -s)

command_output=$(echo $command_output | sed -e ':a' -e 'N' -e '$!ba' -e 's/\n/ /g')
echo $command_output | grep -E 'Process_3 terminated .* Process_1 terminated .* Process_2 terminated .* Process_4 terminated .* Process_5 terminated' > /dev/null

if [ $? == 0 ]; then
	echo "SJF Scheduler,20" >> $vocareumGradeFile
	echo "SJF Scheduler         ...  Passed [20/20]" >> $vocareumReportFile

else
	echo "SJF Scheduler,0" >> $vocareumGradeFile
	echo "SJF Scheduler         ...  Failed [00/20]" >> $vocareumReportFile
fi

# Sub back in the existing main file so that it isn't overwritten
mv saved_main.cpp main.cpp

# Submit final grading things
