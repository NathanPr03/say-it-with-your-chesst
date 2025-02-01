#include <MacTypes.h>
#include "board.h"
#include "pieces.h"

int find_next_empty_pawn_index(Colour colour) {
    OneColoursPieces *pieces = colour == WHITE ? allPieces.whitePieces : allPieces.blackPieces;

    for (int i = 0; i < 8; i++) {
        // TODO: Should we even be catering for null?
        if (pieces->Pawns[i] == NULL || (
            pieces->Pawns[i]->color == 0
            && pieces->Pawns[i]->piece == EMPTY
            && pieces->Pawns[i]->x_coord == 0
            && pieces->Pawns[i]->y_coord == 0)) {
            return i;
        }
    }

    return -1;
}

int find_next_empty_piece_index(Colour colour, Piece piece_type) {
    if(piece_type == PAWN) {
        return find_next_empty_pawn_index(colour);
    }

    return -1;
}

int find_promoted_piece_index_by_coordinate(int x, int y, Colour colour) {
    OneColoursPieces *pieces = colour == WHITE ? allPieces.whitePieces : allPieces.blackPieces;

    for (int i = 0; i < 8; i++) {
        if (pieces->PromotedPieces[i] != NULL &&
        (pieces->PromotedPieces[i]->x_coord == x && pieces->PromotedPieces[i]->y_coord == y)) {
            return i;
        }
    }

    return -1;
}

int find_piece_index_by_coordinate(int x, int y, bool is_promoted_piece) {
    if(is_promoted_piece) {
        return find_promoted_piece_index_by_coordinate(x, y, board[x][y].color);
    }

    return -1;
}