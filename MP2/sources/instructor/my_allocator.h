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
}

class MyAllocator
{
private:
	vector<Addr> *memory_array;
	
	// Note: Basic block size must be a power of two!  If someone passes
	// an argument that isn't a power of two, the program should pick the 
	// next nearest power of two as the basic_block_size.  
	unsigned int basic_block_size;
	unsigned int mem_sze;
	
	// Returns nearest higher power of two
	unsigned int higher_two(unsigned int number);
	
	// Returns nearest lower power of two
	unsigned int lower_two(unsigned int number);
	
	// Checks to see if a number is a power of two
	bool isPowerOfTwo(unsigned int x)


public:
	MyAllocator();
	
	/* This function initializes the memory allocator and makes a portion of 
     * ’_mem_size’ bytes available. The allocator uses a ’_basic_block_size’ as 
     * its minimal unit of allocation. The function returns the amount of 
     * memory made available to the allocator. If an error occurred, 
     * it returns 0. 
     */ 
	unsigned int init_allocator(unsitned int _basic_block_size, 
		unsigned int _mem_size); 
	
	/* This function returns any allocated memory to the operating system. 
     * After this function is called, any allocation fails.
     */ 	
	int release_allocator();
	
	/* Allocate _length number of bytes of free memory and returns the 
     * address of the allocated portion. Returns 0 when out of memory. 
     */ 
	Addr my_malloc(unsigned int _length);
	
	/* Frees the section of physical memory previously allocated 
     * using ’my_malloc’. Returns 0 if everything ok. 
     */ 
	int my_free(Addr _a);
};

#endif 
