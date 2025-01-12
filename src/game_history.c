#include "game_history.h"
#include "stdlib.h"

GameHistory* game_history;

void create_game_history() {
    game_history = (GameHistory*) calloc(1, sizeof(GameHistory));
    game_history->length = 0;
    game_history->moves = (GameHistoryMove*) calloc(1, sizeof(GameHistoryMove));
}

void add_move_to_game_history(Move move, Square from_square, Square to_square, Square** ref_to_all_pieces_ptr) {
    game_history->moves = (GameHistoryMove*) realloc(game_history->moves, (game_history->length + 1) * sizeof(GameHistoryMove));
    game_history->moves[game_history->length].move = move;
    game_history->moves[game_history->length].from_square = from_square;
    game_history->moves[game_history->length].to_square = to_square;
    game_history->moves[game_history->length].ref_to_all_pieces_ptr = ref_to_all_pieces_ptr;

    game_history->length++;
}

/**
 * Function used to check if a given piece is in the game history of moves.
 * Naive approach here, as theres nothing tying the from x/y coord and the piece together.
 * Should still work however, as we know the starting locations of all pieces.
 * Used in castling.
 */
bool does_game_history_contain_any_moves_from_piece(Square square) {
    // We need a move of to and from here as `to_square` and `from_square` are from BEFORE the move was executed
    // Whereas the parameter `square` is after the move was executed
    for (int i = 0; i < game_history->length; i++) {
        if (game_history->moves[i].to_square.x_coord == square.x_coord &&
            game_history->moves[i].to_square.y_coord == square.y_coord &&
            game_history->moves[i].from_square.piece == square.piece &&
            game_history->moves[i].from_square.color == square.color) {

            return true;
        }
    }

    return false;
}

GameHistoryMove get_most_recent_move() {
    if (game_history->length == 0) {
        return (GameHistoryMove) {
            0, 0, 0, 0,0,
            false, EMPTY,
            false, false
        };
    }

    GameHistoryMove most_recent_move = game_history->moves[game_history->length - 1];

    return most_recent_move;
}

// This is needed for en passant. When we execute an en passant move we want to capture the pawn from the previous move
// However due to order of operations in execute_move.c,
// the actual en passant move will be marked as the most recent when it comes to executing itself
GameHistoryMove get_move_before_most_recent_move() {
    if (game_history->length <= 1) {
        return (GameHistoryMove) {
                0, 0, 0, 0,0,
                false, EMPTY,
                false, false
        };
    }

    GameHistoryMove most_recent_move = game_history->moves[game_history->length - 2];

    return most_recent_move;
}

void pop_most_recent_move() {
    game_history->length--;
    game_history->moves = (GameHistoryMove*) realloc(game_history->moves, game_history->length * sizeof(GameHistoryMove));
}