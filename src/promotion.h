#ifndef SAY_IT_WITH_YOUR_CHESST_PROMOTION_H
#define SAY_IT_WITH_YOUR_CHESST_PROMOTION_H
    #include "board.h"

    void promote_pawn_to_other_piece(Square* pawn, Piece pieceToPromoteTo);
    Piece retrieve_promotion_piece(int index);
#endif
