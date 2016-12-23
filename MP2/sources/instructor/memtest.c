/* ------------------------------------------------------------------------ */
/* Developer: Andrew Kirfman, Dr. Betatti                                   */
/* Project: CSCE-313 Machine Problem 2                                      */
/*                                                                          */
/* File: ./memtest.c                                                        */
/* ------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------ */
/* Standard Library Includes                                                */
/* ------------------------------------------------------------------------ */

#include <iostream>

/* ------------------------------------------------------------------------ */
/* User Defined Includes                                                    */
/* ------------------------------------------------------------------------ */

#include "my_allocator.h"

int main(int argc, char ** argv) 
{
	MyAllocator *my_alloc = new MyAllocator();
	
	
	my_alloc->init_allocator(2 << 6, 2 << 15);
	
	my_alloc->release_allocator();
	
	exit(0);
	
	
	my_alloc->print_array();
	
	std::cout << std::endl;

}
