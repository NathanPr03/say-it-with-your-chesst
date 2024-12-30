#ifndef SAY_IT_WITH_YOUR_CHESST_CASTLE_H
#define SAY_IT_WITH_YOUR_CHESST_CASTLE_H

#include <stdbool.h>
#include "board.h"


void mark_king_as_moved(Colour colour);
void mark_rook_as_moved(Piece the_rook, Colour colour);
void mark_king_as_in_check(Colour colour);

bool can_long_castle(Square king);
bool can_short_castle(Square king);

#endif //SAY_IT_WITH_YOUR_CHESST_CASTLE_H
