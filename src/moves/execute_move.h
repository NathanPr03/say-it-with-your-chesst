#ifndef SAY_IT_WITH_YOUR_CHESST_EXECUTE_MOVE_H
#define SAY_IT_WITH_YOUR_CHESST_EXECUTE_MOVE_H
#include "move.h"

void execute_move_new(Move* move, bool is_undoing_move);
void undo_move(Move* move);

#endif //SAY_IT_WITH_YOUR_CHESST_EXECUTE_MOVE_H
