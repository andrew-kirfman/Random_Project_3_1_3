/* --------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman, Margaret Baxter                                  */
/* Project: CSCE-313 Machine Problem #1                                        */
/*                                                                             */
/* File: ./MP1/linked_list.cpp                                                 */
/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/* User Defined Includes                                                       */
/* --------------------------------------------------------------------------- */

#include "linked_list.h"


/* Constructor */
linked_list::linked_list()
{

}


void linked_list::Init(int M, int b)
{

}

void linked_list::Destroy()
{

} 

/* Insert an element into the list with a given key, given data element, and with a given length*/
void linked_list::Insert (int k, char * data_ptr, int data_len)
{

}


int linked_list::Delete(int delete_key)
{

}

/* Iterate through the list, if a given key exists, return the pointer to it's node */
/* otherwise, return NULL                                                           */
struct node* linked_list::Lookup(int lookup_key)
{

}

/* Prints the list by printing the key and the data of each node */
void linked_list::PrintList()
{
	
	/* IMPORTANT NOTE!
	 * 
	 * In order for the script that will grade your assignment to work 
	 * (i.e. so you get a grade higher than a 0),
	 * you need to print out each member of the list using the format below.  
	 * Your print list function should be written as a while loop that prints
	 * the following three lines exactly for each node and nothing else.  If
	 * you have any difficulties, talk to your TA and he will explain it further.  
	 * 
	 * The output lines that you should use are provided so that you will know
	 * exactly what you should output.  
	 */ 
	//std::cout << "Node: " << std::endl;
	//std::cout << " - Key: " << <KEY GOES HERE!> << std::endl;
	//std::cout << " - Data: " << <KEY GOES HERE!> << std::endl;
	
	/* Short example:
	 *   - Assume that you have a list with 4 elements.  
	 *     Your output should appear as follows
	 * 
	 * Node:
	 *  - Key: 1
	 *  - Data: Hello
	 * Node:
	 *  - Key: 2
	 *  - Data: World!
	 * Node:
	 *  - Key: 3
	 *  - Data: Hello
	 * Node:
	 *  - Key: 4
	 *  - Data: World!
	 * 
	 * ^^ Your output needs to exactly match this model to be counted as correct.  
	 * (With the exception that the values for key and data will be different 
	 * depending on what insertions you perform into your list.  The values provided
	 * here are for pedagogical purposes only)
	 */
}

/* Getter Functions */
char* linked_list::getHeadPointer()
{

}

node* linked_list::getFrontPointer()
{

}

node* linked_list::getFreePointer()
{

}

node* linked_list::getFreeDataPointer()
{

}

int linked_list::getBlockSize()
{

}

int linked_list::getMemSize()
{

}

int linked_list::getMaxDataSize()
{

}

bool linked_list::getInitialized()
{

}

/* Setter Functions */
void linked_list::setHeadPointer(char *new_pointer)
{

}

void linked_list::setFrontPointer(node* new_pointer)
{

}

void linked_list::setFreePointer(node* new_pointer)
{

}

void linked_list::setFreeDataPointer(node* new_pointer)
{

}

void linked_list::setBlockSize(int new_block_size)
{

}

void linked_list::setMemSize(int new_mem_size)
{

}

void linked_list::setMaxDataSize(int new_max_data_size)
{

}

void linked_list::setInitialized(bool new_initialized)
{

}
