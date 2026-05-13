#ifndef EVAL_H
#define EVAL_H
#include "move_atomic.h"

static inline void safety(board_t *b_t, board_t *s)
{
    memcpy(s->board, b_t->board, sizeof(s->board));
    memcpy(s->occupancies, b_t->occupancies, sizeof(s->occupancies));
    s->side = b_t->side;
    s->enpassant = b_t->enpassant;
    s->castle = b_t->castle;
}
static inline void restore(board_t *b_t, board_t *s)
{
    memcpy(b_t->board, s->board, sizeof(s->board));
    memcpy(b_t->occupancies, s->occupancies, sizeof(s->occupancies));
    b_t->side = s->side;
    b_t->enpassant = s->enpassant;
    b_t->castle = s->castle;
}
extern const int materiel_score[13];



extern const int pawn_score[64];

extern const int knight_score[64];

extern const int bishop_score[64]; 

extern const int rook_score[64]; 

extern const int king_score[64]; 

extern int mirror_score[128];

extern const  int mvv_lva[12][12];



void print_move_scores(moves *move_list, board_t *b_t);

void merge(int arr[], int left, int mid, int right, board_t *b_t);
void merge_sort(int arr[], int left, int right, board_t *b_t);
static inline int sort_moves(moves *move_list, board_t *b_t)
{
    merge_sort(move_list->moves,0,move_list->count,b_t);
    return 0;
}
static inline int score_move(int move, board_t *b_t)
{
    if (get_move_capture(move))
    {
        int  target_piece, target_square; 
        int  piece = get_move_piece(move);
        target_square = get_move_target(move);  
        if (piece <= K)
            target_piece = p;
        else
            target_piece = P; 
        while (target_piece <=k)
        {
            if (get_bit(b_t->board[target_piece], target_square))
                break;
            target_piece++;
        }
        return mvv_lva[piece][target_piece] + 10000; 
    }
    return 0; 

}


static inline int evaluate_score(board_t *b_t)
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
static inline int quiescence(int alpha , int beta, board_t *b_t, int *ply, long long *nodes)
{
    (*nodes)++;
    int evaluation = evaluate_score(b_t);
    if (evaluation>= beta)
        return beta; 
    if (evaluation > alpha)
        alpha = evaluation;
    board_t save;

    moves move_liste;
    generate_atomic_moves(b_t, &move_liste);
    sort_moves(&move_liste,b_t);
    for (int move_count = 0; move_count < move_liste.count; move_count++)
    {
        safety(b_t,&save);
        (*ply)++;
        if (make_atomic_move(b_t,move_liste.moves[move_count],only_captures) == 0)
        {
            (*ply)--; 
            continue;
        }


        int score = -quiescence(-beta, -alpha, b_t, ply, nodes); 
        restore(b_t,&save);
        (*ply)--;
        if(score >= beta) return beta; 
        if (score > alpha) 
        {
            alpha = score;
        }
        

    }

    return alpha; 
}

static inline int negamax(int  alpha,  int beta, int depth, board_t *b_t, int *ply, int *best_move, long long *nodes)
{
    if (depth == 0) return quiescence( alpha, beta, b_t, ply, nodes);

    (*nodes)++;

    int in_check = 0; 
    if ((b_t->side == white)? b_t->board[K] : b_t->board[k])
        in_check =  is_square_atomically_attacked((b_t->side == white)? get_ls1b_index(b_t->board[K]) : get_ls1b_index(b_t->board[k]), b_t->side ^ 1 , b_t);

    if(in_check) depth++;


    int bmsf;  // best move so far 
    int old_alpha = alpha;
    int legal_move = 0;
  
    


    
    moves move_liste;
    generate_atomic_moves(b_t, &move_liste);
    sort_moves(&move_liste,b_t);
    board_t save; 
    
    
    for (int move_count = 0; move_count < move_liste.count; move_count++)
    {
        safety(b_t,&save);
        
        (*ply)++;
        if (make_atomic_move(b_t,move_liste.moves[move_count],all_moves) == 0)
        {
            (*ply)--; 
            continue;
        }
        legal_move ++; 


        int score = -negamax(-beta, -alpha, depth -1 , b_t, ply, best_move, nodes); 
        restore(b_t,&save);
        (*ply)--;
        if(score >= beta)
        {
            return beta; 
        }
        if (score > alpha) 
        {
            alpha = score;
            if ((*ply) == 0)
            {
                bmsf = move_liste.moves[move_count];
                
            }
        }
        

    }
   
    if (legal_move == 0)
    {
        if (in_check)
            return  (-49000 + *ply);
        else 
            return 0;
    }
    if (old_alpha != alpha)
        *best_move = bmsf; 
    return alpha;
}

int search_position(int depth, board_t *b_t);



#endif