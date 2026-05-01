#ifndef TEST_H
#define TEST_H
#include "bitboard.h"
#include "move.h"




static inline void perft_driver(int depth, board_t *b_t, long long  *nodes)
{
    
    if (depth == 0)
    {
        (*nodes)++;
        return;
    }
    long long  node = 0;
    board_t save;
    moves move_list;
    generate_moves(b_t, &move_list);

    
    for (int move_count = 0; move_count < move_list.count; move_count++)
    {   
        copy_board(b_t);
        if (depth == DEPTH)
        {
            node = *nodes;
        }
        if (!make_move(b_t,move_list.moves[move_count], all_moves))
        {
            continue;
        }
        perft_driver(depth - 1, b_t, nodes);
        restore_board(b_t);
        if (depth == DEPTH)
        {
            print_move_test(move_list.moves[move_count]);
            printf(": %lld\n", *nodes - node);
            node = *nodes;
        }
    }
}
#endif