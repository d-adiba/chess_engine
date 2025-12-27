#ifndef BITBOARD
#define BITBOARD

#include <stdio.h>
#define U64 unsigned long long 

typedef enum  {
        a8, b8, c8, d8, e8, f8, g8, h8,
        a7, b7, c7, d7, e7, f7, g7, h7,
        a6, b6, c6, d6, e6, f6, g6, h6,
        a5, b5, c5, d5, e5, f5, g5, h5,
        a4, b4, c4, d4, e4, f4, g4, h4,
        a3, b3, c3, d3, e3, f3, g3, h3,
        a2, b2, c2, d2, e2, f2, g2, h2,
        a1, b1, c1, d1, e1, f1, g1, h1

} square;

// side to move (colors) 
typedef enum {
	white , black
} side; 

/* get_bit(bitboard, sq)
   Rôle : retourne l’état (0/1) du bit correspondant à la case sq dans bitboard.

   Paramètres :
   - bitboard : bitboard source (U64), non modifié.
   - sq       : index du bit/case à tester.

   Retour :
   - 1 si le bit sq est à 1, sinon 0.

   Préconditions :
   - sq doit être dans [0, 63] (sinon le décalage >> peut être un comportement indéfini en C). */

static inline int get_bit(U64 bitboard, square sq)
{
        return ((bitboard >> sq) & 1ULL);
}

/* set_bit(bitboard, sq)
   Rôle : met à 1 le bit correspondant à la case sq dans *bitboard.

   Paramètres :
   - bitboard : pointeur vers le bitboard à modifier.
   - sq       : index du bit/case à mettre à 1.

   Effet :
   - Après appel, le bit sq de *bitboard vaut 1.

   Préconditions :
   - bitboard != NULL.
   - sq doit être dans [0, 63] (sinon le décalage << peut être un comportement indéfini en C). */
static inline void  set_bit(U64* bitboard, square sq)
{
        *bitboard |= (1ULL << sq);
}

/* pop_bit(bitboard, sq)
   Rôle : enlève (met à 0) le bit correspondant à la case sq dans *bitboard, si ce bit est présent.

   Paramètres :
   - bitboard : pointeur vers le bitboard à modifier.
   - sq       : index du bit/case à retirer.

   Retour :
   - 1 si le bit était à 1 et a été retiré.
   - 0 si le bit était déjà à 0 (bitboard inchangé).

   Préconditions :
   - bitboard != NULL.
   - sq doit être dans [0, 63]. */
static inline int  pop_bit (U64* bitboard, square sq)
{
        U64 mask = (1ULL << sq);
        if ( *bitboard &  mask)
        {
                *bitboard ^= mask;
                return 1;
        }
        return 0;
}

/* print_bitboard(bitboard)
   Rôle : affiche le bitboard sous forme d’échiquier 8x8 (0/1) sur la sortie standard,
          avec les coordonnées (8..1 et a..h) et la valeur entière du bitboard.

   Paramètre :
   - bitboard : bitboard à afficher (U64), non modifié.

   Effet :
   - Écrit sur stdout via printf(). */ 	
void print_bitboard(U64 bitborad);

#endif 
