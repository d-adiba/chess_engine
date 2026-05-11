#ifndef EVAL_H
#define EVAL_H
#include "move_atomic.h"

extern const double materiel_score[13];



extern const int pawn_score[64];

extern const int knight_score[64];

extern const int bishop_score[64]; 

extern const int rook_score[64]; 

extern const int king_score[64]; 

extern int mirror_score[128];


static inline double evaluate_score(board_t *b_t)
{
    double score = 0; 
    U64 bitboard = 0ULL;
    int  sq; 
    for (int bb_piece =P ; bb_piece <=k; bb_piece++)
    {
        
        bitboard = b_t->board[bb_piece]; 
        while (bitboard)
        {
            sq = get_ls1b_index(bitboard);
            score  += materiel_score[bb_piece];
            switch (bb_piece)
            {
                case P: score  += pawn_score[sq]; break;
                case N: score  += knight_score[sq]; break;
                case B: score  += bishop_score[sq]; break;
                case R: score  += rook_score[sq]; break;
                case K: score  += king_score[sq]; break;

                case p: score -= pawn_score[mirror_score[sq]]; break; 
                case n: score -= knight_score[mirror_score[sq]]; break; 
                case b: score -= bishop_score[mirror_score[sq]]; break; 
                case r: score -= rook_score[mirror_score[sq]]; break; 
                case k: score -= king_score[mirror_score[sq]]; break;

            }
           
            pop_bit(&bitboard , (square) sq);
        }
    } 
    if (b_t->side == black) score *= -1;
    return score;  

}

static inline float negamax(float  alpha,  float beta, int depth, board_t *b_t, int *ply, int *best_move, long long *nodes)
{
    int bmsf;  // best move so far 
    float old_alpha; 
    if (depth == 0) return evaluate_score(b_t);
    int move_count;

    (*nodes)++;
    moves move_liste;
    generate_atomic_moves(b_t, &move_liste);
    old_alpha = alpha;
    for (move_count = 0; move_count < move_liste.count; move_count++)
    {
        copy_board((b_t));
        (*ply)++;
        if (make_atomic_move(b_t,move_liste.moves[move_count],all_moves) == 0)
        {
            (*ply)--; 
            continue;
        }
        float score = -negamax(-beta, -alpha, depth -1 , b_t, ply, best_move, nodes); 
        restore_board(b_t);
        (*ply)--;
        if(score >= beta) return beta; 
        if (score > alpha) 
        {
            alpha = score;
            if (*ply == 0)
            {
                bmsf = move_liste.moves[move_count];
                
            }
        }
        

    }
    if (old_alpha != alpha)
        *best_move = bmsf; 
    //else *best_move = move_liste.moves[move_count -1];
    return alpha; 

}

int search_position(int depth, board_t *b_t);

#endif