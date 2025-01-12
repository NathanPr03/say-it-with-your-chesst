#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <printf.h>
#include "move.h"
#include "board.h"
#include "minimax.h"
#include "moves/execute_move.h"
#include "utils/validate_board.h"

//TODO: These should be in move.c once move.c is split up
int compare_moves(const void* a, const void* b) {
    Move* move_a = (Move*)a;
    Move* move_b = (Move*)b;

    if (move_a->score > move_b->score) {
        return -1; // move_a should come before move_b
    } else if (move_a->score < move_b->score) {
        return 1; // move_a should come after move_b
    } else {
        return 0; // equal scores
    }
}

void sort_moves_by_score(Move* moves, int num_moves) {
    qsort(moves, num_moves, sizeof(Move), compare_moves);
}

int count_valid_moves(Move* moves) {
    int count = 0;
    for (int i = 0; i < MAX_POTENTIAL_TOTAL_MOVES_PER_COLOR; i++) {
        if (moves[i].from_x == 0 && moves[i].from_y == 0 && moves[i].to_x == 0 && moves[i].to_y == 0) {
            break;
        }
        count++;
    }
    return count;
}

MinimaxResult minimax(int depth, bool isMaximizingPlayer, double alpha, double beta) {
    if (depth == 0) {
        double score = calculate_board_score(NULL);
        MinimaxResult result = (MinimaxResult) {score, (Move) {-1, -1, -1, -1}};
        return result;
    }

    // TODO: Can we parameterise some of this function?
    if (isMaximizingPlayer) {
        MinimaxResult best_result;
        best_result.score = -INFINITY;
        best_result.best_move = (Move){-1, -1, -1, -1};

        Move* moves = generate_moves_for_one_color(allPieces.whitePieces, true, 2);
        int num_moves = count_valid_moves(moves);
        sort_moves_by_score(moves, num_moves);
        for (int i = 0; i < MAX_POTENTIAL_TOTAL_MOVES_PER_COLOR; i++) {
            Move move = moves[i];
            if (move.from_x == 0 && move.from_y == 0 && move.to_x == 0 && move.to_y == 0) {
                break;
            }

            execute_move(&move, false);

            MinimaxResult current_result = minimax(depth - 1, false, alpha, beta);

            undo_move(&move);
            validate_board_state();

            if (current_result.score > best_result.score) {
                best_result.score = current_result.score;
                best_result.best_move = move;
            }

            alpha = fmax(alpha, best_result.score);
            if (beta <= alpha) {
                break; // Beta cutoff
            }
        }

        return best_result;
    } else {
        MinimaxResult best_result;
        best_result.score = INFINITY;
        best_result.best_move = (Move){-1, -1, -1, -1};

        Move* moves = generate_moves_for_one_color(allPieces.blackPieces, true, 2);
        int num_moves = count_valid_moves(moves);
        sort_moves_by_score(moves, num_moves);
        for (int i = 0; i < MAX_POTENTIAL_TOTAL_MOVES_PER_COLOR; i++) {
            Move move = moves[i];
            if (move.from_x == 0 && move.from_y == 0 && move.to_x == 0 && move.to_y == 0) {
                break;
            }
            execute_move(&move, false);

            MinimaxResult current_result = minimax(depth - 1, true, alpha, beta);

            undo_move(&move);
            validate_board_state();

            if (current_result.score < best_result.score) {
                best_result.score = current_result.score;
                best_result.best_move = move;
            }

            beta = fmin(beta, best_result.score);
            if (beta <= alpha) {
                break; // Alpha cutoff
            }
        }

        return best_result;
    }
}