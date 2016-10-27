/* ------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman                                                 */ 
/* Project: CSCE-313 Episode #5                                              */
/*                                                                           */
/* File: ./shell.cpp                                                         */ 
/* ------------------------------------------------------------------------- */


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

/* ------------------------------------------------------------------------- */
/* Global Variables                                                          */
/* ------------------------------------------------------------------------- */

std::string PS1 = "";
std::string current_prompt = "";


/* User Definable Custom Prompt:
 * 
 * Allows users to define custom prompts using a simple command to change the string.  
 * The following set of macros will be replaced as follows:
 *
 *   - DIR: Current directory 
 *   - WHO: User's name
 *   - DATE: Current date
 *   - TIME: Current time
 */
void update_prompt()
{
    int location = -1;
    current_prompt = PS1;
    bool failure = false;
    std::stringstream ss;

    // Loop until no more substitutions can be made
    while(!failure)
    {
        failure = true;

        // Current Directory
        location = current_prompt.find("DIR");
        if(location != -1)
        {
            failure = false;
            char* current_directory = getcwd(NULL, 1024);
            current_prompt = current_prompt.replace(location, 3, current_directory);
        }

        // User's Name
        location = current_prompt.find("WHO");
        if(location != -1)
        {
            failure = false;
            char *user_name = getlogin();
            current_prompt = current_prompt.replace(location, 3, user_name);
        }

        // Current Date
        location = current_prompt.find("DATE");
        if(location != -1)
        {
            failure = false;
            time_t t = time(0);
            struct tm * now = localtime(&t);
            // Times given in Day:Month:Year format
            ss << now->tm_mday << ":" << now->tm_mon << ":" << now->tm_year;
            current_prompt = current_prompt.replace(location, 4, ss.str());
        }

        // Current Time
        location = current_prompt.find("TIME");
        if(location != -1)
        {
            failure = false;
            time_t t = time(0);
            struct tm * now = localtime(&t);
            
            // Times given in Hour:Minute:Second format
            ss << now->tm_hour << ":" << now->tm_min << ":" << now->tm_sec;
            current_prompt = current_prompt.replace(location, 4, ss.str());
        }
    }
}


int main(int argc, char** argv)
{
    /* Parse command line arguments */
    int option_char;
    bool shell_no_prompt = false;
    while((option_char = getopt(argc, argv, "t")) != EOF)
    {
        switch(option_char)
        {
            case 't': 
                shell_no_prompt = true;
                break;
            
            case '?':
                break;
        }
    }

    /* Initialize prompt to default (current directory followed by a colon */
    PS1 = "DIR: ";

    /* Vector to maintain background processes */
    std::vector<pid_t> background_processes;

    std::string input_command = "";
    while(true)
    {
        /* Wait (with no hang) for all background processes */
        std::vector<pid_t> still_running;
        for(unsigned short int i=0; i<background_processes.size(); i++)
        {
            int proc_status = waitpid(background_processes[i], NULL, WNOHANG);
            if(proc_status != -1)
            {
                still_running.push_back(background_processes[i]);
            }
        }
        background_processes = still_running;

        /* Take in input command */
        if(shell_no_prompt == false)
        {
            update_prompt();
            std::cout << current_prompt;
        }
        std::cin.clear();
        std::getline(std::cin, input_command, '\n');


        // If user just presses enter, continue
        if(input_command == "")
        {
            if(shell_no_prompt == true)
            {
                return 0;
            }
            else
            {
                continue;
            }
        }

        // Strip leading and trailing spaces
        input_command = std::regex_replace(input_command, std::regex("^ +| +$|( ) +"), "");

        /* Tokenize input command */
        std::vector<std::string> tokens;
        std::string curr_token = "";
        char prev_char = '\0';
        bool inside_quotes = false;
        for(unsigned short int i=0; i<input_command.length(); i++)
        {
            // See if we are inside of quotes.  All things inside of quotes 
            // need to be a part of the same token.  
            if(input_command[i] == '"')
            {
                if(inside_quotes == false && prev_char != '\\')
                {
                    inside_quotes = true;
                }
                else if(inside_quotes == true && prev_char != '\\')
                {
                    inside_quotes = false; 
                }
            }

            // Separate out everything into individual words
            if(input_command[i] != ' ' || inside_quotes == true)
            {
                curr_token = curr_token + input_command[i];
            }
            else
            {
                tokens.push_back(curr_token);
                curr_token = "";
            }

            // Also separate out a word if you are at the end of the token stream
            if(i == input_command.length() - 1)
            {
                tokens.push_back(curr_token);
            }

            // Assign previous character at end of loop
            prev_char = input_command[i];
        }

        // The tokenizer may make empty tokens, eliminate them
        std::vector<int> delete_list;
        for(unsigned short int i=0; i<tokens.size(); i++)
        {
            if(tokens[i] == " " || tokens[i] == "\0")
            {
                delete_list.push_back(i);
            }
        }

        for(int i=delete_list.size()-1; i>=0; i++)
        {
            tokens.erase(tokens.begin() + i);
        }

        // Account for special commands
        if(tokens[0] == "exit")
        {
            return 0;
        }
        else if(tokens[0] == "cd")
        {
            if(tokens.size() == 1 || tokens.size() > 2)
            {
                std::cout << "[ERROR]: Improper number of arguments to cd!" << std::endl;
                continue;
            }
            else
            {
                int status = chdir(tokens[1].c_str());
                if(status == -1)
                {
                    std::cout << "[ERROR]: Malformed command!" << std::endl;
                }
            }

            continue;
        }
        else if(tokens[0] == "PS1")
        {
            if(tokens.size() != 3 || tokens[1] != "=")
            {
                std::cout << "[ERROR]: Malformed command!" << std::endl;
            }
            else
            {
                if(tokens[2][0] == '"' && tokens[2][tokens[2].length() - 1] == '"')
                {
                    tokens[2] = tokens[2].replace(0, 1, "");
                    std::string new_prompt = tokens[2].replace(tokens[2].size() - 1, 1, "");
                    PS1 = new_prompt;
                }
                else
                {
                    std::cout << "[ERROR]: Malformed command!" << std::endl;
                }
            }
            continue;
        }

        // Check to see if the process is to run in the background or foreground
        bool background = false;
        if(tokens[tokens.size() - 1] == "&")
        {
            background = true;
            tokens.pop_back();
        }

        /* Remove quotes from beginning and end of tokens */
        for(unsigned short int i=0; i<tokens.size(); i++)
        {
            if(tokens[i][0] == '"')
            {
                tokens[i] = tokens[i].replace(0, 1, "");
            }
            if(tokens[i][tokens[i].length() - 1] == '"')
            {
                tokens[i] = tokens[i].replace(tokens[i].length() - 1, 1, "");
            }
        }

        /* Replace the string \" with the character '\"' */
        for(unsigned short int i=0; i<tokens.size(); i++)
        {
            int quote_position = -1;
            while(true)
            {
                quote_position = tokens[i].find("\\\"");
                if(quote_position == -1)
                {
                    break;
                }
                else
                {
                    tokens[i] = tokens[i].replace(quote_position, 2, "\"");
                }
            }
        }

        /* Detect command pipes */
        std::vector<std::vector<std::string> > commands;
        short int last_pipe = -1;
        for(unsigned short int i=0; i<tokens.size(); i++)
        {
            // If you find a pipe, capture all previous tokens since the last
            // pipe and make them a single command
            if(tokens[i] == "|")
            {
                commands.push_back(std::vector<std::string>());

                for(short int j=last_pipe+1; j<i; j++)
                {
                    commands[commands.size() - 1].push_back(tokens[j]);
                }
                last_pipe = i;
            }

            // Also capture the rest of the command
            if(i == tokens.size()-1)
            {
                commands.push_back(std::vector<std::string>());

                for(short int j=last_pipe+1; j<=i; j++)
                {
                    commands[commands.size() - 1].push_back(tokens[j]);
                }
            }
        }

        /* Set up communication pipes */
        std::vector<int> pipes;
		for(unsigned short int i=0; i<commands.size() + 1; i++)
		{
			int new_pipe[2];
			pipe(new_pipe);
			pipes.push_back(new_pipe[0]);
			pipes.push_back(new_pipe[1]);
		}

        /* Detect Redirection */
        bool redirect_out = false;
        for(unsigned short int i=0; i<tokens.size(); i++)
        {
            // Output Redirection
            if(tokens[i] == ">")
            {
                if(i == tokens.size() - 2)
                {
                    // Open requested file and redirect final writes
                    int write_fd = open(tokens[tokens.size() - 1].c_str(), O_WRONLY | O_CREAT,
                        S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
                    pipes[pipes.size() - 1] = write_fd;
                    redirect_out = true;
                    
                    // Remove extraneous tokens
                    commands[commands.size() - 1].pop_back();
                    commands[commands.size() - 1].pop_back();
                }
            }
            
            // Input Redirection
            if(tokens[i] == "<")
            {
                if(i == tokens.size() - 2)
                {
                    // Open requested file and redirect input
                    int read_fd = open(tokens[tokens.size() - 1].c_str(), O_RDONLY);
                    
                    if(read_fd == -1)
                    {
                        std::cout << "[ERROR]: Could not open file for input!" << std::endl;
                        continue;
                    }
                    pipes[0] = read_fd;

                    // Remove extraneous tokens
                    commands[commands.size() - 1].pop_back();
                    commands[commands.size() - 1].pop_back();
                }
            }

        }

		/* Convert commands into a char** argument vector */
        std::vector<char**> arg_array;
		for(unsigned short int i=0; i<commands.size(); i++)
		{
			arg_array.push_back(new char*[commands[i].size() + 1]);
			for(unsigned short int j=0; j<commands[i].size(); j++)
			{
				arg_array[i][j] = const_cast<char*>(commands[i][j].c_str());
			}
			arg_array[i][commands[i].size()] = NULL;
		}

        /* Execute commands in order */
		for(unsigned short int i=0; i<arg_array.size(); i++)
		{
			pid_t pid = fork();

			if(pid == 0)
			{
				// Read from one pipe
				close(STDIN_FILENO);
				dup(pipes[2*i]);

				// Write to another
                if(arg_array.size() - 1 != i || redirect_out)
                {
				    close(STDOUT_FILENO);
				    dup(pipes[2*i + 3]);
		        }

				// Close extra parts of the pipe
				close(pipes[2*i + 1]);
				close(pipes[2*i]);

				execvp(arg_array[i][0], arg_array[i]);
                std::cout << argv[0] << ": " << arg_array[i][0] << ": command not found" << std::endl;
                exit(0);
			}
			else
			{
				close(pipes[2*i + 1]);
                if(!background || i != arg_array.size() - 1)
                {
				    waitpid(pid, NULL, 0);
                }
                else
                {
                    background_processes.push_back(pid);   
                }
			}
		}
        fflush(NULL);
    }
    return 0;
}
