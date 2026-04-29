#include "attacks.h"
#include "random.h"

int main()
{
	init_all();
	board_t b;
	U64 occupancy = 0ULL;
       set_bit(&occupancy, b6);	
       set_bit(&occupancy, d6);	
       set_bit(&occupancy, f6);	
       set_bit(&occupancy, b4);	
       set_bit(&occupancy, c3);	
       set_bit(&occupancy, d3);	
       set_bit(&occupancy, e3);	
	print_bitboard(get_queen_attacks(d4, occupancy));

}
