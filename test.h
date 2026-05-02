#ifndef TEST_H
#define TEST_H
#include "bitboard.h"
#include "move.h"




static inline void perft_driver(int depth, board_t *b_t, long long  *nodes, int *duration)
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

    if (depth == DEPTH)
    {
        *duration = get_time_ms() ;
        print_board(b_t);
        printf("\n\nMove: Nodes\n");
    }
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
        perft_driver(depth - 1, b_t, nodes, duration);
        restore_board(b_t);
        if (depth == DEPTH)
        {
            print_move_test(move_list.moves[move_count]);
            printf(": %lld\n", *nodes - node);
            node = *nodes;
        }
    }
    if (depth == DEPTH)
    *duration = get_time_ms() - *duration;

}

static inline void perft_test(int depth, board_t *b_t)
{
    long long  nodes = 0;
    int duration = 0;
    printf("\nPerformance  test to depth %d...\n", depth);;
    perft_driver(depth, b_t, &nodes, &duration);
    printf("\nDepth:\t%d\nNodes:\t%lld\n", depth, nodes);
    printf("Time:\t%dms\n", duration);
}

#endif