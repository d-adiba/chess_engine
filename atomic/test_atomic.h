#ifndef TEST_ATOMIC
#define TEST_ATOMIC
#include "move_atomic.h"


static inline void perft_atomic_driver(int depth, board_t *b_t, long long  *nodes)
{
    
    if (depth == 0)
    {
        (*nodes)++;
        return;
    }
    moves move_list;
    generate_atomic_moves(b_t, &move_list);

    for (int move_count = 0; move_count < move_list.count; move_count++)
    {   
        copy_board(b_t);
        if (!make_atomic_move(b_t,move_list.moves[move_count], all_moves))
        {
            continue;
        }
        perft_atomic_driver(depth - 1, b_t, nodes);
        restore_board(b_t);
    }
}

static inline void perft_atomic_test(int depth, board_t *b_t)
{
    long long  nodes = 0;
    long start = 0;
    long end = 0;
    long long  node = 0;
    moves move_list;
    generate_atomic_moves(b_t, &move_list);


   
    printf("\nPerformance  test to depth %d...\n", depth);
    start = get_time_ms();

    for (int move_count = 0; move_count < move_list.count; move_count++)
    {   
        copy_board(b_t);
        if (!make_atomic_move(b_t,move_list.moves[move_count], all_moves))
        {
            continue;
        }
        perft_atomic_driver(depth - 1, b_t, &node);
        restore_board(b_t);
        print_move_test(move_list.moves[move_count]);
        printf(": %lld\n", node);
        nodes += node;
        node = 0;
    }
    end = get_time_ms();
    printf("\nDepth:\t%d\nNodes:\t%lld\n", depth, nodes);
    printf("Time:\t%ldms\n", end - start);
}




#endif