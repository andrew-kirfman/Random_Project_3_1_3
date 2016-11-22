/* ------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman                                                 */
/* Project: CSCE-313 Machine Problem #4                                      */
/*                                                                           */
/* File: ./proctest.cpp (student version)                                    */
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
/* User Defined Includes                                                     */
/* ------------------------------------------------------------------------- */

#include "proctest.h"

/* ------------------------------------------------------------------------- */
/* Proctest Private Member Functions                                         */
/* ------------------------------------------------------------------------- */

/* 
 * This function turns a char* buffer into a string given the address
 * to the buffer.  
 */ 
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

/* 
 * This function takes in a string and breaks it up into a vector
 * of space separated words.  
 */ 
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

    /* Initialize the read buffer */

    /* Read from the stat file */

    /* Read from the status file */

    /* Find the number of open file descriptors by counting /proc/[pid]/fd */

    /* Read Memory Map */
}


Proctest::~Proctest()
{
    // Free buffer memory & clear all vectors
}


std::string Proctest::getpid()
{
    // Dummy return statement
    return "";
}


std::string Proctest::getppid()
{
    // Dummy return statement
    return "";
}

std::string Proctest::geteuid()
{
    // Dummy return statement
    return "";
}

std::string Proctest::getegid()
{
    // Dummy return statement
    return "";
}

std::string Proctest::getruid()
{
    // Dummy return statement
    return "";
}

std::string Proctest::getrgid()
{
    // Dummy return statement
    return "";
}

std::string Proctest::getfsuid()
{
    // Dummy return statement
    return "";
}

std::string Proctest::getfsgid()
{
    // Dummy return statement
    return "";
}

std::string Proctest::getstate()
{
    // Dummy return statement
    return "";
}

std::string Proctest::getthread_count()
{
    // Dummy return statement
    return "";
}

std::string Proctest::getpriority()
{
    // Dummy return statement
    return "";
}

std::string Proctest::getniceness()
{
    // Dummy return statement
    return "";
}

std::string Proctest::getstime()
{
    // Dummy return statement
    return "";
}

std::string Proctest::getutime()
{
    // Dummy return statement
    return "";
}

std::string Proctest::getcstime()
{
    // Dummy return statement
    return "";
}

std::string Proctest::getcutime()
{
    // Dummy return statement
    return "";
}

std::string Proctest::getstartcode()
{
    // Dummy return statement
    return "";
}

std::string Proctest::getendcode()
{
    // Dummy return statement
    return "";
}

std::string Proctest::getesp()
{
    // Dummy return statement
    return "";
}

std::string Proctest::geteip()
{
    // Dummy return statement
    return "";
}

std::string Proctest::getfiles()
{
    // Dummy return statement
    return "";
}

std::string Proctest::getvoluntary_context_switches()
{
    // Dummy return statement
    return "";
}

std::string Proctest::getnonvoluntary_context_switches()
{
    // Dummy return statement
    return "";
}

std::string Proctest::getlast_cpu()
{
    // Dummy return statement
    return "";
}

std::string Proctest::getallowed_cpus()
{
    // Dummy return statement
    return "";
}

std::vector<std::string> Proctest::getmemory_map()
{
    // Dummy return statement
    return std::vector<std::string>();
}
