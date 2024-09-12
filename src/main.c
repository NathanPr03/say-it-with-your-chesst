#include <printf.h>
#include <stdbool.h>
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

        // Print move 10
        printf("Move 10: %d %d %d %d\n", all_whites_moves[10->from_x, white_move->from_y, white_move->to_x, white_move->to_y);
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