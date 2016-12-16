//#include <iostream>
#include <vector>
#include <string>

static int test_static_int = 1;
const int test_const_int = 1;
int test_int = 1;
extern int test_extert_int_initialized = 1;
extern int test_extern_int_uninitialized;
std::vector<int> test_int_vector(100,0);
const std::vector<int> test_const_int_vector(100,0);
std::vector<std::string> test_string_vector_1(100,std::string(""));
std::vector<std::string> test_string_vector_2(100,std::string("abc"));
const std::vector<std::string> test_const_string_vector(100,"");

void myfunc () { return; }

int main(int argc, char** argv) {
	return 0;
}
