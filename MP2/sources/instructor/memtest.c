/* ------------------------------------------------------------------------ */
/* Developer: Andrew Kirfman, Dr. Betatti                                   */
/* Project: CSCE-313 Machine Problem 2                                      */
/*                                                                          */
/* File: ./memtest.c                                                        */
/* ------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------ */
/* User Defined Includes                                                    */
/* ------------------------------------------------------------------------ */

#include "my_allocator.h"

/* ------------------------------------------------------------------------ */
/* Standard Library Includes                                                */
/* ------------------------------------------------------------------------ */

#include <iostream>

int main(int argc, char ** argv) 
{	
	MyAllocator *my_alloc = new MyAllocator();

	/* ------------------------------------------------------ */
	/* Test Init                                              */
	/* ------------------------------------------------------ */

	// Set the basic block size to 128 and the mem size to 2 ^ 18
	my_alloc->init_allocator(2 << 6, 2 << 17);

	int bad_init_values = 0;
	
	char *init_head_pointer = my_alloc->getHeadPointer();
	unsigned int init_mem_size = my_alloc->getMemSize();
	unsigned int init_block_size = my_alloc->getBasicBlockSize();
	unsigned short int init_num_tiers = my_alloc->getNumTiers();
	bool init_initialized = my_alloc->getInitialized();
	std::map<int, Addr> *init_memory_array = my_alloc->getMemoryArray();

	if(init_head_pointer == NULL)
	{
		bad_init_values += 1;
	}
	else
	{
		if(((uintptr_t) init_head_pointer) != ((uintptr_t) init_memory_array->at(2 << 17)))
		{
			bad_init_values += 1;
		}
	}

	if(init_mem_size != (2 << 17))
	{
		bad_init_values += 1;
	}

	if(init_block_size != (2 << 6))
	{
		bad_init_values += 1;
	}

	unsigned int block_power = init_block_size;
	int exponent = 0;
	while(block_power != 1)
	{
		block_power = block_power >> 1;
		
		exponent += 1;
	}
	block_power = exponent;
	
	unsigned int mem_power = init_mem_size;
	exponent = 0;
	while(mem_power != 1)
	{
		mem_power = mem_power >> 1;
		
		exponent += 1;
	}
	mem_power = exponent;
	
	int test_num_tiers = mem_power - block_power;	

	if(init_num_tiers != test_num_tiers)
	{
		bad_init_values += 1;
	}

	if(init_initialized != true)
	{
		bad_init_values += 1;
	}

	if(init_memory_array == NULL)
	{
		bad_init_values += 1;
	}

	header *first_block = (header*) init_memory_array->at(2 << 16);

	if(first_block->in_use != false)
	{
		bad_init_values += 1;
	}

	if(first_block->in_use != (2 << 16))
	{
		bad_init_values += 1;
	}

	if(first_block->next != NULL)
	{
		bad_init_values += 1;
	}
	
	// 9 total values possible
	if(bad_init_values > 0)
	{
		std::cout << "Init Failed " << bad_init_values << " ";
	}
	else
	{
		std::cout << "Init Succeeded ";
	}

	std::cout << "THINGTHINGTHINGTHINGTHINGTHINGTHINGTHINGTHING!!! ";

	return 0;
}
