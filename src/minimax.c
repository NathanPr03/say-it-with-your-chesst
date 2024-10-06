#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include "move.h"
#include "board.h"
#include "minimax.h"


int compare_moves(const void* a, const void* b) {
    Move* moveA = (Move*)a;
    Move* moveB = (Move*)b;

    if (moveA->score > moveB->score) {
        return -1; // moveA should come before moveB
    } else if (moveA->score < moveB->score) {
        return 1; // moveA should come after moveB
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
            Square previous_square_val = board[move.to_x][move.to_y];
            Square* previous_square = &board[move.to_x][move.to_y];

            // Execute move
            Square** just_taken_square = execute_move(move, false);

            MinimaxResult current_result = minimax(depth - 1, false, alpha, beta);

            // Undo move
            execute_move((Move){move.to_x, move.to_y, move.from_x, move.from_y}, false);
            board[move.to_x][move.to_y] = previous_square_val;

            if (just_taken_square != NULL) {
                *just_taken_square = previous_square;
            }

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
            Square previous_square_val = board[move.to_x][move.to_y];
            Square* previous_square = &board[move.to_x][move.to_y];

            // Execute move
            Square** just_taken_square = execute_move(move, false);

            MinimaxResult current_result = minimax(depth - 1, true, alpha, beta);

            // Undo move
            execute_move((Move){move.to_x, move.to_y, move.from_x, move.from_y}, false);
            board[move.to_x][move.to_y] = previous_square_val;

            if (just_taken_square != NULL) {
                *just_taken_square = previous_square;
            }

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