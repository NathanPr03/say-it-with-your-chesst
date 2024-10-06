#include <printf.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "board.h"
#include "move.h"
#include "move_picker.h"
#include "minimax.h"

int main() {
    printf("Hello, World!\n");

    init_board();
    print_board(NULL);

    char buffer[128];


//    printf("Press Enter to continue...");
//    fgets(buffer, sizeof(buffer), stdin);

    for (int i = 0; i < 10000; i++) {
        MinimaxResult meeneymax = minimax(7, true, -INFINITY, INFINITY);
        Move *white_move = &meeneymax.best_move;

        execute_move(*white_move, true);

        printf("\n\n");
        print_board(NULL);

//        printf("Press Enter to continue...");
//        fgets(buffer, sizeof(buffer), stdin);

        Move *all_blacks_moves = generate_moves_for_one_color(allPieces.blackPieces, true, 2);
        if (all_blacks_moves == NULL || (all_blacks_moves[0].to_x == 0 && all_blacks_moves[0].from_x == 0 &&
                                         all_blacks_moves[0].to_y == 0 && all_blacks_moves[0].from_y == 0)) {
            if(is_king_in_check(BLACK, 1)) {
                printf("\nWhite wins! Black is CHECKMATED\n");
                printf("\nNumber of moves played: %d \n", i);
                return 0;
            }
            printf("\nStalemate! It's a draw\n");
            return 0;
        }
        Move *black_move = choose_move(all_blacks_moves);

        execute_move(*black_move, true);

        printf("\n\n");
        print_board(NULL);

//        printf("Press Enter to continue...");
//        fgets(buffer, sizeof(buffer), stdin);
    }
    printf("1000 moves over");
}