//
//  SafeBuffer.cpp
//
//
//  Created by Joshua Higginbotham on 11/4/15.
//
//

#include "SafeBuffer.h"
#include <string>
#include <queue>

SafeBuffer::SafeBuffer() {
	pthread_mutex_init(&data_access_lock, NULL);
}

void SafeBuffer::push_back(std::string req) {
	pthread_mutex_lock(&data_access_lock);
    data.push(req);
	pthread_mutex_unlock(&data_access_lock);
}

std::string SafeBuffer::retrieve_front() {
	pthread_mutex_lock(&data_access_lock);
    std::string retrieval = data.front();
    data.pop();
	pthread_mutex_unlock(&data_access_lock);
    return retrieval;
}

int SafeBuffer::size() {
    pthread_mutex_lock(&data_access_lock);
    int i = data.size();
    pthread_mutex_unlock(&data_access_lock);
    return i;
}
