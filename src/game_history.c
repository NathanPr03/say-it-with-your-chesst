#include "game_history.h"
#include "stdlib.h"

GameHistory* game_history;

void create_game_history() {
    game_history = (GameHistory*) calloc(1, sizeof(GameHistory));
    game_history->length = 0;
    game_history->moves = (GameHistoryMove*) calloc(1, sizeof(GameHistoryMove));
}

void add_move_to_game_history(Move* move, Square from_square, Square to_square) {
    game_history->moves = (GameHistoryMove*) realloc(game_history->moves, game_history->length + 1 * sizeof(GameHistoryMove));
    game_history->moves[game_history->length].move = move;
    game_history->moves[game_history->length].from_square = from_square;
    game_history->moves[game_history->length].to_square = to_square;

    game_history->length++;
}

/**
 * Function used to check if a given piece is in the game history of moves.
 * Naive approach here, as theres nothing tying the from x/y coord and the piece together.
 * Should still work however, as we know the starting locations of all pieces.
 * Used in castling.
 */
bool does_game_history_contain_any_moves_from_piece(Square square) {
    for (int i = 0; i < game_history->length; i++) {
        if (game_history->moves[i].from_square.x_coord == square.x_coord &&
            game_history->moves[i].from_square.y_coord == square.y_coord &&
            game_history->moves[i].from_square.piece == square.piece &&
            game_history->moves[i].from_square.color == square.color) {

            return true;
        }
    }

    return false;
}