#include <stdlib.h>
#include <time.h>
#include "move_picker.h"

int get_random_index(int array_size) {
    // Ensure array_size is greater than 0
    if (array_size <= 0) {
        return -1;  // Error: Invalid array_size
    }

    // Seed the random number generator
    srand(time(NULL));

    // Generate a random number between 0 and array_size - 1
    return rand() % array_size;
}

Move* choose_move(Move* moves) {
    int top_score = -1;
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

Move* bogo_move(Move* moves) {
    int number_of_moves = 5;

    int random_index = get_random_index(number_of_moves);

    return &moves[random_index];
}

int move_score_from_capture(Move* move) {
    int score = 0;
    int to_x = move->to_x;
    int to_y = move->to_y;

    Square* square = &board[to_x][to_y];

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
    }

    return score;
}

void calculate_move_score(Move* move) {
    int score = move_score_from_capture(move);

    move->score = score;
}