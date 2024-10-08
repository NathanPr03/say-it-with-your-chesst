#ifndef SAY_IT_WITH_YOUR_CHESST_MOVE_H
#define SAY_IT_WITH_YOUR_CHESST_MOVE_H

#include <stdbool.h>
#include "board.h"
#pragma once

extern const int MAX_POTENTIAL_TOTAL_MOVES_PER_COLOR;

typedef struct {
    int from_x;
    int from_y;
    int to_x;
    int to_y;
    double score;
} Move;

bool is_king_in_check(Colour colour, int depth);

Move* generate_all_legal_moves();

Move* generate_moves_for_one_color(OneColoursPieces* aColoursPieces, bool include_king, int depth);

// TODO: Shouldnt be in header
Square** execute_move(Move move, bool commit);

#endif //SAY_IT_WITH_YOUR_CHESST_MOVE_H
