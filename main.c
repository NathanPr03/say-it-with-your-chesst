#include <printf.h>
#include <stdbool.h>
#include <stdlib.h>
#include "board.h"
#include "move.h"
#include "move_picker.h"

int main() {
    printf("Hello, World!\n");

    init_board();
    print_board(NULL);

    for(int i = 0; i < 8; i++) {
        Move* all_whites_moves = generate_moves_for_one_color(allPieces.whitePieces, true);
        Move* white_move = bogo_move(all_whites_moves);
        execute_move(*white_move, true);

//        free(all_whites_moves);
//        free(white_move);

        printf("////////////////////////////////////////\n");
        print_board(NULL);

        Move* all_blacks_moves = generate_moves_for_one_color(allPieces.blackPieces, true);
        Move* black_move = bogo_move(all_blacks_moves);
        execute_move(*black_move, true);

//        free(all_blacks_moves);
//        free(black_move);

        printf("////////////////////////////////////////\n");
        print_board(NULL);
    }

}