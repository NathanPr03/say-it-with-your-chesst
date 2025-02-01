#ifndef SAY_IT_WITH_YOUR_CHESST_ZOBRIST_GAME_HISTORY_H
#define SAY_IT_WITH_YOUR_CHESST_ZOBRIST_GAME_HISTORY_H

#include "board/zobrist.h"
#include "../include/uthash.h"

typedef struct {
    ZobristKey hash;   // The Zobrist hash key
    int count;         // Number of times this board state has occurred
    UT_hash_handle hh; // Makes this structure hashable
} BoardState;

void add_zobrist_board_state(ZobristKey hash);

extern BoardState *zobrist_board_states;
#endif //SAY_IT_WITH_YOUR_CHESST_ZOBRIST_GAME_HISTORY_H
