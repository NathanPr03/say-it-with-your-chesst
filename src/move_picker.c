#include <stdlib.h>
#include <time.h>
#include <printf.h>
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

Move* bogo_move(Move* moves) {
    int number_of_moves = 5;

    int random_index = get_random_index(number_of_moves);

    return &moves[random_index];
}


int calculate_piece_value(Piece pieceToBeTaken) {
    switch (pieceToBeTaken) {
        case PAWN:
            return 1;
        case KNIGHT:
            return 3;
        case BISHOP:
            return 3;
        case ROOK:
            return 5;
        case QUEEN:
            return 9;
        case KING:
            return 100;
        default:
            return 0;
    }
}

int calculate_move_score(Move* move) {
    if (move == NULL) {
        printf("Error: Move is NULL\n");
        return 0;
    }
    int destination_x = move->to_x;
    int destination_y = move->to_y;

    Square to_be_taken = board[destination_x][destination_y];
    int value = calculate_piece_value(to_be_taken.piece);

    return value;
}
