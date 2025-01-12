#include <stddef.h>
#include "board.h"
#include "move.h"
#include "moves/execute_move.h"

int next_promotion_index(Colour colour) {
    if (colour == WHITE) {
        for (int i = 0; i < 8; i++) {
            if (allPieces.whitePieces->PromotedPieces[i] == NULL || (
                    allPieces.whitePieces->PromotedPieces[i]->color == NONE
                && allPieces.whitePieces->PromotedPieces[i]->piece == EMPTY
                && allPieces.whitePieces->PromotedPieces[i]->x_coord == 0
                && allPieces.whitePieces->PromotedPieces[i]->y_coord == 0)) {
                return i;
            }
        }
    } else if (colour == BLACK) {
        for (int i = 0; i < 8; i++) {
            if (allPieces.blackPieces->PromotedPieces[i] == NULL || (
                    allPieces.blackPieces->PromotedPieces[i]->color == NONE
                && allPieces.blackPieces->PromotedPieces[i]->piece == EMPTY
                && allPieces.blackPieces->PromotedPieces[i]->x_coord == 0
                && allPieces.blackPieces->PromotedPieces[i]->y_coord == 0)) {
                return i;
            }
        }
    }

    return -1;
}

void promote_pawn_to_other_piece(Square* wasPawn, Piece pieceToPromoteTo){
    Colour colour = wasPawn->color;
    int index = next_promotion_index(colour);

    if (colour == WHITE) {
        wasPawn->piece = pieceToPromoteTo;
        allPieces.whitePieces->PromotedPieces[index] = wasPawn;

        update_piece_pointer(wasPawn, NULL, colour);
    } else if (colour == BLACK) {
        wasPawn->piece = pieceToPromoteTo;
        allPieces.blackPieces->PromotedPieces[index] = wasPawn;

        update_piece_pointer(wasPawn, NULL, colour);
    }
}

Piece retrieve_promotion_piece(int index){
   if(index == 0){
       return QUEEN;
   } else if(index == 1){
       return ROOK;
   } else if(index == 2){
       return BISHOP;
   } else if(index == 3){
       return KNIGHT;
   } else {
       return EMPTY;
   }
}