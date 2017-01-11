# --------------------------------------------------------------------------- #
# Developer: Andrew Kirfman                                                   #
# Project: CSCE-313 Machine Problem #2                                        #
#                                                                             #
# File: ./Grading                                                             #
# --------------------------------------------------------------------------- #

echo 'VOC_NO_REPORT_OUTPUT' >> $vocareumReportFile

# --------------------------------------------------------------------------- #
# File Existance Tests                                                        #
# --------------------------------------------------------------------------- #

FILES_NOT_FOUND=0
if [ ! -f Makefile ]; then
	let "FILES_NOT_FOUND=$FILES_NOT_FOUND+1"
fi

if [ ! -f my_allocator.h ]; then
	let "FILES_NOT_FOUND=$FILES_NOT_FOUND+1"
fi

if [ ! -f my_allocator.c ]; then
	let "FILES_NOT_FOUND=$FILES_NOT_FOUND+1"
fi

if [ ! -f memtest.c ]; then
	let "FILES_NOT_FOUND=$FILES_NOT_FOUND+1"
fi

if [[ $FILES_NOT_FOUND -gt 0 ]]; then
	echo "Required File Not Found." >> $vocareumReportFile
	echo "init_allocator    ...   Untestable [0/15]" >> $vocareumReportFile
	echo "release_allocator ...   Untestable [0/5]" >> $vocareumReportFile
	echo "my_malloc         ...   Untestable [0/20]" >> $vocareumReportFile	
	echo "my_free           ...   Untestable [0/20]" >> $vocareumReportFile
	echo "print_list        ...   Untestable [0/10]" >> $vocareumReportFile
	echo "buddy_address     ...   Untestable [0/10]" >> $vocareumReportFile

	echo "init_allocator,0" >> $vocareumGradeFile
	echo "release_allocator,0" >> $vocareumGradeFile
	echo "my_malloc,0" >> $vocareumGradeFile
	echo "my_free,0" >> $vocareumGradeFile
	echo "print_list,0" >> $vocareumGradeFile
	echo "buddy_address,0" >> $vocareumGradeFile

	exit 0;
fi

# --------------------------------------------------------------------------- #
# Compile All Student's Files                                                 #
# --------------------------------------------------------------------------- #

make &> /dev/null

if [ "$?" != "0" ]; then
	echo "Student's files didn't compile." >> $vocareumReportFile

	echo "init_allocator    ...   Untestable [0/15]" >> $vocareumReportFile
	echo "release_allocator ...   Untestable [0/5]" >> $vocareumReportFile
	echo "my_malloc         ...   Untestable [0/20]" >> $vocareumReportFile	
	echo "my_free           ...   Untestable [0/20]" >> $vocareumReportFile
	echo "print_list        ...   Untestable [0/10]" >> $vocareumReportFile
	echo "buddy_address     ...   Untestable [0/10]" >> $vocareumReportFile

	echo "init_allocator,0" >> $vocareumGradeFile
	echo "release_allocator,0" >> $vocareumGradeFile
	echo "my_malloc,0" >> $vocareumGradeFile
	echo "my_free,0" >> $vocareumGradeFile
	echo "print_list,0" >> $vocareumGradeFile
	echo "buddy_address,0" >> $vocareumGradeFile

	exit 0
fi

# --------------------------------------------------------------------------- #
# Replace Student Main File                                                   #
# --------------------------------------------------------------------------- #

cp memtest.c memtest_old.c

# --------------------------------------------------------------------------- #
# C++ Testing Function Declarations                                           #
# --------------------------------------------------------------------------- #

function test_constructor()
{
echo "
/* -------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman, Dr. Betatti                                     */
/* Project: CSCE-313 Machine Problem 2                                        */
/*                                                                            */
/* File: ./memtest.c                                                          */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* User Defined Includes                                                      */
/* -------------------------------------------------------------------------- */

#include \"my_allocator.h\"

/* ------------------------------------------------------------------------ */
/* Standard Library Includes                                                */
/* ------------------------------------------------------------------------ */

#include <iostream>

int main(int argc, char ** argv) 
{
	MyAllocator *my_alloc = new MyAllocator();
	
	/* ------------------------------------------------------ */
	/* Test Constructor                                       */
	/* ------------------------------------------------------ */

	// Constructor set up is two points of init_allocator grade
	char *constructor_head_pointer = my_alloc->getHeadPointer();
	unsigned int constructor_block_size = my_alloc->getBasicBlockSize();
	unsigned int constructor_mem_size = my_alloc->getMemSize();
	unsigned short int constructor_num_tiers = my_alloc->getNumTiers();
	bool constructor_initialized = my_alloc->getInitialized();
	std::map<int, Addr> *memory_array = my_alloc->getMemoryArray();

	int bad_constructor_values = 0;

	if(constructor_head_pointer != NULL)
	{
		bad_constructor_values += 1;
	}

	if(constructor_block_size != 0)
	{
		bad_constructor_values += 1;
	}

	if(constructor_mem_size != 0)
	{
		bad_constructor_values += 1;
	}

	if(constructor_num_tiers != 0)
	{
		bad_constructor_values += 1;
	}

	if(constructor_initialized != false)
	{
		bad_constructor_values += 1;
	}

	if(memory_array != NULL)
	{
		bad_constructor_values += 1;
	}

	if(bad_constructor_values > 0)
	{
		std::cout << \"Constructor Succeeded \";
	}
	else
	{
		std::cout << \"Constructor Failed \"
	}
	
	return 0;
}" > memtest.c

	make &> /dev/null
	
	if [ "$?" != "0" ]; then
		echo "Constructor Failed "
	else
		test_output=$(memtest 2> /dev/null)

		echo "$test_output"
	fi
}


function test_init()
{
echo "
/* ------------------------------------------------------------------------ */
/* Developer: Andrew Kirfman, Dr. Betatti                                   */
/* Project: CSCE-313 Machine Problem 2                                      */
/*                                                                          */
/* File: ./memtest.c                                                        */
/* ------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------ */
/* User Defined Includes                                                    */
/* ------------------------------------------------------------------------ */

#include \"my_allocator.h\"

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
		std::cout << \"Init Failed \" << bad_init_values << \" \";
	}
	else
	{
		std::cout << \"Init Succeeded \";
	}

	return 0;
}" > memtest.c

	make # &> /dev/null
	
	if [ "$?" != "0" ]; then
		echo "Init Failed 9 "
	else
		./memtest

		test_output=$(./memtest 2> /dev/null)

		echo "$test_output"
	fi
}


function test_split_block()
{
echo "
/* ------------------------------------------------------------------------ */
/* Developer: Andrew Kirfman, Dr. Betatti                                   */
/* Project: CSCE-313 Machine Problem 2                                      */
/*                                                                          */
/* File: ./memtest.c                                                        */
/* ------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------ */
/* User Defined Includes                                                    */
/* ------------------------------------------------------------------------ */

#include \"my_allocator.h\"

/* ------------------------------------------------------------------------ */
/* Standard Library Includes                                                */
/* ------------------------------------------------------------------------ */

#include <iostream>

int main(int argc, char ** argv) 
{
	MyAllocator *my_alloc = new MyAllocator();

	// Set the basic block size to 128 and the mem size to 2 ^ 18
	my_alloc->init_allocator(2 << 6, 2 << 17);

	/* ------------------------------------------------------ */
	/* Test Split Block                                       */
	/* ------------------------------------------------------ */

	int test_split_failures = 0;

	Addr first_block = my_alloc->getMemoryArray()->at(2 << 17);

	my_alloc->split_block((Addr) first_block);

	header *second_block = (header*) my_alloc->getMemoryArray()->at(2 << 16);

	if(second_block != NULL)
	{
		if(second_block->block_size != (2 << 16))
		{
			test_split_failures += 1;
		}

		if(second_block->in_use != false)
		{
			test_split_failures += 1;
		}

		second_block = second_block->next;

		if(second_block != NULL)
		{
			if(second_block->block_size != (2 << 16))
			{
				test_split_failures += 1;
			}

			if(second_block->in_use != false)
			{
				test_split_failures += 1;
			}
		}
		else
		{
			test_split_failures += 2;
		}

		if(test_split_failures > 0)
		{
			std::cout << \"Split Block Failed \" << test_split_failures << \" \";
		}
		else
		{
			std::cout << \"Split Block Succeeded \";
		}
	}
	else
	{
		std::cout << \"Split Block Failed 4 \" << std::endl;
	}

	return 0;
}" > memtest.c

	make &> /dev/null

	if [ "$?" != "0" ]; then
		echo "Split Block Failed 4 "
	else
		test_output=$(memtest 2>/dev/null)

		echo "$test_output"
	fi
}


test_combine_blocks()
{
echo "
/* ------------------------------------------------------------------------ */
/* Developer: Andrew Kirfman, Dr. Betatti                                   */
/* Project: CSCE-313 Machine Problem 2                                      */
/*                                                                          */
/* File: ./memtest.c                                                        */
/* ------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------ */
/* User Defined Includes                                                    */
/* ------------------------------------------------------------------------ */

#include \"my_allocator.h\"

/* ------------------------------------------------------------------------ */
/* Standard Library Includes                                                */
/* ------------------------------------------------------------------------ */

#include <iostream>

int main(int argc, char ** argv) 
{
	MyAllocator *my_alloc = new MyAllocator();

	// Set the basic block size to 128 and the mem size to 2 ^ 18
	my_alloc->init_allocator(2 << 6, 2 << 17);

	/* ------------------------------------------------------ */
	/* Test Combine Blocks                                    */
	/* ------------------------------------------------------ */

	// Split a block in two to combine again
	my_alloc->split_block((Addr) my_alloc->getMemoryArray->at(2 << 17));
	header *second_block = (header*) my_alloc->getMemoryArray->at(2 << 16);

	int test_combine_failures = 0;

	header* iterator = (header*) my_alloc->getMemoryArray()->at(2 << 16);

	if(iterator != NULL && iterator->next != NULL)
	{
		header *block_to_combine_1 = iterator;
		header *block_to_combine_2 = iterator->next;

		my_alloc->combine_blocks(block_to_combine_1, block_to_combine_2);

		
		// Once combined, a single block should be at the next level above
		iterator = (header*) my_alloc->getMemoryArray->at(2 << 17);

		if(iterator != NULL)
		{
			std::cout << \"Combine Blocks Succeeded \" << std::endl;
		}
		else
		{
			std::cout << \"Combine Blocks Failed \" << std::endl;
		}
	}
	else
	{
		std::cout << \"Combine Blocks Failed \" << std::endl;
	}

	return 0;
}
" > memtest.c

	make &> /dev/null
	
	if [ "$?" != "0" ]; then
		echo "Combine Blocks Failed "
	else
		test_output=$(memtest 2> /dev/null)

		echo "$test_output"
	fi
}


test_my_malloc()
{
	# The C++ function ran here performs four tests.  Since my_malloc is
	# worth 20 points total, each tests is worth 5 points
echo "
/* ------------------------------------------------------------------------ */
/* Developer: Andrew Kirfman, Dr. Betatti                                   */
/* Project: CSCE-313 Machine Problem 2                                      */
/*                                                                          */
/* File: ./memtest.c                                                        */
/* ------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------ */
/* Standard Library Includes                                                */
/* ------------------------------------------------------------------------ */

#include <cstdint>
#include <iostream>

/* ------------------------------------------------------------------------ */
/* User Defined Includes                                                    */
/* ------------------------------------------------------------------------ */

#include \"my_allocator.h\"

int main(int argc, char ** argv) 
{
	MyAllocator *my_alloc = new MyAllocator();

	// Set the basic block size to 128 and the mem size to 2 ^ 18
	my_alloc->init_allocator(2 << 6, 2 << 17);

	/* ------------------------------------------------------ */
	/* Test my_malloc()                                       */
	/* ------------------------------------------------------ */

	bool success1 = true;
	bool success2 = true;
	bool success3 = true;
	bool success4_1 = true;
	bool success4_2 = true;

	/* Test1 */

	// Should give the address of a 128 byte block
	void *addr1 = my_alloc->my_malloc(100);
	
	header *test1 = (header*) my_alloc->getMemoryArray()->at(128);

	if(test1 != NULL)
	{
		// Mallocing a block should result in two of the same size,
		// one used and one unused.  If the first block is unused,
		// try to use the second one.  
		if(test1->in_use == true)
		{
			uintptr_t malloc_address = (uintptr_t) test1 + sizeof(header);

			if(malloc_address != (uintptr_t) addr1)
			{
				success1 = false;
			}

			if(test1->block_size != 128)
			{
				success1 = false;
			}
		}
		else
		{
			test1 = test1->next;

			if(test1 != NULL)
			{
				uintptr_t malloc_address = (uintptr_t) test1 + sizeof(header);

				if(malloc_address != (uintptr_t) addr1)
				{
					success1 = false;
				}

				if(test1->block_size != 128)
				{
					success1 = false;
				}
			}
			else
			{
				success1 = false;
			}
		}
	}
	else
	{
		success1 = false;
	}

	
	/* Test2 */

	// Should give the address of a 1024 byte block
	void *addr2 = my_alloc->my_malloc(1000);

	header *test2 = (header*) my_alloc->getMemoryArray()->at(1024);

	if(test2 != NULL)
	{
		if(test2->in_use != false)
		{
			if(test2->next != NULL)
			{
				success2 = false;
			}
			else
			{
				uintptr_t malloc_address = (uintptr_t) test2 + sizeof(header);

				if(malloc_address != (uintptr_t) addr2)
				{
					success2 = false;
				}
				
				if(test2->block_size != 1024)
				{
					success2 = false;
				}
			}
		}
		else
		{
			success2 = false;
		}

	}
	else
	{
		success2 = false;
	}


	/* Test3 */

	// Should give the address of a 1024 byte block.  
	// Will need to split the next 2048 byte block to make this one
	void *addr3 = my_alloc->my_malloc(1000);

	header *test3 = (header*) my_alloc->getMemoryArray()->at(1024);

	bool found_3 = false;
	while(true)
	{
		if(test3 == NULL)
		{
			break;
		}
	
		uintptr_t malloc_address = (uintptr_t) test3 + sizeof(header);

		if(malloc_address == (uintptr_t) addr3)
		{
			if(test3->in_use == true)
			{
				if(test3->block_size == 1024)
				{
					found_3 = true;
					break;
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		}

		test3 = test3->next;
	}

	if(found_3 == true)
	{
		success3 = true;
	}


	/* Test4 */

	// First, tests something that should fail (allocing a huge block)
	// Second, tests allocing a 512 byte block
	void *addr4_1 = my_alloc->my_malloc(1000000);

	if(addr4_1 == (void*) 0)
	{
		success4_1 = true;
	}

	void *addr4_2 = my_alloc->my_malloc(450);

	header *test4 = (header*) my_alloc->getMemoryArray()->at(512);

	bool found_4 = false;
	while(true)
	{
		if(test4 == NULL)
		{
			break;
		}

		uintptr_t malloc_address = (uintptr_t) test4 + sizeof(header);
		
		if((uintptr_t) malloc_address == (uintptr_t) addr4_2)
		{
			found_4 = true;
			break;
		}
		
		test4 = test4->next;
	}

	if(found_4 == true)
	{
		success4_2 = true;
	}

	if(success1 == true)
	{
		std::cout << \"success1 \";
	}
	else
	{
		std::cout << \"failure1 \";
	}

	if(success2 == true)
	{
		std::cout << \"success2 \";
	}
	else
	{
		std::cout << \"failure2 \";
	}

	if(success3 == true)
	{
		std::cout << \"success3 \";
	}
	else
	{
		std::cout << \"failure3 \";
	}

	if(success4_1 == true)
	{
		std::cout << \"success4_1 \";
	}
	else
	{
		std::cout << \"failure4_1 \";
	}
	
	if(success4_2 == true)
	{
		std::cout << \"success4_2 \";
	}
	else
	{
		std::cout << \"failure4_2 \";
	}
	
	return 0;
}
" > memtest.c

	make &> /dev/null

	if [ "$?" != "0" ]; then
		echo "My_Malloc Failed "
	else
		test_output=$(./memtest 2> /dev/null)

		echo "$test_output"
	fi

}


test_my_free()
{
echo "
/* ------------------------------------------------------------------------ */
/* Developer: Andrew Kirfman, Dr. Betatti                                   */
/* Project: CSCE-313 Machine Problem 2                                      */
/*                                                                          */
/* File: ./memtest.c                                                        */
/* ------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------ */
/* Standard Library Includes                                                */
/* ------------------------------------------------------------------------ */

#include <cstdint>
#include <iostream>

/* ------------------------------------------------------------------------ */
/* User Defined Includes                                                    */
/* ------------------------------------------------------------------------ */

#include \"my_allocator.h\"

int main(int argc, char ** argv) 
{
	MyAllocator *my_alloc = new MyAllocator();

	// Set the basic block size to 128 and the mem size to 2 ^ 18
	my_alloc->init_allocator(2 << 6, 2 << 17);

	// Alloc a block so everything gets split up
	Addr test_address = my_alloc->my_malloc(100);

	// Free that block and see if all of the blocks get combined back up
	my_alloc->my_free(test_address);

	bool free_correctly = true;
	for(int i=7; i<18; i++)
    {
    	Addr tier = my_alloc->getMemoryArray()->at(2 << (i - 1));               
        
		if(tier != NULL)
		{
			free_correctly = false;
		}
    }

	if(free_correctly == false)
	{
		std::cout << \"Free Failed  3 \";
		return 1;
	}
	
	int num_failures = 0;
	Addr final_tier = my_alloc->getMemoryArray()->at(2 << 17);
	
	if(final_tier != NULL)
	{
		if(((header*) final_tier)->in_use == false)
		{
			if(((header*) final_tier)->block_size == (2 << 17))
			{
				std::cout << \"Free Succeeded \";
				return 0;
			}
			else
			{
				num_failures += 1;
			}
		}
		else
		{
			num_failures += 2;
		}
	}
	else
	{
		num_failures += 3;
	}

	if(num_failures > 0)
	{
		std::cout << \"Free Failed \" << num_failures << ' ';
	}
	else
	{
		std::cout << \"Free Succeeded \";
		return 0;
	}
                       
	return 0;
}" > memtest.c

	make &> /dev/null

	if [ "$?" != "0" ]; then
		echo "Free Failed 3 "
	else
		test_output=$(./memtest 2> /dev/null)

		echo "$test_output"
	fi 
}

test_release_allocator()
{
echo "
/* ------------------------------------------------------------------------ */
/* Developer: Andrew Kirfman, Dr. Betatti                                   */
/* Project: CSCE-313 Machine Problem 2                                      */
/*                                                                          */
/* File: ./memtest.c                                                        */
/* ------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------ */
/* Standard Library Includes                                                */
/* ------------------------------------------------------------------------ */

#include <cstdint>
#include <iostream>

/* ------------------------------------------------------------------------ */
/* User Defined Includes                                                    */
/* ------------------------------------------------------------------------ */

#include \"my_allocator.h\"

int main(int argc, char ** argv) 
{
	MyAllocator *my_alloc = new MyAllocator();

	// Set the basic block size to 128 and the mem size to 2 ^ 18
	my_alloc->init_allocator(2 << 6, 2 << 17);

	// Alloc some data 
	my_alloc->my_malloc(100);

	my_alloc->release_allocator();

	char* test_head_pointer = my_alloc->getHeadPointer();
	std::map<int, Addr> *test_memory_array = my_alloc->getMemoryArray();

	if(test_head_pointer == NULL && test_memory_array == NULL)
	{
		std::cout << \"Release Allocator Succeeded \";
	}
	else
	{
		std::cout << \"Release Allocator Failed \";
	}

	return 0;
}" > memtest.c

	make &> /dev/null

	if [ "$?" != "0" ]; then
		echo "Release Allocator Failed "
	else
		test_output=$(./memtest 2> /dev/null)

		echo "$test_output"
	fi
}

test_print_list()
{
echo "
/* ------------------------------------------------------------------------ */
/* Developer: Andrew Kirfman, Dr. Betatti                                   */
/* Project: CSCE-313 Machine Problem 2                                      */
/*                                                                          */
/* File: ./memtest.c                                                        */
/* ------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------ */
/* Standard Library Includes                                                */
/* ------------------------------------------------------------------------ */

#include <cstdint>
#include <iostream>

/* ------------------------------------------------------------------------ */
/* User Defined Includes                                                    */
/* ------------------------------------------------------------------------ */

#include \"my_allocator.h\"

int main(int argc, char ** argv) 
{
	MyAllocator *my_alloc = new MyAllocator();

	// Set the basic block size to 128 and the mem size to 2 ^ 18
	my_alloc->init_allocator(2 << 6, 2 << 17);

	// Perform a few calls so that there is something to print
	my_alloc->my_malloc(100);
	my_alloc->my_malloc(1000);
	my_alloc->my_malloc(1000);

	// Actually call PrintList
	my_alloc->print_array();
}" > memtest.c

	make &> /dev/null

	if [ "$?" != "0" ]; then
		echo "print_list Failed "
	else
		test_output=$(./memtest 2> /dev/null)
	
		echo "$test_output"
	fi

}

test_buddy_address()
{
echo "
/* ------------------------------------------------------------------------ */
/* Developer: Andrew Kirfman, Dr. Betatti                                   */
/* Project: CSCE-313 Machine Problem 2                                      */
/*                                                                          */
/* File: ./memtest.c                                                        */
/* ------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------ */
/* Standard Library Includes                                                */
/* ------------------------------------------------------------------------ */

#include <cstdint>
#include <iostream>

/* ------------------------------------------------------------------------ */
/* User Defined Includes                                                    */
/* ------------------------------------------------------------------------ */

#include \"my_allocator.h\"

int main(int argc, char ** argv) 
{
	MyAllocator *my_alloc = new MyAllocator();

	// Set the basic block size to 128 and the mem size to 2 ^ 18
	my_alloc->init_allocator(2 << 6, 2 << 17);

	// Alloc a block so I can calculate its buddy address
	my_alloc->my_malloc(1000);
	
	header* block = (header*) my_alloc->getMemoryArray()->at(1024);

	// There should be two blocks on this list.  The next one is definitely
	// the buddy of the first one.  

	Addr block_1 = (Addr) block;
	Addr block_2 = (Addr) block->next;
	Addr buddy = NULL;
	bool buddy_found = false;

	if(((header*) block_1)->in_use == true)
	{
		buddy = my_alloc->find_unused_buddy(block_1);
		
		if(buddy == block_2)
		{
			buddy_found = true;
		}
	}
	else
	{
		buddy = my_alloc->find_unused_buddy(block_2);
	
		if(buddy == block_1)
		{
			buddy_found = true;
		}
	}

	if(buddy_found == true)
	{
		std::cout << \"Buddy Address Succeeded \";
	}
	else
	{
		std::cout << \"Buddy Address Failed \";
	}

	return 0;
}" > memtest.c

	make &> /dev/null

	if [ "$?" != "0" ]; then
		echo "Buddy Address Failed "
	else
		test_output=$(./memtest 2> /dev/null)

		echo "$test_output"
	fi
}


# --------------------------------------------------------------------------- #
# Point Allocations                                                           #
# --------------------------------------------------------------------------- #

# Point Allocations
init_allocator_points=15
release_allocator_points=5
my_malloc_points=30
my_free_points=30
print_list_points=10
buddy_address_points=10
split_block_points=20
combine_block_points=20

# --------------------------------------------------------------------------- #
# Call Testing Functions                                                      #
# --------------------------------------------------------------------------- #

constructor_result=$(test_constructor)
init_result=$(test_init)
split_block_result=$(test_split_block)
combine_block_result=$(test_combine_blocks)
my_malloc_result=$(test_my_malloc)
my_free_result=$(test_my_free)
release_allocator_result=$(test_release_allocator)
print_list_result=$(test_print_list)
buddy_address_result=$(test_buddy_address)

# --------------------------------------------------------------------------- #
# Check Output & Assign Points                                                #
# --------------------------------------------------------------------------- #

# Constructor
constructor_success=$(echo "$constructor_result" | grep "Constructor Succeeded")
if [ "$constructor_success" == "" ]; then
	let "init_allocator_points=$init_allocator_points-2"
fi

# Init
init_success=$(echo "$init_result" | grep "Init Succeeded")
echo "$init_result"
if [ "$init_success" == "" ]; then

	init_failures=$(echo "$init_result" | sed 's/Init Failed \(.*\) /\1/g')

	if [[ $init_failures -gt 0 && $init_failures -lt 9 ]]; then
		let "init_allocator_points=$init_allocator_points-$init_failures"
	fi

	if [ "$init_failures" == "9" ]; then
		let "init_allocator_points=0"
	fi
fi


# Split Block
split_block_success=$(echo "$split_block_result" | grep "Split Block Succeeded")
if [ "$split_block_success" == "" ]; then
	
	split_failures=$(echo "$split_block_result" | sed 's/Split Block Failed \(.*\) /\1/g')

	if [[ $split_failures -gt 0 && $split_failures -lt 4 ]]; then
		let "split_block_points=$split_block_points-5*$split_failures"
	fi

	if [ "$split_failures" == "4" ]; then
		let "split_block_points=0"
	fi
fi

# Combine Blocks
combine_blocks_success=$(echo "$combine_block_result" | grep "Combine Blocks Succeeded")
if [ "$combine_blocks_success" == "" ]; then
	let "combine_block_points=0"
fi

# My Malloc
my_malloc_failure=$(echo "$my_malloc_result" | grep "My_Malloc Failed")
if [ "$my_malloc_failure" == "" ]; then
	
	# Save each success printing and then deduct points individually
	my_malloc_1=$(echo "$my_malloc_result" | grep "success1")
	my_malloc_2=$(echo "$my_malloc_result" | grep "success2")
	my_malloc_3=$(echo "$my_malloc_result" | grep "success3")
	my_malloc_4_1=$(echo "$my_malloc_result" | grep "success4_1")
	my_malloc_4_2=$(echo "$my_malloc_result" | grep "success4_2")

	# Report points for each successful part
	if [ "$my_malloc_1" == "" ]; then
		let "my_malloc_points=$my_malloc_points-8"
	fi

	if [ "$my_malloc_2" == "" ]; then
		let "my_malloc_points=$my_malloc_points-7"
	fi

	if [ "$my_malloc_3" == "" ]; then
		let "my_malloc_points=$my_malloc_points-8"
	fi

	if [ "$my_malloc_4_1" == "" ]; then
		let "my_malloc_points=$my_malloc_points-2"
	fi

	if [ "$my_malloc_4_2" == "" ]; then
		let "my_malloc_points=$my_malloc_points-5"
	fi
else
	let "my_malloc_points=0"
fi


# My Free
my_free_success=$(echo "$my_free_result" | grep "Free Succeeded")
if [ "$my_free_success" == "" ]; then

	free_failures=$(echo "$my_free_result" | sed 's/Free Failed \(.*\) /\1/g')

	if [[ $free_failures -gt 0 ]]; then
		let "my_free_points=$my_free_points-10*$free_failures"
	fi
fi

# Release Allocator
release_allocator_success=$(echo "$release_allocator_result" | grep "Release Allocator Succeeded")
if [ "$release_allocator_success" == "" ]; then
	let "release_allocator_points=0"
fi

# Print_list
if [ "$print_list_result" == "" ]; then
	let "print_list_points=0"
fi

print_list_failure=$(echo "$print_list_result" | grep "print_list Failed")
if [ "$print_list_failure" != "" ]; then
	let "print_list_points=0"
fi

# Buddy Address
buddy_address_success=$(echo "$buddy_address_result" | grep "Buddy Address Succeeded")
if [ "$buddy_address_success" == "" ]; then
	let "buddy_address_points=0"
fi

# --------------------------------------------------------------------------- #
# Error Checking                                                              #
# --------------------------------------------------------------------------- #

# Each function should return something 


# --------------------------------------------------------------------------- #
# No Negative Points                                                          #
# --------------------------------------------------------------------------- #

if [[ $init_allocator_points -lt 0 ]]; then
	let "init_allocator_ponits=0"
fi

if [[ $release_allocator_points -lt 0 ]]; then
	let "release_allocator_points=0"
fi

if [[ $my_malloc_points -lt 0 ]]; then
	let "my_malloc_points=0"
fi

if [[ $my_free_points -lt 0 ]]; then
	let "my_free_points=0"
fi

if [[ $print_list_points -lt 0 ]]; then
	let "print_list_points=0"
fi

if [[ $buddy_address_points -lt 0 ]]; then
	let "buddy_address_points=0"
fi

if [[ $split_block_points -lt 0 ]]; then
	let "split_block_points=0"
fi

if [[ $combine_block_points -lt 0 ]]; then
	let "combine_block_points=0"
fi

# --------------------------------------------------------------------------- #
# Report Point Totals                                                         #
# --------------------------------------------------------------------------- #

# Init Allocator
if [ "$init_allocator_points" == "15" ]; then
	echo "init_allocator      ...   Passed [15/15]" >> $vocareumReportFile
	echo "init_allocator,15" >> $vocareumGradeFile
else if [[ $init_allocator_points -lt 15 && $init_allocator_points -gt 0 ]]; then
	echo "init_allocator      ...   Partial Pass [$init_allocator_points/15]" >> $vocareumReportFile
	echo "init_allocator,$init_allocator_points" >> $vocareumGradeFile
else if [ "$init_allocator_points" == "0" ]; then
	echo "init_allocator      ...   Failed [0/15]" >> $vocareumReportFile
	echo "init_allocator,0" >> $vocareumGradeFile
fi

# Release Allocator
if [ "$release_allocator_points" == "5" ]; then
	echo "release_allocator   ...   Passed [5/5]" >> $vocareumReportFile
	echo "release_allocator,5" >> $vocareumGradeFile
else if [[ $release_allocator_points -lt 5 && $release_allocator_points -gt 0 ]]; then
	echo "release_allocator   ...   Partial Pass [$release_allocator_points/5]" >> $vocareumReportFile	
	echo "release_allocator,$release_allocator_points" >> $vocareumGradeFile
else if [ "$release_allocator_points" == "0" ]; then
	echo "release_allocator   ...   Failed [0/5]" >> $vocareumReportFile
	echo "release_allocator,0" >> $vocareumGradeFile
fi

# My Malloc
if [ "$my_malloc_points" == "30" ]; then
	echo "my_malloc           ...   Passed [30/30]" >> $vocareumReportFile
	echo "my_malloc,30" >> $vocareumGradeFile
else if [[ $my_malloc_points -lt 30 && $my_malloc_points -gt 0 ]]; then
	echo "my_malloc           ...   Partial Pass [$my_malloc_points/30]" >> $vocareumReportFile
	echo "my_malloc,$my_malloc_points" >> $vocareumGradeFile
else if [ "$my_malloc_points" == "0" ]; then
	echo "my_malloc           ...   Failed [0/30]" >> $vocareumReportFile
	echo "my_malloc,0" >> $vocareumGradeFile
fi

# My Free
if [ ]; then

else if [[ ]]; then

else if [ ]; then

fi

# Print List
if [ "$print_list_points" == 10 ]; then
	echo "print_list          ...   Passed [10/10]" >> $vocareumReportFile
	echo "print_list,10" >> $vocareumGradeFile
else if [[ $print_list_points -lt 10 && $print_list_points -gt 0 ]]; then
	echo "print_list          ...   Partial Pass [$print_list_points/10]" >> $vocareumReportFile
	echo "print_list,$print_list_points" >> $vocareumGradeFile
else if [ "$print_list_points" == "0" ]; then
	echo "print_list          ...   Failed [0/10]" >> $vocareumReportFile
	echo "print_list,0" >> $vocareumGradeFile
fi

# Buddy Address
if [ ]; then

else if [[ ]]; then

else if [ ]; then

fi

# Split Block
if [ ]; then

else if [[ ]]; then

else if [ ]; then

fi

# Combine Block
if [ ]; then

else if [[ ]]; then

else if [ ]; then

fi



#init_allocator_points=15
#release_allocator_points=5
#my_malloc_points=30
#my_free_points=30
#print_list_points=10
#buddy_address_points=10
#split_block_points=20
#combine_block_points=20




# Replace student's main function()
cp memtest_old.c memtest.c

exit 0
