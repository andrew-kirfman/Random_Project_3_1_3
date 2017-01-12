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

}

function getppid()
{

}

function geteuid()
{

}

function getegid()
{

}

function getruid()
{
    
}

function getrgid()
{

}

function getfsuid()
{

}

function getfsgid()
{

}

function getstate()
{

}

function getthread_count()
{
    
}

function getpriority()
{

}

function getniceness()
{

}

function getstime()
{

}

function getutime()
{

}

function getcstime()
{

}

function getcutime()
{

}

function getstartcode()
{

}

function getendcode()
{

}

function getesp()
{

}

function geteip()
{ 

}

function getfiles()
{

}

function getvoluntary_context_switches()
{

}

function getnonvoluntary_context_switches()
{

}

function getlast_cpu()
{

}

function getallowed_cpus()
{

}

function getmemory_map()
{

}

# Main program here
if [ $TESTING == false ]
then
    # Read in the value of the pid that the user would like to examine.  

    # If the value entered is not an integer, try again.  

    # Search the system to make sure that the process exists.  

    # Actually print everything out

    # Quit when done!
    exit 0
fi
