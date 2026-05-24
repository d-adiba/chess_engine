#include "attacks.h"
#include <stdio.h>

const U64 not_a_file = 18374403900871474942ULL;

const U64 not_h_file = 9187201950435737471ULL;

const U64 not_ab_file = 18229723555195321596ULL;

const U64 not_hg_file = 4557430888798830399ULL;


U64 pawn_attacks[2][64];
U64 knight_attacks[64]; 


U64 king_attacks[64];
U64 bishop_masks[64];
U64 rook_masks[64];
U64 mask_atomic_explosion[64];



U64 atomic_explosion_attacks[64][512];
U64 bishop_attacks[64][512];
U64 rook_attacks[64][4096];


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


void init_leaper_attacks()
{
    int sq;
    for (sq = 0; sq < 64; sq++) {

	pawn_attacks[white][sq] = mask_pawn_attacks(white, sq);
	pawn_attacks[black][sq] = mask_pawn_attacks(black, sq);
	knight_attacks[sq] = mask_knight_attacks(sq);
	king_attacks[sq] = mask_king_attacks(sq);
    }
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

void init_mask_slider_attacks()
{
    for (int sq = 0; sq < 64; sq++)
    {
        bishop_masks[sq] = mask_bishop_attacks(sq);
        rook_masks[sq] = mask_rook_attacks(sq);
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

void init_slider_attacks(flags fg)
{
    init_mask_slider_attacks();
    if (fg == bishop)
    {
        for (int sq = 0; sq < 64; sq++)
        {
            for (int index = 0; index < 512; index++)
            {
                bishop_attacks[sq][index] = mask_bishop_attacks_on_the_fly(sq, _pdep_u64(index, bishop_masks[sq]));
            }
        }
    }
    else
    {
        for (int sq = 0; sq < 64; sq++)
        {
            for (int index = 0; index < 4096; index++)
            {
                rook_attacks[sq][index] = mask_rook_attacks_on_the_fly(sq, _pdep_u64(index, rook_masks[sq]));
            }
        }
    }
}

void init_all()
{
	init_leaper_attacks(); 
	init_atomic_explosion_attacks();
	init_slider_attacks(bishop);
	init_slider_attacks(rook);
}
