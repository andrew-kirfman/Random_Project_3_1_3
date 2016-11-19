/* --------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman, Margaret Baxter                                  */
/* Project: CSCE-313 Machine Problem #1                                        */
/*                                                                             */
/* File: ./MP1/linked_list.h                                                   */
/* --------------------------------------------------------------------------- */


// Note: If you've never seen this construct before, it's called an include
// guard.  It prevents your header file from being defined more than once
#ifndef _linked_list_h_
#define _linked_list_h_

/* --------------------------------------------------------------------------- */
/* Standard Library Includes                                                   */
/* --------------------------------------------------------------------------- */

#include <iostream>
#include <string>
#include <climits>
#include <cstring>



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

class linked_list
{
private:
	/* Always should point to the front of the data block allocated during Init() */
    char *head_pointer;     
    
    /* Always points to the front of the initialized list */
    node *front_pointer;
    
    /* Always points to the last initialized node in the list */
    node *free_pointer;
    
    /* Always points to the head of a list of nodes which are not used */ 
    node *free_data_pointer;
    
    int block_size;
    int mem_size;
    int max_data_size;
    bool initialized;

public:
	/* Constructor */
	linked_list();

	/* Initialize the linked list and allocate memory */
	/* Should only be called once from main           */
    void Init(int M, int b);
    
    /* Destroys the linked list and frees all used resources */
    void Destroy(); 		 
    
    /* Inserts thea key and copies the value to the payload */
    void Insert(int key,char * value_ptr, int value_len);
    
    /* Delete the whole block containing a particular key                 */
    /* When multiple entries have the same key, delete only the first one */
    int Delete(int key);
    
    /* Looks up the first item with a given key, and returns a pointer to the value portion */
    /* The user is able to read or modify the value after obtaining the pointer             */
    node* Lookup (int key);
    
    /* Prints the entire list by following the next pointers.             */
    void PrintList ();
    
    /* Getter Functions */
    char *getHeadPointer();
    node *getFrontPointer();
    node *getFreePointer();
    node *getFreeDataPointer();
    
    int getBlockSize();
    int getMemSize();
    int getMaxDataSize();
    bool getInitialized();
    
    /* Setter Functions */
    void setHeadPointer(char* new_pointer);
    void setFrontPointer(node* new_pointer);
    void setFreePointer(node* new_pointer);
    void setFreeDataPointer(node* new_pointer);
    
    void setBlockSize(int new_block_size);
    void setMemSize(int new_mem_size);
    void setMaxDataSize(int new_max_data_size);
    void setInitialized(bool new_initialized);
};

#endif
