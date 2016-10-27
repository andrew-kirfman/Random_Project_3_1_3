all: 
	g++ -std=c++11 main.cpp linked_list.cpp -o List_1

List_1: main.o linked_list.o
	g++ -std=c++11 main.o linked_list.o -o List_1
	
main.o: main.cpp
	g++ -std=c++11 -c main.cpp
	
linked_list.o: linked_list.cpp
	g++ -std=c++11 -c linked_list.cpp
	
clean:
	rm -rf *o List_1