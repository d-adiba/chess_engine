#include "attacks.h"
#include "random.h"
#include "bitboard.h"
#include "move.h"
#include "test.h"
#include "uci.h"

#ifndef VERSION
#define VERSION "dev"
#endif

int main()
{
    init_all();
    board_t b_t; 
    
    uci_loop(&b_t);

    return 0;
}
