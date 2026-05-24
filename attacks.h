#ifndef ATTACKS
#define ATTACKS

#include "bitboard.h"
#include "random.h"
#include <string.h>
#include <stdio.h>
#include <immintrin.h>


typedef enum {
	bishop, rook
} flags;

/* 
  8  0  1  1  1  1  1  1  1 
  7  0  1  1  1  1  1  1  1 
  6  0  1  1  1  1  1  1  1 
  5  0  1  1  1  1  1  1  1 
  4  0  1  1  1  1  1  1  1 
  3  0  1  1  1  1  1  1  1 
  2  0  1  1  1  1  1  1  1 
  1  0  1  1  1  1  1  1  1 

     a  b  c  d  e  f  g  h
 */
extern const U64 not_a_file;	// 18374403900871474942ULL 

/*
  8  1  1  1  1  1  1  1  0 
  7  1  1  1  1  1  1  1  0 
  6  1  1  1  1  1  1  1  0 
  5  1  1  1  1  1  1  1  0 
  4  1  1  1  1  1  1  1  0 
  3  1  1  1  1  1  1  1  0 
  2  1  1  1  1  1  1  1  0 
  1  1  1  1  1  1  1  1  0 

     a  b  c  d  e  f  g  h

 */
extern const U64 not_h_file;	//= 9187201950435737471ULL 

/*
  8  0  0  1  1  1  1  1  1 
  7  0  0  1  1  1  1  1  1 
  6  0  0  1  1  1  1  1  1 
  5  0  0  1  1  1  1  1  1 
  4  0  0  1  1  1  1  1  1 
  3  0  0  1  1  1  1  1  1 
  2  0  0  1  1  1  1  1  1 
  1  0  0  1  1  1  1  1  1 

     a  b  c  d  e  f  g  h

*/
extern const U64 not_ab_file;	//= 18229723555195321596ULL


/*
  8  1  1  1  1  1  1  0  0 
  7  1  1  1  1  1  1  0  0 
  6  1  1  1  1  1  1  0  0 
  5  1  1  1  1  1  1  0  0 
  4  1  1  1  1  1  1  0  0 
  3  1  1  1  1  1  1  0  0 
  2  1  1  1  1  1  1  0  0 
  1  1  1  1  1  1  1  0  0 

     a  b  c  d  e  f  g  h

*/
extern const U64 not_hg_file;	//= 4557430888798830399ULL 

/* Table des attaques de pions pré-calculées.
   pawn_attacks[sd][sq] contient le bitboard des cases attaquées par un pion du côté sd
   placé sur la case sq 
   Déclarée ici (extern) et définie une seule fois dans un attacks.c */
extern U64 pawn_attacks[2][64];

/* Table des attaques de cavalier pré-calculées.
   knight_attacks[sq] contient le bitboard des cases attaquées par le cavalier placé case sq
   Déclarée ici (extern) et définie une seule fois dans attacks.c */
extern U64 knight_attacks[64];

/* Table des attaques de Roi pré-calculées.
   king_attacks[sq] contient le bitboard des cases attaquées par le Roi placé case sq
   Déclarée ici (extern) et définie une seule fois dans attacks.c */
extern U64 king_attacks[64];

extern U64 bishop_attacks[64][512];

extern U64 rook_attacks[64][4096]; 

extern U64 bishop_masks[64];

extern U64 rook_masks[64];

extern U64 mask_atomic_explosion[64];

extern U64 atomic_explosion_attacks[64][512];

static inline U64 get_bishop_attacks(square sq, U64 occupancy)
{
	occupancy = _pext_u64(occupancy, bishop_masks[sq]);
	return bishop_attacks[sq][occupancy]; 
}

static inline U64 get_rook_attacks(square sq, U64 occupancy) 
{
	occupancy &= rook_masks[sq];
	occupancy = _pext_u64(occupancy, rook_masks[sq]);
	return rook_attacks[sq][occupancy]; 
}

static inline U64 get_queen_attacks(square sq, U64 occupancy)
{
	return get_bishop_attacks(sq,occupancy) | get_rook_attacks(sq,occupancy);
}

static inline U64 get_atomic_explosion_attacks(square sq, U64 occupancy)
{
    occupancy &= mask_atomic_explosion[sq];
    occupancy = _pext_u64(occupancy, mask_atomic_explosion[sq]);
    return atomic_explosion_attacks[sq][occupancy];
}

static inline int is_square_attacked(int  sq, side s, board_t *b_t)
{
    if ((s == white) && (pawn_attacks[black][sq] & b_t->board[P])) return 1;
    
    if ((s == black) && (pawn_attacks[white][sq] & b_t->board[p])) return 1;
    if (knight_attacks[sq] & ((s == white) ? b_t->board[N] : b_t->board[n])) return 1;
    
    if (get_bishop_attacks(sq, b_t->occupancies[both]) & ((s == white) ? b_t->board[B] : b_t->board[b])) return 1;

    if (get_rook_attacks(sq, b_t->occupancies[both]) & ((s == white) ? b_t->board[R] : b_t->board[r])) return 1;    

    if (get_queen_attacks(sq, b_t->occupancies[both]) & ((s == white) ? b_t->board[Q] : b_t->board[q])) return 1;
    

    return 0;
}

/* mask_pawn_attacks(sd, sq)
   Rôle : calcule et renvoie le bitboard des cases attaquées par un pion placé sur sq,
          pour le camp/côté sd (ex: WHITE/BLACK).
          Le résultat est un masque d’attaques “brut” (sans tenir compte des pièces sur l’échiquier).

   Paramètres :
   - sd : côté du pion (détermine la direction des attaques).
   - sq : case de départ du pion

   Retour :
   - Un U64 où chaque bit à 1 représente une case attaquée par ce pion depuis sq.

   Préconditions :
   - sq doit être dans [0, 63]. */
U64 mask_pawn_attacks(side sd, square sq);

/* mask_knight_attacks(sq)
   Rôle : calcule et renvoie le bitboard des cases attaquées par un cavalier palcé case sq
          Le résultat est un masque d’attaques “brut” (sans tenir compte des pièces sur l’échiquier).

   Paramètres :
   - sq : case de départ du cavalier

   Retour :
   - Un U64 où chaque bit à 1 représente une case attaquée par ce cavalier depuis sq.

   Préconditions :
   - sq doit être dans [0, 63]. */
U64 mask_knight_attacks(square sq);

/* mask_king_attacks(sq)
   Rôle : calcule et renvoie le bitboard des cases attaquées par un Roi palcé case sq
          Le résultat est un masque d’attaques “brut” (sans tenir compte des pièces sur l’échiquier).

   Paramètres :
   - sq : case de départ du Roi 

   Retour :
   - Un U64 où chaque bit à 1 représente une case attaquée par ce Roi depuis sq.
   Préconditions :
   - sq doit être dans [0, 63]. */
U64 mask_king_attacks(square sq);

/* mask_bishop_attacks(sq)
   Rôle : calcule et renvoie le bitboard des cases attaquées par un Fou  palcé case sq
          Le résultat est un masque d’attaques “brut” (sans tenir compte des pièces sur l’échiquier).

   Paramètres :
   - sq : case de départ du fou 

   Retour :
   - Un U64 où chaque bit à 1 représente une case attaquée par ce Roi depuis sq.
   Préconditions :
   - sq doit être dans [0, 63]. */
U64 mask_bishop_attacks(square sq);

U64 mask_bishop_attacks_on_the_fly(square sq, U64 block);

U64 mask_rook_attacks(square sq);

U64 mask_rook_attacks_on_the_fly(square sq, U64 block);

void init_mask_slider_attacks();
void init_atomic_explosion_mask();
void init_atomic_explosion_attacks();
void init_leaper_attacks(void);
void init_slider_attacks(flags fg);
void init_all();



#endif
