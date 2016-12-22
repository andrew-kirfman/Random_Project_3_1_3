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
#include <climits>

/* --------------------------------------------------------------------------- */
/* User Defined Includes                                                       */
/* --------------------------------------------------------------------------- */

#include \"linked_list2.h\"


int main(int argc, char ** argv) 
{
	/* Variable declarations */
	linked_list2 linkedList2;
	int block_size = 128;
	int mem_size = block_size * 128;
	int tiers = 16;	
	
	/* --------------------------------------------------------------------------- */
	/* Test Constructor                                                            */
	/* --------------------------------------------------------------------------- */

	int bad_constructor_values = 0;

	char** pre_head_pointer = linkedList2.getHeadPointer();

	if(pre_head_pointer != NULL)
	{
		bad_constructor_values += 1;
	}

	if(linkedList2.getFrontPointer() != NULL)
	{
		bad_constructor_values += 1;
	}

	if(linkedList2.getFreePointer() != NULL)
	{
		bad_constructor_values += 1;
	}

	if(linkedList2.getBlockSize() != 0)
	{
		bad_constructor_values += 1;
	}

	if(linkedList2.getMemSize() != 0)
	{
		bad_constructor_values += 1;
	}
	
	if(linkedList2.getMaxDataSize() != 0)
	{
		bad_constructor_values += 1;
	}

	if(linkedList2.getNumTiers() != 0)
	{
		bad_constructor_values += 1;
	}

	if(linkedList2.getInitialized() != false)
	{
		bad_constructor_values += 1;
	}

	if(bad_constructor_values == 0)
	{
		std::cout << \"Class Constructor Succeeded \" << std::endl;
	}

	/*Initialize the list using the given parameters*/
	linkedList2.Init(mem_size, block_size, tiers);

	/* --------------------------------------------------------------------------- */
	/* Test Init                                                                   */
	/* --------------------------------------------------------------------------- */

	int bad_init_values = 0;

	node** init_front_pointer = linkedList2.getFrontPointer();
	node** init_free_pointer = linkedList2.getFreePointer();

	if(linkedList2.getHeadPointer() == NULL 
		|| linkedList2.getHeadPointer() == pre_head_pointer)
	{
		bad_init_values += 1;
	}
	
	bool front_pointer = false;
	for(int i=0; i<tiers; i++)
	{
		if(init_front_pointer[i] != NULL)
		{
			front_pointer = true;
		}
	}
	
	if(front_pointer == true)
	{
		bad_init_values += 1;
	}

	bool free_pointer = false;
	for(int i=0; i<tiers; i++)
	{
		if(init_free_pointer[i] != NULL)
		{
			free_pointer = true;
		}
	}
	
	if(free_pointer == true)
	{
		bad_init_values += 1;
	}
	
	if(linkedList2.getBlockSize() != block_size)
	{
		bad_init_values += 1;
	}

	if(linkedList2.getMemSize() != mem_size)
	{
		bad_init_values += 1;
	}

	if(linkedList2.getMaxDataSize() != (block_size - sizeof(node)))
	{
		bad_init_values += 1;
	}

	if(linkedList2.getNumTiers() != tiers)
	{
		bad_init_values += 1;
	}

	if(linkedList2.getInitialized() != true)
	{
		bad_init_values += 1;
	}

	if(bad_init_values > 0)
	{
		std::cout << \"Bad Init Values \" << bad_init_values
			<< \" \" << std::endl;
	}
	else
	{
		std::cout << \"Init Succeeded \" << std::endl;
	}

	// Insert some stuff in a few different tiers and make sure the data ends 
	// up in the right place
	int numbers_per_tier = INT_MAX/tiers;
	for(unsigned short int i=0; i<16; i++)
    {
		linkedList2.Insert(numbers_per_tier * i, \"Hello World!\", 12);                                      
  	}

	/* Lookup each node to see if it is in the right place */
	int failed_lookups = 0;

	int insert_failures = 0;
	
    node *lookup_test = NULL;
   	for(unsigned short int i=0; i<16; i++)
    {
		lookup_test = linkedList2.Lookup(numbers_per_tier * i);
		if(lookup_test == NULL)
		{
			failed_lookups += 1;
		}
		else
		{
			if(lookup_test->key != numbers_per_tier * i)
			{
				insert_failures += 1;
			}
			else if(lookup_test->value_len != 12)
			{
				insert_failures += 1;
			}
			else
			{
				char* node_address = (char*) lookup_test;
				node_address = node_address + sizeof(node);
					
				// Iterate through pointers to recover the entire string
				std::string data_string = \"\";
				for(int i=0; i<12; i++)
				{
					data_string = data_string + *node_address;
					node_address++;
				}
	
				if(data_string != \"Hello World!\")
				{
					insert_failures += 1;
				}
			}
		}
	}
	
	if(failed_lookups > 0)
	{
		std::cout << \"Failed Lookup \" << failed_lookups
			<< \" \" << std::endl;
	}
	else
	{
		std::cout << \"Lookup Succeeded \" << std::endl;
	}
	
	if(insert_failures > 0)
	{
		std::cout << \"Failed Insert \" << insert_failures
			<< \" \" << std::endl;
	}
	else
	{
		std::cout << \"Insert Succeeded \" << std::endl;
	}
	
	
	int failed_tiers = 0;
	
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
	else
	{
		std::cout << \"Tier Setup Succeeded \" << std::endl;
	}
		
	/* Test Deletions */
	int failed_deletions = 0;
	
	for(int i=0; i<16; i++)
	{
		linkedList2.Delete(numbers_per_tier * i);
                       
        if(linkedList2.Lookup(numbers_per_tier * i) != NULL)
        {
        	failed_deletions += 1;              
        }
	}

	if(failed_deletions > 0)
	{
		std::cout << \"Failed Delete \" << failed_deletions
			<< \" \" << std::endl;
	}
	else
	{
		std::cout << \"Delete Succeeded \" << std::endl;
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
	node** post_destroy_front_pointer = linkedList2.getFrontPointer();
	node** post_destroy_free_pointer = linkedList2.getFreePointer();
	
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
	
	if(post_destroy_block_size != 0
		|| pre_destroy_block_size == post_destroy_block_size)	
	{
		failed_destroy += 1;
	}
	
	if(post_destroy_mem_size != 0
		|| pre_destroy_mem_size == post_destroy_mem_size)
	{
		failed_destroy += 1;
	}
	
	if(post_destroy_max_data_size != 0
		|| pre_destroy_mem_size == post_destroy_mem_size)
	{
		failed_destroy += 1;
	}
	
	if(post_destroy_num_tiers != 0
		|| pre_destroy_num_tiers == post_destroy_num_tiers)
	{
		failed_destroy += 1;
	}
	
	if(post_destroy_initialized != false
		|| pre_destroy_initialized == post_destroy_initialized)
	{
		failed_destroy += 1;
	}
	
	if(failed_destroy > 0)
	{
		std::cout << \"Failed Destroy \" << failed_destroy
			<< \" \" << std::endl;
	}
	else
	{
		std::cout << \"Destroy Succeeded \" << std::endl;
	}

	return 0;
}" > main.cpp

make &> /dev/null

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

# Maximum points for each category
init_points=16
destroy_points=6
insert_points=16
delete_points=16
lookup_points=8
printlist_points=8
bonus_points=10

# Constructor 
if [ "$(echo "$test_output" | grep "Class Constructor Succeeded")" == "" ]; then
	let "init_points=$init_points-8"
fi

# Init
if [ "$(echo "$test_output" | grep "Init Succeeded")" == "" ]; then
	number_bad=$(echo "$test_output" | grep "Bad Init Values")

	if [ "$number_bad" != "" ]; then
		temp1=$(echo "$test_output" | sed 's/.*Bad Init Values[ ]\([0-9]*\).*/\1/g')
		
		init_subtraction=$temp1

		if [[ $init_subtraction -gt 8 ]]; then
			let "init_subtraction=8"
		fi

		let "init_points=$init_points-$init_subtraction"
	fi
fi

# Lookup
if [ "$(echo "$test_output" | grep "Lookup Succeeded")" == "" ]; then
	number_bad=$(echo "$test_output" | grep "Faled Lookup")

	if [ "$number_bad" != "" ]; then
		temp2=$(echo "$test_output" | sed 's/.*Failed Lookup[ ]\([0-9]*\).*/\1/g')

		lookup_subtraction=$temp2

		if [[ $lookup_subtraction -gt 8 ]]; then
			let "lookup_subtraction=8"
		fi

		let "lookup_points=$lookup_points-$lookup_subtraction"
	fi
fi

# Insert
if [ "$(echo "$test_output" | grep "Insert Succeeded")" == "" ]; then
	number_bad=$(echo "$test_output" | grep "Failed Insert")
	
	if [ "$number_bad" != "" ]; then
		temp3=$(echo "$test_output" | sed 's/.*Failed Insert[ ]\([0-9]*\).*/\1/g')

		insert_subtraction=$temp3
		
		if [[ $insert_subtraction -gt 16 ]]; then
			let "insert_subtraction=16"
		fi
	
	
		let "insert_points-$insert_points-$insert_subtraction"
	fi

fi

# Tier Setup (Pairs with init)
if [ "$(echo "$test_output" | grep "Tier Setup Succeeded")" == "" ]; then
	temp4=$(echo "$test_lookup" | sed 's/.*Failed Tier Setup[ ]\([0-9]*\).*/\1/g')
	
	tier_subtraction=$temp4
	
	if [[ $tier_subtraction -gt 0 ]]; then
		let "init_points=0"
	fi
fi

# Delete
if [ "$(echo "$test_output" | grep "Delete Succeeded")" == "" ]; then
	temp5=$(echo "$test_delete" | sed 's/.*Failed Delete[ ]\([0-9]*\).*/\1/g')

	delete_subtraction=$temp5

	if [[ $delete_subtraction -gt 16 ]]; then
		let "delete_subtraction=16"
	fi
	
	if [[ $delete_subtraction -gt 0 ]]; then
		let "delete_points=$delete_points-$delete_subtraction"
	fi
fi

# Destroy
if [ "$(echo "$test_output" | grep "Destroy Succeeded")" == "" ]; then
	temp6=$(echo "$test_destroy" | sed 's/.*Failed Destroy[ ]\([0-9]*\).*/\1/g')

	destroy_subtraction=$temp6
	
	if [[ $destroy_subtraction -gt 6 ]]; then
		let "destroy_subtraction=6"
	fi
	
	if [[ $destroy_subtraction -gt 0 ]]; then
		let "destroy_points=$destroy_points-$destroy_subtraction"
	fi
fi


# Testing Bonus

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
#include <climits>

/* --------------------------------------------------------------------------- */
/* User Defined Includes                                                       */
/* --------------------------------------------------------------------------- */

#include \"linked_list2.h\"


int main(int argc, char ** argv) 
{
	/* Variable declarations */
	linked_list2 linkedList2;
	int block_size = 128;
	int mem_size = block_size * 128;
	int tiers = 16;	

	/*Initialize the list using the given parameters*/
	linkedList2.Init(mem_size, block_size, tiers);
	int total_num_blocks = mem_size / block_size;
	
	/* 
	 * If the list looses track of data when delete is called, then
	 * the list should eventually run out of free data.  If it doesn't
	 * run out, then the bonus portion should be working correctly.  
	 */
	bool failure = false;
	for(int i=0; i<2 * total_num_blocks; i++)
	{
		linkedList2.Insert(0, \"Hello World!\", 12);
		node* test_node = linkedList2.Lookup(0);
		if(test_node == NULL)
		{
			failure = true;
			break;
		}
		linkedList2.Delete(0);
		
		test_node = linkedList2.Lookup(0);
		if(test_node != NULL)
		{
			failure = true;
			break;
		}
	}

	if(failure == false)
	{
		std::cout << \"Bonus Succeeded\" << std::endl;
	}
}
" > main.cpp


 
make &> /dev/null

if [ "$?" != 0 ]; then
	let "bonus_points=0"
else
	program_output=$(./List_2)

	test_bonus=$(echo $program_output | grep "Bonus Succeeded")
	if [ "$test_bonus" == "" ]; then
		let "bonus_points=0"
	fi
fi

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

# Bonus
if [ "$bonus_points" == "10" ]; then
	echo "Testing Bonus     ...   Passed [10/10]" >> $vocareumReportFile
	echo "Bonus,10" >> $vocareumGradeFile
fi

if [ "$bonus_points" == "0" ]; then
	echo "Testing Bonus     ...   Failed [0/10]" >> $vocareumReportFile
	echo "Bonus,0" >> $vocareumGradeFile
fi

# Sub back in the existing main file so that it isn't overwritten
mv saved_main.cpp main.cpp

exit 0
