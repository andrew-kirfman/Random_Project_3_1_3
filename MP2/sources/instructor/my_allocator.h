/* ------------------------------------------------------------------------ */
/* Developer: Andrew Kirfman, Dr. Betatti                                   */
/* Project: CSCE-313 Machine Problem 2                                      */
/*                                                                          */
/* File: ./my_allocator.h                                                   */
/* ------------------------------------------------------------------------ */

#ifndef _my_allocator_h_
#define _my_allocator_h_

/*--------------------------------------------------------------------------*/
/* Standard Library Includes                                                */ 
/*--------------------------------------------------------------------------*/

#include<vector>
#include<iostream>
#include<map>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

/*--------------------------------------------------------------------------*/
/* Standard Library Includes                                                */ 
/*--------------------------------------------------------------------------*/

#include "utility.h"

/*--------------------------------------------------------------------------*/
/* Data Structures                                                          */ 
/*--------------------------------------------------------------------------*/

typedef void * Addr; 

/*--------------------------------------------------------------------------*/
/* Module MyAllocator                                                       */
/*--------------------------------------------------------------------------*/

struct header
{
	header* next;
	int block_size;
	bool in_use;
};

class MyAllocator
{
private:
	//std::map<int, Addr> *memory_array;
	
	// Maintain a poitner to the head of the list
	char *head_pointer;
	
	// Note: Basic block size must be a power of two!  If someone passes
	// an argument that isn't a power of two, the program should pick the 
	// next nearest power of two as the basic_block_size.  
	unsigned int basic_block_size;
	unsigned int mem_size;
	
	// Number of size tiers in memory array
	unsigned short int num_tiers;
	
	// Returns nearest higher power of two
	unsigned int higher_two(unsigned int number);
	
	// Returns nearest lower power of two
	unsigned int lower_two(unsigned int number);
	
	// Has the list been set up
	bool initialized;
	
	// Checks to see if a number is a power of two
	bool isPowerOfTwo(unsigned int x);

	// Functions to split/combine blocks of memory
	//bool split_block(Addr start_address);
	//bool combine_blocks(Addr start_address1, Addr start_address2);
	//bool are_buddies(Addr start_address1, Addr start_address2);

public:

	bool are_buddies(Addr start_address1, Addr start_address2);
	bool combine_blocks(Addr start_address1, Addr start_address2);
	bool split_block(Addr start_address);

	/* Function to search for a block's brother.  Returns
	 * (Addr) NULL if the brother is in use or is not found.  
	 */
	Addr find_unused_buddy(Addr sibling_block);

	// MOVE THIS LATER!!!
	std::map<int, Addr> *memory_array;

	MyAllocator();
	
	/* This function initializes the memory allocator and makes a portion of 
     * ’_mem_size’ bytes available. The allocator uses a ’_basic_block_size’ as 
     * its minimal unit of allocation. The function returns the amount of 
     * memory made available to the allocator. If an error occurred, 
     * it returns 0. 
     */ 
	unsigned int init_allocator(unsigned int _basic_block_size, 
		unsigned int _mem_size); 
	
	/* This function returns any allocated memory to the operating system. 
     * After this function is called, any allocation fails.
     */ 	
	void release_allocator();
	
	/* Allocate _length number of bytes of free memory and returns the 
     * address of the allocated portion. Returns 0 when out of memory. 
     */ 
	Addr my_malloc(unsigned int _length);
	
	/* Frees the section of physical memory previously allocated 
     * using ’my_malloc’. Returns true if everything ok, false otherwise. 
     */ 
	bool my_free(Addr _a);
	
	/* Diagnostic function to print out your entire memory array.  
	 * Use this to view the state of your program for debugging
	 */
	 void print_array();
	// Note from one student to another.  You will not be graded on the
	// print_array function other than that it needs to output something
	// reasonably coherent.  I would strongly recommend putting some time into
	// your version of this function as it will greatly help you debug your
	// program during development.  The designer of this assignment (i.e. me)
	// greatly benefited from being able to see the state of his code in action.  
};

#endif 
