
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
/* File: ./MP1/Part_1/main.cpp                                                 */
/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/* Standard Library Includes                                                   */
/* --------------------------------------------------------------------------- */

#include <iostream>

/* --------------------------------------------------------------------------- */
/* User Defined Includes                                                       */
/* --------------------------------------------------------------------------- */

#include \"linked_list.h\"

int main(int argc, char **argv) 
{
	int b = 128;
	int M = b * 11;

    /* --------------------------------------------------------------------------- */
    /* Test Class Constructor                                                      */
    /* --------------------------------------------------------------------------- */

    linked_list* test_list = new linked_list();

	char* pre_head_pointer = test_list->getHeadPointer();
	node* pre_front_pointer = test_list->getFrontPointer();
	node* pre_free_pointer = test_list->getFreePointer();

	int pre_block_size = test_list->getBlockSize();
	int pre_mem_size = test_list->getMemSize();
	int pre_max_data_size = test_list->getMaxDataSize();
	int pre_initialized = test_list->getInitialized();

	int unset_values = 0;

	if(pre_head_pointer != NULL)
	{
		unset_values += 1;
	}

	if(pre_front_pointer != NULL)
	{
		unset_values += 1;
	}

	if(pre_free_pointer != NULL)
	{
		unset_values += 1;
	}

	if(pre_block_size != 0)
	{
		unset_values += 1;
	}

	if(pre_mem_size != 0)
	{
		unset_values += 1;
	}

	if(pre_max_data_size != 0)
	{
		unset_values += 1;
	}

	if(pre_initialized != false)
	{
		unset_values += 1;
	}

	if(unset_values == 0)
	{
		std::cout << \"Class Constructor Succeeded \" << std::endl;
	}

    /* --------------------------------------------------------------------------- */
    /* Test Init                                                                   */
    /* --------------------------------------------------------------------------- */

	test_list->Init(M, b); 

	int bad_init_values = 0;

	if(test_list->getHeadPointer() == NULL 
		|| test_list->getHeadPointer() == pre_head_pointer)
	{
		bad_init_values += 1;
	}

	if(test_list->getFrontPointer() != NULL)
	{
		bad_init_values += 1;
	}

	if(test_list->getFreePointer() != NULL)
	{
		bad_init_values += 1;
	}

	if(test_list->getBlockSize() == 0 || test_list->getBlockSize() == pre_block_size)
	{
		bad_init_values += 1;
	}

	if(test_list->getMemSize() == 0 || test_list->getMemSize() == pre_mem_size)
	{
		bad_init_values += 1;
	}

	if(test_list->getMaxDataSize() == 0 
		|| test_list->getMaxDataSize() == pre_max_data_size)
	{
		bad_init_values += 1;
	}

	if(test_list->getInitialized() == false 
		|| test_list->getInitialized() == pre_initialized)
	{
		bad_init_values += 1;
	}

	if(bad_init_values > 0)
	{
		std::cout << \"Bad Init Values \" << bad_init_values << std::endl;
	}

	// M = 128 * 11, b = 128; M / b = 11
	int number_of_blocks = M / b;
	node *init_node = (node*) test_list->getHeadPointer();
	int init_blocks_visited = 0;

	for(int i=0; i<number_of_blocks; i++)
    {
       	if(init_node == NULL)
        {
        	break;                            
        }
                                     
  		init_node = init_node->next;
		init_blocks_visited += 1;
    }

	if(init_blocks_visited == number_of_blocks)
	{
		std::cout << \"Init Succeeded\" << std::endl;
	}

    /* --------------------------------------------------------------------------- */
    /* Test Insert                                                                 */
    /* --------------------------------------------------------------------------- */

	for(int i=0; i<number_of_blocks; i++)
    {
    	test_list->Insert(i, \"Hello World!\", 12);                            
    }

	node* lookup_node = test_list->Lookup(0);

	int blocks_iterated = 0;

	if(lookup_node == NULL)
	{
		std::cout << \"Lookup Failed\" << std::endl;
	}
	else
	{
		std::cout << \"Lookup Succeeded\" << std::endl;

		if(lookup_node->value_len != 12)
		{
			std::cout << \"Value Length Not Saved Correctly\" << std::endl;
		}
		else
		{
			std::cout << \"Value Length Saved Correctly\" << std::endl;
		}

		char *data_ptr = reinterpret_cast<char*>(lookup_node) + sizeof(node); 

		std::string data_value = \"\";

		for(int i=0; i < lookup_node->value_len; i++)
		{
			// Grab a character fron the value and add it to a string
			data_value = data_value + *(data_ptr);
			
			// Move onto the next character
			data_ptr += 1;
		}

		if(data_value == \"Hello World!\")
		{
			std::cout << \"Value Saved Correctly\" << std::endl;
		}
		else
		{
			std::cout << \"Value Not Saved Correctly\" << std::endl;
		}

		blocks_iterated += 1;
		lookup_node = lookup_node->next;

		for(int i=0; i<number_of_blocks - 1; i++)
        {
        	if(lookup_node == NULL)
			{
                break;
			}

			if(lookup_node->value_len != 12)
			{
				break;
			}

			char *data_ptr = reinterpret_cast<char*>(lookup_node) + sizeof(node); 

			std::string data_value = \"\";

			for(int i=0; i < lookup_node->value_len; i++)
			{
				// Grab a character fron the value and add it to a string
				data_value = data_value + *(data_ptr);
			
				// Move onto the next character
				data_ptr += 1;
			}

			if(data_value != \"Hello World!\")
			{
				break;
			}                         
                                         
        	blocks_iterated += 1;
			lookup_node = lookup_node->next;
		}

		if(blocks_iterated == number_of_blocks)
		{
			std::cout << \"List Set Up Correctly\" << std::endl;
		}
	}

    /* --------------------------------------------------------------------------- */
    /* Test Delete                                                                 */
    /* --------------------------------------------------------------------------- */

	// Attempt to delete the entire list
	for(int i=0; i<number_of_blocks; i++)
    {
    	test_list->Delete(i);                              
   	}
	
	// Attempt to perform lookups on the entire list to make sure that everything is gone. 
	bool delete_succeeded = true;
	for(int i=0; i<number_of_blocks; i++)
    {
    	node* test_lookup = test_list->Lookup(i);
		
		if(test_lookup != NULL)
		{
			std::cout << \"Delete Failed\" << std::endl;
			delete_succeeded = false;
			break;
		}
    }

	if(delete_succeeded == true)
	{
		std::cout << \"Delete Succeeded\" << std::endl;
	}
}
" > main.cpp

# Run the testing main function and capture output
test_output=""

# First, see if the code compiles
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

test_output=$(./List_1)

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

# If the constructor doesn't work correctly, deduct 6 points from Init score
if [ "$(echo "$test_output" | grep "Class Constructor Succeeded")" == "" ]; then
	let "init_points=$init_points-6"
fi

if [ "$(echo "$test_output" | grep "Init Succeeded")" == "" ]; then
	let "init_points=$init_points-10"
fi

if [ "$(echo "$test_output" | grep "Lookup Succeeded")" == "" ]; then
	let "lookup_points=0"
fi

if [ "$(echo "$test_output" | grep "Value Length Saved Correctly")" == "" ]; then
	let "insert_points=$insert_points-4"
fi

if [ "$(echo "$test_output" | grep "Value Saved Correctly")" == "" ]; then
	let "insert_points=$insert_points-4"
fi

if [ "$(echo "$test_output" | grep "List Set Up Correctly")" == "" ]; then
	let "insert_points=$insert_points-8"
fi

if [ "$(echo "$test_output" | grep "Delete Succeeded")" == "" ]; then
	let "delete_points=$delete_points-16"
fi


# Still need to do destroy and print list

# ---------------------------------------------------------------------------- #
# Test PrintList                                                               #
# ---------------------------------------------------------------------------- #

echo "
/* --------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman, Margaret Baxter                                  */
/* Project: CSCE-313 Machine Problem #1                                        */
/*                                                                             */
/* File: ./MP1/Part_1/main.cpp                                                 */
/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/* Standard Library Includes                                                   */
/* --------------------------------------------------------------------------- */

#include <iostream>

/* --------------------------------------------------------------------------- */
/* User Defined Includes                                                       */
/* --------------------------------------------------------------------------- */

#include \"linked_list.h\"

int main(int argc, char **argv) 
{
    /* --------------------------------------------------------------------------- */
    /* Test Class Constructor                                                      */
    /* --------------------------------------------------------------------------- */

    linked_list* test_list = new linked_list();

	test_list->Init(128 * 11, 128);

    test_list->Insert(0, \"Hello World!\", 12);  
    test_list->Insert(1, \"Hello World!\", 12);  
    test_list->Insert(2, \"Hello World!\", 12); 
    test_list->Insert(3, \"Hello World!\", 12); 
    test_list->Insert(4, \"Hello World!\", 12); 

	test_list->PrintList();

	return 0;
}
" > main.cpp

make &> /dev/null

if [ "$?" != "0" ]; then
	echo "Cannot Execute Test PrintList ./List_1" >> $vocareumReportFile

	let "printlist_points=0"
else
    print_list_output="$(./List_1)"
    
    if [ "$print_list_output" == "" ]; then
        let "printlist_points=0"
    else
        let "printlist_points=8"
    fi
fi

# ---------------------------------------------------------------------------- #
# Test Destroy                                                                 #
# ---------------------------------------------------------------------------- #

echo "
/* --------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman, Margaret Baxter                                  */
/* Project: CSCE-313 Machine Problem #1                                        */
/*                                                                             */
/* File: ./MP1/Part_1/main.cpp                                                 */
/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/* User Defined Includes                                                       */
/* --------------------------------------------------------------------------- */

#include \"linked_list.h\"

int main(int argc, char ** argv) 
{
	int b = 128;
	int M = b * 11;

    /* --------------------------------------------------------------------------- */
    /* Test Class Constructor                                                      */
    /* --------------------------------------------------------------------------- */

    linked_list* test_list = new linked_list();

	test_list->Init(M,b);

    test_list->Insert(0, \"Hello World!\", 12);  
    test_list->Insert(1, \"Hello World!\", 12);  
    test_list->Insert(2, \"Hello World!\", 12); 
    test_list->Insert(3, \"Hello World!\", 12); 
    test_list->Insert(4, \"Hello World!\", 12); 

	test_list->Destroy();

	if(test_list->getHeadPointer() != NULL
		|| test_list->getFrontPointer() != NULL
        || test_list->getFreePointer() != NULL)
	{
		return 1;
	}

	return 0;
}
" > main.cpp

make &> /dev/null

if [ "$?" != "0" ]; then
	echo "Cannot Execute Test Destroy ./List_1" >> $vocareumReportFile

	let "destroy_points=0"
else
    ./List_1 > /dev/null
    
    if [ "$?" == 0 ]; then
        let "destroy_points=6"
    else
        let "destroy_points=0"
    fi
fi

# ---------------------------------------------------------------------------- #
# Advanced Tests                                                               #
# ---------------------------------------------------------------------------- #

echo "
/* --------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman, Margaret Baxter                                  */
/* Project: CSCE-313 Machine Problem #1                                        */
/*                                                                             */
/* File: ./MP1/Part_1/main.cpp                                                 */
/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/* Standard Library Includes                                                   */
/* --------------------------------------------------------------------------- */

#include<iostream>

/* --------------------------------------------------------------------------- */
/* User Defined Includes                                                       */
/* --------------------------------------------------------------------------- */

#include \"linked_list.h\"

int main(int argc, char ** argv) 
{
	int b = 128;
	int M = b * 11;

    /* --------------------------------------------------------------------------- */
    /* Test Class Constructor                                                      */
    /* --------------------------------------------------------------------------- */

    linked_list* test_list = new linked_list();

	test_list->Init(M, b);

    test_list->Insert(0, \"Hello World!\", 12);  
    test_list->Insert(1, \"Hello World!\", 12);  
    test_list->Insert(2, \"Hello World!\", 12); 
    test_list->Insert(3, \"Hello World!\", 12); 
    test_list->Insert(4, \"Hello World!\", 12); 

	// Delete an element from the middle of the list 
	test_list->Delete(2);

	if(test_list->Lookup(2) == (node*) NULL)
	{
		std::cout << \"Succeeded Delete \" << std::endl;
	}

	// Make sure the pointers are still set up correctly
	test_list->Delete(1);
	test_list->Delete(3);
	
	node *head_pointer = (node*) test_list->getHeadPointer();
	bool success = true;

	for(unsigned short int i=0; i<2; i++)
   	{
    	if(head_pointer == NULL)
        {
            success = false;
        	break;                             
        }
        
		head_pointer = head_pointer->next;
    }

	if(success == true)
	{
		std::cout << \"Succeeded Pointer Setup \" << std::endl;
	}

	test_list->Insert(1, \"Hello World!\", 12);
	test_list->Insert(2, \"Hello World!\", 12);
	test_list->Insert(3, \"Hello World!\", 12);
	test_list->Insert(5, \"Hello World!\", 12);
	test_list->Insert(6, \"Hello World!\", 12);

	success = true;
	head_pointer = (node*) test_list->getHeadPointer();

	for(unsigned short int i=0; i<6; i++)
    {
    	if(head_pointer == NULL)
        {
        	success = false;
            break;
        }
        
        head_pointer = head_pointer->next;
    }

	if(success == true)
	{
		std::cout << \"Succeeded Insert \" << std::endl;
	}
}
" > main.cpp

make &> /dev/null


if [ "$?" != "0" ]; then
	echo "Advanced Tests Compile Failed" >> $vocareumReportFile
	let "insert_points=$insert_points-8"
	let "delete_points=$delete_points-8"
else
	advanced_output=$(./List_1)

	test_delete_1=$(echo $advanced_output | grep "Succeeded Delete")
	test_delete_2=$(echo $advanced_output | grep "Succeeded Pointer Setup")
	test_insert_1=$(echo $advanced_output | grep "Succeeded Insert")

	if [ "$test_delete_1" == "" ]; then
		let "delete_points=delete_points-4"
	fi

	if [ "$test_delete_2" == "" ]; then
		let "delete_points=delete_points-4"
	fi

	if [ "$test_insert_1" == "" ]; then
		let "insert_points=insert_points-8"
	fi
fi


# ---------------------------------------------------------------------------- #
# Test Bonus                                                                   #
# ---------------------------------------------------------------------------- #

echo " 
/* --------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman, Margaret Baxter                                  */
/* Project: CSCE-313 Machine Problem #1                                        */
/*                                                                             */
/* File: ./MP1/Part_1/main.cpp                                                 */
/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/* Standard Library Includes                                                   */
/* --------------------------------------------------------------------------- */

#include <iostream>

/* --------------------------------------------------------------------------- */
/* User Defined Includes                                                       */
/* --------------------------------------------------------------------------- */

#include \"linked_list.h\"

int main(int argc, char ** argv) 
{
	int b = 128;
	int M = b * 11;

    /* --------------------------------------------------------------------------- */
    /* Test Class Constructor                                                      */
    /* --------------------------------------------------------------------------- */

    linked_list* test_list = new linked_list();

	test_list->Init(M, b);

	node* free_list = test_list->getFreeDataPointer();

	int num_blocks = 11;
	int blocks_visited = 0;

	while(free_list != NULL)
	{
		blocks_visited += 1;

		free_list = free_list->next;
	}

	if(num_blocks == blocks_visited)
	{
		std::cout << \"success_1 \" << std::endl;
	}

	test_list->Insert(0, \"Hello World!\", 12);

	free_list = test_list->getFreeDataPointer();

	blocks_visited = 0;

	while(free_list != NULL)
	{
		blocks_visited += 1;

		free_list = free_list->next;
	}

	if(blocks_visited == (num_blocks - 1))
	{
		std::cout << \"success_2 \" << std::endl;
	}

	test_list->Delete(0);

	free_list = test_list->getFreeDataPointer();

	blocks_visited = 0;

	while(free_list != NULL)
	{
		blocks_visited += 1;

		free_list = free_list->next;
	}

	if(blocks_visited == num_blocks)
	{
		std::cout << \"success_3 \" << std::endl;
	}

	return 0;
}" > main.cpp

make &> /dev/null

if [ "$?" != "0" ]; then
	echo "Testing Bonus     ...   Failed [0/10]" >> $vocareumReportFile
	echo "Bonus,0" >> $vocareumGradeFile
else
	bonus_output=$(./List_1)

	success_1=$(echo $bonus_output | grep "success_1")
	success_2=$(echo $bonus_output | grep "success_2")
	success_3=$(echo $bonus_output | grep "success_3")

	bonus_points=0

	if [ "$success_1" != "" ]; then
		let "bonus_points=$bonus_points+3"
	fi

	if [ "$success_2" != "" ]; then
		let "bonus_points=$bonus_points+3"
	fi

	if [ "$success_3" != "" ]; then
		let "bonus_points=$bonus_points+4"
	fi

	if [ "$bonus_points" == "10" ]; then
		echo "Testing Bonus     ...   Passed [0/10]" >> $vocareumReportFile
		echo "Bonus,10" >> $vocareumGradeFile		
	fi

	if [[ $bonus_points -gt 0 && $bonus_points -lt 10 ]]; then
		echo "Testing Bonus     ...   Partial Pass [$bonus_points/10]" >> $vocareumReportFile
		echo "Bonus,$bonus_points" >> $vocareumGradeFile
	fi

	if [ "$bonus_points" == "0" ]; then
		echo "Testing Bonus     ...   Partial Pass [0/10]" >> $vocareumReportFile
		echo "Bonus,0" >> $vocareumGradeFile
	fi
fi

# ---------------------------------------------------------------------------- #
# Final Grade Reporting                                                        #
# ---------------------------------------------------------------------------- #

# INIT - 16 points
if [ "$init_points" == "16" ]; then
	echo "Testing Init      ...   Passed [16/16]" >> $vocareumReportFile
	echo "Init,16" >> $vocareumGradeFile
fi

if [[ $init_points -gt 0 && $init_points -lt 16 ]]; then
	echo "Testing Init      ...   Partial Pass [$init_points/16]" >> $vocareumReportFile
	echo "Init,$init_points" >> $vocareumGradeFile
fi

if [ $init_points -le 0 ]; then
	echo "Testing Init      ...   Failed [0/16]" >> $vocareumReportFile
	echo "Init,$init_points" >> $vocareumGradeFile
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
