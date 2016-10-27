#!/bin/bash

# --------------------------------------------------------------------------- #
# Developer: Andrew Kirfman                                                   #
# Project: CSCE-313 Machine Problem #4                                        #
#                                                                             #
# File: ./proctest.sh                                                         #
# --------------------------------------------------------------------------- #

# Forward declare the PID variable
PID=""

# Need to filter program so that only the functions are defined on a test
TESTING=false
if [ $# -gt 0 ]
then
    TESTING=true
fi

function getpid()
{
    OUTPUT="$(perl -l -0777 -ne '@f = /\(.*\)|\S+/g; print $f[0]' "/proc/$PID/stat")"
    OUTPUT="$(echo $OUTPUT | sed 's/[ \\n]//g')"
    if [ $? -eq 0 ]
    then
        printf "$OUTPUT"
        return 0
    else
        printf "ERROR"
        return 1
    fi
}

function getppid()
{
    OUTPUT="$(perl -l -0777 -ne '@f = /\(.*\)|\S+/g; print $f[3]' "/proc/$PID/stat")"
    OUTPUT="$(echo $OUTPUT | sed 's/[ \\n]//g')"
    if [ $? -eq 0 ]
    then
        printf "$OUTPUT"
        return 0
    else
        printf "ERROR"
        return 1
    fi
}

function geteuid()
{
    OUTPUT="$(grep 'Uid' /proc/$PID/status | awk '{print $3}')"
    OUTPUT="$(echo $OUTPUT | sed 's/[ \\n]//g')"
    if [ $? -eq 0 ]
    then
        printf "$OUTPUT"
        return 0
    else
        printf "ERROR"
        return 1
    fi
}

function getegid()
{
    OUTPUT="$(grep 'Gid' /proc/$PID/status | awk '{print $3}')"
    OUTPUT="$(echo $OUTPUT | sed 's/[ \\n]//g')"
    if [ $? -eq 0 ]
    then
        printf "$OUTPUT"
        return 0
    else
        printf "ERROR"
        return 1
    fi
}

function getruid()
{
    OUTPUT="$(grep 'Uid' /proc/$PID/status | awk '{print $2}')"
    OUTPUT="$(echo $OUTPUT | sed 's/[ \\n]//g')"
    if [ $? -eq 0 ]
    then
        printf "$OUTPUT"
        return 0
    else
        printf "ERROR"
        return 1
    fi
}

function getrgid()
{
    OUTPUT="$(grep 'Gid' /proc/$PID/status | awk '{print $2}')"
    OUTPUT="$(echo $OUTPUT | sed 's/[ \\n]//g')"
    if [ $? -eq 0 ]
    then
        printf "$OUTPUT"
        return 0
    else
        printf "ERROR"
        return 1
    fi
}

function getfsuid()
{
    OUTPUT="$(grep 'Uid' /proc/$PID/status | awk '{print $5}')"
    OUTPUT="$(echo $OUTPUT | sed 's/[ \\n]//g')"
    if [ $? -eq 0 ]
    then
        printf "$OUTPUT"
        return 0
    else
        printf "ERROR"
        return 1
    fi
}

function getfsgid()
{
    OUTPUT="$(grep 'Gid' /proc/$PID/status | awk '{print $5}')"
    OUTPUT="$(echo $OUTPUT | sed 's/[ \\n]//g')"
    if [ $? -eq 0 ]
    then
        printf "$OUTPUT"
        return 0
    else
        printf "ERROR"
        return 1
    fi
}

function getstate()
{
    OUTPUT="$(cat "/proc/$PID/status" | grep "State" | sed 's/State:\t//')\n"
    OUTPUT="$(echo $OUTPUT | sed 's/(.*)//g')"
    OUTPUT="$(echo $OUTPUT | sed 's/[ \\n]//g')"
    if [ $? -eq 0 ]
    then
        printf "$OUTPUT"
        return 0
    else
        printf "ERROR"
        return 1
    fi
}

function getthread_count()
{
    OUTPUT="$(perl -l -0777 -ne '@f = /\(.*\)|\S+/g; print $f[19]' "/proc/$PID/stat") \n"
    OUTPUT="$(echo $OUTPUT | sed 's/[ \\n]//g')"
    if [ $? -eq 0 ]
    then
        printf "$OUTPUT"
        return 0
    else
        printf "ERROR"
        return 1
    fi
}

function getpriority()
{
    OUTPUT="$(perl -l -0777 -ne '@f = /\(.*\)|\S+/g; print $f[17]' "/proc/$PID/stat") \n"
    OUTPUT="$(echo $OUTPUT | sed 's/[ \\n]//g')"
    if [ $? -eq 0 ]
    then
        printf "$OUTPUT"
        return 0
    else
        printf "ERROR"
        return 1
    fi
}

function getniceness()
{
    OUTPUT="$(perl -l -0777 -ne '@f = /\(.*\)|\S+/g; print $f[18]' "/proc/$PID/stat") \n"
    OUTPUT="$(echo $OUTPUT | sed 's/[ \\n]//g')"
    if [ $? -eq 0 ]
    then
        printf "$OUTPUT"
        return 0
    else
        printf "ERROR"
        return 1
    fi
}

function getstime()
{
    OUTPUT="$(perl -l -0777 -ne '@f = /\(.*\)|\S+/g; print $f[14]' "/proc/$PID/stat") \n"
    OUTPUT="$(echo $OUTPUT | sed 's/[ \\n]//g')"
    if [ $? -eq 0 ]
    then
        printf "$OUTPUT"
        return 0
    else
        printf "ERROR"
        return 1
    fi
}

function getutime()
{
    OUTPUT="$(perl -l -0777 -ne '@f = /\(.*\)|\S+/g; print $f[13]' "/proc/$PID/stat") \n"
    OUTPUT="$(echo $OUTPUT | sed 's/[ \\n]//g')"
    if [ $? -eq 0 ]
    then
        printf "$OUTPUT"
        return 0
    else
        printf "ERROR"
        return 1
    fi
}

function getcstime()
{
    OUTPUT="$(perl -l -0777 -ne '@f = /\(.*\)|\S+/g; print $f[16]' "/proc/$PID/stat") \n"
    OUTPUT="$(echo $OUTPUT | sed 's/[ \\n]//g')"
    if [ $? -eq 0 ]
    then
        printf "$OUTPUT"
        return 0
    else
        printf "ERROR"
        return 1
    fi
}

function getcutime()
{
    OUTPUT="$(perl -l -0777 -ne '@f = /\(.*\)|\S+/g; print $f[15]' "/proc/$PID/stat") \n"
    OUTPUT="$(echo $OUTPUT | sed 's/[ \\n]//g')"
    if [ $? -eq 0 ]
    then
        printf "$OUTPUT"
        return 0
    else
        printf "ERROR"
        return 1
    fi
}

function getstartcode()
{
    OUTPUT="$(perl -l -0777 -ne '@f = /\(.*\)|\S+/g; print $f[25]' "/proc/$PID/stat") \n"
    OUTPUT="$(echo $OUTPUT | sed 's/[ \\n]//g')"
    if [ $? -eq 0 ]
    then
        printf "$OUTPUT"
        return 0
    else
        printf "ERROR"
        return 1
    fi
}

function getendcode()
{
    OUTPUT="$(perl -l -0777 -ne '@f = /\(.*\)|\S+/g; print $f[25]' "/proc/$PID/stat") \n"
    OUTPUT="$(echo $OUTPUT | sed 's/[ \\n]//g')"
    if [ $? -eq 0 ]
    then
        printf "$OUTPUT"
        return 0
    else
        printf "ERROR"
        return 1
    fi
}

function getesp()
{
    OUTPUT="$(perl -l -0777 -ne '@f = /\(.*\)|\S+/g; print $f[28]' "/proc/$PID/stat") \n"
    OUTPUT="$(echo $OUTPUT | sed 's/[ \\n]//g')"
    if [ $? -eq 0 ]
    then
        printf "$OUTPUT"
        return 0
    else
        printf "ERROR"
        return 1
    fi
}

function geteip()
{ 
    OUTPUT="$(perl -l -0777 -ne '@f = /\(.*\)|\S+/g; print $f[29]' "/proc/$PID/stat") \n"
    OUTPUT="$(echo $OUTPUT | sed 's/[ \\n]//g')"
    if [ $? -eq 0 ]
    then
        printf "$OUTPUT"
        return 0
    else
        printf "ERROR"
        return 1
    fi
}

function getfiles()
{
    ls -la "/proc/$PID/fd" 2>1 > /dev/null
    if [ $? -eq 2 ]
    then
        printf "permission denied"
        return 1
    else
        declare -i OUTPUT=$(ls -la "/proc/$PID/fd" | wc -l)
        if [ $? -eq 0 ]
        then
            OUTPUT=$(($OUTPUT - 3))
            printf "$OUTPUT"
            return 0
        else
            printf "ERROR"
            return 1
        fi
    fi
}

function getvoluntary_context_switches()
{
    OUTPUT=$(grep "voluntary" "/proc/$PID/status" | grep -v "non" | sed 's/voluntary_ctxt_switches:\t//')
    OUTPUT="$(echo $OUTPUT | sed 's/[ \\n]//g')"
    if [ $? -eq 0 ]
    then
        printf "$OUTPUT"
        return 0
    else
        printf "ERROR"
        return 1
    fi
}

function getnonvoluntary_context_switches()
{
    OUTPUT=$(grep "nonvoluntary" "/proc/$PID/status" | sed 's/nonvoluntary_ctxt_switches:\t//')
    OUTPUT="$(echo $OUTPUT | sed 's/[ \\n]//g')"
    if [ $? -eq 0 ]
    then
        printf "$OUTPUT"
        return 0
    else
        printf "ERROR"
        return 1
    fi
}

function getlast_cpu()
{
    OUTPUT=$(perl -l -0777 -ne '@f = /\(.*\)|\S+/g; print $f[38]' "/proc/$PID/stat")
    OUTPUT="$(echo $OUTPUT | sed 's/[ \\n]//g')"
    if [ $? -eq 0 ]
    then
        printf "$OUTPUT"
        return 0
    else
        printf "ERROR"
        return 1
    fi
}

function getallowed_cpus()
{
    OUTPUT=$(grep "Cpus_allowed_list" "/proc/$PID/status" | sed 's/Cpus_allowed_list:\t//')
    OUTPUT="$(echo $OUTPUT | sed 's/[ \\n]//g')"
    if [ $? -eq 0 ]
    then
        printf "$OUTPUT"
        return 0
    else
        printf "$OUTPUT"
        return 1
    fi
}

function getmemory_map()
{
    cat /proc/$PID/smaps 1> /dev/null 2> /dev/null
    if [ $? -ne 0 ]
    then
        printf "permission denied"
        return 1
    fi

    cat /proc/$PID/smaps | grep -v '^[A-Z[:punct:]]' | while read line
    do
        echo "$line"
    done

    return 0
}

# Main program here
if [ $TESTING == false ]
then
    # Read in the value of the pid that the user would like to examine.  
    printf "Enter the pid of a process: " 
    read PID

    # If the value entered is not an integer, try again.  
    while ! [ "$PID" -eq "$PID" ] 2>/dev/null
    do
        printf "[ERROR]: Number was not a valid integer.  Try again: " 
        read PID
    done

    # Search the system to make sure that the process exists.  
    ls "/proc/$PID" > /dev/null
    if [ $? == 1 ]
    then
        echo "[ERROR]: A Process with the given ID does not exist on this system! - Exiting!"
        exit 1
    fi

    printf "\nProcess Information: " 
    
    printf "\n1) Identifiers\n"
    printf "   PID:   $(getpid) \n"
    printf "   PPID:  $(getppid) \n"
    printf "   EUID:  $(geteuid) \n"
    printf "   EGID:  $(getegid) \n"
    printf "   RUID:  $(getruid) \n"
    printf "   RGID:  $(getrgid) \n"
    printf "   FSUID: $(getfsuid) \n"
    printf "   FSGID: $(getfsgid) \n"
    
    printf "\n2) State: \n"
    printf "   State: $(getstate)\n"
    
    printf "\n3) Thread Information: \n"
    printf "   Thread Count: $(getthread_count) \n"
    
    printf "\n4) Priority: \n"
    printf "   Priority Number: $(getpriority) \n"
    printf "   Niceness Value:  $(getniceness) \n"
    
    printf "\n5) Time Information: \n"
    printf "   stime:  $(getstime) \n"
    printf "   utime:  $(getutime) \n"
    printf "   cstime: $(getcstime) \n"
    printf "   cutime: $(getcutime) \n"
    
    printf "\n6) Address Space: \n"
    printf "   Startcode: $(getstartcode) \n"
    printf "   Endcode:   $(getendcode) \n"
    printf "   ESP:       $(getesp) \n"
    printf "   EIP:       $(geteip) \n"
    
    printf "\n7) Resourses: \n"
    printf "   File Handles: $(getfiles) \n"
    printf "   Voluntary Context Switches: $(getvoluntary_context_switches)\n"
    printf "   Involuntary Context Switches: $(getnonvoluntary_context_switches)\n"
    
    printf "\n8) Processors: \n"
    printf "   Last processor: $(getlast_cpu) \n"
    printf "   Allowed Cores:  $(getallowed_cpus) \n"
        
    printf "\n9) Memory Map: \n"
    printf "$(getmemory_map) \n"

    exit 0
fi
