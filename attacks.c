#include "attacks.h"
#include <stdio.h>

const U64 not_a_file = 18374403900871474942ULL;

const U64 not_h_file = 9187201950435737471ULL;

const U64 not_ab_file = 18229723555195321596ULL;

const U64 not_hg_file = 4557430888798830399ULL;

const int bishop_relevant_bits[64] = {
 6, 5, 5, 5, 5, 5, 5, 6,
 5, 5, 5, 5, 5, 5, 5, 5,
 5, 5, 7, 7, 7, 7, 5, 5,
 5, 5, 7, 9, 9, 7, 5, 5,
 5, 5, 7, 9, 9, 7, 5, 5,
 5, 5, 7, 7, 7, 7, 5, 5,
 5, 5, 5, 5, 5, 5, 5, 5,
 6, 5, 5, 5, 5, 5, 5, 6
};

const int rook_relevant_bits[64] = {
 12, 11, 11, 11, 11, 11, 11, 12,
 11, 10, 10, 10, 10, 10, 10, 11,
 11, 10, 10, 10, 10, 10, 10, 11,
 11, 10, 10, 10, 10, 10, 10, 11,
 11, 10, 10, 10, 10, 10, 10, 11,
 11, 10, 10, 10, 10, 10, 10, 11,
 11, 10, 10, 10, 10, 10, 10, 11,
 12, 11, 11, 11, 11, 11, 11, 12
};

U64 pawn_attacks[2][64];
U64 knight_attacks[64]; 


U64 king_attacks[64];
U64 bishop_masks[64];
U64 rook_masks[64];
U64 mask_atomic_explosion[64];



U64 atomic_explosion_attacks[64][512];
U64 bishop_attacks[64][512];
U64 rook_attacks[64][4096];
U64 rook_magic_number[64];
U64 bishop_magic_number[64]; 


U64 mask_pawn_attacks(side sd, square sq)
{
    U64 attacks_result = 0ULL;
    U64 bitboard = 0ULL;
    set_bit(&bitboard, sq);

    // white pawn   
    if (!sd) {
	if ((bitboard >> 7) & not_a_file)
	    attacks_result |= (bitboard >> 7);
	if ((bitboard >> 9) & not_h_file)
	    attacks_result |= (bitboard >> 9);

    }
    // black pawn 
    else {
	if ((bitboard << 7) & not_h_file)
	    attacks_result |= (bitboard << 7);
	if ((bitboard << 9) & not_a_file)
	    attacks_result |= (bitboard << 9);

    }
    return attacks_result;
}



U64 mask_knight_attacks(square sq)
{
    U64 attacks_result = 0ULL;
    U64 bitboard = 0ULL;
    set_bit(&bitboard, sq);

    if ((bitboard >> 17) & not_h_file) attacks_result |= (bitboard >> 17);
    if ((bitboard >> 15) & not_a_file) attacks_result |= (bitboard >> 15); 
    if ((bitboard >> 10) & not_hg_file) attacks_result |= (bitboard >> 10); 
    if ((bitboard >> 6) & not_ab_file) attacks_result |= (bitboard >> 6);

    if ((bitboard << 17) & not_a_file) attacks_result |= (bitboard << 17);
    if ((bitboard << 15) & not_h_file) attacks_result |= (bitboard << 15); 
    if ((bitboard << 10) & not_ab_file) attacks_result |= (bitboard << 10); 
    if ((bitboard << 6) & not_hg_file) attacks_result |= (bitboard << 6);

    return attacks_result; 
}

U64 mask_king_attacks(square sq)
{
    U64 attacks_result = 0ULL;
    U64 bitboard = 0ULL;
    set_bit(&bitboard, sq);

    if (bitboard >> 8)  attacks_result |= (bitboard >> 8);
    if ((bitboard >> 7) & not_a_file) attacks_result |= (bitboard >> 7);
    if ((bitboard >> 9) & not_h_file) attacks_result |= (bitboard >> 9); 
    if ((bitboard >> 1) & not_h_file) attacks_result |= (bitboard >> 1); 
    
    if (bitboard << 8)  attacks_result |= (bitboard << 8);
    if ((bitboard << 7) & not_h_file) attacks_result |= (bitboard << 7);
    if ((bitboard << 9) & not_a_file) attacks_result |= (bitboard << 9); 
    if ((bitboard << 1) & not_a_file) attacks_result |= (bitboard << 1); 

    return attacks_result; 
}



U64 mask_bishop_attacks(square sq)
{
    int r,f, tr,tf; 
    U64 attacks_result = 0ULL; 

    tr = sq / 8; 
    tf = sq % 8;

    for (r = tr + 1, f = tf + 1; r <= 6 && f <= 6; r++, f++)
    {
	  attacks_result |= (1ULL << ( r * 8 + f));
    }
    for (r = tr - 1, f = tf + 1; r >= 1 && f <= 6; r--, f++)
    {
	  attacks_result |= (1ULL << ( r * 8 + f));
    }


    for (r = tr + 1, f = tf - 1; r <= 6 && f >= 1; r++, f--)
    {
	  attacks_result |= (1ULL << ( r * 8 + f));
    }
    for (r = tr - 1, f = tf - 1; r >= 1 && f >= 1; r--, f--)
    {
	  attacks_result |= (1ULL << ( r * 8 + f));
    }

    return attacks_result; 
}

U64 mask_bishop_attacks_on_the_fly(square sq, U64 block)
{
    int r,f, tr,tf; 
    U64 attacks_result = 0ULL; 

    tr = sq / 8; 
    tf = sq % 8;

    for (r = tr + 1, f = tf + 1; r <= 7 && f <= 7; r++, f++)
    {
	  attacks_result |= (1ULL << ( r * 8 + f));
	  if ( block & (1ULL << ( r * 8 + f))) break;
    }
    for (r = tr - 1, f = tf + 1; r >= 0 && f <= 7; r--, f++)
    {
	  attacks_result |= (1ULL << ( r * 8 + f));
	  if ( block & (1ULL << ( r * 8 + f))) break;
    }


    for (r = tr + 1, f = tf - 1; r <= 7 && f >= 0; r++, f--)
    {
	  attacks_result |= (1ULL << ( r * 8 + f));
	  if ( block & (1ULL << ( r * 8 + f))) break;
    }
    for (r = tr - 1, f = tf - 1; r >= 0 && f >= 0; r--, f--)
    {
	  attacks_result |= (1ULL << ( r * 8 + f));
	  if ( block & (1ULL << ( r * 8 + f))) break;
    }

    return attacks_result; 
}

U64 mask_rook_attacks_on_the_fly(square sq, U64 block)
{
    int r,f, tr,tf; 
    U64 attacks_result = 0ULL; 

    tr = sq / 8; 
    tf = sq % 8;

    for (r = tr + 1; r <= 7 ; r++)
    {
	  attacks_result |= (1ULL << ( r * 8 + tf));
	  if ( block & (1ULL << ( r * 8 + tf))) break;
    }
    for (f = tf + 1; f <= 7; f++)
    {
	  attacks_result |= (1ULL << ( tr * 8 + f));
	  if ( block & (1ULL << ( tr * 8 + f))) break;
    }


    for ( r = tr - 1; r >= 0; r--)
    {
	  attacks_result |= (1ULL << ( r * 8 + tf));
	  if ( block & (1ULL << ( r * 8 + tf))) break;
    }
    for (f = tf - 1;  f >= 0; f--)
    {
	  attacks_result |= (1ULL << ( tr * 8 + f));
	  if ( block & (1ULL << ( tr * 8 + f))) break;
    }

    return attacks_result; 
}

U64 mask_rook_attacks(square sq)
{
    int r,f, tr,tf; 
    U64 attacks_result = 0ULL; 

    tr = sq / 8; 
    tf = sq % 8;

    for (r = tr + 1; r <= 6 ; r++)
    {
	  attacks_result |= (1ULL << ( r * 8 + tf));
    }
    for (f = tf + 1; f <= 6; f++)
    {
	  attacks_result |= (1ULL << ( tr * 8 + f));
    }


    for ( r = tr - 1; r >= 1; r--)
    {
	  attacks_result |= (1ULL << ( r * 8 + tf));
    }
    for (f = tf - 1;  f >= 1; f--)
    {
	  attacks_result |= (1ULL << ( tr * 8 + f));
    }

    return attacks_result; 
}
void init_leaper_attacks()
{
    int sq;
    for (sq = 0; sq < 64; sq++) {
	//table d'attaque des pawn
	pawn_attacks[white][sq] = mask_pawn_attacks(white, sq);
	pawn_attacks[black][sq] = mask_pawn_attacks(black, sq);
	//table d'attaque des knights 
	knight_attacks[sq] = mask_knight_attacks(sq);
	//table  d'attaque du king
	king_attacks[sq] = mask_king_attacks(sq);
    }
}



U64 set_occupancy( int index , int bits_in_mask, U64 mask_attacks)
{
	U64 occupancy = 0ULL;
	U64 mask = mask_attacks; 
	square sq; 	
	for ( int i = 0 ; i < bits_in_mask ; i++)
	{
		sq = get_ls1b_index(mask);	
		pop_bit(&mask, sq);
		if ( index & (1 << i))
			occupancy |= (1ULL << sq); 
	}
	return occupancy;
}

U64 find_magic_number(square sq, int relevant_bits, flags f)
{
	U64 occupancies[4096] , attacks[4096], used_attacks[4096];
	U64 attack_mask = (f == bishop)  ? mask_bishop_attacks(sq): 
		mask_rook_attacks(sq);
	int occupancy_indicies, index; 
        occupancy_indicies =  1 << relevant_bits;
 	for (index = 0; index < occupancy_indicies; index++)
	{
		occupancies[index] = set_occupancy(index, relevant_bits, attack_mask);
		attacks [index] = ( f == bishop) ? mask_bishop_attacks_on_the_fly(sq,
				occupancies[index]) : mask_rook_attacks_on_the_fly (sq,
					occupancies[index]);
	}
	int magic_index, fail, index_t; 
	U64  magic_number;
	for( int i = 0;  i < 100000000; i++) 
	{
		magic_number = get_magic_number();
		if ( count_bits(((attack_mask * magic_number) & 
				0xFF00000000000000)) < 6 ) continue; 
		memset(used_attacks, 0ULL, sizeof(used_attacks));
		fail = 0; 
		for (index_t = 0; !fail && 
				index_t < occupancy_indicies; index_t++)
		{
			magic_index =  (int)  ((occupancies[index_t] * magic_number) >> ( 64 - relevant_bits)); 
		       if (used_attacks[magic_index] == 0ULL)
				used_attacks[magic_index] = attacks[index_t];
			else if (used_attacks[magic_index] != attacks[index_t])
				 fail = 1;
		}
		if (!fail)
			return magic_number;
	}
	printf("Magic number not working");
	return 0ULL; 	
}

void init_atomic_explosion_mask()
{
    for (int sq = 0; sq < 64; sq++)
    {
        U64 mask = 0ULL;
        set_bit(&mask, sq);
        if (sq + 8 <= h1) set_bit(&mask, sq + 8);
        if (sq - 8 >= a8) set_bit(&mask, sq - 8);
        if ((sq + 1) % 8 != 0) 
        {
            set_bit(&mask, sq + 1);
            if (sq + 9 <= h1) set_bit(&mask, sq + 9);
            if (sq - 7 >= a8) set_bit(&mask, sq - 7);
        }
        if (sq % 8 != 0)    
        {
            set_bit(&mask, sq - 1);
            if (sq + 7 <= h1) set_bit(&mask, sq + 7);
            if (sq - 9 >= a8) set_bit(&mask, sq - 9);
        }
        mask_atomic_explosion[sq] = mask;
    }
}

void init_atomic_explosion_attacks()
{
	init_atomic_explosion_mask();
    for (int sq = 0; sq < 64; sq++)
    {
        for (int index = 0; index < 512; index++)
        {
            atomic_explosion_attacks[sq][index] =  _pdep_u64(index, mask_atomic_explosion[sq]);
        }
    }
}

void init_magic_number()
{
	for ( int i = 0; i < 64; i++)
	{
		rook_magic_number[i] =  find_magic_number (i, rook_relevant_bits[i], rook); 
	}
	for ( int i = 0; i < 64; i++)
	{
		bishop_magic_number[i] = find_magic_number(i, bishop_relevant_bits[i],bishop); 
	}
}
void init_all()
{
	init_leaper_attacks(); 
	init_magic_number();
	init_atomic_explosion_attacks();
	init_slider_attacks(bishop);
	init_slider_attacks(rook);
}


void init_slider_attacks(flags fg)
{
	U64 attack_mask, occupancy; 
	int magic_index, relevant_bits_count, occupancy_indicies;	
	for (int sq = 0; sq < 64 ;  sq++)
	{
		bishop_masks[sq] = mask_bishop_attacks(sq); 
		rook_masks[sq] = mask_rook_attacks(sq); 
		attack_mask = (fg == bishop) ? bishop_masks[sq] : rook_masks[sq];
		relevant_bits_count = count_bits(attack_mask); 
		occupancy_indicies =  (1 << relevant_bits_count);
		for ( int index = 0;  index < occupancy_indicies; index++)
		{
			if ( fg == bishop) 
			{
				occupancy = set_occupancy (index, relevant_bits_count, attack_mask);
				magic_index = (occupancy * bishop_magic_number[sq])>> (64 - bishop_relevant_bits[sq]);
				bishop_attacks[sq][magic_index] = mask_bishop_attacks_on_the_fly(sq, occupancy); 	
			}
			else
			{
				 occupancy = set_occupancy(index, relevant_bits_count, attack_mask); 
				 magic_index = (occupancy * rook_magic_number[sq])>> (64 - rook_relevant_bits[sq]);
				 rook_attacks[sq][magic_index] = mask_rook_attacks_on_the_fly(sq, occupancy); 	
			}
		}
	}
}

