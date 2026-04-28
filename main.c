#include "attacks.h"
#include "random.h"

int main()
{
	init_all();
	U64 occupancy = 0ULL;
	set_bit(&occupancy, c5);
	set_bit(&occupancy, e7);
	set_bit(&occupancy, h5); 	
	print_bitboard(occupancy); 
	print_bitboard(get_rook_attacks(e5, occupancy));
}
