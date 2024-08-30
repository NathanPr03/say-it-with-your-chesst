#include <printf.h>
#include "board.h"
#include "move.h"

int main() {
    printf("Hello, World!\n");

    init_board();
    print_board(NULL);

    Move* moves = generate_legal_moves_for_cell(&board[6][1]);

    return 0;
}