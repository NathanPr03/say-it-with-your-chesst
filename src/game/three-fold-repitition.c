#include <stdbool.h>
#include "three-fold-repitition.h"
#include "../include/uthash.h"
#include "board/zobrist.h"
#include "zobrist_game_history.h"

bool is_threefold_repetition() {
    BoardState *state = NULL;

    HASH_FIND(hh, zobrist_board_states, &current_board_hash, sizeof(ZobristKey), state);

    return (state != NULL && state->count >= 3);
}