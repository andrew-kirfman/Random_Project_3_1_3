/* ------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman                                                 */ 
/* Project: CSCE-313 Episode #6                                              */
/*                                                                           */
/* File: ./shell.h                                                           */ 
/* ------------------------------------------------------------------------- */

#ifndef _SHELL_H_
#define _SHELL_H_

/* ------------------------------------------------------------------------- */
/* Standard Libaray Includes                                                 */
/* ------------------------------------------------------------------------- */

#include<iostream>
#include<cstdlib>
#include<ctime>
#include<unistd.h>
#include<stdlib.h>
#include<regex>
#include<string>
#include<vector>
#include<stdio.h>
#include<string.h>
#include<string>
#include<sstream>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

int process_shell(std::string input_command);

#endif
