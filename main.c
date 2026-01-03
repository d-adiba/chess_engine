#include "attacks.h"


int main()
{
	init_leaper_attacks(); 
	printf("a4\n");
	print_bitboard(knight_attacks[a4]);
	printf("b4\n");
	print_bitboard(knight_attacks[b4]);
	printf("c4\n");
 	print_bitboard(knight_attacks[c4]);
	printf("d4\n");
	print_bitboard(knight_attacks[d4]);
	printf("e4\n");
	print_bitboard(knight_attacks[e4]);
	printf("f4\n");
 	print_bitboard(knight_attacks[f4]);
 	printf("g4\n");
	print_bitboard(knight_attacks[g4]);
	printf("h4\n");
	print_bitboard(knight_attacks[h4]);
 
 
    return 0;
}
