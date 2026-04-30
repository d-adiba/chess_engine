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

void generate_pawn_moves(board_t *b_t, int piece, moves *move_list)
{
    square source_square, target_square;
    U64 bitboard, attacks;
        bitboard = b_t->board[piece];
        
        if (b_t->side == white)
        {
            if (piece == P)
            {
                while (bitboard)
                {
                    source_square = get_ls1b_index(bitboard);
                    target_square = source_square - 8;
                    if (!(target_square < a8) && !get_bit(b_t->occupancies[both], target_square))
                    {
                        if (source_square >= a7 && source_square <= h7)
                        {
                            add_move(move_list, encode_move(source_square, target_square, piece, Q, 0, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, R, 0, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, B, 0, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, N, 0, 0, 0, 0));
                        }
                        
                        else
                        {
                            add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
                            
                            if ((source_square >= a2 && source_square <= h2) && !get_bit(b_t->occupancies[both], target_square - 8))
                            {
                                add_move(move_list, encode_move(source_square, target_square - 8, piece, 0, 0, 1, 0, 0));
                            }
                        }
                    }
                    
                    attacks = pawn_attacks[b_t->side][source_square] & b_t->occupancies[black];
                    
                    while (attacks)
                    {
                        target_square = get_ls1b_index(attacks);
                        
                        if (source_square >= a7 && source_square <= h7)
                        {
                            add_move(move_list, encode_move(source_square, target_square, piece, Q, 1, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, R, 1, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, B, 1, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, N, 1, 0, 0, 0));
                        }
                        
                        else
                        {
                            add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
                        }
                        pop_bit(&attacks, target_square);
                    }
                    
                    if (b_t->enpassant != no_sq)
                    {
                        U64 enpassant_attacks = pawn_attacks[b_t->side][source_square] & (1ULL << b_t->enpassant);
                        
                        if (enpassant_attacks)
                        {
                            int target_enpassant = get_ls1b_index(enpassant_attacks);
                            add_move(move_list, encode_move(source_square, target_enpassant, piece, 0, 1, 0, 1, 0));
                        }
                    }
                    
                    pop_bit(&bitboard, source_square);
                }
            }
        }
        
        else
        {
            if (piece == p)
            {
                while (bitboard)
                {
                    source_square = get_ls1b_index(bitboard);
                    
                    target_square = source_square + 8;
                    
                    if (!(target_square > h1) && !get_bit(b_t->occupancies[both], target_square))
                    {
                        if (source_square >= a2 && source_square <= h2)
                        {
                            add_move(move_list, encode_move(source_square, target_square, piece, q, 0, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, r, 0, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, b, 0, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, n, 0, 0, 0, 0));   
                        }
                        
                        else
                        {
                            add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
                            if ((source_square >= a7 && source_square <= h7) && !get_bit(b_t->occupancies[both], target_square + 8))
                            {
                                add_move(move_list, encode_move(source_square, target_square + 8, piece, 0, 0, 1, 0, 0));
                            }
                        }
                    }
                    attacks = pawn_attacks[b_t->side][source_square] & b_t->occupancies[white];
                    
                    while (attacks)
                    {
                        target_square = get_ls1b_index(attacks);
                        
                        if (source_square >= a2 && source_square <= h2)
                        {
                            add_move(move_list, encode_move(source_square, target_square, piece, q, 1, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, r, 1, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, b, 1, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, n, 1, 0, 0, 0));   
                        }
                        
                        else
                        {
                            add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
                        }
                        pop_bit(&attacks, target_square);
                    }
                    
                    if (b_t->enpassant != no_sq)
                    {
                        U64 enpassant_attacks = pawn_attacks[b_t->side][source_square] & (1ULL << b_t->enpassant);
                        
                        if (enpassant_attacks)
                        {
                            int target_enpassant = get_ls1b_index(enpassant_attacks);
                            add_move(move_list, encode_move(source_square, target_enpassant, piece, 0, 1, 0, 1, 0));
                        }
                    }
                    
                    pop_bit(&bitboard, source_square);
                }
            }
        }

    
}
void generate_castling_moves(board_t *b_t, int piece, moves *move_list)
{
            if (piece == K)
            {
                if (b_t->castle & wk)
                {
                    if (!get_bit(b_t->occupancies[both], f1) && !get_bit(b_t->occupancies[both], g1))
                    {
                        if (!is_square_attacked(e1, black, b_t) && !is_square_attacked(f1, black, b_t))
                        {
                            add_move(move_list, encode_move(e1, g1, piece, 0, 0, 0, 0, 1));
                        }
                    }
                }
                if (b_t->castle & wq)
                {
                    if (!get_bit(b_t->occupancies[both], d1) && !get_bit(b_t->occupancies[both], c1) && !get_bit(b_t->occupancies[both], b1))
                    {
                        if (!is_square_attacked(e1, black, b_t) && !is_square_attacked(d1, black, b_t))
                        {
                            add_move(move_list, encode_move(e1, c1, piece, 0, 0, 0, 0, 1));
                        }
                    }
                }
            }
             if (piece == k)
            {
                if (b_t->castle & bk)
                {
                    if (!get_bit(b_t->occupancies[both], f8) && !get_bit(b_t->occupancies[both], g8))
                    {
                        if (!is_square_attacked(e8, white, b_t) && !is_square_attacked(f8, white, b_t))
                        {
                            add_move(move_list, encode_move(e8, g8, piece, 0, 0, 0, 0, 1));
                        }
                    }
                }
                
                if (b_t->castle & bq)
                {
                    if (!get_bit(b_t->occupancies[both], d8) && !get_bit(b_t->occupancies[both], c8) && !get_bit(b_t->occupancies[both], b8))
                    {
                        if (!is_square_attacked(e8, white, b_t) && !is_square_attacked(d8, white, b_t))
                        {                            
                             add_move(move_list, encode_move(e8, c8, piece, 0, 0, 0, 0, 1));   
                        }
                     }
                }
            }
}
void generate_knight_moves(board_t *b_t, int piece, moves *move_list)
{
    U64 attacks,  bitboard;
    bitboard = b_t->board[piece];
    int source_square, target_square;

    if ((b_t->side == white) ? piece == N : piece == n)
        {
            while (bitboard)
            {
                source_square = get_ls1b_index(bitboard);
                
                attacks = knight_attacks[source_square] & ((b_t->side == white) ? ~b_t->occupancies[white] : ~b_t->occupancies[black]);
                
                while (attacks)
                {
                    target_square = get_ls1b_index(attacks);    
                    
                    if (!get_bit(((b_t->side == white) ? b_t->occupancies[black] : b_t->occupancies[white]), target_square))
                    {
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
                    }   
                    else
                    {
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));   
                    }
                       
                    pop_bit(&attacks, target_square);
                }
                
                
                pop_bit(&bitboard, source_square);
            }
        }

}
void generate_bishop_moves(board_t *b_t, int piece, moves *move_list)
{
    U64 attacks,  bitboard;
    bitboard = b_t->board[piece];
    int source_square, target_square;
     if ((b_t->side == white) ? piece == B : piece == b)
        {
            while (bitboard)
            {
                source_square = get_ls1b_index(bitboard);
                
                attacks = get_bishop_attacks(source_square, b_t->occupancies[both]) & ((b_t->side == white) ? ~b_t->occupancies[white] : ~b_t->occupancies[black]);
                
                while (attacks)
                {
                    target_square = get_ls1b_index(attacks);    
                    
                    if (!get_bit(((b_t->side == white) ? b_t->occupancies[black] : b_t->occupancies[white]), target_square))
                    {
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
                    }
                    else
                    {
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
                    }
                    
                    pop_bit(&attacks, target_square);
                }
                
                
                pop_bit(&bitboard, source_square);
            }
        }
}
void generate_rook_moves(board_t *b_t, int piece, moves *move_list)
{
    U64 attacks,  bitboard;
    bitboard = b_t->board[piece];
    int source_square, target_square;
     if ((b_t->side == white) ? piece == R : piece == r)
        {
            while (bitboard)
            {
                source_square = get_ls1b_index(bitboard);
                
                attacks = get_rook_attacks(source_square, b_t->occupancies[both]) & ((b_t->side == white) ? ~b_t->occupancies[white] : ~b_t->occupancies[black]);
                
                while (attacks)
                {
                    target_square = get_ls1b_index(attacks);    
                    
                    if (!get_bit(((b_t->side == white) ? b_t->occupancies[black] : b_t->occupancies[white]), target_square))
                    {
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
                    }
                    else
                    {
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
                    }
                    
                    pop_bit(&attacks, target_square);
                }
                
                
                pop_bit(&bitboard, source_square);
            }
        }
}
void generate_queen_moves(board_t *b_t, int piece, moves *move_list)
{
    U64 attacks,  bitboard;
    bitboard = b_t->board[piece];
    int source_square, target_square;
     if ((b_t->side == white) ? piece == Q : piece == q)
        {
            while (bitboard)
            {
                source_square = get_ls1b_index(bitboard);
                
                attacks = get_queen_attacks(source_square, b_t->occupancies[both]) & ((b_t->side == white) ? ~b_t->occupancies[white] : ~b_t->occupancies[black]);
                
                while (attacks)
                {
                    target_square = get_ls1b_index(attacks);    
                    
                    if (!get_bit(((b_t->side == white) ? b_t->occupancies[black] : b_t->occupancies[white]), target_square))
                    {       
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
                    }
                    else
                    {
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
                    }
                    
                    pop_bit(&attacks, target_square);
                }
                
                
                pop_bit(&bitboard, source_square);
            }
        }
}

void generate_king_moves(board_t *b_t, int piece, moves *move_list)
{
        U64 attacks,  bitboard;
    bitboard = b_t->board[piece];
    int source_square, target_square;
     if ((b_t->side == white) ? piece == K : piece == k)
        {
            while (bitboard)
            {
                source_square = get_ls1b_index(bitboard);
                
                attacks = king_attacks[source_square] & ((b_t->side == white) ? ~b_t->occupancies[white] : ~b_t->occupancies[black]);
                
                while (attacks)
                {
                    target_square = get_ls1b_index(attacks);    
                    
                    if (!get_bit(((b_t->side == white) ? b_t->occupancies[black] : b_t->occupancies[white]), target_square))
                    {
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
                    }
                    else
                    {
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
                    }
                    
                    pop_bit(&attacks, target_square);
                }

                pop_bit(&bitboard, source_square);
            }
            generate_castling_moves(b_t, piece, move_list);
        }
}

void generate_moves(board_t *b_t, moves *move_list)
{

    if (!(move_list->count ))
        printf("        No moves in move list  \n");
    for (int piece = P; piece <= k; piece++)
    {
        if(piece == P || piece == p)
        {
            generate_pawn_moves(b_t, piece, move_list);
        }
       else if (piece == N || piece == n)
       {
           generate_knight_moves(b_t, piece, move_list);
       }
       else if (piece == B || piece == b)
       {
           generate_bishop_moves(b_t, piece, move_list);
       }
       else if (piece == R || piece == r)
       {
           generate_rook_moves(b_t, piece, move_list);
       }
       else if (piece == Q || piece == q)
       {
           generate_queen_moves(b_t, piece, move_list);
       }
       else if (piece == K || piece == k)
       {
           generate_king_moves(b_t, piece, move_list);
       }
    }
}


void print_move(int move)
{
    printf("\t%s%s%c\t%s\t%d\t%d\t%d\t\t%d\n", get_square(get_move_source(move)), get_square(get_move_target(move)), 
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

