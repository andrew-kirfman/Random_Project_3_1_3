//
//  SafeBuffer.h
//  
//
//  Created by Joshua Higginbotham on 11/4/15.
//
//

#ifndef SafeBuffer_h
#define SafeBuffer_h

#include <stdio.h>
#include <queue>
#include <string>
#include <pthread.h>

class SafeBuffer {
    pthread_mutex_t data_access_lock; //Thread-safe: only allows one at a time
    std::queue<std::string> data;
    
public:
    SafeBuffer();
    void push_back(std::string str);
    std::string retrieve_front();
    int size();
};

#endif /* SafeBuffer_ */
