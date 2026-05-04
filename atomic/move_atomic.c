#include "move_atomic.h"

U64 mask_atomic_explosion[64];
U64 atomic_explosion_attacks[64][512];


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
    for (int sq = 0; sq < 64; sq++)
    {
        for (int index = 0; index < 512; index++)
        {
            atomic_explosion_attacks[sq][index] =  _pdep_u64(index, mask_atomic_explosion[sq]);
        }
    }
}