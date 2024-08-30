

#ifndef SAY_IT_WITH_YOUR_CHESST_MOVE_H
#define SAY_IT_WITH_YOUR_CHESST_MOVE_H

#include "board.h"

typedef struct {
    int from_x;
    int from_y;
    int to_x;
    int to_y;
} Move;

Move* generate_legal_moves_for_cell(Square *square);

#endif //SAY_IT_WITH_YOUR_CHESST_MOVE_H
