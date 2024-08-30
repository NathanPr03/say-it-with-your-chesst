#include "move.h"
#include "board.h"
#include <printf.h>
#include <stdlib.h>

const int MAX_POTENTIAL_MOVES = 27;

Move* generate_legal_moves_for_cell(Square *square) {
    Move* moves = (Move*) malloc(MAX_POTENTIAL_MOVES * sizeof(Move));

    Piece piece = square->piece;
    Colour colour = square->color;
    int x = square->x_coord;
    int y = square->y_coord;

    if (piece == EMPTY) {
        return NULL;
    }

    if (piece == PAWN) {
        if (colour == WHITE) {
            //TODO: How do we prevent out of bounds?

            // Move forward
            if(board[x-1][y].piece == EMPTY) {
                moves[0] = (Move) {x, y, x-1, y};
            }

            // Take to left
            if (board[x-1][y-1].color == BLACK) {
                moves[1] = (Move) {x, y, x-1, y-1};
            }

            // Take to right
            if (board[x-1][y+1].color == BLACK) {
                moves[2] = (Move) {x, y, x-1, y+1};
            }

            // Promote TODO: Implement promotion
            if(x == 1 && (board[0][y].piece == EMPTY || board[0][y-1].piece == EMPTY) || board[0][y+1].piece == EMPTY){
                moves[3] = (Move) {x, y, 99999, 99999};
            }
        }
    }

    return moves;
}