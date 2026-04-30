#include "attacks.h"
#include "random.h"
#include "bitboard.h"
#include "move.h"
#define tricky_position "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 "
#define killer_position "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"
#define cmk_position "r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9 "

int main()
{
    init_all();
    board_t b_t;
    moves move_list;
    move_list.count = 0;
    parse_fen(tricky_position, &b_t);
    print_board(&b_t);
   
    printf("Move list for position : %s\n\n", tricky_position);
    generate_moves(&b_t, &move_list);
    printf("Total moves : %d\n\n", move_list.count);
    print_move_list(&move_list);
}
