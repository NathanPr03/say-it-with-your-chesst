#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "board.h"
#include "move.h"
#include "move_picker.h"
#include "minimax.h"

// Function to convert 1-indexed user input to 0-indexed coordinates
void convert_user_input_to_move(char* input, Move* move) {
    move->from_y = input[0] - '1';
    move->from_x = input[1] - '1';
    move->to_y = input[3] - '1';
    move->to_x = input[4] - '1';
}

void play_against_user(int move_counter) {
    char buffer[128];

    bool valid_move = false;
    Move users_move = {-1, -1, -1, -1, -1, false, EMPTY, false};
    Move black_move = {-1, -1, -1, -1, -1, false, EMPTY, false};

    while (!valid_move) {
        printf("Enter black's move (e.g., 21 23): ");
        fgets(buffer, sizeof(buffer), stdin);

        convert_user_input_to_move(buffer, &users_move);

        // Validate the move
        Move* all_blacks_moves = generate_moves_for_one_color(allPieces.blackPieces, true, 2);
        if (all_blacks_moves == NULL || (all_blacks_moves[0].to_x == 0 && all_blacks_moves[0].from_x == 0 &&
                                         all_blacks_moves[0].to_y == 0 && all_blacks_moves[0].from_y == 0)) {
            if(is_king_in_check(BLACK, 1)) {
                printf("\nWhite wins! Black is CHECKMATED, after %d moves\n", move_counter);
                exit(0);
            }
            printf("\nStalemate! It's a draw, after %d moves\n", move_counter);
            exit(0);
        }

        for (int j = 0; j < MAX_POTENTIAL_TOTAL_MOVES_PER_COLOR; j++) {
            Move* a_move = &all_blacks_moves[j];
            if (a_move->from_x == users_move.from_x && a_move->from_y == users_move.from_y &&
                a_move->to_x == users_move.to_x && a_move->to_y == users_move.to_y) {
                valid_move = true;
                // We want to get all the move information, i.e is promotion, is en passant. The user doesn't supply it, its generated programatically
                black_move = *a_move;
                break;
            }
        }
        free(all_blacks_moves);

        printf("Invalid move. Try again.\n");
    }

    execute_move(black_move, true);
    previous_move = black_move;

    printf("\n\n");
    print_board(NULL);
}

void play_against_bad_bot (int move_counter) {
    Move *all_blacks_moves = generate_moves_for_one_color(allPieces.blackPieces, true, 2);
    if (all_blacks_moves == NULL || (all_blacks_moves[0].to_x == 0 && all_blacks_moves[0].from_x == 0 &&
                                     all_blacks_moves[0].to_y == 0 && all_blacks_moves[0].from_y == 0)) {
        if(is_king_in_check(BLACK, 1)) {
            printf("\nWhite wins! Black is CHECKMATED, after %d moves\n", move_counter);
            exit(0);
        }
        printf("\nStalemate! It's a draw, after %d moves\n", move_counter);
        exit(0);
    }
    Move *black_move = choose_move(all_blacks_moves);

    execute_move(*black_move, true);
    previous_move = *black_move;

    printf("\n\n");
    print_board(NULL);
}

void play_against_good_bot(int move_counter) {
    MinimaxResult meeneymax = minimax(5, false, -INFINITY, INFINITY);
    Move *black_move = &meeneymax.best_move;

    if(black_move->to_x == -1 && black_move->to_y == -1) {
        if(is_king_in_check(BLACK, 1)) {
            printf("\nWhite wins! Black is CHECKMATED, after %d moves\n", move_counter);
            exit(0);
        }
        printf("\nStalemate! It's a draw, after %d moves\n", move_counter);
        exit(0);
    }

    execute_move(*black_move, true);
    previous_move = *black_move;

    printf("\n\n");
    print_board(NULL);
}

int main(int argc, char *argv[]) {
    printf("Hello, World!\n");

    init_board();
    print_board(NULL);

    void (*playModeFuncPtr)(int);

    const char *play_mode = "good-bot";
    if (argc > 1) {
        play_mode = argv[1];
    }

    if (strcmp(play_mode, "good-bot") == 0) {
        printf("Playing against good bot");
        playModeFuncPtr = play_against_good_bot;
    } else if (strcmp(play_mode, "bad-bot") == 0) {
        printf("Playing against bad bot");
        playModeFuncPtr = play_against_bad_bot;
    } else if (strcmp(play_mode, "user") == 0) {
        printf("Playing against user\n");
        playModeFuncPtr = play_against_user;
    } else {
        printf("Unknown play mode: %s\n", play_mode);
        return 1;
    }

    int move_counter = 0;
    for (int i = 0; i < 10000; i++) {
        move_counter++;
        MinimaxResult meeneymax = minimax(5, true, -INFINITY, INFINITY);
        Move *white_move = &meeneymax.best_move;

        if(white_move->to_x == -1 && white_move->to_y == -1) {
            if(is_king_in_check(BLACK, 1)) {
                printf("\nBlack wins! White is CHECKMATED, after %d moves\n", move_counter);
                exit(0);
            }
            printf("\nStalemate! It's a draw, after %d moves\n", move_counter);
            exit(0);
        }

        execute_move(*white_move, true);
        previous_move = *white_move;
        printf("\n\n");
        print_board(NULL);

        playModeFuncPtr(move_counter);
    }
}