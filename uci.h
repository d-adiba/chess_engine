#ifndef UCI_H
#define UCI_H

#include "bitboard.h"
#include "move.h"
#include <stdlib.h>
#include "eval.h"
#include "test.h"




int parse_move(char *move_str, board_t *b_t);
void parse_position(char *command, board_t *b_t);
void parse_go(char *command, board_t *b_t);
void uci_loop(board_t *b_t);

#endif