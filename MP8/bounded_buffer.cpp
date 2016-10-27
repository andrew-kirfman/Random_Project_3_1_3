//
//  bounded_buffer.cpp
//  
//
//  Created by Joshua Higginbotham on 11/4/15.
//
//

#include "bounded_buffer.h"
#include <string>
#include <queue>

bounded_buffer::bounded_buffer(int _capacity) : capacity(_capacity), full(0), empty(_capacity) { pthread_mutex_init(&data_access_lock, NULL); }

void bounded_buffer::push_back(std::string req) {
    empty.P();
    pthread_mutex_lock(&data_access_lock);
    data.push(req);
    pthread_mutex_unlock(&data_access_lock);
    full.V();
}

std::string bounded_buffer::retrieve_front() {
    full.P();
    pthread_mutex_lock(&data_access_lock);
    std::string retrieval = data.front();
    data.pop();
    pthread_mutex_unlock(&data_access_lock);
    empty.V();
    return retrieval;
}

int bounded_buffer::size() {
    pthread_mutex_lock(&data_access_lock);
    int i = data.size();
    pthread_mutex_unlock(&data_access_lock);
    return i;
}