/* ------------------------------------------------------------------------ */
/* Developer: Andrew Kirfman, Dr. Betatti                                   */
/* Project: CSCE-313 Machine Problem 2                                      */
/*                                                                          */
/* File: ./memtest.c                                                        */
/* ------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------ */
/* User Defined Includes                                                    */
/* ------------------------------------------------------------------------ */

#include "ackerman.h"
#include "my_allocator.h"

int main(int argc, char ** argv) 
{
	MyAllocator *my_alloc = new MyAllocator();
	
	
	my_alloc->init_allocator(2 << 6, 2 << 15);
	
	void* addr = my_alloc->my_malloc(235);

	header *block_1 = (header*)my_alloc->memory_array->at(256);
	header *block_2 = block_1->next;
	
	std::cout << "Buddies: " << my_alloc->are_buddies((Addr) block_1, (Addr) block_2) << std::endl;

	my_alloc->combine_blocks((Addr) block_1, (Addr) block_2);
	
	header *test_block = (header*)my_alloc->memory_array->at(2 << 6);
	
	std::cout << "TEST: " << test_block << std::endl;
	std::cout << "Size: " << test_block->block_size << std::endl;

	std::cout << "THTHTH: " << addr << std::endl;

	_exit(0);


  // input parameters (basic block size, memory length)

  // init_allocator(basic block size, memory length)

  ackerman_main(my_alloc);

  // release_allocator()
}
