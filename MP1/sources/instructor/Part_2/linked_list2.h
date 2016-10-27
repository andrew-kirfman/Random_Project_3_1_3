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

struct node 
{
    node* next;
    int key, value_len;
};

class linked_list2
{
private:
    char **head_pointer = NULL; //Always points to the front of the data
    node **front_pointer;       //Always points to the front of the initialized list  
    node **free_pointer;        //Always points to the last initialized node in the list
    node **free_data_pointer;   //Always points to the head of a list of nodes which are not used
    int block_size = 0;
    int mem_size = 0;
    int max_data_size = 0;
    int num_tiers = 0;
    bool initialized = false;


public:
void Init(int M, int b, int t); // initializes the linked list, should be called once from the main
void Destroy(); 		 //	destroys the linked list and cleans resources
void Insert(int key, char * value_ptr, int value_len); // inserts the key and copies the value to the payload
int Delete(int key); 	 // delete the whole block containing that particular key. When multiple entries with the same key, delete only the first one	
int Find_tier(int key); // determines the tier to which the given key belongs
node* Lookup(int key);	 //	Looks up the first item with the given and returns a pointer to the value portion (the value length and the actual value) 
							//(the user can read or modify after obtaining the pointer)
void PrintList();		// prints the entire list by following the next pointers. Print the keys and the length	of the value
int getBlockSize() { return block_size; }
};

std::string print_usage();

#endif 
