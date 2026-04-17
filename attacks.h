#ifndef ATTACKS
#define ATTACKS

#include "bitboard.h"
#include "random.h"
#include <string.h>
#include <stdio.h>

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

/* Table des attaques de fou pré-calculées.
   bishop_attacks[sq] contient le bitboard des cases attaquées par le fou  placé case sq
   Déclarée ici (extern) et définie une seule fois dans attacks.c */
extern U64 bishop_attacks[64];


extern U64 bishop_attacks[64];


/*
 * Tableau recapitulant  le nombre de deplacment selon 
 * le masque d'attaque par case pour un bihop 
	6, 5, 5, 5, 5, 5, 5, 6,
	5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 7, 7, 7, 7, 5, 5,
	5, 5, 7, 9, 9, 7, 5, 5,
	5, 5, 7, 9, 9, 7, 5, 5,
	5, 5, 7, 7, 7, 7, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5,
	6, 5, 5, 5, 5, 5, 5, 6 
*/

extern const int bishop_relevant_bits[64];
 /*
 * Tableau recapitulant  le nombre de deplacment selon 
 * le masque d'attaque par case pour un rook 
 12, 11, 11, 11, 11, 11, 11, 12,
 11, 10, 10, 10, 10, 10, 10, 11,
 11, 10, 10, 10, 10, 10, 10, 11,
 11, 10, 10, 10, 10, 10, 10, 11,
 11, 10, 10, 10, 10, 10, 10, 11,
 11, 10, 10, 10, 10, 10, 10, 11,
 11, 10, 10, 10, 10, 10, 10, 11,
 12, 11, 11, 11, 11, 11, 11, 12
*/
extern const int rook_relevant_bits[64];
extern U64 rook_magic_number[64];
extern U64 bishop_magic_number[64];
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

U64 find_magic_number(square sq, int relevant_bits, flags f);
void init_magic_number();
/* init_leaper_attacks()
   Rôle : pré-calcule/initialise les tables d’attaques des différentes pieces
          utilisées par le moteur pour obtenir rapidement les attaques depuis n’importe quelle case.

   Effet :
   - Remplit les tableaux globaux (ex: pawn_attacks, knight_attacks ... etc) avec les masques calculés.*/
void init_leaper_attacks(void);

#endif
