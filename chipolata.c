#include "attacks.h"
#include "random.h"
#include "bitboard.h"
#include "move.h"
#include "uci.h"
#include "test.h"
#include "eval.h"
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


    /**
     * >>>>>>Programme de la demo 
     * --------------------------
     * Aborder l'influence de l'evlaution positionnelle sur le comportement 
     * -> cas des cavalier version optimisé vs classique 
     * Aborder en quoi l'evolution du hardware peut influencer les performance 
     * -> cas de _pext_u64 et  _pdep_u64
     * -> finir en faisant jouer quelqu'un contre mon engine
     * 
     * 
     * 
     */

}
