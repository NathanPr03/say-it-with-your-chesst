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

    for(int i = 0; i < 10000; i++) {
        Move* all_whites_moves = generate_moves_for_one_color(allPieces.whitePieces, true);
        if (all_whites_moves == NULL) {
            printf("White has no moves left. Checkmate?\n");
            break;
        }
        Move* white_move = choose_move(all_whites_moves);

        execute_move(*white_move, true);

        printf("\n\n");
        print_board(NULL);

        char buffer[128];

        printf("Press Enter to continue...");
        fgets(buffer, sizeof(buffer), stdin);

        Move* all_blacks_moves = generate_moves_for_one_color(allPieces.blackPieces, true);
        Move* black_move = choose_move(all_blacks_moves);
        execute_move(*black_move, true);

        printf("\n\n");
        print_board(NULL);

        printf("Press Enter to continue...");
        fgets(buffer, sizeof(buffer), stdin);
    }

}