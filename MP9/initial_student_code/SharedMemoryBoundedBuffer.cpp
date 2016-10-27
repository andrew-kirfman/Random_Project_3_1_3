/*
	SharedMemoryBoundedBuffer.cpp
 
	Created by Joshua Higginbotham on 6/24/16.
 */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "sync_lib.h"

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

/* --- none --- */

/*--------------------------------------------------------------------------*/
/* HELPER FUNCTIONS */
/*--------------------------------------------------------------------------*/

/* --- none --- */

/*--------------------------------------------------------------------------*/
/* IMPLEMENTATION   S h a r e d M e m o r y B o u n d e d B u f f e r */
/*--------------------------------------------------------------------------*/

SharedMemoryBoundedBuffer::SharedMemoryBoundedBuffer(std::string _name, int _num_items, int _block_size)
{
	
}

SharedMemoryBoundedBuffer::~SharedMemoryBoundedBuffer() {

}

std::string SharedMemoryBoundedBuffer::push_back(std::string req) {
	
}

std::string SharedMemoryBoundedBuffer::retrieve_front() {
	
	
}

int SharedMemoryBoundedBuffer::size() {
	return shm_total_bytes;
}

std::string SharedMemoryBoundedBuffer::get_name() {
	return my_name;
}

int SharedMemoryBoundedBuffer::get_blocks_taken() {
	return blocks_taken;
}

int SharedMemoryBoundedBuffer::get_block_size() {
	return block_size;
}

void SharedMemoryBoundedBuffer::set_unlink_on_destruction(bool _unlink_afterwards) {
	unlink_on_destruction = _unlink_afterwards;
	full.set_unlink_on_destruction(_unlink_afterwards);
	empty.set_unlink_on_destruction(_unlink_afterwards);
	seg_access_lock.set_unlink_on_destruction(_unlink_afterwards);
}

bool SharedMemoryBoundedBuffer::get_already_existed() {
	return already_existed;
}