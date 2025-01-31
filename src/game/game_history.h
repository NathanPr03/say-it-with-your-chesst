#ifndef SAY_IT_WITH_YOUR_CHESST_GAME_HISTORY_H
#define SAY_IT_WITH_YOUR_CHESST_GAME_HISTORY_H

#include "move.h"

typedef struct {
    Move move;
    Square from_square;
    Square to_square;
    Square** ref_to_all_pieces_ptr; // This is a reference to the pointer in the allPieces struct. Needed for undos
} GameHistoryMove;

typedef struct {
    GameHistoryMove* moves;
    int length;
} GameHistory;

extern GameHistory *game_history;

void create_game_history();
void add_move_to_game_history(Move move, Square from_square, Square to_square, Square** ref_to_all_pieces_ptr);
bool does_game_history_contain_any_moves_from_piece(Square square);

GameHistoryMove get_most_recent_move();
GameHistoryMove get_move_before_most_recent_move();
void pop_most_recent_move();

bool add_move_to_game_history_zobrist(Move* move, Square* from_square, Square* to_square);

#endif //SAY_IT_WITH_YOUR_CHESST_GAME_HISTORY_H
