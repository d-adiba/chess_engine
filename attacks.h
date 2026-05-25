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
   Déclarée ici (extern) et définie  dans un attacks.c */
extern U64 pawn_attacks[2][64];

/* Table des attaques de cavalier pré-calculées.
   knight_attacks[sq] contient le bitboard des cases attaquées par le cavalier placé case sq
   Déclarée ici (extern) et définie  dans attacks.c */
extern U64 knight_attacks[64];

/* Table des attaques de Roi pré-calculées.
   king_attacks[sq] contient le bitboard des cases attaquées par le Roi placé case sq
   Déclarée ici (extern) et définie  dans attacks.c */
extern U64 king_attacks[64];

/**
 * Tables des attaques de Fou
   bishop_attacks[sq][index] contient le bitboard des cases attaquées par un fou placé case sq
   avec une occupation donnée par index
   Déclarées ici (extern) et définies une seule fois dans attacks.c
 */
extern U64 bishop_attacks[64][512];


/**
 * Tables des attaques de Tour
   rook_attacks[sq][index] contient le bitboard des cases attaquées par une tour placée case sq
   avec une occupation donnée par index
   Déclarées ici (extern) et définies une seule fois dans attacks.c
 */
extern U64 rook_attacks[64][4096];

/**
 * Table des attaques d'explosion atomique
   atomic_explosion_attacks[sq][index] contient le bitboard des cases affectées par l'explosion résultante de l'attaque d'une pièce placée case sq
   avec une occupation donnée par index
   Déclarées ici (extern) et définies une seule fois dans attacks.c
 */
extern U64 atomic_explosion_attacks[64][512];

/*
 * Table des masques  de Fou 
 */
extern U64 bishop_masks[64];

/*
 * Table des masques  de Tour 
 */
extern U64 rook_masks[64];

/*
 * Table des masques  d'explosion atomique 
 */
extern U64 mask_atomic_explosion[64];



/**
 * get_bishop_attacks(sq, occupancy)
   Rôle : retourne le bitboard des cases affectées par l'attaque d'un fou placé sur la case sq 
         en tenant compte de l'occupation actuelle de l'échiquier (occupancy).

   Paramètres :
   - sq : la case où se trouve le fou (0-63).
   - occupancy : un bitboard représentant l'occupation actuelle de l'échiquier.

   Retour :
   - Un bitboard représentant les cases affectées par l'attaque du fou sur la case sq.

   Préconditions :
   - sq doit être dans [0, 63].
   - occupancy doit être un bitboard valide.
 */
static inline U64 get_bishop_attacks(square sq, U64 occupancy)
{
	occupancy = _pext_u64(occupancy, bishop_masks[sq]);
	return bishop_attacks[sq][occupancy]; 
}

/**
 * get_rook_attacks(sq, occupancy)
   Rôle : retourne le bitboard des cases affectées par l'attaque d'une tour placée sur la case sq 
         en tenant compte de l'occupation actuelle de l'échiquier (occupancy).

   Paramètres :
   - sq : la case où se trouve la tour (0-63).
   - occupancy : un bitboard représentant l'occupation actuelle de l'échiquier.

   Retour :
   - Un bitboard représentant les cases affectées par l'attaque de la tour sur la case sq.

   Préconditions :
   - sq doit être dans [0, 63].
   - occupancy doit être un bitboard valide.
 */
static inline U64 get_rook_attacks(square sq, U64 occupancy) 
{
	occupancy &= rook_masks[sq];
	occupancy = _pext_u64(occupancy, rook_masks[sq]);
	return rook_attacks[sq][occupancy]; 
}

/**
 * get_queen_attacks(sq, occupancy)
   Rôle : retourne le bitboard des cases affectées par l'attaque d'une reine placée sur la case sq 
         en tenant compte de l'occupation actuelle de l'échiquier (occupancy).

   Paramètres :
   - sq : la case où se trouve la reine (0-63).
   - occupancy : un bitboard représentant l'occupation actuelle de l'échiquier.

   Retour :
   - Un bitboard représentant les cases affectées par l'attaque de la reine sur la case sq.

   Préconditions :
   - sq doit être dans [0, 63].
   - occupancy doit être un bitboard valide 
 */
static inline U64 get_queen_attacks(square sq, U64 occupancy)
{
	return get_bishop_attacks(sq,occupancy) | get_rook_attacks(sq,occupancy);
}

/**
 * get_atomic_explosion_attacks(sq, occupancy)
   Rôle : retourne le bitboard des cases affectées par l'explosion résultante de l'attaque d'une piece placée sur la case sq 
         en tenant compte de l'occupation actuelle de l'échiquier (occupancy).

   Paramètres :
   - sq : la case où se trouve la pièce attaquante (0-63).
   - occupancy : un bitboard représentant l'occupation actuelle de l'échiquier.

   Retour :
   - Un bitboard représentant les cases affectées par l'explosion résultante de l'attaque d'une pièce sur la case sq.

   Préconditions :
   - sq doit être dans [0, 63].
   - occupancy doit être un bitboard valide 
 */
static inline U64 get_atomic_explosion_attacks(square sq, U64 occupancy)
{
   occupancy &= mask_atomic_explosion[sq];
   occupancy = _pext_u64(occupancy, mask_atomic_explosion[sq]);
   return atomic_explosion_attacks[sq][occupancy];
}

/**
 * is_king_attacked(s, b_t)
   Rôle : vérifie si le roi du côté s est attaqué sur le board de référence b_t.

   Paramètres :
   - s : le côté pour lequel on vérifie l'attaque (white ou black).
   - b_t : un pointeur vers la structure représentant l'état actuel de l'échiquier.

   Fonctionnement :
   - Si le roi est directement attaqué on verifie si sa destruction n'affecte pas le roi opposé via une explosion atomique, 
      si c'est le cas alors le roi est considéré comme non attaqué
   - Dans le cas d'une attaque indirecte par explosion du roi, on vérifie que cette explosion n'affecte pas le roi opposé, 
      si c'est le cas alors le roi est considéré comme non attaqué

   Retour :
   - 1 si le roi du côté s est attaqué, 0 sinon.

   Préconditions :
   - s doit être soit white soit black.
   - b_t doit pointer vers une structure de board_t correctement initialisée.
 */
static inline int is_king_attacked(side s, board_t *b_t)
{
   int  explosion_center ; 
   square king_sq = (s == white) ? get_ls1b_index(b_t->board[K]) : get_ls1b_index(b_t->board[k]);
   square opposite_king_sq = (s == white) ? get_ls1b_index(b_t->board[k]) : get_ls1b_index(b_t->board[K]);

   if (is_classically_attacked(king_sq, !s, b_t))
   {
      if(get_atomic_explosion_attacks(king_sq,b_t->board[(s == white) ? k : K])) return 0;
      return 1;
   }
   explosion_center = can_square_explode(king_sq, !s, b_t);
   if (explosion_center != -1)
   {
      if (get_atomic_explosion_attacks(explosion_center,b_t->board[(s == white) ? k : K])) return 0;
      return 1;
   }
   return 0;
}


/**
 * can_square_explode(sq, s, b_t)
   Rôle : vérifie si la case sq est succeptible d'être detruite par l'explosion résultante de l'attaque 
         d'une des pieces alliées voisines par une piece de couleur s sur le board de référence b_t.

   Paramètres :
   - sq : la case à vérifier (0-63).
   - s : le côté pour lequel on vérifie l'attaque (white ou black).
   - b_t : un pointeur vers la structure représentant l'état actuel de l'échiquier.

   Retour :
   - la position de la piece dont l'attaque  pourrait detruire la piece  posée sq  via une explosion, 
      -1 si la piece case  sq  ne peut etre explosée.

   Préconditions :
   - sq doit être dans [0, 63].
   - s doit être soit white soit black.
   - b_t doit pointer vers une structure de board_t correctement initialisée.
 */
static inline int can_square_explode(int  sq, side s, board_t *b_t)
{
   //on recupère les  cases allièes dont l'explosion pourrait détruire la piece sur sq 
   U64 explosion_range = get_atomic_explosion_attacks(sq,b_t->occupancies[!s]); 
   
   //pour chacune de ces pieces alliées on verifie toutes attaques directes
   while(explosion_range)
   {
      sq = get_ls1b_index(explosion_range);
      if (is_classically_attacked(sq, s, b_t)) return sq;
      pop_bit(&explosion_range, sq);
   }
   return -1;
}


/**
 * is_classically_attacked(sq, s, b_t)
   Rôle : vérifie si la case sq est  directement attaquée par une pièce du côté s  sur le board de référence b_t.

   Paramètres :
   - sq : la case à vérifier (0-63).
   - s : le côté pour lequel on vérifie l'attaque (white ou black).
   - b_t : un pointeur vers la structure représentant l'état actuel de l'échiquier.

   Fonctionnement :
   - une case est attaquée par une piece coté s si réciproquement la même piece coté !s placé sq peut attaquer 
      cette derniere
   Retour :
   - 1 si la case sq est attaquée par une pièce du côté s, 0 sinon.

   Préconditions :
   - sq doit être dans [0, 63].
   - s doit être soit white soit black.
   - b_t doit pointer vers une structure de board_t correctement initialisée.
 */
static inline int is_classically_attacked(square sq, side s, board_t *b_t)
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
   Rôle : calcule et renvoie le bitboard  des cases attaquées par un Fou  placé case sq
          Le résultat est un masque d’attaques “brut” (sans tenir compte des pièces sur l’échiquier et des bordures).

   Paramètres :
   - sq : case de départ du fou 

   Retour :
   - Un U64 où chaque bit à 1 représente une case attaquée par ce Roi depuis sq.
   Préconditions :
   - sq doit être dans [0, 63]. */
U64 mask_bishop_attacks(square sq);

/**
 * mask_rook_attacks(sq)
   Rôle : calcule et renvoie le bitboard des cases attaquées par une Tour  palcé case sq
          Le résultat est un masque d’attaques “brut” (sans tenir compte des pièces sur l’échiquier et des bordures).

   Paramètres :
   - sq : case de départ de la tour 

   Retour :
   - Un U64 où chaque bit à 1 représente une case attaquée par cette tour depuis sq.
   Préconditions :
   - sq doit être dans [0, 63].
 */
U64 mask_rook_attacks(square sq);

/**
 * mask_bishop_attacks_on_the_fly(sq, block)
   Rôle : calcule et renvoie le bitboard des cases attaquées par un Fou placé case sq en tenant compte de l'occupancy block
          Le résultat est un masque d’attaques “réel” (en tenant compte des pièces sur l’échiquier et des bordures).

   Paramètres :
   - sq : case de départ du fou 
   - block : bitboard représentant les pièces bloquantes pour le fou (occupancy)

   Retour :
   - Un U64 où chaque bit à 1 représente une case attaquée par ce fou depuis sq en tenant compte de block.
   Préconditions :
   - sq doit être dans [0, 63].
 */
U64 mask_bishop_attacks_on_the_fly(square sq, U64 block);

/**
 * mask_rook_attacks_on_the_fly(sq, block)
   Rôle : calcule et renvoie le bitboard des cases attaquées par une Tour placée case sq en tenant compte de l'occupancy block
          Le résultat est un masque d’attaques “réel” (en tenant compte des pièces sur l’échiquier et des bordures).

   Paramètres :
   - sq : case de départ de la tour 
   - block : bitboard représentant les pièces bloquantes pour la tour (occupancy)

   Retour :
   - Un U64 où chaque bit à 1 représente une case attaquée par cette tour depuis sq en tenant compte de block.
   Préconditions :
   - sq doit être dans [0, 63].
 */

U64 mask_rook_attacks_on_the_fly(square sq, U64 block);

/**
 * init_mask_slider_attacks()
   Rôle : initialise les tables de masques d'attaques pour les pièces glissantes (fou, tour) en 
            calculant les masques d'attaques pour chaque case de l'échiquier. 
*/
void init_mask_slider_attacks();

/**
 * init_atomic_explosion_attacks()
   Rôle : initialise la table de masques d'explosion atomique  pour chaque case de l'échiquier.
 */
void init_atomic_explosion_mask();

/**
 * init_atomic_explosion_attacks()
   Rôle : initialise la table d'attaques par explosion atomique pour chaque case de l'échiquier 
            et pour chaque configuration d'occupancy possible.
 */
void init_atomic_explosion_attacks();

/**
 * init_leaper_attacks()
   Rôle : initialise les tables d'attaques pour les pièces non glissantes (pion, cavalier, roi) en calculant les attaques 
      pour chaque case de l'échiquier.
 */
void init_leaper_attacks();

/**
 * init_slider_attacks(fg)
   Rôle : initialise les tables d'attaques pour les pièces glissantes (fou, tour) en calculant les attaques 
      pour chaque case de l'échiquier et pour chaque configuration d'occupancy possible. 
   Paramètres :
   - fg : un flag indiquant le type de pièce glissante (bishop ou rook) pour laquelle initialiser les attaques.
 */
void init_slider_attacks(flags fg);

/**
 * init_all()
   Rôle : initialise toutes les tables d'attaques.
 */
void init_all();



#endif
