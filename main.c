#include "bitboard.h"

int main()
{
        U64 bitboard = 0ULL;
        set_bit(&bitboard, e4);
        set_bit(&bitboard, c3);
        set_bit(&bitboard, f2);

        print_bitboard(bitboard);
        pop_bit(&bitboard, e4);
        print_bitboard(bitboard);
        pop_bit(&bitboard, e4);
        print_bitboard(bitboard);
        return 0;
}

