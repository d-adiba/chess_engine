#include "move.h"
#include "attacks.h"
#include "bitboard.h"
const int  promoted_pieces[]= {
    [Q] ='q',
    [R] ='r',
    [B] ='b',
    [N] ='n',
    [r] = 'r',
    [b] = 'b',
    [n] = 'n',
    [q] = 'q',
}; 

void print_move_test(int move)
{
    printf("%s%s", get_square(get_move_source(move)), get_square(get_move_target(move)));
}
void print_move(int move)
{
    printf("%s%s%c\t%s\t%d\t%d\t%d\t\t%d\n", get_square(get_move_source(move)), get_square(get_move_target(move)), 
            get_move_promoted(move) ? promoted_pieces[get_move_promoted(move)] : ' ', 
            unicode_pieces[get_move_piece(move)], 
            get_move_capture(move)? 1 : 0, 
            get_move_double(move)? 1 : 0, 
            get_move_enpassant(move)? 1 : 0, 
            get_move_castling(move) ? 1 : 0);
}

void print_move_list(moves *move_list)
{
    printf("\n\tmove\tpiece\tcapture\tdouble\tenpassant\tcastling\n\n");
    for (int i = 0; i < move_list->count; i++)
    {
        print_move(move_list->moves[i]);
        printf("\n");
    }
    printf("\tTotal moves: %d\n\n", move_list->count);
}

