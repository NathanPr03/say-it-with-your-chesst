#ifndef SAY_IT_WITH_YOUR_CHESST_MOVE_PICKER_H
#define SAY_IT_WITH_YOUR_CHESST_MOVE_PICKER_H
    #include "move.h"
    #pragma once
    extern const int MAX_POTENTIAL_MOVES_FOR_ONE_PIECE;

    Move* choose_move(Move* moves);
    Move* bogo_move(Move* moves);

    void calculate_move_score(Move* move);
#endif
