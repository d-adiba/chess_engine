#include "attacks.h"
#include <stdio.h>

const U64 not_a_file = 18374403900871474942ULL;

const U64 not_h_file = 9187201950435737471ULL;

const U64 not_ab_file = 18229723555195321596ULL;

const U64 not_hg_file = 4557430888798830399ULL;

const int bishop_relevant_bits[64] = {
 6, 5, 5, 5, 5, 5, 5, 6,
 5, 5, 5, 5, 5, 5, 5, 5,
 5, 5, 7, 7, 7, 7, 5, 5,
 5, 5, 7, 9, 9, 7, 5, 5,
 5, 5, 7, 9, 9, 7, 5, 5,
 5, 5, 7, 7, 7, 7, 5, 5,
 5, 5, 5, 5, 5, 5, 5, 5,
 6, 5, 5, 5, 5, 5, 5, 6
};

const int rook_relevant_bits[64] = {
 12, 11, 11, 11, 11, 11, 11, 12,
 11, 10, 10, 10, 10, 10, 10, 11,
 11, 10, 10, 10, 10, 10, 10, 11,
 11, 10, 10, 10, 10, 10, 10, 11,
 11, 10, 10, 10, 10, 10, 10, 11,
 11, 10, 10, 10, 10, 10, 10, 11,
 11, 10, 10, 10, 10, 10, 10, 11,
 12, 11, 11, 11, 11, 11, 11, 12
};

U64 pawn_attacks[2][64];
U64 knight_attacks[64]; 
U64 king_attacks[64]; 
U64 bishop_attacks[64];
U64 rook_attacks[64];
U64 rook_magic_number[64];
U64 bishop_magic_number[64]; 
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

U64 mask_king_attacks(square sq)
{
    U64 attacks_result = 0ULL;
    U64 bitboard = 0ULL;
    set_bit(&bitboard, sq);

    if (bitboard >> 8)  attacks_result |= (bitboard >> 8);
    if ((bitboard >> 7) & not_a_file) attacks_result |= (bitboard >> 7);
    if ((bitboard >> 9) & not_h_file) attacks_result |= (bitboard >> 9); 
    if ((bitboard >> 1) & not_h_file) attacks_result |= (bitboard >> 1); 
    
    if (bitboard << 8)  attacks_result |= (bitboard << 8);
    if ((bitboard << 7) & not_h_file) attacks_result |= (bitboard << 7);
    if ((bitboard << 9) & not_a_file) attacks_result |= (bitboard << 9); 
    if ((bitboard << 1) & not_a_file) attacks_result |= (bitboard << 1); 

    return attacks_result; 
}

/**
 * @brief Calcule les positions d’attaque d’un fou  à partir d’une case donnée.
 *
 * @context
 * - Utilisée lors du calcul / pré-calcul de la table d’attaque des fou 
 *
 * @inputs
 * - square : index de la case du pion, dans l’intervalle 0..63].
 *
 * @outputs
 * - Retourne un U64 (bitboard) dont les bits à 1 représentent les cases attaquées
 *   par ce fou  depuis la case square.
 * - Si la valeur retournée est 0, alors le pion n’a aucune case attaquable
 *
 * @errors / undefined behavior
 * - Comportement indéfini si square n’est pas dans [0..63] (décalage des bits invalide).
 *
 * @algorithm
 * 1) Principe (bitboards) :
 *    - Les attaques d’un  fou  sont obtenues via  les calcul de rangée et de colonne
 *      la rangée ou se trouve ce dernier est déterminé via le quotien de la division
 *      de sa postion par 8 et sa colonne par le reste de cette division entière 
 *    - Les positions d'attaques sont obtenues en decalant cette rangée 
 *    	soit vers le haut ou le bas et en décalant la colonne soit vers la droite      
 *    	ou la gauche et en repetant le processus à partir de la dernier positon utilisée
 *
 * 2) illustartion :
 *
 *
 * 			c5	e5 <- f6 <- g7 <- succcession de case deduide de e5 
 *
 * 			    d4 <- case de depart du fou 
 *
 *			c3	e3 <- case obtenue a partir d'un décalage colonne/range
 *
 */


U64 mask_bishop_attacks(square sq)
{
    int r,f, tr,tf; 
    U64 attacks_result = 0ULL; 

    tr = sq / 8; 
    tf = sq % 8;

    for (r = tr + 1, f = tf + 1; r <= 6 && f <= 6; r++, f++)
    {
	  attacks_result |= (1ULL << ( r * 8 + f));
    }
    for (r = tr - 1, f = tf + 1; r >= 1 && f <= 6; r--, f++)
    {
	  attacks_result |= (1ULL << ( r * 8 + f));
    }


    for (r = tr + 1, f = tf - 1; r <= 6 && f >= 1; r++, f--)
    {
	  attacks_result |= (1ULL << ( r * 8 + f));
    }
    for (r = tr - 1, f = tf - 1; r >= 1 && f >= 1; r--, f--)
    {
	  attacks_result |= (1ULL << ( r * 8 + f));
    }

    return attacks_result; 
}

U64 mask_bishop_attacks_on_the_fly(square sq, U64 block)
{
    int r,f, tr,tf; 
    U64 attacks_result = 0ULL; 

    tr = sq / 8; 
    tf = sq % 8;

    for (r = tr + 1, f = tf + 1; r <= 6 && f <= 6; r++, f++)
    {
	  attacks_result |= (1ULL << ( r * 8 + f));
	  if ( block & (1ULL << ( r * 8 + f))) break;
    }
    for (r = tr - 1, f = tf + 1; r >= 1 && f <= 6; r--, f++)
    {
	  attacks_result |= (1ULL << ( r * 8 + f));
	  if ( block & (1ULL << ( r * 8 + f))) break;
    }


    for (r = tr + 1, f = tf - 1; r <= 6 && f >= 1; r++, f--)
    {
	  attacks_result |= (1ULL << ( r * 8 + f));
	  if ( block & (1ULL << ( r * 8 + f))) break;
    }
    for (r = tr - 1, f = tf - 1; r >= 1 && f >= 1; r--, f--)
    {
	  attacks_result |= (1ULL << ( r * 8 + f));
	  if ( block & (1ULL << ( r * 8 + f))) break;
    }

    return attacks_result; 
}

U64 mask_rook_attacks_on_the_fly(square sq, U64 block)
{
    int r,f, tr,tf; 
    U64 attacks_result = 0ULL; 

    tr = sq / 8; 
    tf = sq % 8;

    for (r = tr + 1; r <= 6 ; r++)
    {
	  attacks_result |= (1ULL << ( r * 8 + tf));
	  if ( block & (1ULL << ( r * 8 + tf))) break;
    }
    for (f = tf + 1; f <= 6; f++)
    {
	  attacks_result |= (1ULL << ( tr * 8 + f));
	  if ( block & (1ULL << ( tr * 8 + f))) break;
    }


    for ( r = tr - 1; r >= 1; r--)
    {
	  attacks_result |= (1ULL << ( r * 8 + tf));
	  if ( block & (1ULL << ( r * 8 + tf))) break;
    }
    for (f = tf - 1;  f >= 1; f--)
    {
	  attacks_result |= (1ULL << ( tr * 8 + f));
	  if ( block & (1ULL << ( tr * 8 + f))) break;
    }

    return attacks_result; 
}

U64 mask_rook_attacks(square sq)
{
    int r,f, tr,tf; 
    U64 attacks_result = 0ULL; 

    tr = sq / 8; 
    tf = sq % 8;

    for (r = tr + 1; r <= 6 ; r++)
    {
	  attacks_result |= (1ULL << ( r * 8 + tf));
    }
    for (f = tf + 1; f <= 6; f++)
    {
	  attacks_result |= (1ULL << ( tr * 8 + f));
    }


    for ( r = tr - 1; r >= 1; r--)
    {
	  attacks_result |= (1ULL << ( r * 8 + tf));
    }
    for (f = tf - 1;  f >= 1; f--)
    {
	  attacks_result |= (1ULL << ( tr * 8 + f));
    }

    return attacks_result; 
}
void init_leaper_attacks()
{
    int sq;
    for (sq = 0; sq < 64; sq++) {
	//table d'attaque des pawn
	pawn_attacks[white][sq] = mask_pawn_attacks(white, sq);
	pawn_attacks[black][sq] = mask_pawn_attacks(black, sq);
	//table d'attaque des knights 
	knight_attacks[sq] = mask_knight_attacks(sq);
	//table  d'attaque du king
	king_attacks[sq] = mask_king_attacks(sq);
	//masque d'attaque du bishop	
	bishop_attacks[sq] = mask_bishop_attacks(sq);
	//masque d'attage du rook	
	rook_attacks[sq] = mask_rook_attacks(sq); 	
    }
}


/*
 *index:représente un encodage des bits du mask d'attaque qui sont occupés 
 *	sa valeur binaire indique les bit succesif de poids faible qui 
 *	deja occupé sur le mask
 *	eg:	10011= 19
 *		ici si le mask dispose de 12 bits non nuls représentant
 *		les positions de déplacements alors si ont les classe 
 *		de 0 à 11 on a :10011
 *				43210 <-les position des differents bit
 *					composant 13
 *				alors les bits de poids faible en position
 *				successif 0,1, et 4 sont occupés
 *	ici : index & (1 << i) vérifie si le bit en position i dans index 
 *	est à 1 
 *	eg:  10011 & ( 1 << 0) = 10011 & 1 = 1 soit le premier bit faible
 *	est obstrué   
 *					
 * */
U64 set_occupancy( int index , int bits_in_mask, U64 mask_attacks)
{
	U64 occupancy = 0ULL;
	U64 mask = mask_attacks; 
	square sq; 	
	for ( int i = 0 ; i < bits_in_mask ; i++)
	{
		sq = get_ls1b_index(mask);	
		pop_bit(&mask, sq);
		if ( index & (1 << i))
			occupancy |= (1ULL << sq); 
	}
	return occupancy;
}

U64 find_magic_number(square sq, int relevant_bits, flags f)
{
	U64 occupancies[4096] , attacks[4096], used_attacks[4096];
	U64 attack_mask = (f == bishop)  ? mask_bishop_attacks(sq): 
		mask_rook_attacks(sq);
	int occupancy_indicies, index; 
        occupancy_indicies =  1 << relevant_bits;
 	for (index = 0; index < occupancy_indicies; index++)
	{
		occupancies[index] = set_occupancy(index, relevant_bits, attack_mask);
		attacks [index] = ( f == bishop) ? mask_bishop_attacks_on_the_fly(sq,
				occupancies[index]) : mask_rook_attacks_on_the_fly (sq,
					occupancies[index]);
	}
	int magic_index, fail, index_t; 
	U64  magic_number;
	for( int i = 0;  i < 100000000; i++) 
	{
		magic_number = get_magic_number();
		if ( count_bits(((attack_mask * magic_number) & 
				0xFF00000000000000)) < 6 ) continue; 
		memset(used_attacks, 0ULL, sizeof(used_attacks));
		fail = 0; 
		for (index_t = 0; !fail && 
				index_t < occupancy_indicies; index_t++)
		{
			magic_index =  (int)  ((occupancies[index_t] * magic_number) >> ( 64 - relevant_bits)); 
		       if (used_attacks[magic_index] == 0ULL)
				used_attacks[magic_index] = attacks[index_t];
			else if (used_attacks[magic_index] != attacks[index_t])
				 fail = 1;
		}
		if (!fail)
			return magic_number;
	}
	printf("Magic number not working");
	return 0ULL; 	
}

void init_magic_number()
{
	for ( int i = 0; i < 64; i++)
	{
		rook_magic_number[i] =  find_magic_number (i, rook_relevant_bits[i], rook);
		printf(" 0x%llxULL,\n", rook_magic_number[i]); 
	}
	printf ("\n++++++++++++bishop+++++++++++\n");
	for ( int i = 0; i < 64; i++)
	{
		bishop_magic_number[i] = find_magic_number(i, bishop_relevant_bits[i],bishop); 
		printf(" 0x%llxULL,\n", bishop_magic_number[i]);
	}
}

