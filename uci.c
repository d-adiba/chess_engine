
#include "uci.h"



int parse_move(char *move_str, board_t *b_t)
{
    moves move_list;
    generate_moves(b_t, &move_list);
    int move; 
    int source_square = (move_str[0] - 'a') + (8 - (move_str[1] - '0')) * 8;
    int target_square = (move_str[2] - 'a') + (8 - (move_str[3] - '0')) * 8;
    int promotion_piece =  (move_str[4]) ? char_pieces[move_str[4]] : 0;
    printf("Source square: %s, Target square: %s, Promotion piece: %c\n", get_square(source_square), get_square(target_square), promotion_piece ? move_str[4] : ' ');
    if (promotion_piece)
    {
        promotion_piece = (b_t->side == black) ? char_pieces[move_str[4]] : char_pieces[move_str[4] - 32];

    }
    for (int move_count = 0; move_count < move_list.count; move_count++)
    {
        move  =  move_list.moves[move_count];
        if (get_move_source(move) == source_square && get_move_target(move) == target_square && get_move_promoted(move) == promotion_piece)
        {
            return move;
        }
        
    }
    return 0;   
}