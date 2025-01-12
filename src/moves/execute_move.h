#ifndef SAY_IT_WITH_YOUR_CHESST_EXECUTE_MOVE_H
#define SAY_IT_WITH_YOUR_CHESST_EXECUTE_MOVE_H
#include "move.h"

void execute_move(Move* move, bool is_undoing_move);
void undo_move(Move* move);

Square** update_piece_pointer(Square* from, Square* to, Colour colour);

#endif //SAY_IT_WITH_YOUR_CHESST_EXECUTE_MOVE_H
