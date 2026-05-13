#ifndef MOVE_ATOMIC
#define MOVE_ATOMIC

#include "../bitboard.h"
#include "../move.h"
#include "../attacks.h"
#include "../uci.h"
#include <immintrin.h>

extern U64 mask_atomic_explosion[64];
extern U64 atomic_explosion_attacks[64][512];


void init_atomic_explosion_mask();
void init_atomic_explosion_attacks();


static inline U64 get_atomic_explosion_attacks(square sq, U64 occupancy)
{
    occupancy &= mask_atomic_explosion[sq];
    occupancy = _pext_u64(occupancy, mask_atomic_explosion[sq]);
    return atomic_explosion_attacks[sq][occupancy];
}
static inline int is_square_atomically_attacked(int  sq, side s, board_t *b_t)
{
    if ((s == white) && (pawn_attacks[black][sq] & b_t->board[P])) return 1;
    
    if ((s == black) && (pawn_attacks[white][sq] & b_t->board[p])) return 1;
    if (knight_attacks[sq] & ((s == white) ? b_t->board[N] : b_t->board[n])) return 1;
    
    if (get_bishop_attacks(sq, b_t->occupancies[both]) & ((s == white) ? b_t->board[B] : b_t->board[b])) return 1;

    if (get_rook_attacks(sq, b_t->occupancies[both]) & ((s == white) ? b_t->board[R] : b_t->board[r])) return 1;    

    if (get_queen_attacks(sq, b_t->occupancies[both]) & ((s == white) ? b_t->board[Q] : b_t->board[q])) return 1;
    

    return 0;
}

static inline int  make_atomic_move(board_t *b_t, int move, int move_flag)
{
    
    if (move_flag == all_moves)
    {
        copy_board(b_t);
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
            U64 mask = get_atomic_explosion_attacks(target_square, b_t->occupancies[both]);
            for (int i = P; i <= k; i++)
            {
                if (i== P || i == p)
                continue ;
                b_t->board[i] &= ~mask;
            }
            if (b_t->side == white)
            {
                if (!(b_t->board[k]) || !(b_t->board[r]))
                {
                    b_t->castle  &= castling_rights[e8];
                }
                else if (!get_bit(b_t->board[r], h8))
                {
                   b_t->castle  &= castling_rights[h8];
                }
                else if  (!get_bit(b_t->board[r], a8))
                {
                   b_t->castle  &= castling_rights[a8];
                }
            }
            else
            {
                if (!(b_t->board[K]) || !(b_t->board[R]))
                {
                    b_t->castle  &= castling_rights[e1];
                }
                else if (!get_bit(b_t->board[R], h1))
                {
                   b_t->castle  &= castling_rights[h1];
                }
                else if  (!get_bit(b_t->board[R], a1))
                {
                   b_t->castle  &= castling_rights[a1];
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
        b_t->castle &= castling_rights[source_square];
        b_t->castle &= castling_rights[target_square];
        b_t->side = (b_t->side == white) ? black: white;
        memset(b_t->occupancies, 0, sizeof(b_t->occupancies));
        for (int i = P; i <= K; i++)
        {
            b_t->occupancies[white] |= b_t->board[i];
        }
        for (int i = p; i <= k; i++)
        {
            b_t->occupancies[black] |= b_t->board[i];
        }
        b_t->occupancies[both] = b_t->occupancies[white] | b_t->occupancies[black];
        if (!((b_t->side == white) ? b_t->board[k] : b_t->board[K]))
        {
            restore_board(b_t);
            return 0;
        }
        else if (is_square_atomically_attacked((b_t->side == white) ? get_ls1b_index(b_t->board[k]) : get_ls1b_index(b_t->board[K]), b_t->side, b_t))
        {
            // l'idéé ici est de surveillé les moves ou le roi est attaqué mais dont le roi adverse est dans la zone d'explosion ; ces
            // moves deviennent ainsi legaux car ce derniers n'est plus attaquable
            /*
            *une fois avoir vérifié que le roi est sous attack on calcul le bitboard resultant de son explosion et de celui du roi adverse;
            * si ce dernier n'est pas nulle alors le roi adverse sera detruit en cas d'attaque du roi courant donc le mouv devient valid 
            *
            */
            
            int piece = (b_t->side == white)? k : K;
            int piece_inv = (b_t->side == white)? K : k;
            if(!get_atomic_explosion_attacks(get_ls1b_index(b_t->board[piece]),b_t->board[piece_inv]))
            {
                restore_board(b_t);
                return 0;
            }
            else 
            {
                return 1;
            }
           
        }
        else  
            return 1;
    }
    
    else
    {
        if (get_move_capture(move))
            make_atomic_move(b_t, move, all_moves);
        else
            return 0;
    }
    return 0; 
}

static inline void generate_atomic_moves(board_t *b_t, moves *move_list)
{
    move_list->count = 0;

    int source_square, target_square;
    
    U64 bitboard, attacks;
    
    for (int piece = P; piece <= k; piece++)
    {
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
                            
                            if ((source_square >= a2 && source_square <= h2) && !get_bit(b_t->occupancies[both], (target_square - 8)))
                                add_move(move_list, encode_move(source_square, (target_square - 8), piece, 0, 0, 1, 0, 0));
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
                            add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
                        
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
            
            if (piece == K)
            {
                if (b_t->castle & wk)
                {
                    if (!get_bit(b_t->occupancies[both], f1) && !get_bit(b_t->occupancies[both], g1))
                    {
                        if (!is_square_atomically_attacked(e1, black,b_t) && !is_square_atomically_attacked(f1, black,b_t))
                            add_move(move_list, encode_move(e1, g1, piece, 0, 0, 0, 0, 1));
                    }
                }
                
                if (b_t->castle & wq)
                {
                    if (!get_bit(b_t->occupancies[both], d1) && !get_bit(b_t->occupancies[both], c1) && !get_bit(b_t->occupancies[both], b1))
                    {
                        if (!is_square_atomically_attacked(e1, black,b_t) && !is_square_atomically_attacked(d1, black,b_t))
                            add_move(move_list, encode_move(e1, c1, piece, 0, 0, 0, 0, 1));
                    }
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
                            
                            if ((source_square >= a7 && source_square <= h7) && !get_bit(b_t->occupancies[both], (target_square + 8)))
                                add_move(move_list, encode_move(source_square, (target_square + 8), piece, 0, 0, 1, 0, 0));
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
                            add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
                        
                        pop_bit(& attacks, target_square);
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
            
            if (piece == k)
            {
                if (b_t->castle & bk)
                {
                    if (!get_bit(b_t->occupancies[both], f8) && !get_bit(b_t->occupancies[both], g8))
                    {
                        if (!is_square_atomically_attacked(e8, white,b_t) && !is_square_atomically_attacked(f8, white,b_t))
                            add_move(move_list, encode_move(e8, g8, piece, 0, 0, 0, 0, 1));
                    }
                }
                
                if (b_t->castle & bq)
                {
                    if (!get_bit(b_t->occupancies[both], d8) && !get_bit(b_t->occupancies[both], c8) && !get_bit(b_t->occupancies[both], b8))
                    {
                        if (!is_square_atomically_attacked(e8, white,b_t) && !is_square_atomically_attacked(d8, white,b_t))
                            add_move(move_list, encode_move(e8, c8, piece, 0, 0, 0, 0, 1));
                    }
                }
            }
        }
        
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
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
                    
                    else
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
                    
                    pop_bit(&attacks, target_square);
                }
                
                
                pop_bit(&bitboard, source_square);
            }
        }
        
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
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
                    
                    else
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
                    
                    pop_bit(&attacks, target_square);
                }
                
                
                pop_bit(&bitboard, source_square);
            }
        }
        
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
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
                    
                    else
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
                    
                    pop_bit(&attacks, target_square);
                }
                
                
                pop_bit(&bitboard, source_square);
            }
        }
        
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
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
                    
                    else
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
                    
                    pop_bit(&attacks, target_square);
                }
                
                
                pop_bit(&bitboard, source_square);
            }
        }

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
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
                    
                    else
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
                    
                    pop_bit(&attacks, target_square);
                }

                pop_bit(&bitboard, source_square);
            }
        }
    }
   
}


#endif
