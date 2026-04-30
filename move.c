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

void generate_pawn_moves(board_t *b_t, int piece)
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
                            printf("%s%sq pawn promotion\n", get_square(source_square), get_square(target_square));
                            printf("%s%sr pawn promotion\n", get_square(source_square), get_square(target_square));
                            printf("%s%sb pawn promotion\n", get_square(source_square), get_square(target_square));
                            printf("%s%sn pawn promotion\n", get_square(source_square), get_square(target_square));
                        }
                        
                        else
                        {
                            printf("%s%s  pawn push\n", get_square(source_square), get_square(target_square));
                            
                            if ((source_square >= a2 && source_square <= h2) && !get_bit(b_t->occupancies[both], target_square - 8))
                                printf("%s%s  double pawn push\n", get_square(source_square), get_square(target_square - 8));
                        }
                    }
                    
                    attacks = pawn_attacks[b_t->side][source_square] & b_t->occupancies[black];
                    
                    while (attacks)
                    {
                        target_square = get_ls1b_index(attacks);
                        
                        if (source_square >= a7 && source_square <= h7)
                        {
                            printf("%s%sq pawn promotion capture\n", get_square(source_square), get_square(target_square));
                            printf("%s%sr pawn promotion capture\n", get_square(source_square), get_square(target_square));
                            printf("%s%sb pawn promotion capture\n", get_square(source_square), get_square(target_square));
                            printf("%s%sn pawn promotion capture\n", get_square(source_square), get_square(target_square));
                        }
                        
                        else
                            printf("%s%s  pawn capture\n", get_square(source_square), get_square(target_square));
                        
                        pop_bit(&attacks, target_square);
                    }
                    
                    if (b_t->enpassant != no_sq)
                    {
                        U64 enpassant_attacks = pawn_attacks[b_t-> side][source_square] & (1ULL << b_t->enpassant);
                        
                        if (enpassant_attacks)
                        {
                            int target_enpassant = get_ls1b_index(enpassant_attacks);
                            printf("%s%s  pawn enpassant capture\n", get_square(source_square), get_square(target_enpassant));
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
                            printf("%s%sq pawn promotion\n", get_square(source_square), get_square(target_square));
                            printf("%s%sr pawn promotion\n", get_square(source_square), get_square(target_square));
                            printf("%s%sb pawn promotion\n", get_square(source_square), get_square(target_square));
                            printf("%s%sn pawn promotion\n", get_square(source_square), get_square(target_square));
                        }
                        
                        else
                        {
                            printf("%s%s  pawn push\n", get_square(source_square), get_square(target_square));
                            
                            if ((source_square >= a7 && source_square <= h7) && !get_bit(b_t->occupancies[both], target_square + 8))
                                printf("%s%s  double pawn push\n", get_square(source_square), get_square(target_square + 8));
                        }
                    }
                    
                    attacks = pawn_attacks[b_t->side][source_square] & b_t->occupancies[white];
                    
                    while (attacks)
                    {
                        target_square = get_ls1b_index(attacks);
                        
                        if (source_square >= a2 && source_square <= h2)
                        {
                            printf("%s%sq pawn promotion capture\n", get_square(source_square), get_square(target_square));
                            printf("%s%sr pawn promotion capture\n", get_square(source_square), get_square(target_square));
                            printf("%s%sb pawn promotion capture\n", get_square(source_square), get_square(target_square));
                            printf("%s%sn pawn promotion capture\n", get_square(source_square), get_square(target_square));
                        }
                        
                        else
                            printf("%s%s  pawn capture\n", get_square(source_square), get_square(target_square));
                        
                        pop_bit(&attacks, target_square);
                    }
                    
                    if (b_t->enpassant != no_sq)
                    {
                        U64 enpassant_attacks = pawn_attacks[b_t->side][source_square] & (1ULL << b_t->enpassant);
                        
                        if (enpassant_attacks)
                        {
                            int target_enpassant = get_ls1b_index(enpassant_attacks);
                            printf("%s%s  pawn enpassant capture\n", get_square(source_square), get_square(target_enpassant));
                        }
                    }
                    
                    pop_bit(&bitboard, source_square);
                }
            }
        }

}
void generate_castling_moves(board_t *b_t, int piece)
{
            if (piece == K)
            {
                if (b_t->castle & wk)
                {
                    if (!get_bit(b_t->occupancies[both], f1) && !get_bit(b_t->occupancies[both], g1))
                    {
                        if (!is_square_attacked(e1, black, b_t) && !is_square_attacked(f1, black, b_t))
                            printf("e1g1  castling move\n");
                    }
                }
                if (b_t->castle & wq)
                {
                    if (!get_bit(b_t->occupancies[both], d1) && !get_bit(b_t->occupancies[both], c1) && !get_bit(b_t->occupancies[both], b1))
                    {
                        if (!is_square_attacked(e1, black, b_t) && !is_square_attacked(d1, black, b_t))
                            printf("e1c1  castling move\n");
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
                            printf("e8g8  castling move\n");
                    }
                }
                
                if (b_t->castle & bq)
                {
                    if (!get_bit(b_t->occupancies[both], d8) && !get_bit(b_t->occupancies[both], c8) && !get_bit(b_t->occupancies[both], b8))
                    {
                        if (!is_square_attacked(e8, white, b_t) && !is_square_attacked(d8, white, b_t))
                            printf("e8c8  castling move: \n");
                    }
                }
            }
}
void generate_knight_moves(board_t *b_t, int piece)
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
                        printf("%s%s  piece quiet move\n", get_square(source_square), get_square(target_square));
                    
                    else
                        printf("%s%s  piece capture\n", get_square(source_square), get_square(target_square));
                    
                    pop_bit(&attacks, target_square);
                }
                
                
                pop_bit(&bitboard, source_square);
            }
        }

}
void generate_bishop_moves(board_t *b_t, int piece)
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
                        printf("%s%s  piece quiet move\n", get_square(source_square), get_square(target_square));
                    
                    else
                        printf("%s%s  piece capture\n", get_square(source_square), get_square(target_square));
                    
                    pop_bit(&attacks, target_square);
                }
                
                
                pop_bit(&bitboard, source_square);
            }
        }
}
void generate_rook_moves(board_t *b_t, int piece)
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
                        printf("%s%s  piece quiet move\n", get_square(source_square), get_square(target_square));
                    
                    else
                        printf("%s%s  piece capture\n", get_square(source_square), get_square(target_square));
                    
                    pop_bit(&attacks, target_square);
                }
                
                
                pop_bit(&bitboard, source_square);
            }
        }
}
void generate_queen_moves(board_t *b_t, int piece)
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
                        printf("%s%s  piece quiet move\n", get_square(source_square), get_square(target_square));
                    
                    else
                        printf("%s%s  piece capture\n", get_square(source_square), get_square(target_square));
                    
                    pop_bit(&attacks, target_square);
                }
                
                
                pop_bit(&bitboard, source_square);
            }
        }
}

void generate_king_moves(board_t *b_t, int piece)
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
                        printf("%s%s  piece quiet move\n", get_square(source_square), get_square(target_square));
                    
                    else
                        printf("%s%s  piece capture\n", get_square(source_square), get_square(target_square));
                    
                    pop_bit(&attacks, target_square);
                }

                pop_bit(&bitboard, source_square);
            }
            generate_castling_moves(b_t, piece);
        }
}

void generate_moves(board_t *b_t)
{
    for (int piece = P; piece <= k; piece++)
    {
        if(piece == P || piece == p)
        {
            generate_pawn_moves(b_t, piece);
        }
       else if (piece == N || piece == n)
       {
           generate_knight_moves(b_t, piece);
       }
       else if (piece == B || piece == b)
       {
           generate_bishop_moves(b_t, piece);
       }
       else if (piece == R || piece == r)
       {
           generate_rook_moves(b_t, piece);
       }
       else if (piece == Q || piece == q)
       {
           generate_queen_moves(b_t, piece);
       }
       else if (piece == K || piece == k)
       {
           generate_king_moves(b_t, piece);
       }
    }
}


void print_move(int move)
{
    printf("\t%s%s%c\t%s\t%d\t%d\t%d\t\t%d\n", get_square(get_move_source(move)), get_square(get_move_target(move)), 
            promoted_pieces[get_move_promoted(move)], unicode_pieces[get_move_piece(move)], 
            get_move_capture(move)? 1 : 0, get_move_double(move)? 1 : 0, get_move_enpassant(move)? 1 : 0, get_move_castling(move) ? 1 : 0);
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

