#ifndef MOVE_H
#define MOVE_H

#include "bitboard.h"
#include "attacks.h"


/*
          binary move bits                               hexidecimal constants
    
    0000 0000 0000 0000 0011 1111    source square       0x3f
    0000 0000 0000 1111 1100 0000    target square       0xfc0
    0000 0000 1111 0000 0000 0000    piece               0xf000
    0000 1111 0000 0000 0000 0000    promoted piece      0xf0000
    0001 0000 0000 0000 0000 0000    capture flag        0x100000
    0010 0000 0000 0000 0000 0000    double push flag    0x200000
    0100 0000 0000 0000 0000 0000    enpassant flag      0x400000
    1000 0000 0000 0000 0000 0000    castling flag       0x800000
*/

// encode move
#define encode_move(source, target, piece, promoted, capture, double, enpassant, castling) \
    (source) |          \
    (target << 6) |     \
    (piece << 12) |     \
    (promoted << 16) |  \
    (capture << 20) |   \
    (double << 21) |    \
    (enpassant << 22) | \
    (castling << 23)    \
    

#define get_move_source(move) (move & 0x3f)


#define get_move_target(move) ((move & 0xfc0) >> 6)


#define get_move_piece(move) ((move & 0xf000) >> 12)


#define get_move_promoted(move) ((move & 0xf0000) >> 16)


#define get_move_capture(move) (move & 0x100000)


#define get_move_double(move) (move & 0x200000)


#define get_move_enpassant(move) (move & 0x400000)


#define get_move_castling(move) (move & 0x800000)


typedef struct  {
    int moves[256];
    int count;
} moves;

enum { all_moves, only_captures};

static inline int  make_move(board_t *b_t,board_t *save, int move, int move_flag)
{
    if (move_flag == all_moves)
    {
        copy_board(save, b_t);
        int source_square = get_move_source(move);
        int target_square = get_move_target(move);
        int piece = get_move_piece(move);
        int promoted_piece = get_move_promoted(move);
        int capture = get_move_capture(move);
        int double_push = get_move_double(move);
        int enpassant = get_move_enpassant(move);
        int castling = get_move_castling(move);

        pop_bit((b_t->board) + piece, source_square);
        set_bit((b_t->board) + piece, target_square);      
        
        if (capture)
        {
            int start, end; 
            if (b_t->side == white)
            {
                start = p;
                end = k;
            }
            else
            {
                start = P;
                end = K;
            }
            for (int i = start; i <= end; i++)
            {
                if (get_bit(b_t->board[i], target_square))
                {
                    pop_bit(b_t->board + i, target_square);
                    break;
                }
            }

        }
        if (promoted_piece)
        {
            pop_bit(b_t->board + (b_t->side == white ? P : p), target_square);
            set_bit(b_t->board + promoted_piece, target_square);
        }
        if (enpassant)
        {
            if (b_t->side  == white)
            {
                pop_bit(b_t->board + p, target_square+8);
            }
            else
            {
                pop_bit(b_t->board + P, target_square-8);
            }
        }
        b_t->enpassant = no_sq;
        if (double_push)
        {         
            if (b_t->side == white)
                b_t->enpassant = target_square + 8;
            else
                b_t->enpassant = target_square - 8;
        }
        if (castling)
        {
            if (target_square == g1)
            {
                pop_bit(b_t->board + R, h1);
                set_bit(b_t->board + R, f1);
            }
            else if (target_square == c1)
            {
                pop_bit(b_t->board + R, a1);
                set_bit(b_t->board + R, d1);
            }
            else if (target_square == g8)
            {
                pop_bit(b_t->board + r, h8);
                set_bit(b_t->board + r, f8);
            }
            else if (target_square == c8)
            {
                pop_bit(b_t->board + r, a8);
                set_bit(b_t->board + r, d8);
            }
        }
    }
    
    else
    {
        if (get_move_capture(move))
        make_move(b_t, save , move, all_moves);
        else
            return 0;
    }



}


static inline void add_move(moves *move_list, int move)
{
    move_list->moves[move_list->count] = move;
    move_list->count++;
}
void generate_pawn_moves(board_t *b_t, int piece, moves *move_list);
void generate_knight_moves(board_t *b_t, int piece, moves *move_list);
void generate_bishop_moves(board_t *b_t, int piece, moves *move_list);
void generate_rook_moves(board_t *b_t, int piece, moves *move_list);
void generate_queen_moves(board_t *b_t, int piece, moves *move_list);
void generate_king_moves(board_t *b_t, int piece, moves *move_list);
void generate_castling_moves(board_t *b_t, int piece, moves *move_list);
void generate_moves(board_t *b_t, moves *move_list);
void print_move(int move);
void print_move_list(moves *move_list);

#endif 
