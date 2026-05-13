
#include "uci.h"
#include "atomic/eval.h"
#include "atomic/test_atomic.h"

int parse_move(char *move_str, board_t *b_t)
{
    moves move_list;
    generate_moves(b_t, &move_list);
    int move; 
    int source_square = (move_str[0] - 'a') + (8 - (move_str[1] - '0')) * 8;
    int target_square = (move_str[2] - 'a') + (8 - (move_str[3] - '0')) * 8;
    int promotion_piece =  (move_str[4]) ? char_pieces[move_str[4]] : 0;
    if (promotion_piece)
    {
        promotion_piece = (b_t->side == black) ? char_pieces[move_str[4]] : char_pieces[move_str[4] - 32];

    }
    for (int move_count = 0; move_count < move_list.count; move_count++)
    {
        move  =  move_list.moves[move_count];
        if (get_move_source(move) == source_square && get_move_target(move) == target_square && get_move_promoted(move) == promotion_piece)
        {
            return move;
        }
        
    }
    return 0;   
}

void parse_position(char *command, board_t *b_t)
{
    command += 9;
    char *current_char = command;
    
    if (strncmp(command, "startpos", 8) == 0)
        parse_fen(start_position, b_t);
    
    else
    {
        current_char = strstr(command, "fen");
        
        if (current_char == NULL)
            parse_fen(start_position, b_t);
            
        else
        {
            current_char += 4;
            
            parse_fen(current_char, b_t);
        }
    }
    
    current_char = strstr(command, "moves");
    
    if (current_char != NULL)
    {
        current_char += 6;
        
        while(*current_char)
        {
            int move = parse_move(current_char, b_t);
            
            if (move == 0)
                break;
            
            make_move(b_t, move, all_moves);
            
            while (*current_char && *current_char != ' ') current_char++;
            
            current_char++;
        }
        
    }
    print_board(b_t);
    printf("\n");
}

void parse_go(char *command, board_t *b_t)
{
    int depth = -1;
    
    char *current_depth = NULL;
    
    if (current_depth = strstr(command, "depth"))
    {
        depth = atoi(current_depth + 6);
        if (depth == 0) depth = 6; 
        search_position(depth, b_t);

    }
    else
    {
        if (current_depth = strstr(command, "perft"))
        {
            depth = atoi(current_depth + 6);
            if (depth == 0) depth = 6;
            perft_atomic_test(depth,b_t);
        }
    } 
    if(current_depth = strstr(command, "wtime"))
    {
        search_position(6,b_t);
    }
}

void uci_loop(board_t *b_t)
{
    setbuf(stdout, NULL);
    setbuf(stdin, NULL);

    char input[2000];
    


    while(1)
    {
        memset(input, 0, sizeof(input));
        fflush(stdout);
        
        if (!fgets(input, sizeof(input), stdin))
            continue;
        
        if (input[0] == '\n')
            continue;
        
        if (strncmp(input, "isready", 7) == 0)
        {
            printf("readyok\n");
            continue;
        }    
        if (strncmp(input, "position", 8) == 0)
        {
            parse_position(input, b_t);
            continue;
        }
        if (strncmp(input, "go", 2) == 0)
        {
            parse_go(input, b_t);
            continue;
        }
        if (strncmp(input, "ucinewgame", 10) == 0)
        {
            parse_position("position startpos", b_t);
            continue;
        }
        if (strncmp(input, "quit", 4) == 0)
        {
            break;
        }
        if(strncmp(input, "uci", 3) == 0)
        {
            printf("id name Chipolata Chess %s\n", VERSION);
            printf("id author Adiba Detche\n");
            printf("option name UCI_Variant type combo default atomic var atomic\n");
            printf("uciok\n");
            continue;
        }
        if(strncmp(input, "mm", 2) == 0)
        {
            make_atomic_move(b_t,parse_move(input + 3, b_t),all_moves);
            print_board(b_t);
            printf("\n");
        }
    }
}