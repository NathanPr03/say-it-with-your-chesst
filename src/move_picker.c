#include <time.h>
#include "move_picker.h"
#include "postional_score_matrices.h"

const int MAX_POTENTIAL_MOVES_FOR_ONE_PIECE = 27;

Move* choose_move(Move* moves) {
    double top_score = -100;
    Move* top_move = NULL;
    for (int i = 0; i < MAX_POTENTIAL_MOVES_FOR_ONE_PIECE; i++) {
        Move* move = &moves[i];

        if (move->from_x == 0 && move->from_y == 0 && move->to_x == 0 && move->to_y == 0) {
            return top_move;
        }

        if (move->score > top_score) {
            top_score = move->score;
            top_move = move;
        }
    }

    return top_move;
}

double move_score_from_capture(Move* move) {
    double score = 0;
    int to_x = move->to_x;
    int to_y = move->to_y;

    OneColoursPieces other_pieces;
    Square* square = &board[to_x][to_y];
    Square* current_square = &board[move->from_x][move->from_y];

    if (current_square->color == WHITE) {
        other_pieces = *allPieces.blackPieces;
    } else {
        other_pieces = *allPieces.whitePieces;
    }

    if (square->piece == PAWN) {
      score += 1;
    } else if (square->piece == KNIGHT) {
      score += 3;
    } else if (square->piece == BISHOP) {
      score += 3;
    } else if (square->piece == ROOK) {
        score += 5;
    } else if (square->piece == KING) {
        score += 9999999; //TODO: How much should checks be worth?
    } else if (square->piece == QUEEN) {
        score += 9;
    } else if (other_pieces.King->x_coord == to_x && other_pieces.King->y_coord == to_y) { // Check
        score += 10; //TODO: How much should checks be worth?
    }

    return score;
}

double positional_move_score(Move* move) {
    double score = 0;

    int from_x = move->from_x;
    int from_y = move->from_y;

    int to_x = move->to_x;
    int to_y = move->to_y;

    Square* current_square = &board[move->from_x][move->from_y];

    if(current_square->color == WHITE) {
        if (current_square->piece == PAWN) {
            score += white_pawn_position_score[to_x][to_y] - white_pawn_position_score[from_x][from_y];
        } else if (current_square->piece == KNIGHT) {
            score += white_knight_position_score[to_x][to_y] - white_knight_position_score[from_x][from_y];
        } else if (current_square->piece == BISHOP) {
            score += white_bishop_position_score[to_x][to_y] - white_bishop_position_score[from_x][from_y];
        } else if (current_square->piece == ROOK) {
            score += white_rook_position_score[to_x][to_y] - white_rook_position_score[from_x][from_y];
        } else if (current_square->piece == KING) {
            score += white_king_position_score[to_x][to_y] - white_king_position_score[from_x][from_y];
        } else if (current_square->piece == QUEEN) {
            score += white_queen_position_score[to_x][to_y] - white_queen_position_score[from_x][from_y];
        }
    }else if(current_square->color == BLACK) {
        if (current_square->piece == PAWN) {
            score += black_pawn_position_score[to_x][to_y] - black_pawn_position_score[from_x][from_y];
        } else if (current_square->piece == KNIGHT) {
            score += black_knight_position_score[to_x][to_y] - black_knight_position_score[from_x][from_y];
        } else if (current_square->piece == BISHOP) {
            score += black_bishop_position_score[to_x][to_y] - black_bishop_position_score[from_x][from_y];
        } else if (current_square->piece == ROOK) {
            score += black_rook_position_score[to_x][to_y] - black_rook_position_score[from_x][from_y];
        } else if (current_square->piece == KING) {
            score += black_king_position_score[to_x][to_y] - black_king_position_score[from_x][from_y];
        } else if (current_square->piece == QUEEN) {
            score += black_queen_position_score[to_x][to_y] - black_queen_position_score[from_x][from_y];
        }
    }


    return score;
}

void calculate_move_score(Move* move) {
    double score = move_score_from_capture(move);
    score += positional_move_score(move);
    move->score = score;
}