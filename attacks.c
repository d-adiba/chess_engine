#include "attacks.h"


const U64 not_a_file = 18374403900871474942ULL;

const U64 not_h_file = 9187201950435737471ULL;

const U64 not_ab_file = 18229723555195321596ULL;

const U64 not_hg_file = 4557430888798830399ULL;

U64 pawn_attacks[2][64];

U64 knight_attacks[64]; 

/**
 * @brief Calcule les positions d’attaque d’un pion à partir d’une case donnée.
 *
 * @context
 * - Utilisée lors du calcul / pré-calcul de la table d’attaque des pions.
 *
 * @inputs
 * - square : index de la case du pion, dans l’intervalle [0..63].
 * - color  : couleur du pion (ex. WHITE / BLACK).
 *
 * @outputs
 * - Retourne un U64 (bitboard) dont les bits à 1 représentent les cases attaquées
 *   par ce pion depuis la case square.
 * - Si la valeur retournée est 0, alors le pion n’a aucune case attaquable
 *
 * @errors / undefined behavior
 * - Comportement indéfini si square n’est pas dans [0..63] (décalage de bits invalide).
 *
 * @algorithm
 * 1) Principe (bitboards) :
 *    - Les attaques d’un pion sont obtenues via des décalages de 7 et 9 bits
 *      (selon la convention d’orientation du bitboard et la couleur).
 *    - Les deux bitboards (capture gauche / capture droite) sont combinés par OU.
 *
 * 2) Gestion des cas limites (débordements de colonnes) :
 *    - Sans masquage, un pion sur la colonne A ou H peut “wrap” sur l’autre côté
 *      lors du décalage (effet de débordement horizontal).
 *    - On masque donc les résultats avec :
 *      - not_a_file pour éliminer les attaques qui débordent depuis la colonne A,
 *      - not_h_file pour éliminer les attaques qui débordent depuis la colonne H.
 *
 */

U64 mask_pawn_attacks(side sd, square sq)
{
    U64 attacks_result = 0ULL;
    U64 bitboard = 0ULL;
    set_bit(&bitboard, sq);

    // white pawn   
    if (!sd) {
	if ((bitboard >> 7) & not_a_file)
	    attacks_result |= (bitboard >> 7);
	if ((bitboard >> 9) & not_h_file)
	    attacks_result |= (bitboard >> 9);

    }
    // black pawn 
    else {
	if ((bitboard << 7) & not_h_file)
	    attacks_result |= (bitboard << 7);
	if ((bitboard << 9) & not_a_file)
	    attacks_result |= (bitboard << 9);

    }
    return attacks_result;
}

/**
 * @brief Calcule les positions d’attaque d’un cavalier  à partir d’une case donnée.
 *
 * @context
 * - Utilisée lors du calcul / pré-calcul de la table d’attaque des pieces.
 *
 * @inputs
 * - square : index de la case du cavalier, dans l’intervalle [0..63].
 *
 * @outputs
 * - Retourne un U64 (bitboard) dont les bits à 1 représentent les cases attaquées
 *   par ce cavalier depuis la case square.
 *
 * @errors / undefined behavior
 * - Comportement indéfini si square n’est pas dans [0..63] (décalage de bits invalide).
 *
 * @algorithm
 * 1) Principe (bitboards) :
 *    - Les attaques d’un cavalier  sont obtenues via des décalages (17, 15, 10, 6) de bit
 *    - Ces decalages sont effectuées dans les deux sens à partir de la position de depart
 *
 * 2) Gestion des cas limites (débordements de colonnes) :
 *    - les debordement occasionnels (cas h4 par exemple) sont gérés comme avec les pions
 *
 */

U64 mask_knight_attacks(square sq)
{
    U64 attacks_result = 0ULL;
    U64 bitboard = 0ULL;
    set_bit(&bitboard, sq);

    if ((bitboard >> 17) & not_h_file) attacks_result |= (bitboard >> 17);
    if ((bitboard >> 15) & not_a_file) attacks_result |= (bitboard >> 15); 
    if ((bitboard >> 10) & not_hg_file) attacks_result |= (bitboard >> 10); 
    if ((bitboard >> 6) & not_ab_file) attacks_result |= (bitboard >> 6);

    if ((bitboard << 17) & not_a_file) attacks_result |= (bitboard << 17);
    if ((bitboard << 15) & not_h_file) attacks_result |= (bitboard << 15); 
    if ((bitboard << 10) & not_ab_file) attacks_result |= (bitboard << 10); 
    if ((bitboard << 6) & not_hg_file) attacks_result |= (bitboard << 6);

    return attacks_result; 
}


void init_leaper_attacks()
{
    int sq;
    for (sq = 0; sq < 64; sq++) {
	//table d'attaque des pions
	pawn_attacks[white][sq] = mask_pawn_attacks(white, sq);
	pawn_attacks[black][sq] = mask_pawn_attacks(black, sq);
	//table d'attaque des cavaliers 
	knight_attacks[sq] = mask_knight_attacks(sq); 
    }
}
