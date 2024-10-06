#ifndef SAY_IT_WITH_YOUR_CHESST_MINIMAX_H
#define SAY_IT_WITH_YOUR_CHESST_MINIMAX_H

#include <stdbool.h>
#include "move.h"

typedef struct {
    double score;
    Move best_move;
} MinimaxResult;

// Intentionally not returning a pointer as struct is small and easier to copy.
// They can be cached in registry which = fast operations
MinimaxResult minimax(int depth, bool isMaximizingPlayer, double alpha, double beta);

#endif //SAY_IT_WITH_YOUR_CHESST_MINIMAX_H
