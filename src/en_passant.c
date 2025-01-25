#include <stdbool.h>
#include <stdlib.h>
#include "board.h"
#include "move.h"
#include "game/game_history.h"

bool is_previous_move_en_passantable() {
    Move the_previous_move = get_most_recent_move().move;
    Piece piece = board[the_previous_move.to_x][the_previous_move.to_y].piece;
    bool was_piece_moved_two_squares = abs(the_previous_move.from_x - the_previous_move.to_x) == 2;

    return piece == PAWN && was_piece_moved_two_squares;
}