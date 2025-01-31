
#include "zobrist_game_history.h"

BoardState *zobrist_board_states = NULL;
void add_zobrist_board_state(ZobristKey hash) {
    BoardState *state = NULL;

    HASH_FIND(hh, zobrist_board_states, &hash, sizeof(ZobristKey), state);

    if (state == NULL) {
        state = malloc(sizeof(BoardState));
        state->hash = hash;
        state->count = 1;
        HASH_ADD(hh, zobrist_board_states, hash, sizeof(ZobristKey), state);
    } else {
        state->count++;
    }
}


void add_move_to_game_history_zobrist(Move* move, Square* from_square, Square* to_square) {
    update_zobrist_hash(move, from_square, to_square);

    add_zobrist_board_state(current_board_hash);
}
