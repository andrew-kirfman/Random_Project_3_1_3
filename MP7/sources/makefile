# makefile

all: dataserver client client_solution

reqchannel.o: reqchannel.h reqchannel.cpp
	g++ -std=c++11 -c -g reqchannel.cpp

SafeBuffer.o: SafeBuffer.h SafeBuffer.cpp
	g++ -std=c++11 -c -g SafeBuffer.cpp

dataserver: dataserver.cpp reqchannel.o 
	g++ -std=c++11 -g -O1 -o dataserver dataserver.cpp reqchannel.o -lpthread -lrt

client: client_E7.cpp reqchannel.o SafeBuffer.o
	g++ -std=c++11 -g -O1 -o client client_E7.cpp reqchannel.o SafeBuffer.o -lpthread -lrt

client_solution: client_solution_E7.cpp reqchannel.o SafeBuffer.o
	g++ -std=c++11 -g -O1 -o client_solution client_solution_E7.cpp reqchannel.o SafeBuffer.o -lpthread -lrt

clean:
	rm -rf *.o fifo* *.DS_Store dataserver dataserver_debug client client_solution client_debug client_solution_debug *output*
