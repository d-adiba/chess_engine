#ifndef PAWN
#define PAWN

#include "bitboard.h"



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
extern  const U64 not_a_file ; // 18374403900871474942ULL 

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
extern const U64 not_h_file; //= 9187201950435737471ULL 

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
extern const U64 not_ab_file; //= 18229723555195321596ULL


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
extern const U64 not_hg_file ; //= 4557430888798830399ULL 

/* Table des attaques de pions pré-calculées.
   pawn_attacks[sd][sq] contient le bitboard des cases attaquées par un pion du côté sd
   placé sur la case sq 
   Déclarée ici (extern) et définie une seule fois dans un pawn.c. */
extern U64 pawn_attacks[2][64];


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

/* init_pawn_leaper_attacks()
   Rôle : pré-calcule/initialise les tables d’attaques des pions (et éventuellement autres “leapers”)
          utilisées par le moteur pour obtenir rapidement les attaques depuis n’importe quelle case.

   Effet :
   - Remplit les tableaux globaux (ex: pawn_attacks[side][square]) avec les masques calculés.*/
void init_pawn_leaper_attacks(void);

#endif
