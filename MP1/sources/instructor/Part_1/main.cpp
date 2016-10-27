/* --------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman, Margaret Baxter                                  */
/* Project: CSCE-313 Machine Problem #1                                        */
/*                                                                             */
/* File: ./MP1/main.cpp                                                        */
/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/* Standard Library Includes                                                   */
/* --------------------------------------------------------------------------- */

#include <iostream>
#include <string.h>
#include <getopt.h>
#include <climits>
#include <iomanip>

/* --------------------------------------------------------------------------- */
/* User Defined Includes                                                       */
/* --------------------------------------------------------------------------- */

#include "linked_list.h"


int main(int argc, char ** argv)
{
	/* Variable declarations */
	linked_list linkedList;
	int option_char;
	int block_size = 128;  // Default value is 128 Bytes
	int mem_size = 512000; // Default value is 512 KBytes

	/* Argparse for command line options lives here */
	while(( option_char = getopt(argc, argv, "b:s:h") ) != EOF)
	{
		switch(option_char)
		{
			case 'b': block_size = atoi(optarg); break;
			case 's': mem_size = atoi(optarg); break;
            case 'h': std::cout << print_usage(); break;
            case ':': std::cout << "Unknown option!\n"; return 1;
		}
	}

	/* Check input arguments for validity */
	if(block_size < 0)
	{
		int new_block_size = 0;
        std::cout << "[ERROR]: Block size can not be negative!" << std::endl
		     << "Please enter a new block size: ";
		while (!(std::cin >> new_block_size) || (new_block_size < 0))
        {
            std::cout << "Bad input - try again: ";
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
        }
        block_size = new_block_size;
	}
	if(mem_size < 0)
	{
		int new_memory_size = 0;
        std::cout << "[ERROR]: Memory size can not be negative" << std::endl
		     << "Please enter a new memory size: ";
		while (!(std::cin >> new_memory_size) || (new_memory_size < 0))
        {
            std::cout << "Bad input - try again: ";
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
        }
        mem_size = new_memory_size;

	}

	/* Initialize the list using the given parameters */
	linkedList.Init(mem_size, block_size);

	/* UI for dynamic use from command prompt */
    std::string usage = "";
	usage = std::string("USAGE:                                                    \n")
	      + std::string("  Insert <int key> <data> <data length in bytes>          \n")
	      + std::string("  - Inserts data with a given key                         \n")
	      + std::string("  Lookup <int key>                                        \n")
	      + std::string("  - Returns the data element stored at key if it exists   \n")
	      + std::string("  Delete <int key>                                        \n")
	      + std::string("  - Delets an element with a specified key                \n")
	      + std::string("  Print                                                   \n")
	      + std::string("  - Prints the entire list                                \n")
	      + std::string("  Destroy                                                 \n")
	      + std::string("  - Deletes all list elements and frees memory            \n")
	      + std::string("  Initialize                                              \n")
	      + std::string("  - Initializes the list and allocates memory             \n")
	      + std::string("  - Note: Can only be done if the list has been destroyed \n")
	      + std::string("                                                          \n")
	      + std::string("Type \"Usage\" to see this message again!                 \n")
	      + std::string("Type \"Exit\" or \"Quit\" to leave the program            \n");
	system("clear");
    std::cout << "Enter commands to modify the linked list" << std::endl
	     << usage                                           << std::endl;
	while(true)
	{
        std::string command = "";
		int key = 0;
		char * data = new char[linkedList.getBlockSize() - sizeof(node)];
        std::cout << ">>> ";
		if (std::cin >> command)
		{
			if (command == "Usage")
			{
                std::cout << usage;
			}
			else if (command == "Print")
			{
				linkedList.PrintList();
			}
			else if (command == "Exit" || command == "Quit")
			{
				linkedList.Destroy();
				return 0;
			}

			/* Cases which will require a second input variable*/
			else if (command == "Lookup")
			{
			    if (std::cin >> key)
			    {
			    	node * tempNode = linkedList.Lookup(key);
			    	if (tempNode != NULL)
			    	{
                        std::cout << "A node with key " << key << " exists in the list" << std::endl;
			    	}
			    	else
			    	{
                        std::cout << "There is no node with the given key in the list" << std::endl;
			    	}
			    }
			    else
			    {
                    std::cout << "Bad input to Lookup!" << std::endl;
			    }
			}
			else if (command == "Delete")
			{
				if (std::cin >> key)
				{
					int return_code = linkedList.Delete(key);
					if (return_code == 0)
					{
                        std::cout << "Delete completed successfully!" << std::endl;
					}
					else
					{
                        std::cout << "Something went wrong in the delete routine!" << std::endl;
					}
				}
				else
				{
                    std::cout << "Bad input to Delete!" << std::endl;
				}
			}
			else if (command == "Insert")
			{
				if (std::cin >> key)
				{
					if (std::cin >> std::setw(block_size - sizeof(node)) >> data)
					{
						int data_length = 0;
						if (std::cin >> data_length)
						{
							if (data_length <= (block_size - sizeof(node)))
							{
								linkedList.Insert(key, data, data_length);
							}
							else
							{
                                std::cout << "Bad data length!" << std::endl;
							}
						}
						else
						{
                            std::cout << "Bad data input size!" << std::endl;
						}
					}
					else
					{
                        std::cout << "Bad input data" << std::endl;
					}
				}
				else
				{
                    std::cout << "Bad input key!" << std::endl;
				}
			}
			else if (command == "Initialize")
			{
				linkedList.Init(mem_size, block_size);
			}
			else if (command == "Destroy")
			{
				linkedList.Destroy();
			}
			else
			{
                std::cout << "Bad Command!  Please try again!" << std::endl;
			}
		}
        std::cin.clear();
        std::cin.ignore(INT_MAX, '\n');
	}
}
