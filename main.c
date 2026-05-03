#include "attacks.h"
#include "random.h"
#include "bitboard.h"
#include "move.h"
#include "test.h"
#include "uci.h"
#define tricky_position "r3k2r/p1ppqpP1/bn2pnp1/3PN3/1p2P3/2N2Q2/PPPBBPpP/R3K2R w KQkq - 0 1"
#define killer_position "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"
#define cmk_position "r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9 "
#define start_position "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define test "4k3/3N4/8/8/8/8/8/8 b KQkq - 0 1" 
int main()
{
    init_all();
    board_t b_t; 
    moves move_list;
    long long  nodes = 0;
    
    parse_fen(tricky_position, &b_t);
    print_board(&b_t);
    int move = parse_move("g7g8q", &b_t);
    if (move)
        make_move(&b_t, move, all_moves);
     else
        printf("Illegal move\n");
    print_board(&b_t);
    printf("\n");
    generate_moves(&b_t, &move_list);
    printf("Generated moves:\n");
    for (int i = 0; i < move_list.count; i++)
    {
        copy_board((&b_t));
        if (!make_move(&b_t, move_list.moves[i], all_moves))
        {
            continue;
        }
        print_move(move_list.moves[i]);
        printf("\n");
        restore_board((&b_t));
    }
    return 0;
}
