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

/*--------------------------------------------------------------------------*/
/* User Defined Includes                                                    */
/*--------------------------------------------------------------------------*/

#include "my_allocator.h"

/*--------------------------------------------------------------------------*/
/* Functions For Module MyAllocator                                         */
/*--------------------------------------------------------------------------*/

unsigned int MyAllocator::higher_two(unsigned int number)
{
	unsigned int exponent = 0;
	while(true)
	{
		int power_two = 2 << exponent;
		if(power_two > number)
		{
			break;
		}
		
		exponent += 1;
	}
	
	return 2 << exponent;
}

unsigned int MyAllocator::lower_two(unsigned int number)
{
	unsigned int exponent = 0;
	while(true)
	{
		int power_two = 2 << exponent;
		int next_two = 2 << (exponent + 1);
		if(power_two < number && next_two > number)
		{
			break;
		}
		
		exponent += 1;
	}
	
	return 2 << exponent;
}

bool MyAllocator::isPowerOfTwo(unsigned int x)
{
	/* While x is even and > 1 */
	while (((x & 1) == 0) && x > 1)
	{
		x >>= 1;
	}

	return (x == 1);
}

MyAllocator::MyAllocator()
{
	memory_array = NULL;
	basic_block_size = 0;
	mem_size = 0;
	initialized = false;
}

// If successful, returns the amount of memory allocated.  If failure
// occurs, returns 0.  
unsigned int MyAllocator::init_allocator(unsigned int _basic_block_size,
	unsigned int _mem_size)
{
	// Don't run the initialization routine again if the mem array has already been set up
	if(initialized == true)
	{
		return 0;
	}
	
	
	// If the mem_size is greater than the block_size, return an error
	// Obviously, you can't fit a block that's larger than available memory
	if(_basic_block_size > _mem_size)
	{
		return 0;
	}
	
	// If the block size provided is smaller than the size of the header 
	// struct, then return failure (0).  If there isn't enough room for
	// header, then there definitely isn't enough room to contain any data.  
	if(_basic_block_size < sizeof(header))
	{
		return 0;
	}
	
	// Round block_size up to a power of two
	if(!isPowerOfTwo(_basic_block_size))
	{
		_basic_block_size = higher_two(_basic_block_size);
	}
	
	// Round mem_size up to a power of two
	if(!isPowerOfTwo(_mem_size))
	{
		_mem_size = higher_two(_mem_size);
	}
	
	// Set block and mem sizes
	basic_block_size = _basic_block_size;
	mem_size = _mem_size;
	
	// Setup memory array
	unsigned int block_power = _basic_block_size;
	int exponent = 0;
	while(block_power != 1)
	{
		block_power = block_power >> 1;
		
		exponent += 1;
	}
	block_power = exponent;
	
	unsigned int mem_power = _mem_size;
	exponent = 0;
	while(mem_power != 1)
	{
		mem_power = mem_power >> 1;
		
		exponent += 1;
	}
	mem_power = exponent;
	
	num_tiers = mem_power - block_power;
	
	// Allocate the header pointer
	void *memory = malloc(mem_size);
	
	// Set up the first header structure
	header* primary_header = reinterpret_cast<header*>(memory);
	
	std::cout << "Header: " << primary_header << std::endl;
	
	primary_header->next = NULL;
	primary_header->block_size = mem_size;
	primary_header->in_use = false;
	
	// Correctly set up the memory_array
	memory_array = new std::map<int, Addr>;
	for(int i=0; i<=num_tiers; i++)
	{
		int setup_power = 2 << (6 + i);
		memory_array->insert(std::pair<int, Addr>(setup_power, (Addr) NULL));
	}
	
	// The first block in the system will be the one just malloc'ed 
	memory_array->at(mem_size) = memory;
	
	// The list is now properly initialized
	initialized = true;
}

void MyAllocator::release_allocator()
{
	// Coalesce allocated memory regions back into one that can be freed.  
	while(true)
	{
		
		for(auto it = memory_array->begin(); it != memory_array->end(); ++it)
		{
			
		
			
		}
		
		
		
	}
	
	
	
	// Also set initialized = false to indicate that memory was freed.  
	initialized = false;
}


Addr MyAllocator::my_malloc(unsigned int _length)
{
	// The required length is _length + the space taken up by the header structure
	int needed_length = _length + sizeof(header);
	
	// But since we can only allocate in powers of two, round up
	int next_power_of_two = higher_two(needed_length);
	
	// Go through the list and find a block for the memory region
	for(int i=next_power_of_two; i<=mem_size; i = i * 2)
	{
		std::cout << "Hello World!" << std::endl;
		Addr array_pointer = memory_array->at(i);
		
		if(array_pointer != NULL)
		{
			std::cout << "Flag1" << std::endl;
			
			header *memory_block = reinterpret_cast<header*>(array_pointer);
			
			
			// Test to see if we can use this block.  If not, move onto the next tier
			if(memory_block->in_use == true)
			{
				if(memory_block->next != NULL)
				{
					while(true)
					{
						memory_block = memory_block->next;
						
						// Use this block
						if(memory_block->in_use == false)
						{
							break;
						}
						
						// Reached the end without finding a block
						if(memory_block->next == NULL)
						{
							memory_block = NULL;
							break;
						}
					}
					
					if(memory_block == NULL)
					{
						continue;
					}

					if(memory_block->in_use == true)
					{
						continue;
					}
					
				}
				else
				{
					continue;
				}
			}
			
			std::cout << "Flag2" << std::endl;
			std::cout << "Block: " << memory_block << std::endl;
			
			// Once you get down here, memory_block should be good.  
			// If we don't find one on this level, move onto the next one			
			if(memory_block == NULL)
			{
				// If we're at the end of the list and there isn't an 
				// available block, we're out of memory.  Return 0.  
				if(i == mem_size)
				{
					return (Addr) 0;
				}
				else
				{
					continue;
				}
			}
			
			std::cout << "Hello World!" << std::endl;
			
			split_block(reinterpret_cast<void*>(memory_block));



			std::cout << "THING! " << i << std::endl;
			// Work with memory block, split as necessary
			

		}
		else
		{
			continue;
		}
	}
}

void MyAllocator::split_block(Addr start_address)
{
	// Don't do anything if they pass a null pointer
	if(start_address == NULL)
	{
		return;
	}
	
	header *memory_block = reinterpret_cast<header*>(start_address);
	
	// Don't split a block that's in use
	if(memory_block->in_use == true)
	{
		return;
	}
	
	// Don't do anything if the block size is already the basic block size
	if(memory_block->block_size == basic_block_size)
	{
		return;
	}
	
	std::cout << "Flag2" << std::endl;
	
	int current_size = memory_block->block_size;
	std::cout << "Curr Size: " << current_size << std::endl;
	std::cout << "THINGTHINGTHING: " << memory_array->at(current_size) << std::endl;
	
	
	// Remove the block from its current list.  
	header *start_of_tier = reinterpret_cast<header*>(memory_array->at(current_size));
	
	std::cout << "THTHTHTTH  " << start_of_tier << std::endl;
	
	// Two cases:
	// 1) The list has only one element
	// 2) The list has multiple elements
	//   - beginning of list
	//   - end of list   
	
	// Once we leave this list, start_of_tier should be
	// the block one before memory_block or memory_block itself
	while(true)
	{
		
		if(start_of_tier->next == NULL)
		{
			break;
		}
		
		if(start_of_tier->next == memory_block)
		{
			break;
		}
		
		start_of_tier = start_of_tier->next;
	}
	
	if(start_of_tier != memory_block)
	{
		// Remove memory_block from the tier
		start_of_tier->next = memory_block->next;
	}
	
	if(start_of_tier == memory_block)
	{
		// Check to make sure that memory_block is the first element in the list
		memory_array->at(current_size) = NULL;
	}
	
	// Split the block into two smaller blocks
	// Weridness with pointer arithmetic using void*.  
	// Temporarily switch to char* to perform the math. 
	
	
	// WHY CAN't I CONVERT THE ADDRESES!!!!
	void* temp_address = reinterpret_cast<void*>(memory_block);
	char* address = reinterpret_cast<char*>(temp_address);
	
	std::cout << "Address: " << address << std::endl;
	
	 
	header *block_address = memory_block;
	header *second_block_address = block_address + (current_size / 2);
	
	std::cout << "FLAGFLAG" << std::endl;
	std::cout << "THTH: " << block_address << std::endl;
	std::cout << "THTTH: " << second_block_address << std::endl;
	
}

void MyAllocator::combine_blocks(Addr start_address1, Addr start_address2)
{
	header *memory_block_1 = reinterpret_cast<header*>(start_address1);
	header *memory_block_2 = reinterpret_cast<header*>(start_address2);
	
	if(memory_block_1 == NULL || memory_block_2 == NULL)
	{
		return;
	}
	
	if(memory_block_1->in_use == true || memory_block_2->in_use == true)
	{
		return;
	}
	
	
}

int MyAllocator::my_free(Addr _a)
{
	
}

















