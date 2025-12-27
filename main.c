#include "pawn.h"


int main ()
{
	init_pawn_leaper_attacks();
	for (int i = 0; i< 64; i++)  print_bitboard(pawn_attacks[black][i]); 
	return 0; 
}
