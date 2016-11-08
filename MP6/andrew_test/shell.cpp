/* ------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman                                                 */ 
/* Project: CSCE-313 Episode #5                                              */
/*                                                                           */
/* File: ./shell.cpp                                                         */ 
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
/* User Defined Includes                                                     */
/* ------------------------------------------------------------------------- */

#include "shell.h"


int process_shell(std::string input_command)
{
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

    /* Detect Redirection */
    /* 
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
                tokens[tokens.size() - 1].pop_back();
                tokens[tokens.size() - 1].pop_back();
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
                tokens[tokens.size() - 1].pop_back();
                tokens[tokens.size() - 1].pop_back();
            }
        }
    } */

	/* Convert command into a char** argument array */
    char **arg_array = new char*[tokens.size() + 1];
    for(unsigned short int i=0; i<tokens.size(); i++)
    {
        arg_array[i] = const_cast<char*>(tokens[i].c_str());  
    }
    arg_array[tokens.size()] = NULL;

    /* Execute command */
	pid_t pid = fork();

	if(pid == 0)
	{
		execvp(arg_array[0], arg_array);

        // Something bad happened
        return -1;
	}
	else
	{
        


        kill(pid, SIGSTOP);
        return pid;
    }

    return 0;
}
