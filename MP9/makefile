# makefile

all: client dataserver reqchannel_test

ThreadSharedSemaphore.o: sync_lib.h ThreadSharedSemaphore.cpp
	g++ -std=c++11 -c -g ThreadSharedSemaphore.cpp

ProcessSharedSemaphore.o: sync_lib.h ProcessSharedSemaphore.cpp
	g++ -std=c++11 -c -g ProcessSharedSemaphore.cpp

ThreadSharedBoundedBuffer.o: sync_lib.h ThreadSharedBoundedBuffer.cpp ThreadSharedSemaphore.o
	g++ -std=c++11 -c -g ThreadSharedBoundedBuffer.cpp

SharedMemoryBoundedBuffer.o: sync_lib.h SharedMemoryBoundedBuffer.cpp ProcessSharedSemaphore.o
	g++ -std=c++11 -c -g SharedMemoryBoundedBuffer.cpp

MessageQueueBoundedBuffer.o: sync_lib.h MessageQueueBoundedBuffer.cpp ProcessSharedSemaphore.o
	g++ -std=c++11 -c -g MessageQueueBoundedBuffer.cpp

NamedPipeRequestChannel.o: sync_lib.h NamedPipeRequestChannel.h NamedPipeRequestChannel.cpp
	g++ -std=c++11 -c -g NamedPipeRequestChannel.cpp

UnnamedPipeRequestChannel.o: sync_lib.h UnnamedPipeRequestChannel.h UnnamedPipeRequestChannel.cpp
	g++ -std=c++11 -c -g UnnamedPipeRequestChannel.cpp

MessageQueueRequestChannel.o: sync_lib.h MessageQueueRequestChannel.h MessageQueueRequestChannel.cpp
	g++ -std=c++11 -c -g MessageQueueRequestChannel.cpp

SharedMemoryRequestChannel.o: sync_lib.h SharedMemoryBoundedBuffer.o SharedMemoryRequestChannel.h SharedMemoryRequestChannel.cpp
	g++ -std=c++11 -c -g SharedMemoryRequestChannel.cpp

SignalRequestChannel.o: sync_lib.h SignalRequestChannel.h SignalRequestChannel.cpp
	g++ -std=c++11 -c -g SignalRequestChannel.cpp

Dataserver.o: sync_lib.h Dataserver.h Dataserver.cpp NamedPipeRequestChannel.o UnnamedPipeRequestChannel.o MessageQueueRequestChannel.o ProcessSharedSemaphore.o SharedMemoryBoundedBuffer.o SharedMemoryRequestChannel.o SignalRequestChannel.o
	g++ -std=c++11 -c -g Dataserver.cpp

dataserver: sync_lib.h dataserver_main.cpp Dataserver.o NamedPipeRequestChannel.o UnnamedPipeRequestChannel.o ProcessSharedSemaphore.o MessageQueueBoundedBuffer.o MessageQueueRequestChannel.o SharedMemoryBoundedBuffer.o SharedMemoryRequestChannel.o SignalRequestChannel.o
	g++ -std=c++11 -g dataserver_main.cpp *.o -lpthread -lrt -o dataserver

client: sync_lib.h client.cpp ThreadSharedSemaphore.o ThreadSharedBoundedBuffer.o Dataserver.o NamedPipeRequestChannel.o UnnamedPipeRequestChannel.o ProcessSharedSemaphore.o MessageQueueBoundedBuffer.o MessageQueueRequestChannel.o SharedMemoryBoundedBuffer.o SharedMemoryRequestChannel.o SignalRequestChannel.o
	g++ -std=c++11 -g client.cpp *.o -lpthread -lrt -o client

reqchannel_test: sync_lib.h RequestChannelTest.cpp ThreadSharedSemaphore.o ThreadSharedBoundedBuffer.o Dataserver.o NamedPipeRequestChannel.o UnnamedPipeRequestChannel.o ProcessSharedSemaphore.o MessageQueueBoundedBuffer.o MessageQueueRequestChannel.o SharedMemoryBoundedBuffer.o SharedMemoryRequestChannel.o SignalRequestChannel.o
	g++ -std=c++11 -g RequestChannelTest.cpp *.o -lpthread -lrt -o reqchannel_test

clean:
	rm -rf *.o fifo* *_sock *.txt *.DS_Store client dataserver reqchannel_test