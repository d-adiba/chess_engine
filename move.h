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




static inline void add_move(moves *move_list, int move)
{
    move_list->moves[move_list->count] = move;
    move_list->count++;
}
void generate_pawn_moves(board_t *b_t, int piece);
void generate_knight_moves(board_t *b_t, int piece);
void generate_bishop_moves(board_t *b_t, int piece);
void generate_rook_moves(board_t *b_t, int piece);
void generate_queen_moves(board_t *b_t, int piece);
void generate_king_moves(board_t *b_t, int piece);
void generate_castling_moves(board_t *b_t, int piece);
void generate_moves(board_t *b_t);
void print_move(int move);
void print_move_list(moves *move_list);

#endif 
