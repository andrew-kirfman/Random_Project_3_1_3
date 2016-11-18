/* --------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman, Margaret Baxter                                  */
/* Project: CSCE-313 Machine Problem #1                                        */
/*                                                                             */
/* File: ./MP1/linked_list2.cpp                                                */
/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/* Standard Library Includes                                                   */
/* --------------------------------------------------------------------------- */

#include <iostream>
#include <string.h>
#include <climits>

/* --------------------------------------------------------------------------- */
/* User Defined Includes                                                       */
/* --------------------------------------------------------------------------- */

#include "linked_list2.h"

linked_list2::linked_list2()
{
	
}

void linked_list2::Init(int M, int b, int t)
{

}


void linked_list2::Destroy()
{

}

void linked_list2::Insert(int k,char * data_ptr, int data_len)
{

}

int linked_list2::Delete(int delete_key)
{

}

node* linked_list2::Lookup(int lookup_key)
{

}

void linked_list2::PrintList()
{
	
	/* IMPORTANT NOTE!
	 * 
	 * In order for the script that will grade your assignment to work 
	 * (i.e. so you get a grade higher than a 0),
	 * you need to print out each member of the list using the format below.  
	 * Your print list function should be written as a while loop that prints 
	 * the tier of the list and then each node underneath it.  
	 * the following four lines exactly for each node and nothing else.  If
	 * you have any difficulties, talk to your TA and he will explain it further. 
	 * 
	 * The output lines that you should use are provided so that you will know
	 * exactly what you should output.  
	 */ 
	// Do this for every tier
	// std::cout << "Tier " << <TIER NUMBER GOESHERE> << std::endl;
	
	// Do this for every node 
	//std::cout << "Node: " << std::endl;
	//std::cout << " - Key: " << <KEY GOES HERE!> << std::endl;
	//std::cout << " - Data: " << <KEY GOES HERE!> << std::endl;
	
	/* Short example:
	 *   - Assume a list with two tiers each with two elements in each tier.  
	 *     the print out would appear as follows
	 * 
	 * Tier 0
	 * Node: 
	 *  - Key: 1
	 *  - Data: Hello
	 * Node:
	 *  - Key: 2
	 *  - Data: World!
	 * Tier 1
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

int linked_list2::Find_tier(int key)
{

}

/* Getter Functions */
char** linked_list2::getHeadPointer()
{

}

node** linked_list2::getFrontPointer()
{

}

node** linked_list2::getFreePointer()
{

}

node** linked_list2::getFreeDataPointer()
{

}

int linked_list2::getBlockSize()
{

}

int linked_list2::getMemSize()
{

}

int linked_list2::getMaxDataSize()
{

}

int linked_list2::getNumTiers()
{

}

int linked_list2::getInitialized()
{

}

/* Setter Functions */
void linked_list2::setHeadPointer(char** new_head_pointer)
{

}

void linked_list2::setFrontPointer(node** new_front_pointer)
{

}

void linked_list2::setFreePointer(node** new_free_pointer)
{

}

void linked_list2::setFreeDataPointer(node** new_free_data_pointer)
{

}

void linked_list2::setBlockSize(int new_block_size)
{

}

void linked_list2::setMemSize(int new_mem_size)
{

}

void linked_list2::setMaxDataSize(int new_max_data_size)
{

}

void linked_list2::setNumTiers(int new_num_tiers)
{

}

void linked_list2::setInitialized(bool new_initialized)
{

}
