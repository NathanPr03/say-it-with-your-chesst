#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include "move.h"
#include "board.h"
#include "minimax.h"
#include "pieces.h"

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

    // TODO: Can we parameterise come of this function?
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
            Move two_moves_ago = previous_move;
            previous_move = move; // Set this global variable for en passant

            MinimaxResult current_result = minimax(depth - 1, false, alpha, beta);

            // Undo move
            execute_move((Move){move.to_x, move.to_y, move.from_x, move.from_y}, false);
            previous_move = two_moves_ago;
            board[move.to_x][move.to_y] = previous_square_val;

            // Promotion requires a more complicated undo. This is because it's moving two different piece types.
            if(move.is_promotion) {
                OneColoursPieces **pieces = (board[move.from_x][move.from_y].color == WHITE) ?
                                            &allPieces.whitePieces : &allPieces.blackPieces;

                board[move.from_x][move.from_y].piece = PAWN;

                int pawn_index = find_next_empty_piece_index(board[move.from_x][move.from_y].color, PAWN);
                (*pieces)->Pawns[pawn_index] = &board[move.from_x][move.from_y];

                int promoted_piece_index = find_piece_index_by_coordinate(move.from_x, move.from_y, EMPTY, true);
                (*pieces)->PromotedPieces[promoted_piece_index] = NULL;
            } else if(move.is_en_passant) {
                Square* the_moved_to_square = &board[move.to_x+1][move.to_y]; // An en passant moves the pawn to the square behind the taken pawn
                Colour opposite_colour = BLACK;
                the_moved_to_square->color = opposite_colour;
                the_moved_to_square->piece = PAWN;

                *just_taken_square = the_moved_to_square;
            } else if(move.is_castling) {
                // Long castle
                if(move.to_y == 2) {
                    Square* where_rook_should_be = &board[move.to_x][0];
                    Square* moved_rook = &board[move.to_x][3];

                    where_rook_should_be->piece = moved_rook->piece;
                    where_rook_should_be->color = moved_rook->color;

                    moved_rook->piece = EMPTY;
                    moved_rook->color = NONE;

                    *just_taken_square = where_rook_should_be;
                }else if(move.to_y == 6) { // Short castle
                    Square* where_rook_should_be = &board[move.to_x][7];
                    Square* moved_rook = &board[move.to_x][5];

                    where_rook_should_be->piece = moved_rook->piece;
                    where_rook_should_be->color = moved_rook->color;

                    moved_rook->piece = EMPTY;
                    moved_rook->color = NONE;

                    *just_taken_square = where_rook_should_be;
                }
            } else if (just_taken_square != NULL) {
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
            Move two_moves_ago = previous_move;
            previous_move = move; // Set this global variable for en passants

            MinimaxResult current_result = minimax(depth - 1, true, alpha, beta);

            // Undo move
            execute_move((Move){move.to_x, move.to_y, move.from_x, move.from_y}, false);
            previous_move = two_moves_ago;
            board[move.to_x][move.to_y] = previous_square_val;

            // Promotion requires a more complicated undo. This is because it's moving two different piece types.
            if(move.is_promotion) {
                OneColoursPieces **pieces = (board[move.from_x][move.from_y].color == WHITE) ?
                                            &allPieces.whitePieces : &allPieces.blackPieces;

                board[move.from_x][move.from_y].piece = PAWN;

                int pawn_index = find_next_empty_piece_index(board[move.from_x][move.from_y].color, PAWN);
                (*pieces)->Pawns[pawn_index] = &board[move.from_x][move.from_y];

                int promoted_piece_index = find_piece_index_by_coordinate(move.from_x, move.from_y, EMPTY, true);
                (*pieces)->PromotedPieces[promoted_piece_index] = NULL;
            } else if(move.is_en_passant) {
                Square* the_moved_to_square = &board[move.to_x-1][move.to_y]; // An en passant moves the pawn to the square behind the taken pawn
                Colour opposite_colour = WHITE;
                the_moved_to_square->color = opposite_colour;
                the_moved_to_square->piece = PAWN;

                *just_taken_square = the_moved_to_square;
            } else if(move.is_castling) {
                // Long castle
                if(move.to_y == 2) {
                    Square* where_rook_should_be = &board[move.to_x][0];
                    Square* moved_rook = &board[move.to_x][3];

                    where_rook_should_be->piece = moved_rook->piece;
                    where_rook_should_be->color = moved_rook->color;

                    moved_rook->piece = EMPTY;
                    moved_rook->color = NONE;

                    *just_taken_square = where_rook_should_be;
                }else if(move.to_y == 6) { // Short castle
                    Square* where_rook_should_be = &board[move.to_x][7];
                    Square* moved_rook = &board[move.to_x][5];

                    where_rook_should_be->piece = moved_rook->piece;
                    where_rook_should_be->color = moved_rook->color;

                    moved_rook->piece = EMPTY;
                    moved_rook->color = NONE;

                    *just_taken_square = where_rook_should_be;
                }
            } else if (just_taken_square != NULL) {
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