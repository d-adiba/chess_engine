#ifndef BITBOARD
#define BITBOARD


#include <stdio.h>
#include <string.h>


#define U64 unsigned long long
#define start_position "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

#define get_ls1b_index(bitboard) (__builtin_ctzll(bitboard))
#define count_bits(bitboard) (__builtin_popcountll(bitboard))


typedef struct {
	U64 board[12];  
	U64 occupancies[3];
	int side; 
	int enpassant;
	int castle; 
} board_t; 


extern const char squares[64][3];
extern const char  *unicode_pieces[12];


/*

    bin  dec
    
   0001    1  white king can castle to the king side
   0010    2  white king can castle to the queen side
   0100    4  black king can castle to the king side
   1000    8  black king can castle to the queen side

   examples

   1111       both sides an castle both directions
   1001       black king => queen side
              white king => king side

*/

enum { wk = 1, wq = 2, bk = 4, bq = 8 };
typedef enum {
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1, no_sq
} square;

typedef enum {
    white, black, both
} side;


typedef enum { P, N, B, R, Q, K, p, n, b, r, q, k } pieces;




static const char ascii_pieces[13] = "PNBRQKpnbrqk";
static const int char_pieces[] = {
    ['P'] = P,
    ['N'] = N,
    ['B'] = B,
    ['R'] = R,
    ['Q'] = Q,
    ['K'] = K,
    ['p'] = p,
    ['n'] = n,
    ['b'] = b,
    ['r'] = r,
    ['q'] = q,
    ['k'] = k
};


static inline void copy_board(board_t *b_t, board_t *s)
{
    memcpy(s->board, b_t->board, sizeof(s->board));
    memcpy(s->occupancies, b_t->occupancies, sizeof(s->occupancies));
    s->side = b_t->side;
    s->enpassant = b_t->enpassant;
    s->castle = b_t->castle;
}
static inline void restore_board(board_t *b_t, board_t *s)
{
    memcpy(b_t->board, s->board, sizeof(s->board));
    memcpy(b_t->occupancies, s->occupancies, sizeof(s->occupancies));
    b_t->side = s->side;
    b_t->enpassant = s->enpassant;
    b_t->castle = s->castle;
}

static inline int get_bit(U64 bitboard, square sq)
{
    return (((bitboard) >> sq) & 1ULL);
}


static inline void set_bit(U64 *bitboard, square sq)
{
    *bitboard |= (1ULL << sq);
}


static inline int pop_bit(U64 *bitboard, square sq)
{
    U64 mask = (1ULL << sq);
    if (*bitboard & mask) {
	*bitboard ^= mask;
	return 1;
    }
    return 0;
}


void print_bitboard(U64 bitborad);

static inline const char *get_square(square sq)
{
	return squares[sq];
}

void print_board(board_t *b);
void parse_fen(char *fen, board_t *b);
void print_attacked_squares(side s, board_t *b);

#endif
