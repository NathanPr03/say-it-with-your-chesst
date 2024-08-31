#include <printf.h>
#include <stdbool.h>
#include "board.h"
#include "move.h"

int main() {
    printf("Hello, World!\n");

    init_board();
    print_board(NULL);

    bool myCon = is_king_in_check(WHITE);

    Move* moves = generate_all_legal_moves();
    for(int i=0; i<258; i++){
        printf("Move %d: %d %d %d %d\n", i, moves[i].from_x, moves[i].from_y, moves[i].to_x, moves[i].to_y);
    }
    return 0;
}