#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include "move.h"
#include "move_picker.h"
#include "board.h"
#include "minimax.h"

MinimaxResult minimax(int depth, bool isMaximizingPlayer) {
    if (depth == 0) {
        double score = calculate_board_score(NULL);
        MinimaxResult result = (MinimaxResult) {score, (Move) {-1, -1, -1, -1}};
        return result;
    }

    if (isMaximizingPlayer) {
        MinimaxResult best_result;
        best_result.score = -INFINITY;
        best_result.best_move = (Move){-1, -1, -1, -1};

        Move* moves = generate_moves_for_one_color(allPieces.whitePieces, true);
        for (int i = 0; i < MAX_POTENTIAL_TOTAL_MOVES_PER_COLOR; i++) {
            Move move = moves[i];
            if(move.from_x == 0 && move.from_y == 0 && move.to_x == 0 && move.to_y == 0) {
                break;
            }
            Square previous_square_val = board[move.to_x][move.to_y];
            Square* previous_square = &board[move.to_x][move.to_y];

            // Execute move
            Square** just_taken_square = execute_move(move, false);

            MinimaxResult current_result = minimax(depth - 1, false);

            // Undo move
            execute_move((Move) {move.to_x, move.to_y, move.from_x, move.from_y}, false);
            board[move.to_x][move.to_y] = previous_square_val;

            if(just_taken_square != NULL) {
                *just_taken_square = previous_square;
            }

            if(move.from_x == 0 && move.from_y == 0 && move.to_x == 0 && move.to_y == 0) {
                current_result.score = -INFINITY;
            }

            if (current_result.score > best_result.score) {
                best_result.score = current_result.score;
                best_result.best_move = move;
            }
        }

        return best_result;
    } else {
        MinimaxResult best_result;
        best_result.score = INFINITY;
        best_result.best_move = (Move){-1, -1, -1, -1};

        Move* moves = generate_moves_for_one_color(allPieces.blackPieces, true);
        for (int i = 0; i < MAX_POTENTIAL_TOTAL_MOVES_PER_COLOR; i++) {
            Move move = moves[i];
            if(move.from_x == 0 && move.from_y == 0 && move.to_x == 0 && move.to_y == 0) {
                break;
            }
            Square previous_square_val = board[move.to_x][move.to_y];
            Square* previous_square = &board[move.to_x][move.to_y];

            // Execute move
            Square** just_taken_square = execute_move(move, false);

            MinimaxResult current_result = minimax(depth - 1, true);

            // Undo move
            execute_move((Move) {move.to_x, move.to_y, move.from_x, move.from_y}, false);
            board[move.to_x][move.to_y] = previous_square_val;

            if(just_taken_square != NULL) {
                *just_taken_square = previous_square;
            }

            if(move.from_x == 0 && move.from_y == 0 && move.to_x == 0 && move.to_y == 0) {
                current_result.score = -INFINITY;
            }

            if (current_result.score < best_result.score) {
                best_result.score = current_result.score;
                best_result.best_move = move;
            }
        }

        return best_result;
    }
}
