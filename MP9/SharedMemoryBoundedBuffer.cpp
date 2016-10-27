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

const int MY_MMAP_PROT		= PROT_READ | PROT_WRITE;
const int MY_MMAP_FLAGS		= MAP_SHARED;
const int MY_MMAP_OFFSET	= 0;
const bool VERBOSE = false;
const bool VERBOSE_DEBUG = false;

/*--------------------------------------------------------------------------*/
/* HELPER FUNCTIONS */
/*--------------------------------------------------------------------------*/

/* --- none --- */

/*--------------------------------------------------------------------------*/
/* IMPLEMENTATION   S h a r e d M e m o r y B o u n d e d B u f f e r */
/*
	Realized that msync must be done all at one whop on account
 	of it won't take a first argument that's at an address which
 	isn't a multiple of the page size.

	Class destructor is short since destructors are defined
	for all data members of this class, and should get
	called automatically. Please correct me if that
	is a wrong assumption.
 
 	Failure of other parts of an application is fine, but memory leaks
 	in the kernel shall not be tolerated. Hence the underlying shared
 	memory segment is unlinked in every exit path, except where
 	there is certainty that it was never created to begin with.
*/
/*--------------------------------------------------------------------------*/

SharedMemoryBoundedBuffer::SharedMemoryBoundedBuffer(std::string _name, int _num_items, int _block_size) :
max_items(_num_items),
block_size(_block_size + 1), //add 1 to include null byte in memory block
shm_total_bytes((2 * sizeof(int)) + (max_items * block_size)), //2 ints for 2 block indices
blocks_taken(0),
my_name(_name),
seg_name("/" + my_name + "_shm_seg"),
full("/" + my_name + "_shm_full_sema", 0),
empty("/" + my_name + "_shm_empty_sema", max_items),
seg_access_lock("/" + my_name + "_shm_access_sema", 1)
{
	empty_constructor_used = false;
	if(VERBOSE) {
		threadsafe_console_output.println("SHARED_MEMORY_BOUNDED_BUFFER:" + my_name + ": Pagesize: " + std::to_string(sysconf(_SC_PAGESIZE)) + "; Attempting to construct bounded buffer for " + seg_name);
	}
	
	int shm_fd = shm_open(seg_name.c_str(), O_RDWR | O_CREAT | O_EXCL, MY_CREATION_FLAGS);

	if(VERBOSE) {
		threadsafe_console_output.println("SHARED_MEMORY_BOUNDED_BUFFER:" + my_name + ": shm_open originally returns shm_fd == " + std::to_string(shm_fd) + " for " + seg_name);
	}
	
	if(shm_fd < 0 && errno == EEXIST) {
		shm_fd = shm_open(seg_name.c_str(), O_RDWR, MY_CREATION_FLAGS);
		if(shm_fd < 0) {
			int prev_errno = errno;
			if(shm_unlink(seg_name.c_str()) < 0 && errno != ENOENT) {
				threadsafe_console_output.perror("SHARED_MEMORY_BOUNDED_BUFFER:" + my_name + ": failed on unlink for shm seg " + seg_name);
			}
			errno = prev_errno;
			throw sync_lib_exception("SHARED_MEMORY_BOUNDED_BUFFER:" + my_name + ": failed to open pre-existing shared memory segment " + seg_name);
		}
		already_existed = true;
		if(VERBOSE) {
			threadsafe_console_output.println("SHARED_MEMORY_BOUNDED_BUFFER:" + my_name + ": successfully opened pre-existing shm_seg " + seg_name + " for reading and writing, shm_fd == " + std::to_string(shm_fd));
		}
	}
	else if(shm_fd < 0) {
		throw sync_lib_exception("SHARED_MEMORY_BOUNDED_BUFFER:" + my_name + ": failed to create/open shared memory segment " + seg_name);
	}

	if(ftruncate(shm_fd, shm_total_bytes) < 0) {
		int prev_errno = errno;
		if(shm_unlink(seg_name.c_str()) < 0 && errno != ENOENT) {
			threadsafe_console_output.perror("SHARED_MEMORY_BOUNDED_BUFFER:" + my_name + ": failed on unlink for shm seg " + seg_name);
		}
		errno = prev_errno;
		throw sync_lib_exception("SHARED_MEMORY_BOUNDED_BUFFER:" + my_name + ": failed on ftruncate for shared memory segment " + seg_name);
	}
	if(VERBOSE) {
		threadsafe_console_output.println("SHARED_MEMORY_BOUNDED_BUFFER:" + my_name + ": created new shm_seg for " + seg_name + ", shm_fd == " + std::to_string(shm_fd) + ", successfully truncated to " + std::to_string(shm_total_bytes));
	}
	
	if(VERBOSE && VERBOSE_DEBUG) {
		std::string mmap_stats = "";
		mmap_stats += "SHARED_MEMORY_BOUNDED_BUFFER:" + my_name + ": Calling mmap for " + seg_name + " with arguments:\n";
		mmap_stats += "SHARED_MEMORY_BOUNDED_BUFFER:" + my_name + ": \t" + seg_name + ":addr == NULL\n";
		mmap_stats += "SHARED_MEMORY_BOUNDED_BUFFER:" + my_name + ": \t" + seg_name + ":length == " + std::to_string(shm_total_bytes) + "\n";
		mmap_stats += "SHARED_MEMORY_BOUNDED_BUFFER:" + my_name + ": \t" + seg_name + ":prot == " + std::to_string(MY_MMAP_PROT) + "\n";
		mmap_stats += "SHARED_MEMORY_BOUNDED_BUFFER:" + my_name + ": \t" + seg_name + ":flags == " + std::to_string(MY_MMAP_FLAGS) + "\n";
		mmap_stats += "SHARED_MEMORY_BOUNDED_BUFFER:" + my_name + ": \t" + seg_name + ":fd == " + std::to_string(shm_fd) + "\n";
		mmap_stats += "SHARED_MEMORY_BOUNDED_BUFFER:" + my_name + ": \t" + seg_name + ":offset == " + std::to_string(MY_MMAP_OFFSET) + "\n";
		threadsafe_console_output.println(mmap_stats);
	}
	void * shm_seg_status;
	shm_seg_status = mmap(NULL, shm_total_bytes, MY_MMAP_PROT, MY_MMAP_FLAGS, shm_fd, MY_MMAP_OFFSET);
	if(shm_seg_status == MAP_FAILED) {
		int prev_errno = errno;
		if(shm_unlink(seg_name.c_str()) < 0 && errno != ENOENT) {
			threadsafe_console_output.perror("SHARED_MEMORY_BOUNDED_BUFFER:" + my_name + ": failed on unlink for shm seg " + seg_name);
		}
		errno = prev_errno;
		throw sync_lib_exception("SHARED_MEMORY_BOUNDED_BUFFER:" + my_name + ": failed to map shared memory segment " + seg_name + ": length == " + std::to_string(shm_total_bytes) + ", fd == " + std::to_string(shm_fd));
	}
	close(shm_fd);
	
	shm_seg = (char*)shm_seg_status;
	shm_seg_blocks_start = shm_seg + (2 * sizeof(int));
	
	if(VERBOSE) threadsafe_console_output.println("SHARED_MEMORY_BOUNDED_BUFFER:" + my_name + ": Bounded buffer for " + seg_name + " successfully constructed");
}

SharedMemoryBoundedBuffer::~SharedMemoryBoundedBuffer() {
	if(VERBOSE) threadsafe_console_output.println("SHARED_MEMORY_BOUNDED_BUFFER:" + my_name + ": destructing IPC mechanisms for shm_bbuf...");
	if(!empty_constructor_used) {
		if(VERBOSE_DEBUG) threadsafe_console_output.println("SHARED_MEMORY_BOUNDED_BUFFER:" + my_name + ": munmapping shm_seg " + seg_name);
		if(munmap(shm_seg, shm_total_bytes) < 0) {
			threadsafe_console_output.perror("SHARED_MEMORY_BOUNDED_BUFFER:" + my_name + ": failed on munmap for shm seg " + seg_name);
		}
		if(unlink_on_destruction) {
			if(VERBOSE_DEBUG) threadsafe_console_output.println("SHARED_MEMORY_BOUNDED_BUFFER:" + my_name + ": unlinking shm_seg " + seg_name);
			if(shm_unlink(seg_name.c_str()) < 0 && errno != ENOENT) {
				threadsafe_console_output.perror("SHARED_MEMORY_BOUNDED_BUFFER:" + my_name + ": failed on unlink for shm seg " + seg_name);
			}
		}
	}
}

std::string SharedMemoryBoundedBuffer::push_back(std::string req) {
	
	if(req.length() > block_size - 1) {
		errno = EINVAL;
		return "ERROR_REQUEST_LONGER_THAN_BLOCK_SIZE[" + std::to_string(block_size - 1) + "]";
	}
	
	if(VERBOSE && VERBOSE_DEBUG) threadsafe_console_output.println("SHARED_MEMORY_BOUNDED_BUFFER:" + my_name + ":push_back: waiting for empty sema...");
	if(empty.P() != 0) {
		return "ERROR_EMPTY_WAIT_SEMA_FAILED";
	}
	
	if(VERBOSE && VERBOSE_DEBUG) threadsafe_console_output.println("SHARED_MEMORY_BOUNDED_BUFFER:" + my_name + ":push_back: waiting for seg_access_lock sema...");
	if(seg_access_lock.P() != 0) {
		return "ERROR_ACCESS_WAIT_SEMA_FAILED";
	}
	
	write_index = *((int*)(shm_seg + sizeof(int)));
	strcpy(shm_seg_blocks_start + (write_index * block_size), req.c_str());
	write_index += 1;
	if(write_index == max_items) write_index = 0;
	*((int*)(shm_seg + sizeof(int))) = write_index;
	
	if(msync(shm_seg, shm_total_bytes, MS_SYNC | MS_INVALIDATE) < 0) {
		return "ERROR_MSYNC_WRITE_MESSAGE_FAILED";
	}
	
	--blocks_taken;
	
	if(VERBOSE && VERBOSE_DEBUG) threadsafe_console_output.println("SHARED_MEMORY_BOUNDED_BUFFER:" + my_name + ":push_back: releasing seg_access_lock sema...");
	if(seg_access_lock.V() != 0) {
		return "ERROR_ACCESS_POST_SEMA_FAILED";
	}
	
	if(VERBOSE && VERBOSE_DEBUG) threadsafe_console_output.println("SHARED_MEMORY_BOUNDED_BUFFER:" + my_name + ":push_back: releasing full sema...");
	if(full.V() != 0) {
		return "ERROR_FULL_POST_SEMA_FAILED";
	}
	
	return req;
}

std::string SharedMemoryBoundedBuffer::retrieve_front() {
	
	char read_buf[block_size];
	std::string read_result;
	
	if(VERBOSE && VERBOSE_DEBUG) threadsafe_console_output.println("SHARED_MEMORY_BOUNDED_BUFFER:" + my_name + ":retrieve_front: waiting for full sema...");
	if(full.P() != 0) {
		return "ERROR_FULL_WAIT_SEMA_FAILED";
	}
	
	if(VERBOSE && VERBOSE_DEBUG) threadsafe_console_output.println("SHARED_MEMORY_BOUNDED_BUFFER:" + my_name + ":retrieve_front: waiting for seg_access_lock sema...");
	if(seg_access_lock.P() != 0) {
		return "ERROR_ACCESS_WAIT_SEMA_FAILED";
	}
	
	read_index = *((int*)(shm_seg));
	strcpy(read_buf, shm_seg_blocks_start + (read_index * block_size));
	read_result = read_buf;
	memset(shm_seg_blocks_start + (read_index * block_size), '\0', read_result.length() + 1);
	read_index += 1;
	if(read_index == max_items) read_index = 0;
	*((int*)(shm_seg)) = read_index;
	
	if(msync(shm_seg, shm_total_bytes, MS_SYNC | MS_INVALIDATE) < 0) {
		return "ERROR_MSYNC_READ_MESSAGE_FAILED";
	}
	
	++blocks_taken;
	
	if(VERBOSE && VERBOSE_DEBUG) threadsafe_console_output.println("SHARED_MEMORY_BOUNDED_BUFFER:" + my_name + ":retrieve_front: releasing seg_access_lock sema...");
	if(seg_access_lock.V() != 0) {
		return "ERROR_ACCESS_POST_SEMA_FAILED";
	}
	
	if(VERBOSE && VERBOSE_DEBUG) threadsafe_console_output.println("SHARED_MEMORY_BOUNDED_BUFFER:" + my_name + ":retrieve_front: releasing empty sema...");
	if(empty.V() != 0) {
		return "ERROR_EMPTY_POST_SEMA_FAILED";
	}
	
	return read_result;
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