#include "bitboard.h"
#include <string.h>
#include "attacks.h"


const char  *unicode_pieces[12] = {"♙", "♘", "♗", "♖", "♕", "♔", "♟︎", "♞", "♝", "♜", "♛", "♚"};


const char squares[64][3] = {
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"
};





void print_bitboard(U64 bitboard)
{
    int rank, file, sq;
    for (rank = 0; rank < 8; rank++) {
	printf("  %d ", (8 - rank));
	for (file = 0; file < 8; file++) {
	    sq = rank * 8 + file;
	    printf(" %d ", (int) ((bitboard >> sq) & 1ULL));
	}
	printf("\n");
    }
    printf("\n     a  b  c  d  e  f  g  h\n\n");
    printf("     Bitboard: %llu\n\n", bitboard);
}

void print_board(board_t  *b)
{

	int rank, file, sq, piece, i;
	printf("\n"); 
	for (rank = 0; rank < 8; rank++) 
    	{
		printf("  %d ", (8 - rank));
		for (file = 0; file < 8; file++) 
		{
	    		sq = rank * 8 + file; 
	   		piece = -1;
			for (i = P; i <= k; i++)
			{
				if (get_bit(b->board[i],sq) == 1)
				{
					piece = i;
					break;
				}
			}
	    		printf(" %s ", (piece == -1) ? "." : unicode_pieces[piece]);
		}
		printf("\n");
	}
    	printf("\n     a  b  c  d  e  f  g  h\n\n");
	printf("\n     Side: %s",  b->side == black ? "black": "white");
	printf("\n     Enpassant: %s", (b->enpassant != no_sq) ? get_square(b->enpassant): "no");
	printf("\n     Castling: %c%c%c%c", (b->castle & wk)? 'K':'-',  (b->castle & wq)? 'Q':'-',   (b->castle & bk)? 'k':'-',   (b->castle & bq)? 'q' : '-');   
	
	
}

void parse_fen(char *fen, board_t *b)
{
    int sq, piece, offset; 
    memset(b->board, 0ULL, sizeof(b->board));
    memset(b->occupancies, 0ULL, sizeof(b->occupancies));
    b->side = 0;
    b->enpassant = no_sq;
    b->castle = 0;
    
    for (int rank = 0; rank < 8; rank++)
    {
        for (int file = 0; file < 8; file++)
        {
             sq = rank * 8 + file;
            
            if ((*fen >= 'a' && *fen <= 'z') || (*fen >= 'A' && *fen <= 'Z'))
            {
                piece = char_pieces[*fen];
                set_bit(b->board + piece, sq);
            }
            
            if (*fen >= '0' && *fen <= '9')
            {
                offset = *fen - '0';
                file += offset -1 ;
            }
	        fen++;
            
        }
        if (*fen == '/') fen++;
    }
    fen++;
    (*fen == 'w') ? (b->side = white) : (b->side = black);
    
    fen += 2;
    
    while (*fen != ' ')
    {
        switch (*fen)
        {
            case 'K': b->castle |= wk; break;
            case 'Q': b->castle |= wq; break;
            case 'k': b->castle |= bk; break;
            case 'q': b->castle |= bq; break;
            case '-': break;
        }

        fen++;
    }
    
    fen++;
    
    if (*fen != '-')
    {
        int file = fen[0] - 'a';
        int rank = 8 - (fen[1] - '0');
        
        b->enpassant = rank * 8 + file;
    }
    else
        b->enpassant = no_sq;
    
    for (int piece = P; piece <= K; piece++)
        b->occupancies[white] |= b->board[piece];
    
    for (int piece = p; piece <= k; piece++)
        b->occupancies[black] |= b->board[piece];
    
    b->occupancies[both] |= b->occupancies[white];
    b->occupancies[both] |= b->occupancies[black];
}

void print_attacked_squares(side s, board_t *b)
{
	int file, rank, sq; 
	printf("\n");
	for (rank = 0; rank < 8; rank++)
	{
		printf("  %d ", 8 - rank);
		for ( file = 0; file < 8; file++)
        	{
            		 sq = rank * 8 + file;
           		 printf(" %d", is_square_attacked(sq, s, b) ? 1 : 0);
        	}
       		 printf("\n");
    }
    printf("\n     a b c d e f g h\n\n");
}