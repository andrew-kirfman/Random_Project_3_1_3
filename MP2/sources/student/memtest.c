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

int main(int argc, char ** argv) 
{
	MyAllocator *my_alloc = new MyAllocator();
	

	// Use getopt to grab command line arguments
	// from argv.  Use these arguments as inputs to your program
	my_alloc->init_allocator(2 << 6, 2 << 15);

    std::cout << "Hello World!" << std::endl;

	return 0;
}
