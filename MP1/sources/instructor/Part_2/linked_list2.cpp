/* --------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman, Margaret Baxter                                  */
/* Project: CSCE-313 Machine Problem #1                                        */
/*                                                                             */
/* File: ./MP1/linked_list2.cpp                                                */
/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/* Standard Library Includes                                                   */
/* --------------------------------------------------------------------------- */

#include <iostream>
#include <string.h>
#include <climits>

/* --------------------------------------------------------------------------- */
/* User Defined Includes                                                       */
/* --------------------------------------------------------------------------- */

#include "linked_list2.h"

linked_list2::linked_list2()
{
	head_pointer = NULL;
	front_pointer = NULL;
	free_pointer = NULL;
	free_data_pointer = NULL;
	
	block_size = 0;
	mem_size = 0;
	max_data_size = 0;
	num_tiers = 0;
	initialized = false;
}

void linked_list2::Init(int M, int b, int t)
{
    /* If the list has already been initialized, don't do anything */
    if (initialized == true)
    {
        std::cout << "The list has already been initialized!" << std::endl;
        return;
    }

    /* User can't define a block size bigger than the total amount of memory */
    if (M < b)
    {
        do
        {
            int new_memory_size = M;

            std::cout << std::string("Memory reserved can not be less than block size!\n")
                  + std::string("Memory size given: ") + std::to_string(M) + '\n'
                  + std::string("Block size given:  ") + std::to_string(b) + '\n'
                  + std::string("Please enter a new value for memory: ");
            while (!(std::cin >> new_memory_size) || new_memory_size < b)
            {
                std::cout << "Bad input - try again: ";
                std::cin.clear();
                std::cin.ignore(INT_MAX, '\n');

                if (new_memory_size >= b)
                {
                    break;
                }
            }
            M = new_memory_size;
        }
        while(M < b);
    }

    /* The block size can't be less than the size of a node item */
    else if (b < sizeof(node))
    {
        do
        {
            int new_block_size = b;

            std::cout << std::string("Block size is too small!\n")
                  + std::string("Minimum block size: ") + std::to_string(sizeof(node)) + '\n'
                  + std::string("Block size given:  ") + std::to_string(b) + '\n'
                  + std::string("Please enter a new value for block_size: ");

            while (!(std::cin >> new_block_size) || new_block_size < sizeof(node))
            {
                std::cout << "Bad input - try again: ";
                std::cin.clear();
                std::cin.ignore(INT_MAX, '\n');

                if (new_block_size >= sizeof(node))
                {
                    break;
                }
            }
            b = new_block_size;
        }
        while(b < sizeof(node));
    }

    /* Save mem_size and block_size to linked_list class */
    mem_size = M;
    block_size = b;
    num_tiers = t;

    /* Only allocate a number of blocks (per tier) that will fit inside of the memory that the user specified */
    int number_of_blocks = M / b;

    /* Save maximum data size */
    max_data_size = (block_size - sizeof(node));

    /* Make the list of pointers to tiers */
    char* pointer_size = NULL;
    head_pointer = (char**) malloc(t*sizeof(pointer_size));

    /* Allocate memory and assign all values */
    for(int i = 0; i < t; i++)
    {
        head_pointer[i] = (char*) malloc(block_size*number_of_blocks);
    }

    char** tier_pointer = head_pointer;

    /* Initialize the nodes */
    for(int i = 0; i < t; i++)
    {
        /* Set a pointer to head_pointer for momentary use in the initialization */
        char* char_iterator = tier_pointer[i];
        node* node_iterator = reinterpret_cast<node*>(head_pointer[i]);

        /* Initialize the nodes */
        /* Use the two pointers to set next pointer for nodes */
        for(int i = 0; i < number_of_blocks; i++)
        {
            /* Set the data in nodes to 'empty' values */
            node_iterator->value_len = -1;
            node_iterator->key = -1;

            /* Initialize all the nodes */
            if(i == (number_of_blocks - 1))
            {
               /* This is the last element, set it's pointer to null */
                node_iterator->next = NULL;
            }
            else
            {
                char_iterator += block_size;

                /* Catch node pointer up to movement in allocated memory */
                node_iterator->next = reinterpret_cast<node*>(char_iterator);

                /* Move free_pointer to the next node as specified earlier */
                node_iterator = node_iterator->next;
            }
        }
    }
    free_data_pointer = new node*[t];
    free_pointer = new node*[t];
    front_pointer = new node*[t];

    /* Set free_data_pointer equal head_pointer, as the list is empty */
    /* At this point, free_data_pointer points to the whole list in every tier */
    for(int i = 0; i < t; i++)
    {
        free_data_pointer[i] = reinterpret_cast<node*>(head_pointer[i]);
        free_pointer[i] = NULL;
        front_pointer[i] = NULL;
    }

    /* Set the initialized flag in the linked_list class. */
    initialized = true;
}


void linked_list2::Destroy()
{
    /* If the list has already been destroyed, don't do anything */
    if (initialized == false)
    {
        std::cout << "There is no list here to destroy!" << std::endl;
        return;
    }

    /* Iteratively delete the entire list */
    for(int i = 0; i < num_tiers; i++)
    {
        free(head_pointer[i]);
        head_pointer[i] = NULL;
        front_pointer[i] = NULL;
        free_pointer[i] = NULL;
        free_data_pointer[i] = NULL;
    }

	block_size = 0;
	mem_size = 0;
	max_data_size = 0;
	num_tiers = 0;

    free(head_pointer);
    head_pointer = NULL;
    initialized = false;
}

void linked_list2::Insert(int k,char * data_ptr, int data_len)
{
    try
    {
        /* Check that key is not negative */
        if (k < 0)
        {
            throw 4;
        }

        /* Check for duplicate keys */
        if (Lookup(k) != NULL)
        {
            throw 1;
        }

        /* Check that there is enough size in the next empty block */
        if (data_len > block_size - sizeof(node))
        {
            throw 2;
        }

        /* Find the tier in which the key belongs */
        int tier = Find_tier(k);

        /* If there is an empty block */
        if(free_data_pointer[tier] != NULL)
        {
            /* Insert data values */
            free_data_pointer[tier]->key = k;
            free_data_pointer[tier]->value_len = data_len;

            /* Offset the location where data is to be copied by the size of the given node */
            char * data_pointer = reinterpret_cast<char*>(free_data_pointer[tier]);
			memcpy(data_pointer + sizeof(node), data_ptr, data_len);

			/* There are two possible insert cases                                     */
            /*  - Case #1: list empty, all nodes in free_data_pointer list             */
            /*  - Case #2: free_data_pointer and free_pointer point to different lists */
            /* Connect node from free_data_pointer to the end of the linked list       */
            if (front_pointer[tier] == NULL)    //The list was empty
            {
                /* Since the list is empty, free pointer was NULL */
                front_pointer[tier] = free_data_pointer[tier];
                free_pointer[tier] = front_pointer[tier];

                /* Move on free_data_pointer */
                if (free_data_pointer[tier]->next != NULL)
                {
                    free_data_pointer[tier] = free_data_pointer[tier]->next;
                }
                else
                {
                    free_data_pointer[tier] = NULL;
                }
                free_pointer[tier]->next = NULL;
            }
            else
            {
                /* Move free_pointer and free_data_pointer and connect inserted node to the linked list */
                free_pointer[tier]->next = free_data_pointer[tier];             //Connect the previous last node to the inserted node
                free_pointer[tier] = free_pointer[tier]->next;                  //Reference the inserted node as the last node in the list

                if (free_data_pointer[tier]->next != NULL)
                {
                    /* The free data list has at least one item left after the insert */
                    free_data_pointer[tier] = free_data_pointer[tier]->next;        //Move the free_data_pointer to new head of empty list
                }
                else
                {
                    /* The free data list on this tier is now empty */
                    free_data_pointer[tier] = NULL;
                }
                free_pointer[tier]->next = NULL;
            }
        }
        else
        {
            throw 3;
        }
    }
    catch(int error_code)
    {
        if (error_code == 1)
        {
            std::cout << "An element with this key (" << k << ") already exists." << std::endl;
            return;
        }
        else if (error_code == 2)
        {
            std::cout << "The data trying to be stored is too large: " << data_len << std::endl;
            return;
        }
        else if (error_code == 3)
        {
            std::cout << "There is no free space left in the list." << std::endl;
            return;
        }
        else if (error_code == 4)
        {
            std::cout << "The key cannot be negative" << std::endl;
            return;
        }
        else
        {
            throw;
        }
    }
    catch(std::exception& e)
    {
        std::cout << "Insert Error: " << e.what() << std::endl;
        return;
    }
}

int linked_list2::Delete(int delete_key)
{
    try
    {
        /* Find the tier that the delete_key corresponds to */
        int tier = Find_tier(delete_key);

        /* If the list on this tier is not already empty */
        if (front_pointer[tier] != NULL)
        {
            node* delete_node = Lookup(delete_key);

            /* Check that the key exists */
            if (delete_node == NULL)
            {
                throw 1;
            }

            /* Set the data to all \0s */
            char * delete_position = reinterpret_cast<char*>(delete_node);
            memset(delete_position + sizeof(node), '\0', (block_size - sizeof(node)));

            /* Delete the values in node */
            delete_node->key = 0;
            delete_node->value_len = -1;

            /* Check if delete node is the only node in the list or if it is the front node*/
            if (delete_node == front_pointer[tier])
            {
                if (delete_node->next != NULL)
                {
                    /* There are more nodes after delete_node */
                    front_pointer[tier] = delete_node->next;
                }
                else
                {
                    /* delete_node is the last node in the list */
                    front_pointer[tier] = NULL;
                    free_pointer[tier] = NULL;
                }
            }
            else
            {
                /* Iterate through list to find the node that points to delete node */
                struct node* prev_node = front_pointer[tier];

                /* Find the node before delete_node */
                while(prev_node->next != delete_node)
                {
                    prev_node = prev_node->next;
                }

                if (delete_node->next != NULL)
                {
                    /* There are more nodes after delete_node */
                    prev_node->next = delete_node->next;            //Skip over delete_node by setting prev_node's pointer to the next element in list
                }
                else
                {
                    /* delete_node is the last node in the list */
                    prev_node->next = NULL;
                }
            }

            /* Add delete_node to the free_data list */
            delete_node->next = free_data_pointer[tier];
            free_data_pointer[tier] = delete_node;
        }
        else
        {
            std::cout << "The list is currently empty." << std::endl;
        }
    }
    catch(int error_code)
    {
        if (error_code == 1)
        {
            std::cout << "A node with the key to be deleted: " << delete_key << " does not exist." << std::endl;
            return 0;
        }
        else
        {
            throw;
        }
    }
    catch(std::exception& e)
    {
        std::cout << "Deletion error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}

node* linked_list2::Lookup(int lookup_key)
{
    /* Find the tier that the delete_key corresponds to */
    int tier = Find_tier(lookup_key);

    node* iteration_pointer = front_pointer[tier];

    /* If the list is empty, we don't need to do anything */
    if (iteration_pointer == NULL)
    {
        return NULL;
    }

    /* If there is stuff in the list, iterate until we reach the end or find the node we want */
    while(true)
    {
        if (iteration_pointer->key == lookup_key)
        {
            return iteration_pointer;
        }
        if (iteration_pointer->next == NULL)
        {
            return NULL;
        }
        iteration_pointer = iteration_pointer->next;
    }
    return NULL;
}

void linked_list2::PrintList ()
{
    std::cout << "Contents of the linked list:" << std::endl;

    for (int i = 0; i < num_tiers; i++)
    {
        node *iteration_pointer = front_pointer[i];

        if (iteration_pointer != NULL)
        {
            std::cout << "Tier " << i << std::endl;
            while(true)
            {
                std::cout << "Node: " << std::endl;
                std::cout << " - Key: " << iteration_pointer->key << std::endl;
                std::cout << " - Data: ";
                char * data_pointer = reinterpret_cast<char*>(iteration_pointer);
                data_pointer = data_pointer + sizeof(node);
                for (int i = 0; i < iteration_pointer->value_len; i++)
                {
                    std::cout << *(data_pointer);
                    data_pointer += 1;
                }
                std::cout << std::endl;
                if(iteration_pointer->next == NULL)
                {
                    break;
                }
                iteration_pointer = iteration_pointer->next;
            }
        }
    }
}

int linked_list2::Find_tier(int key)
{
	// Use integer division to find the approximate tier size.   
	// This won't be exact because depending on the number
	// of tiers, there will be a few numbers left over at the
	// very end of the integer range.  
	// 
	// For this problem, incorporate those few potentially extra
	// left over numbers into the nth tier (i.e. the one that goes
	// from [(n - 1) * tier_size, INT_MAX)).  
    int tier_size = INT_MAX/num_tiers;

    for (int i = 1; i < num_tiers + 1; i++)
    {
		if(i == num_tiers)
		{
			std::cout << "Hello World!" << std::endl;
			return num_tiers - 1;
		}
		
        if(key < tier_size * i)
        {
			std::cout << "Hello World! " << std::to_string(i) << std::endl;
            return (i - 1);
        }
    }
    return -1;
}

/* Getter Functions */
char** linked_list2::getHeadPointer()
{
	return head_pointer;
}

node** linked_list2::getFrontPointer()
{
	return front_pointer;
}

node** linked_list2::getFreePointer()
{
	return free_pointer;
}

node** linked_list2::getFreeDataPointer()
{
	return free_data_pointer;
}

int linked_list2::getBlockSize()
{
	return block_size;
}

int linked_list2::getMemSize()
{
	return mem_size;
}

int linked_list2::getMaxDataSize()
{
	return max_data_size;
}

int linked_list2::getNumTiers()
{
	return num_tiers;
}

int linked_list2::getInitialized()
{
	return initialized;
}

/* Setter Functions */
void linked_list2::setHeadPointer(char** new_head_pointer)
{
	head_pointer = new_head_pointer;
}

void linked_list2::setFrontPointer(node** new_front_pointer)
{
	front_pointer = new_front_pointer;
}

void linked_list2::setFreePointer(node** new_free_pointer)
{
	free_pointer = new_free_pointer;
}

void linked_list2::setFreeDataPointer(node** new_free_data_pointer)
{
	free_data_pointer = new_free_data_pointer;
}

void linked_list2::setBlockSize(int new_block_size)
{
	block_size = new_block_size;
}

void linked_list2::setMemSize(int new_mem_size)
{
	mem_size = new_mem_size;
}

void linked_list2::setMaxDataSize(int new_max_data_size)
{
	max_data_size = new_max_data_size;
}

void linked_list2::setNumTiers(int new_num_tiers)
{
	num_tiers = new_num_tiers;
}

void linked_list2::setInitialized(bool new_initialized)
{
	initialized = new_initialized;
}

/* Prints a pretty string corresponding to the command line usage of the program */
std::string print_usage()
{
    std::string output_string = "";
    output_string =  std::string("Usage: ./List_1 [OPTIONS]\n")
                  +  std::string("Creates a linked_list and performs some operations on it\n")
                  +  std::string("as defined in the main.cpp file.  \n\n")
                  +  std::string("Arguments:\n")
                  +  std::string("  -s [INT]        Value defining the block size of the list items in Bytes.\n")
                  +  std::string("                  Minimum block size is 16 bytes.  Default = 128 Bytes.  \n")
                  +  std::string("  -b [INT]        Value defining the total amount of memory to reserve\n")
                  +  std::string("                  for the list in bytes.  Default = 512000 Bytes.\n\n")
                  +  std::string("Exit string: \n")
                  +  std::string("  0  if Ok!\n")
                  +  std::string("  1  if error constructing the list\n")
                  +  std::string("  2  if error issuing a command to the list\n");
    return output_string;
}
