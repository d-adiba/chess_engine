#include "attacks.h"
#include "random.h"

int main()
{
		xorshift_init(); 
		for ( int j = 0 ; j < 4; j++)
		{
		 	print_bitboard(get_random_u64());
		}
       		printf("\n");
	    	return 0;
}
