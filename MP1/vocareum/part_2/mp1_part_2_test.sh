echo 'VOC_NO_REPORT_OUTPUT'    >> $vocareumReportFile 

# ---------------------------------------------------------------------------- #
# Substitute Main File                                                         #
# ---------------------------------------------------------------------------- #

# Replace students main file for a temporary one
mv main.cpp saved_main.cpp

echo "
/* --------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman, Margaret Baxter                                  */
/* Project: CSCE-313 Machine Problem #1                                        */
/*                                                                             */
/* File: ./MP1/main.cpp                                                        */
/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/* Standard Library Includes                                                   */
/* --------------------------------------------------------------------------- */

#include <iostream>
#include <sstream>
#include <string.h>
#include <getopt.h>
#include <climits>
#include <iomanip>

/* --------------------------------------------------------------------------- */
/* User Defined Includes                                                       */
/* --------------------------------------------------------------------------- */

#include \"linked_list2.h\"


int main(int argc, char ** argv) 
{
	/* Variable declarations */
	linked_list2 linkedList2;
	int option_char;
	int block_size = 128;
	int mem_size = block_size * 4;
	int tiers = 16;	
	
	char buf [1024];
	memset (buf, 1, 1024);	

	/* Argparse for command line options lives here */
	while(( option_char = getopt(argc, argv, \"b:s:t:h\") ) != EOF)
	{
		switch(option_char)
		{
			case 'b': block_size = atoi(optarg); break;
			case 's': mem_size = atoi(optarg); break;
			case 't': tiers = atoi(optarg); break;
            case 'h': std::cout << print_usage(); break;
            case ':': std::cout << \"Unknown option!\n\"; return 1;
		}
	}
	
	/* Check input arguments for validity */
	if(block_size < 0)
	{
		int new_block_size = 0;
        std::cout << \"ERROR: Block size can not be negative!\" << std::endl
		          << \"Please enter a new block size: \";
		while (!(std::cin >> new_block_size) || (new_block_size < 0))
        {
            std::cout << \"Bad input - try again: \";
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n'); // NB: preferred method for flushing cin
        }
        block_size = new_block_size;
	}
	if(mem_size < 0)
	{
		int new_memory_size = 0;
        std::cout << \"ERROR: Memory size can not be negative\" << std::endl
		          << \"Please enter a new memory size: \";
		while (!(std::cin >> new_memory_size) || (new_memory_size < 0))
        {
            std::cout << \"Bad input - try again: \";
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n'); // NB: preferred method for flushing cin
        }
        mem_size = new_memory_size;
	}
	if(tiers < 0)
	{
		int new_tiers = 0;
        std::cout << \"ERROR: Number of tiers can not be negative\" << std::endl
		          << \"Please enter a new number of tiers: \";
		while (!(std::cin >> new_tiers) || (new_tiers < 0))
        {
            std::cout << \"Bad input - try again: \";
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n'); // NB: preferred method for flushing cin
        }
        tiers = new_tiers;
		
	}
	
	/* --------------------------------------------------------------------------- */
	/* Test Constructor                                                            */
	/* --------------------------------------------------------------------------- */

	int bad_constructor_values = 0;

	char** pre_head_pointer = linkedList2.getHeadPointer();
	node** pre_front_pointer = linkedList2.getFrontPointer();
	node** pre_free_pointer = linkedList2.getFreePointer();

	int pre_block_size = linkedList2.getBlockSize();
	int pre_mem_size = linkedList2.getMemSize();
	int pre_max_data_size = linkedList2.getMaxDataSize();
	int pre_num_tiers = linkedList2.getNumTiers();
	bool pre_initialized = linkedList2.getInitialized();

	if(pre_head_pointer != NULL)
	{
		bad_constructor_values += 1;
	}

	if(pre_front_pointer != NULL)
	{
		bad_constructor_values += 1;
	}

	if(pre_free_pointer != NULL)
	{
		bad_constructor_values += 1;
	}

	if(pre_block_size != 0)
	{
		bad_constructor_values += 1;
	}

	if(pre_mem_size != 0)
	{
		bad_constructor_values += 1;
	}
	
	if(pre_max_data_size != 0)
	{
		bad_constructor_values += 1;
	}

	if(pre_num_tiers != 0)
	{
		bad_constructor_values += 1;
	}

	if(pre_initialized != false)
	{
		bad_constructor_values += 1;
	}

	if(bad_constructor_values > 0)
	{
		std::cout << \"Class Constructor Failed \" << std::endl;
	}
	else
	{
		std::cout << \"Class Constructor Succeeded \" << std::endl;
	}

	/*Initialize the list using the given parameters*/
	linkedList2.Init(mem_size, block_size, tiers);

	/* --------------------------------------------------------------------------- */
	/* Test Init                                                                   */
	/* --------------------------------------------------------------------------- */

	int bad_init_values = 0;

	char** init_head_pointer = linkedList2.getHeadPointer();
	node** init_front_pointer = linkedList2.getFrontPointer();
	node** init_free_pointer = linkedList2.getFreePointer();

	int init_block_size = linkedList2.getBlockSize();
	int init_mem_size = linkedList2.getMemSize();
	int init_max_data_size = linkedList2.getMaxDataSize();
	int init_num_tiers = linkedList2.getNumTiers();
	bool init_initialized = linkedList2.getInitialized();

	if(init_head_pointer == NULL || init_head_pointer == pre_head_pointer)
	{
		bad_init_values += 1;
	}
	
	if(init_front_pointer != NULL)
	{
		bad_init_values += 1;
	}

	if(init_free_pointer != NULL)
	{
		bad_init_values += 1;
	}

	if(init_block_size != block_size)
	{
		bad_init_values += 1;
	}

	if(init_mem_size != mem_size)
	{
		bad_init_values += 1;
	}

	if(init_max_data_size != (block_size - sizeof(node)))
	{
		bad_init_values += 1;
	}

	if(init_num_tiers != num_tiers)
	{
		bad_init_values += 1;
	}

	if(init_initialized != true)
	{
		bad_init_values += 1;
	}

	if(bad_init_values > 0)
	{
		std::cout << \"Bad Init Values \" << bad_init_values
			<< \" \" << std::endl;
	}

	// Insert some stuff in a few different tiers and make sure the data ends 
	// up in the right place
	int numbers_per_tier = INT_MAX/tiers;
	linkedList2.Insert(0, \"Hello World!\", 12);
	linkedList2.Insert(numbers_per_tier * 1, \"Hello World!\", 12);
	linkedList2.Insert(numbers_per_tier * 2, \"Hello World!\", 12);
	linkedList2.Insert(numbers_per_tier * 3, \"Hello World!\", 12);
	linkedList2.Insert(numbers_per_tier * 4, \"Hello World!\", 12);
	linkedList2.Insert(numbers_per_tier * 5, \"Hello World!\", 12);
	linkedList2.Insert(numbers_per_tier * 6, \"Hello World!\", 12);
	linkedList2.Insert(numbers_per_tier * 7, \"Hello World!\", 12);
	linkedList2.Insert(numbers_per_tier * 8, \"Hello World!\", 12);
	linkedList2.Insert(numbers_per_tier * 9, \"Hello World!\", 12);
	linkedList2.Insert(numbers_per_tier * 10, \"Hello World!\", 12);
	linkedList2.Insert(numbers_per_tier * 11, \"Hello World!\", 12);
	linkedList2.Insert(numbers_per_tier * 12, \"Hello World!\", 12);
	linkedList2.Insert(numbers_per_tier * 13, \"Hello World!\", 12);
	linkedList2.Insert(numbers_per_tier * 14, \"Hello World!\", 12);
	linkedList2.Insert(numbers_per_tier * 15, \"Hello World!\", 12);

	/* Lookup each node to see if it is in the right place */
	node* lookup_node = NULL;
	int failed_lookups = 0;

	if(linkedList2.Lookup(numbers_per_tier * 0) == NULL)
	{
		failed_lookups += 1;
	}
	
	if(linkedList2.Lookup(numbers_per_tier * 1) == NULL)
	{
		failed_lookups += 1;
	}
	
	if(linkedList2.Lookup(numbers_per_tier * 2) == NULL)
	{
		failed_lookups += 1;
	}
	
	if(linkedList2.Lookup(numbers_per_tier * 3) == NULL)
	{
		failed_lookups += 1;
	}
	
	if(linkedList2.Lookup(numbers_per_tier * 4) == NULL)
	{
		failed_lookups += 1;
	}
	
	if(linkedList2.Lookup(numbers_per_tier * 5) == NULL)
	{
		failed_lookups += 1;
	}
	
	if(linkedList2.Lookup(numbers_per_tier * 6) == NULL)
	{
		failed_lookups += 1;
	}
	
	if(linkedList2.Lookup(numbers_per_tier * 7) == NULL)
	{
		failed_lookups += 1;
	}
	
	if(linkedList2.Lookup(numbers_per_tier * 8) == NULL)
	{
		failed_lookups += 1;
	}
	
	if(linkedList2.Lookup(numbers_per_tier * 9) == NULL)
	{
		failed_lookups += 1;
	}
	
	if(linkedList2.Lookup(numbers_per_tier * 10) == NULL)
	{
		failed_lookups += 1;
	}
	
	if(linkedList2.Lookup(numbers_per_tier * 11) == NULL)
	{
		failed_lookups += 1;
	}
	
	if(linkedList2.Lookup(numbers_per_tier * 12) == NULL)
	{
		failed_lookups += 1;
	}
	
	if(linkedList2.Lookup(numbers_per_tier * 13) == NULL)
	{
		failed_lookups += 1;
	}
	
	if(linkedList2.Lookup(numbers_per_tier * 14) == NULL)
	{
		failed_lookups += 1;
	}
	
	if(linkedList2.Lookup(numbers_per_tier * 15) == NULL)
	{
		failed_lookups += 1;
	}
	
	if(failed_lookups > 0)
	{
		std::cout << \"Failed Lookup \" << failed_lookups
			<< \" \" << std::endl;
	}
	
	failed_tiers = 0;
	
	for(int i=0; i<tiers; i++)
	{
		node* tier_list = reinterpret_cast<node*>(linkedList2.getHeadPointer()[i]);
	
		if(tier_list == NULL)
		{
			failed_tiers += 1;
			continue;
		}
		
		if(tier_list->key != numbers_per_tier * i)
		{
			failed_tiers += 1;
			continue;
		}
	}
	
	if(failed_tiers > 0)
	{
		std::cout << \"Failed Tier Setup \" << failed_tiers
			<< \" \" << std::endl;
	}
		
		
	/* Test Deletions */
	int failed_deletions = 0;
	
	for(int i=0; i<16; i++)
	{
		linkedList2.Delete(numbers_per_tier * i);
	}
	
	for(int i=0; i<16; i++)
	{
		node* test_node = linkedList2.Lookup(numbers_per_tier * i);
		
		if(test_node == NULL)
		{
			failed_deletions += 1;
		}
	}
	
	if(failed_deletions > 0)
	{
		std::cout << \"Failed Delete \" << failed_deletions
			<< \" \" << std::endl;
	}

	/* Test Destroy */
	
	// Before calling destroy, grab all relavent data values
	char** pre_destroy_head_pointer = linkedList2.getHeadPointer();
	node** pre_destroy_front_pointer = linkedList2.getFrontPointer();
	node** pre_destroy_free_pointer = linkedList2.getFreePointer();

	int pre_destroy_block_size = linkedList2.getBlockSize();
	int pre_destroy_mem_size = linkedList2.getMemSize();
	int pre_destroy_max_data_size = linkedList2.getMaxDataSize();
	int pre_destroy_num_tiers = linkedList2.getNumTiers();
	bool pre_destroy_initialized = linkedList2.getInitialized();

	// Call destroy function
	linkedList2.Destroy();
	
	// After calling destroy, find data values again for comparison
	char** post_destroy_head_pointer = linkedList2.getHeadPointer();
	char** post_destroy_front_pointer = linkedList2.getFrontPointer();
	char** post_destroy_free_pointer = linkedList2.getFreePointer();
	
	int post_destroy_block_size = linkedList2.getBlockSize();
	int post_destroy_mem_size = linkedList2.getMemSize();
	int post_destroy_max_data_size = linkedList2.getMaxDataSize();
	int post_destroy_num_tiers = linkedList2.getNumTiers();
	int post_destroy_initialized = linkedList2.getInitialized();
	
	int failed_destroy = 0;
	
	if(post_destroy_head_pointer != NULL 
		|| pre_destroy_head_pointer == post_destroy_head_pointer)
	{
		failed_destroy += 1;
	}
	
	if(post_destroy_front_pointer != NULL
		|| pre_destroy_front_pointer == post_destroy_front_pointer)
	{
		failed_destroy += 1;
	}
	
	if(post_destroy_free_pointer != NULL
		|| pre_destroy_free_pointer == post_destroy_free_pointer)
	{
		failed_destroy += 1;
	}
	
	if(post_destroy_
	
	
	


	return 0;
}


" > main.cpp

echo "Hello World!"


# Replace this with make &> /dev/null
make 

if [ "$?" != "0" ]; then
	# If the student's code does not compile, then there's no reason to continue
	echo "Code Compiles     ...   Failed" >> $vocareumReportFile
    echo "Testing Init      ...   Untestable [0/16]" >> $vocareumReportFile
    echo "Testing Destroy   ...   Untestable [0/6]" >> $vocareumReportFile
    echo "Testing Insert    ...   Untestable [0/16]" >> $vocareumReportFile
    echo "Testing Delete    ...   Untestable [0/16]" >> $vocareumReportFile
    echo "Testing Lookup    ...   Untestable [0/8]" >> $vocareumReportFile
    echo "Testing PrintList ...   Untestable [0/8]" >> $vocareumReportFile

	echo "Init,0" >> $vocareumGradeFile
	echo "Destroy,0" >> $vocareumGradeFile
	echo "Insert,0" >> $vocareumGradeFile
	echo "Delete,0" >> $vocareumGradeFile
	echo "Lookup,0" >> $vocareumGradeFile
	echo "PrintList,0" >> $vocareumGradeFile

	exit 1
fi

test_output=$(./List_2)

if [ "$test_output" == "" ]; then
	echo "Cannot Execute ./List_1" >> $vocareumReportFile
    echo "Testing Init      ...   Untestable [0/16]" >> $vocareumReportFile
    echo "Testing Destroy   ...   Untestable [0/6]" >> $vocareumReportFile
    echo "Testing Insert    ...   Untestable [0/16]" >> $vocareumReportFile
    echo "Testing Delete    ...   Untestable [0/16]" >> $vocareumReportFile
    echo "Testing Lookup    ...   Untestable [0/8]" >> $vocareumReportFile
    echo "Testing PrintList ...   Untestable [0/8]" >> $vocareumReportFile

	echo "Init,0" >> $vocareumGradeFile
	echo "Destroy,0" >> $vocareumGradeFile
	echo "Insert,0" >> $vocareumGradeFile
	echo "Delete,0" >> $vocareumGradeFile
	echo "Lookup,0" >> $vocareumGradeFile
	echo "PrintList,0" >> $vocareumGradeFile

	exit 1
fi

# Search to see how the test program performed
test_constructor=$(echo "$test_output" | grep "Class Constructor Succeeded")
test_init=$(echo "$test_output" | grep "Init Succeeded")
test_lookup=$(echo "$test_output" | grep "Lookup Succeeded")
test_value_length=$(echo "$test_output" | grep "Value Length Saved Correctly")
test_value=$(echo "$test_output" | grep "Value Saved Correctly")
test_list_set_up=$(echo "$test_output" | grep "List Set Up Correctly")
test_delete=$(echo "$test_output" | grep "Delete Succeeded")

# Maximum points for each category
init_points=16
destroy_points=6
insert_points=16
delete_points=16
lookup_points=8
printlist_points=8
bonus_points=10



# Init
if [ "$init_points" == "16" ]; then
    echo "Testing Init      ...   Passed [0/16]" >> $vocareumReportFile
	echo "Init,16" >> $vocareumGradeFile
fi

if [[ $init_points -lt 16 && $init_points -gt 0 ]]; then
    echo "Testing Init      ...   Partial Pass [$init_points/16]" >> $vocareumReportFile
	echo "Init,$init_points" >> $vocareumGradeFile
fi

if [ "$init_points" == "0" ]; then
    echo "Testing Init      ...   Failed [0/16]" >> $vocareumReportFile
	echo "Init,0" >> $vocareumGradeFile
fi

# INSERT - 16 points
if [ "$insert_points" == "16" ]; then
	echo "Testing Insert    ...   Passed [16/16]" >> $vocareumReportFile
	echo "Insert,16" >> $vocareumGradeFile
fi

if [[ $insert_points -gt 0 && $insert_points -lt 16 ]]; then
	echo "Testing Insert    ...   Partial Pass [$insert_points/16]" >> $vocareumReportFile
	echo "Insert,$insert_points" >> $vocareumGradeFile
fi

if [ $insert_points -le 0 ]; then
	echo "Testing Insert    ...   Failed [0/16]" >> $vocareumReportFile
	echo "Insert,0" >> $vocareumGradeFile
fi

# DELETE - 16 points
if [ "$delete_points" == "16" ]; then
	echo "Testing Delete    ...   Passed [16/16]" >> $vocareumReportFile
	echo "Delete,16" >> $vocareumGradeFile
fi

if [[ $delete_points -gt 0 && $delete_points -lt 16 ]]; then
	echo "Testing Delete    ...   Partial Pass [$delete_points/16]" >> $vocareumReportFile
	echo "Delete,$delete_points" >> $vocareumGradeFile
fi

if [ $delete_points -le 0 ]; then
	echo "Testing Delete    ...   Failed [0/16]" >> $vocareumReportFile
	echo "Delete,0" >> $vocareumGradeFile
fi

# LOOKUP - 8 points
if [ "$lookup_points" == "8" ]; then
	echo "Testing Lookup    ...   Passed [8/8]" >> $vocareumReportFile
	echo "Lookup,8" >> $vocareumGradeFile
fi

if [[ $lookup_points -gt 0 && $lookup_points -lt 8 ]]; then
	echo "Testing Lookup    ...   Partial Pass [$lookup_points/8]" >> $vocareumReportFile
	echo "Lookup,$lookup_points" >> $vocareumGradeFile
fi

if [ $lookup_points -le 0 ]; then
	echo "Testing Lookup    ...   Failed [0/8]" >> $vocareumReportFile
	echo "Lookup,0" >> $vocareumGradeFile
fi

# PRINTLIST
if [ "$printlist_points" == "-1" ]; then
    echo "Testing PrintList ...   Untestable [0/8]" >> $vocareumReportFile
	echo "PrintList,0" >> $vocareumGradeFile		
else
	if [ "$printlist_points" == "8" ]; then
		echo "Testing PrintList ...   Passed [8/8]" >> $vocareumReportFile
		echo "PrintList,8" >> $vocareumGradeFile
	fi

	if [[ $printlist_points -gt 0 && $printlist_points -lt 8 ]]; then
		echo "Testing PrintList ...   Partial Pass [$printlist_points/8]" >> $vocareumReportFile
		echo "PrintList,$printlist_points" >> $vocareumGradeFile
	fi

	if [ $printlist_points -le 0 ]; then
		echo "Testing Printlist ...   Failed [0/8]" >> $vocareumReportFile
		echo "PrintList,0" >> $vocareumGradeFile
	fi
fi


# DESTROY
if [ "$destroy_points" == "-1" ]; then
    echo "Testing Destroy   ...   Untestable [0/6]" >> $vocareumReportFile
	echo "Destroy,0" >> $vocareumGradeFile
else
	if [ "$destroy_points" == "6" ]; then
		echo "Testing Destroy ...   Passed [6/6]" >> $vocareumReportFile
		echo "Destroy,6" >> $vocareumGradeFile
	fi

	if [[ $destroy_points -gt 0 && $destroy_points -lt 6 ]]; then
		echo "Testing Destroy ...   Partial Pass [$destroy_points/6]" >> $vocareumReportFile
		echo "Destroy,$destroy_points" >> $vocareumGradeFile
	fi

	if [ $destroy_points -le 0 ]; then
		echo "Testing Destroy ...   Failed [0/6]" >> $vocareumReportFile
		echo "Destroy,0" >> $vocareumGradeFile
	fi
fi



# Sub back in the existing main file so that it isn't overwritten
mv saved_main.cpp main.cpp

exit 0
