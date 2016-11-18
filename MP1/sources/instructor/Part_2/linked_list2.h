/* --------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman, Margaret Baxter                                  */
/* Project: CSCE-313 Machine Problem #1                                        */
/*                                                                             */
/* File: ./MP1/main.cpp                                                        */
/* --------------------------------------------------------------------------- */

#ifndef _linked_list_2_h_
#define _linked_list_2_h_

/* --------------------------------------------------------------------------- */
/* Standard Library Includes                                                   */
/* --------------------------------------------------------------------------- */

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>



/* IMPORTANT NOTE!!!
 * 
 * 
 * 
 * 
 * 
 * 
 * This sample code provided to you includes templates of functions which 
 * count as extra credit in this assignment (namely, keeping track of free
 * nodes).  If you decide not to implement the extra credit portion of the 
 * assignment, be sure to delete those functions from your final submitted
 * code.  
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 */ 




struct node 
{
    node* next;
    int key, value_len;
};

class linked_list2
{
private:
	/* Always points to the front of the data */
    char **head_pointer;
    
    /* Always points to the front of the initialized list */
    node **front_pointer;
    
    /* Always points to the last initialized node in the list */
    node **free_pointer;
    
    /* Always points to the head of a list of nodes which are not used */
    node **free_data_pointer;
    
    /* Other data values */
    int block_size;
    int mem_size;
    int max_data_size;
    int num_tiers;
    bool initialized;

public:
	/* Constructor */
	linked_list2();
	
	/* Initializes the linked list.  Should only be called once */
	void Init(int M, int b, int t);
	
	/* Destroys the linked list and frees all used resources */
	void Destroy();
	
	/* Inserts the key and copies the value to the payload */
	void Insert(int key, char * value_ptr, int value_len);
	
	/* Delete the whole block containing that particular key                        */
	/* When there are multiple entries with the same key, delete only the first one */
	int Delete(int key);
	
	/* Determines the tier to which the given key belongs */	
	int Find_tier(int key);
	
	/* Looks up the first item with the given key and returns a pointer to the header structure */
	node* Lookup(int key);
	
	/* Prints the entire list by following the next pointers */
	void PrintList();
	
	/* Getter Functions */
	char** getHeadPointer();
	node** getFrontPointer();
	node** getFreePointer();
	node** getFreeDataPointer();
	
	int getBlockSize();
	int getMemSize();
	int getMaxDataSize();
	int getNumTiers();
	int getInitialized();

	/* Setter Functions */
	void setHeadPointer(char** new_head_pointer);
	void setFrontPointer(node** new_front_pointer);
	void setFreePointer(node** new_free_pointer);
	void setFreeDataPointer(node** new_free_data_pointer);
	
	void setBlockSize(int new_block_size);
	void setMemSize(int new_mem_size);
	void setMaxDataSize(int new_max_data_size);
	void setNumTiers(int new_num_tiers);
	void setInitialized(bool new_initialized);
};

std::string print_usage();

#endif 
