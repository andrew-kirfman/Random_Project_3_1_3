all: 
	g++ -std=c++11 main.cpp linked_list2.cpp -o List_2

List_2: main.o linked_list2.o
	g++ -std=c++11 main.o linked_list2.o -o List_2
	
main.o: main.cpp
	g++ -std=c++11 -c main.cpp
	
linked_list2.o: linked_list2.cpp
	g++ -std=c++11 -c linked_list2.cpp
	
clean:
	rm -rf *o List_2