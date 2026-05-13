#include "attacks.h"
#include "random.h"
#include "bitboard.h"
#include "move.h"
#include "test.h"
#include "uci.h"
#include "atomic/move_atomic.h"
#include "atomic/test_atomic.h"
#include "atomic/eval.h"
#include <unistd.h>

#define tricky_pos "6QQ/8/8/4k3/4K3/8/8/8 w - - 0 1 "


#ifndef VERSION
#define VERSION "dev"
#endif

int main()
{
    
    init_all();
    init_atomic_explosion_mask();
    init_atomic_explosion_attacks();
    board_t b_t;
    uci_loop(&b_t);

}
