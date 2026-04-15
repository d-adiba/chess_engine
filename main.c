#include "attacks.h"


int main()
{
	for ( int i = 0 ; i < 8; i++)
	{
		for ( int j = 0 ; j < 8; j++)
		{
		 printf(" %d,",count_bits(mask_rook_attacks(i * 8 + j)));
		}
       		printf("\n");
	}	
	    	return 0;
}
