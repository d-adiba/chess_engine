#include "attacks.h"
#include "random.h"
#include "bitboard.h"
#include "move.h"
#define tricky_position "r3k2r/p2pqpP1/bn2pnp1/3pN3/Pp2pP3/1N2Q1p/1PPBBPPP/R3K2R w KQkq a3 0 1 "
#define killer_position "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"
#define cmk_position "r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9 "

int main()
{
    init_all();
    board_t b_t, save;
    moves move_list;
    move_list.count = 0;
    parse_fen(tricky_position, &b_t);
   
    generate_moves(&b_t, &move_list);
   
    for (int i = 0; i < move_list.count; i++)
    {
        int move = move_list.moves[i];
        copy_board(&save, &b_t);
        print_board(&b_t);
        make_move(&b_t, &save, move, all_moves);
        print_board(&b_t);
        getchar();
        restore_board(&b_t, &save);
    }
}
