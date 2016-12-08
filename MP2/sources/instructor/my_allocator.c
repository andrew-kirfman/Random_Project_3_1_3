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
		int power_two = 2 ** exponent;
		if(power_two > number)
		{
			break;
		}
		
		exponent += 1;
	}
	
	return 2 ** exponent;
}

unsigned int MyAllocator::lower_two(unsigned int number)
{
	unsigned int exponent = 0;
	while(true)
	{
		int power_two = 2 ** exponent;
		int next_two = 2 ** (exponent + 1);
		if(power_two < number && next_two > number)
		{
			break;
		}
		
		exponent += 1;
	}
	
	return 2 ** exponent;
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
}

unsigned int MyAllocator::init_allocator(unsigned int _basic_block_size,
	unsigned int _mem_size)
{
	// Round block_size up to a power of two
	if(!isPowerOfTwo(_basic_block_size))
	{
		_basic_block_size = higher_two(_basic_block_size);
	}
	
	// Set block and mem sizes
	basic_block_size = _basic_block_size;
	mem_size = _mem_size;
	
	// Setup memory array
	unsigned int find_power = _basic_block_size;
	int exponent = 0;
	while(find_power != 1)
	{
		find_power = find_power >> 1
		exponent += 1;
	}
	
	std::cout << "Power!!!!! " << exponent << std::endl;
	
	
	
}

int MyAllocator::release_allocator()
{
	
}


Addr MyAllocator::my_malloc(unsigned int _length)
{
	
}

int MyAllocator::my_free(Addr _a)
{
	
}


















