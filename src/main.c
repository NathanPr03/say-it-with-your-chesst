#include <printf.h>
#include <stdbool.h>
#include <stdlib.h>
#include "board.h"
#include "move.h"
#include "move_picker.h"
#include "minimax.h"

int main() {
    printf("Hello, World!\n");

    init_board();
    print_board(NULL);

    char buffer[128];


    printf("Press Enter to continue...");
    fgets(buffer, sizeof(buffer), stdin);

    for (int i = 0; i < 10000; i++) {
        Move *all_whites_moves = generate_moves_for_one_color(allPieces.whitePieces, true);
        if (all_whites_moves == NULL || (all_whites_moves->to_x == 0 && all_whites_moves->from_x == 0 &&
                                         all_whites_moves->to_y == 0 && all_whites_moves->from_y == 0)) {
            printf("\nBlack wins! White is CHEKMATED \n");
            break;
        }
        MinimaxResult meeneymax = minimax(3, true);
        Move *white_move = &meeneymax.best_move;

        execute_move(*white_move, true);

        printf("\n\n");
        print_board(NULL);

        printf("Press Enter to continue...");
        fgets(buffer, sizeof(buffer), stdin);

        Move *all_blacks_moves = generate_moves_for_one_color(allPieces.blackPieces, true);
        if (all_blacks_moves == NULL || (all_blacks_moves->to_x == 0 && all_blacks_moves->from_x == 0 &&
                                         all_blacks_moves->to_y == 0 && all_blacks_moves->from_y == 0)) {
            printf("\nWhite wins! Black is CHEKMATED \n");
            return 0;
        }
        Move *black_move = choose_move(all_blacks_moves);
        execute_move(*black_move, true);

        printf("\n\n");
        print_board(NULL);

        printf("Press Enter to continue...");
        fgets(buffer, sizeof(buffer), stdin);
    }

}