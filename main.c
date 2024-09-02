#include <printf.h>
#include <stdbool.h>
#include "board.h"
#include "move.h"

int main() {
    printf("Hello, World!\n");

    init_board();
    print_board(NULL);

    Move* moves = generate_all_legal_moves();
    for(int i=0; i<258; i++){
        if (moves[i].from_x == 0 && moves[i].from_y == 0 &&
                moves[i].to_x == 0 && moves[i].to_y == 0) {
            break; // Skip empty moves
        }
        printf("Move %d: %d %d %d %d\n", i, moves[i].from_x, moves[i].from_y, moves[i].to_x, moves[i].to_y);
    }
    return 0;
}