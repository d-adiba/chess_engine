#include "bitboard.h"

void  print_bitboard(U64 bitboard)
{
	int rank, file, sq;
	for (rank = 0; rank < 8; rank ++)
	{
		printf("  %d ", (8 - rank)); 	
		for (file = 0; file < 8; file++)
		{ 
			sq  = rank * 8 + file;
			printf(" %d ", (int)((bitboard  >> sq) & 1ULL)); 
		}
		printf("\n");
	}
	printf("\n     a  b  c  d  e  f  g  h\n\n");
	printf("     Bitboard: %llu\n\n", bitboard); 
}
