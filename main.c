#include "attacks.h"
#include "random.h"

int main()
{
	init_all();
	board_t b_t;
	b_t.side = 0;
        b_t.castle  = 13;	
	b_t.enpassant = no_sq;
	U64 * chessboard = b_t.board; 
	for ( int  j = P; j < k; j++)
	{
		chessboard[j] = 0ULL;
	}
	for ( int i = 0; i < 8; i++)
	{
		set_bit(chessboard + P, 8*6 + i);

	}
	// rook
	set_bit(chessboard + R, a1);
	set_bit(chessboard + R, h1);
	//bishop 
	set_bit(chessboard + B, c1);
	set_bit(chessboard + B, f1);
	//king
	set_bit(chessboard + K, e1);
	//queen
	set_bit(chessboard + Q, d1);
	//knight
	set_bit(chessboard + N, b1);
	set_bit(chessboard + N, g1);
	chessboard[p] = chessboard[P] >> ( 8*5); 
	chessboard[r] = chessboard[R] >> ( 8*7); 
	chessboard[b] = chessboard[B] >> ( 8*7); 
	chessboard[k] = chessboard[K] >> ( 8*7); 
	chessboard[q] = chessboard[Q] >> ( 8*7); 
	chessboard[n] = chessboard[N] >> ( 8*7);
	print_board(&b_t);
	pop_bit(chessboard +k, b2);
	set_bit(chessboard + k, a3);
	print_board(&b_t);

}
