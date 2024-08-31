

#ifndef SAY_IT_WITH_YOUR_CHESST_MOVE_H
#define SAY_IT_WITH_YOUR_CHESST_MOVE_H

#include <stdbool.h>
#include "board.h"

typedef struct {
    int from_x;
    int from_y;
    int to_x;
    int to_y;
} Move;

bool is_king_in_check(Colour colour);

Move* generate_all_legal_moves();


#endif //SAY_IT_WITH_YOUR_CHESST_MOVE_H
