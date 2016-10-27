//
//  bounded_buffer.hpp
//  
//
//  Created by Joshua Higginbotham on 11/4/15.
//
//

#ifndef bounded_buffer_h
#define bounded_buffer_h

#include <stdio.h>
#include <queue>
#include <string>
#include <pthread.h>
#include "semaphore.h"

class bounded_buffer {
    int capacity;
    semaphore full; //No overflow
    semaphore empty; //No underflow
    pthread_mutex_t data_access_lock; //Thread-safe: only allows one at a time
    std::queue<std::string> data;
    
public:
    bounded_buffer(int _capacity);
    void push_back(std::string str);
    std::string retrieve_front();
    int size();
};

#endif /* bounded_buffer_h */
