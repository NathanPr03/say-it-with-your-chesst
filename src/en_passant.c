#include <stdbool.h>
#include <stdlib.h>
#include "board.h"
#include "move.h"

bool is_move_en_passantable(Move move) {
    Piece piece = board[move.to_x][move.to_y].piece;
    bool was_piece_moved_two_squares = abs(move.from_x - move.to_x) == 2;

    return piece == PAWN && was_piece_moved_two_squares;
}