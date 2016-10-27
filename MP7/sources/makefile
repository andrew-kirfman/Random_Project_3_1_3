# makefile

all: dataserver client client_solution
student: dataserver client
solution: dataserver client_solution

reqchannel.o: reqchannel.h reqchannel.cpp
	g++ -std=c++11 -c -g reqchannel.cpp

dataserver: dataserver.cpp reqchannel.o 
	g++ -std=c++11 -g -o dataserver dataserver.cpp reqchannel.o -lpthread

client: client_E7.cpp reqchannel.o
	g++ -std=c++11 -g -o client client_E7.cpp reqchannel.o -lpthread

client_solution: client_solution_E7.cpp reqchannel.o
	g++ -std=c++11 -g -o client_solution client_solution_E7.cpp reqchannel.o -lpthread

clean:
	rm *.o fifo* dataserver client client_solution output*