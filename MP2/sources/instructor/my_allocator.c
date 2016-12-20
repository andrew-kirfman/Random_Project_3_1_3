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
	if(isPowerOfTwo(number) == true)
	{
		return number;
	}
	
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
	
	head_pointer = (char*) memory;
	
	// Set up the first header structure
	header* primary_header = reinterpret_cast<header*>(memory);
	
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
	// If array isn't initialized, don't do anything
	if(initialized == false)
	{
		return;
	}
	
	// Mark all regions as unused
	for(int i=basic_block_size; i<mem_size; i = i * 2)
	{
		header *iterator = (header*) memory_array->at(i);
		
		while(true)
		{
			if(iterator == (header*) NULL)
			{
				break;
			}
			
			iterator->in_use = false;
			
			iterator = iterator->next;
		}
	}

	// Coalesce allocated memory regions back into one that can be freed.  
	for(int i=basic_block_size; i<mem_size; i = i * 2)
	{
		header *iterator = (header*) memory_array->at(i);
		
		while(true)
		{
			if(iterator == (header*) NULL)
			{
				break;
			}
			
			header* buddy = (header*) find_unused_buddy((Addr) iterator);
			
			if(buddy != (header*) NULL)
			{
				// Combine the two blocks together
				combine_blocks((Addr) iterator, (Addr) buddy);
				
				// Reset the loop so that it goes through the ith tier again
				i = i >> 1;
				break;
			}

			iterator = iterator->next;
		}
	}
	
	// Actually free the head pointer
	free(head_pointer);
	
	// Also set initialized = false to indicate that memory was freed.  
	initialized = false;
}


Addr MyAllocator::my_malloc(unsigned int _length)
{	
	// If the array isn't initialized, don't do anything
	if(initialized == false)
	{
		return (Addr) 0;
	}
	
	// The required length is _length + the space taken up by the header structure
	int needed_length = _length + sizeof(header);
	
	// But since we can only allocate in powers of two, round up
	int next_power_of_two = higher_two(needed_length);

	// Function shouldn't try to allocate regions larger than
	// total available memory.  
	if(next_power_of_two > mem_size)
	{
		return (Addr) 0;
	}
	
	// Go through the list and find a block for the memory region
	for(int i=next_power_of_two; i<=mem_size; i = i * 2)
	{
		Addr array_pointer = memory_array->at(i);
		
		if(array_pointer != NULL)
		{	
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
			
			int save_1 = memory_block->block_size;
			int save_2 = next_power_of_two;
			
			int power_1 = 0;
			int power_2 = 0;
			
			while(true)
			{
				save_1 = save_1 >> 1;
				power_1 += 1;
				
				if(save_1 == 1)
				{
					break;
				}
			}
			
			while(true)
			{
				save_2 = save_2 >> 1;
				power_2 += 1;
				
				if(save_2 == 1)
				{
					break;
				}
			}
			
			int number_of_splits = power_1 - power_2;
			
			Addr block_to_split = (Addr) memory_block;
			int size = memory_block->block_size / 2;
			
			for(int j=0; j<number_of_splits; j++)
			{	
				split_block(block_to_split);
				
				// Should be the first element in the array.  Just check
				// to be absolutely sure.  
				header* test_block = (header*) memory_array->at(size);
				
				while(true)
				{
					if(test_block == NULL)
					{
						return (Addr) 0;
					}
					
					if(test_block->in_use == false)
					{
						break;
					} 
					
					test_block = test_block->next;
				}
				
				block_to_split = (Addr) test_block;
				size = test_block->block_size / 2;
			}

			((header*) block_to_split)->in_use = true;
			uintptr_t final_block = (uintptr_t) block_to_split + 16;
			
			return (Addr) final_block;
		}
		else
		{
			continue;
		}
	}
	
	return (Addr) 0;
}

bool MyAllocator::split_block(Addr start_address)
{
	// If array isn't initialized, don't do anything
	if(initialized == false)
	{
		return false;
	}
	
	// Don't do anything if they pass a null pointer
	if(start_address == NULL)
	{
		return false;
	}
	
	header *memory_block = (header*) start_address;
	
	// Make sure that they give a block that is valid
	// i.e. there are no bad data values
	if(memory_block->block_size == 0)
	{
		return false;
	}
	
	// Don't split a block that's in use
	if(memory_block->in_use == true)
	{
		return false;
	}
	
	// Don't do anything if the block size is already the basic block size
	if(memory_block->block_size == basic_block_size)
	{
		return false;
	}

	int current_size = memory_block->block_size;
	
	// Remove the block from its current list.  
	header *start_of_tier = (header*) memory_array->at(current_size);
	
	// Two cases:
	// 1) The list has only one element
	// 2) The list has multiple elements
	//   - beginning of list
	//   - end of list   
	
	// Once we leave this list, start_of_tier should be
	// the block one before memory_block or memory_block itself
	while(true)
	{
		if(start_of_tier == memory_block)
		{
			break;
		}
		
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
		memory_array->at(current_size) = start_of_tier->next;
	}
	
	// Split the block into two smaller blocks
	// Weridness with pointer arithmetic using void*.  
	// Temporarily switch to char* to perform the math. 
	uintptr_t block_address = (uintptr_t) memory_block;
	uintptr_t second_block_address = block_address + (current_size / 2);
	
	// Cast back to headers
	header *block = (header*) block_address;
	header *second_block = (header*) second_block_address;
	
	// Set data values and insert into new list
	block->block_size = current_size / 2;
	second_block->block_size = current_size / 2;
	
	block->in_use = false;
	second_block->in_use = false;
	
	header *existing_block = (header*) memory_array->at(current_size / 2);
	
	
	if(existing_block == NULL)
	{
		block->next = second_block;
		second_block->next = NULL;
		
		memory_array->at(current_size / 2) = (Addr) block;
	}
	else
	{
		block->next = second_block;
		second_block->next = existing_block;
		
		memory_array->at(current_size / 2) = (Addr) block;
	}
	
	return true;
}

bool MyAllocator::are_buddies(Addr start_address1, Addr start_address2)
{
	// If array isn't initialized, don't do anything
	if(initialized == false)
	{
		return false;
	}
	
	header *memory_block_1 = reinterpret_cast<header*>(start_address1);
	header *memory_block_2 = reinterpret_cast<header*>(start_address2);
	
	if(memory_block_1 == NULL || memory_block_2 == NULL)
	{
		return false;
	}
	
	// Make sure the two blocks are the same size
	if(memory_block_1->block_size != memory_block_2->block_size)
	{
		return false;
	}
	
	// The two blocks can't be the same
	if(memory_block_1 == memory_block_2)
	{
		return false;
	}
	
	// Check to see if the blocks are buddies 
	// In order to be a block's buddy, the address must = 
	// the head_pointer + (address - head_pointer) ^ block_size
	uintptr_t buddy_address = ((uintptr_t) memory_block_1 - (uintptr_t) head_pointer) 
		^ memory_block_1->block_size;
	
	buddy_address = buddy_address + (uintptr_t) head_pointer;
	
	if((uintptr_t) memory_block_2 == buddy_address)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool MyAllocator::combine_blocks(Addr start_address1, Addr start_address2)
{
	// If array isn't initialized, don't do anything
	if(initialized == false)
	{
		return false;
	}
	
	header *memory_block_1 = reinterpret_cast<header*>(start_address1);
	header *memory_block_2 = reinterpret_cast<header*>(start_address2);
	
	if(memory_block_1 == NULL || memory_block_2 == NULL)
	{
		return false;
	}
	
	if(memory_block_1->in_use == true || memory_block_2->in_use == true)
	{
		return false;
	}
	
	// Make sure the two blocks are the same size
	if(memory_block_1->block_size != memory_block_2->block_size)
	{
		return false;
	}
	
	// The two blocks can't be the same
	if(memory_block_1 == memory_block_2)
	{
		return false;
	}
	
	// Check to see if the blocks are buddies 
	// In order to be a block's buddy, the address must = the address + block_size
	bool *mem_1_addr = (bool*) memory_block_1;
	bool *mem_2_addr = (bool*) memory_block_2;
	int current_size = memory_block_1->block_size;
	
	// Make sure that mem_1 address comes before mem_2 address
	// I.e. the order of the arguments shouldn't determine whether the program
	// is successful.  If two blocks are buddies, we should find them.  
	if(mem_1_addr > mem_2_addr)
	{
		header *temp_holder = memory_block_1;
		memory_block_1 = memory_block_2;
		memory_block_2 = temp_holder;
	}
	
	// If the blocks aren't buddies, they can't be combined.  
	if(are_buddies((Addr) memory_block_1, (Addr) memory_block_2) == false)
	{
		return false;
	}
	
	// Remove the two blocks from the list they belong to
	header *start_of_list = (header*) memory_array->at(current_size);
	
	// We should be guaranteed to find the elements in the list.  
	// Find the element before that so that I can remove it.  
	
	// remove memory_block_1
	while(true)
	{
		// In the event that the block isn't found, exit
		if(start_of_list == NULL)
		{
			return false;
		}
		
		if(start_of_list == memory_block_1)
		{
			break;
		}
		
		if(start_of_list->next == memory_block_1)
		{
			break;
		}
		
		start_of_list = start_of_list->next;
	}
	
	// After the loop, start_of_list will either be the element before
	// memory_block_1 or memory_block_1 itself
	if(start_of_list == memory_block_1)
	{
		// start_of_list is the only element in the list.  Remove it.  
		memory_array->at(current_size) = start_of_list->next;
	}
	else if(start_of_list->next == memory_block_1)
	{
		start_of_list->next = memory_block_1->next;
	}
	else
	{
		// Something bad happened if we get here
		return false;
	}

	// remove memory_block_2
	start_of_list = (header*) memory_array->at(current_size);
	while(true)
	{
		// In the event that the block isn't found, exit
		if(start_of_list == NULL)
		{
			return false;
		}
		
		if(start_of_list == memory_block_2)
		{
			break;
		}
		
		if(start_of_list->next == memory_block_2)
		{
			break;
		}
		
		start_of_list = start_of_list->next;
	}
	
	// After the loop, start_of_list will either be the element before
	// memory_block_1 or memory_block_1 itself
	if(start_of_list == memory_block_2)
	{
		// start_of_list is the only element in the list.  Remove it.  
		memory_array->at(current_size) = start_of_list->next;
	}
	else if(start_of_list->next == memory_block_2)
	{
		start_of_list->next = memory_block_2->next;
	}
	else
	{
		// Something bad happened if we get here
		return false;
	}	
	
	// Reset data values
	memory_block_1->next = NULL;
	memory_block_1->block_size = current_size << 1;
	memory_block_1->in_use = false;
	memory_block_2->next = NULL;
	memory_block_2->block_size = 0;
	memory_block_2->in_use = false;
	
	// Insert memory_block_1 into the list
	memory_block_1->next = (header*) memory_array->at(current_size << 1);
	memory_array->at(current_size << 1) = memory_block_1;
}

Addr MyAllocator::find_unused_buddy(Addr sibling_block)
{
	// If array isn't initialized, don't do anything
	if(initialized == false)
	{
		return (Addr) NULL;
	}
	
	header *sibling = (header*) sibling_block;
	int current_size = sibling->block_size;
	
	header* current_level = (header*) memory_array->at(current_size);

	// Check to see if the blocks are buddies 
	// In order to be a block's buddy, the address must = 
	// the head_pointer + (address - head_pointer) ^ block_size
	uintptr_t buddy_address = ((uintptr_t) sibling - (uintptr_t) head_pointer) 
		^ sibling->block_size;
	
	buddy_address = buddy_address + (uintptr_t) head_pointer;
	
	while(true)
	{	
		// True if at the end of the tier
		if(current_level == NULL)
		{
			return (Addr) 0;
		}
		
		// True if the block is in use
		if(current_level->in_use == true)
		{
			current_level = current_level->next;
			continue;
		}
		
		// True if the block is the buddy
		if(((uintptr_t) current_level) == buddy_address)
		{
			return (Addr) current_level;
		}
		
		// Reached if the block is not in use but is not the buddy
		current_level = current_level->next;
	}
}


bool MyAllocator::my_free(Addr _a)
{
	// If the array isn't initialized, don't do anything
	if(initialized == false)
	{
		return false;
	}
	
	uintptr_t starting_address = ((uintptr_t) _a) - sizeof(header);
	_a = (Addr) starting_address;
	
	header *block_to_free = (header*) _a;
	
	// Block to free cannot be NULL
	if(block_to_free == (header*) NULL)
	{
		return false;
	}
	
	// Block should be in use
	if(block_to_free->in_use != true)
	{
		return false;
	}
	
	// Sanity check #1: Block size should be a power of two
	if(isPowerOfTwo(block_to_free->block_size) == false)
	{
		return false;
	}
	
	// Sanity check #2: Block size shouldn't be zero
	if(block_to_free->block_size == 0)
	{
		return false;
	}
	
	// Block should exist in the list that corresponds to its size
	int current_size = block_to_free->block_size;
	
	header* current_tier = (header*) memory_array->at(current_size);
	
	while(true)
	{
		// Reached end of list without finding (header*) _a
		if(current_tier == NULL)
		{
			return false;
		}
		
		// Found the block
		if(current_tier == block_to_free)
		{
			break;
		}
		
		current_tier = current_tier->next;
	}
	
	// Free the block by setting in_use == false
	block_to_free->in_use = false;
	
	// Find the block's buddy and combine if necessary
	header* buddy_block = (header*) find_unused_buddy((Addr) block_to_free);
	
	if(buddy_block == (header*) NULL)
	{
		return true;
	}
	else
	{
		int starting_size = block_to_free->block_size;
		for(int i=starting_size; i<mem_size; i = (i << 1))
		{
			
			header *iterator = (header*) memory_array->at(i);
			
			while(true)
			{
				if(iterator == NULL)
				{
					break;
				}
				
				if(iterator->in_use == false)
				{
					header *buddy = (header*) find_unused_buddy((Addr) iterator);
					
					if(buddy != NULL)
					{
						combine_blocks((Addr) iterator, (Addr) buddy);
						i = i >> 1;
						break;
					}
				}
				
				iterator = iterator->next;
				
			}
		}
		
		combine_blocks((Addr) block_to_free, (Addr) buddy_block);
	}
}

void MyAllocator::print_array()
{
	// If allocator isn't initialized, don't do anything
	if(initialized == false)
	{
		return;
	}
	
	// Get the length of the longest size
	int max_str_length = 0;
	for(auto it = memory_array->begin(); it != memory_array->end(); ++it)
	{
		char *buffer = (char*) malloc(100 * sizeof(char));

		// Convert integer to string
		sprintf(buffer, "%d", it->first);
		std::string size_string = std::string(buffer);
		free(buffer);
		
		int string_length = size_string.length();
		
		if(string_length > max_str_length)
		{
			max_str_length = string_length;
		}
	}
	
	for(auto it = memory_array->begin(); it != memory_array->end(); ++it)
	{
		char *buffer = (char*) malloc(100 * sizeof(char));

		// Convert integer to string
		sprintf(buffer, "%d", it->first);
		std::string size_string = std::string(buffer);
		free(buffer);
		
		std::string padding = "";
		
		for(int i=0; i<(max_str_length - size_string.length()); i++)
		{
			padding = padding + ' ';
		}
		
		std::cout << BOLDGREEN << it->first << padding << ": " << RESET;
		
		header* block_iterator = (header*) it->second;
		
		while(true)
		{
			if(block_iterator == NULL)
			{
				break;
			}
			
			if(block_iterator->in_use == true)
			{
				std::cout << BOLDMAGENTA << block_iterator << BOLDWHITE << " -> " << RESET;
			}
			else
			{
				std::cout << BOLDBLUE << block_iterator << BOLDWHITE << " -> " << RESET;
			}
			
			block_iterator = block_iterator->next;
		}
		
		std::cout << BOLDRED << "[NULL]" << RESET << std::endl;	
	}
}

/* Getter Functions */
char *MyAllocator::getHeadPointer()
{
	return head_pointer;
}

unsigned int MyAllocator::getBasicBlockSize()
{
	return basic_block_size;
}

unsigned int MyAllocator::getMemSize()
{
	return mem_size;
}

unsigned short int MyAllocator::getNumTiers()
{
	return num_tiers;
}

bool MyAllocator::getInitialized()
{
	return initialized;
}
	
std::map<int, Addr> *MyAllocator::getMemoryArray()
{
	return memory_array;
}
