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

Move* bogo_move(Move* moves) {
    int number_of_moves = 5;

    int random_index = get_random_index(number_of_moves);

    return &moves[random_index];
}
