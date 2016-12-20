/* ------------------------------------------------------------------------ */
/* Developer: Andrew Kirfman, Dr. Betatti                                   */
/* Project: CSCE-313 Machine Problem 2                                      */
/*                                                                          */
/* File: ./my_allocator.c                                                   */
/* ------------------------------------------------------------------------ */

/*--------------------------------------------------------------------------*/
/* Standard Library Includes                                                */
/*--------------------------------------------------------------------------*/

#include <stdlib.h>
#include <cstdint>

/*--------------------------------------------------------------------------*/
/* User Defined Includes                                                    */
/*--------------------------------------------------------------------------*/

#include "my_allocator.h"

/*--------------------------------------------------------------------------*/
/* Functions For Module MyAllocator                                         */
/*--------------------------------------------------------------------------*/

unsigned int MyAllocator::higher_two(unsigned int number)
{
	
}

unsigned int MyAllocator::lower_two(unsigned int number)
{
	
}

bool MyAllocator::isPowerOfTwo(unsigned int x)
{
	
}

MyAllocator::MyAllocator()
{
	
}

unsigned int MyAllocator::init_allocator(unsigned int _basic_block_size,
	unsigned int _mem_size)
{

}

void MyAllocator::release_allocator()
{

}


Addr MyAllocator::my_malloc(unsigned int _length)
{	
	
}

bool MyAllocator::split_block(Addr start_address)
{
	
}

bool MyAllocator::are_buddies(Addr start_address1, Addr start_address2)
{
	
}

bool MyAllocator::combine_blocks(Addr start_address1, Addr start_address2)
{
	
}

Addr MyAllocator::find_unused_buddy(Addr sibling_block)
{
	
}


bool MyAllocator::my_free(Addr _a)
{
	
}

void MyAllocator::print_array()
{
	
}

/* Getter Functions */
char *MyAllocator::getHeadPointer()
{
	
}

unsigned int MyAllocator::getBasicBlockSize()
{
	
}

unsigned int getMemSize()
{
	
}

unsigned short int getNumTiers()
{
	
}

bool getInitialized()
{
	
}
	
std::map<int, Addr> *getMemoryArray()
{
	
}
