#ifndef SAY_IT_WITH_YOUR_CHESST_ZOBRIST_H
#define SAY_IT_WITH_YOUR_CHESST_ZOBRIST_H

#include "board.h"
#include "move.h"

void init_zobrist_table();
void update_zobrist_hash(Move* move, Square* from_square, Square* to_square);

#endif //SAY_IT_WITH_YOUR_CHESST_ZOBRIST_H
