#include <stdlib.h>
#include <string.h>
#include <string.h>

int main(int argc, char ** argv) 
{
	int b = 128;
	int M = b * 16;  	// so we have space for 16 items in the whole list
	int t = 4;			// 4 tiers and 4 items per tier
	
	char buf [1024];
	memset (buf, 1, 1024);		// set each byte to 1
	
	char * msg = "a sample message";
	
	Init (M, b, t); // initialize
	// test operations
	int testnums [] = {0, 1<<29 , (1<<29) + 5 , 50, (1<<30) + 5, (1<<30) - 500};
	int i = 0;
	// some sample insertions
	for (i=0; i< 6; i ++)
	{
		Insert (testnums [i], buf, 50);   // insert 50 bytes from the buffer as value for each of the insertions
	}
	PrintList ();
		
	// end test operations	
	Destroy ();
	
}
