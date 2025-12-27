#include "pawn.h" 


const U64 not_a_file = 18374403900871474942ULL; 

const U64 not_h_file = 9187201950435737471ULL; 

const U64 not_ab_file =  18229723555195321596ULL;

const U64 not_hg_file = 4557430888798830399ULL; 

U64 pawn_attacks[2][64];      

U64  mask_pawn_attacks (side sd, square sq)
{
	U64 attacks_result = 0ULL; 
	U64  bitboard = 0ULL;
	set_bit(&bitboard, sq);
	print_bitboard(bitboard);

	// white pawn	
	if (!sd)
	{
		if ((bitboard >> 7) & not_a_file)   attacks_result |= (bitboard >> 7) ;
		if ((bitboard >> 9) & not_h_file)   attacks_result |= (bitboard >> 9) ;

	}

	// black pawn 
	else 
	{
		if ((bitboard << 7) & not_h_file)   attacks_result |= (bitboard << 7) ;
		if ((bitboard << 9) & not_a_file)   attacks_result |= (bitboard << 9) ;

	}
	return attacks_result;
}

void init_pawn_leaper_attacks()
{
	int sq; 
	for (sq = 0; sq < 64; sq++)
	{ 
		pawn_attacks[white][sq] = mask_pawn_attacks(white, sq);
		pawn_attacks[black][sq] = mask_pawn_attacks(black, sq);
	}
}

