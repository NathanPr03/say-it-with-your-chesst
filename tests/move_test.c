#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdlib.h>
#include "board.h"
#include "move.h"
#include "move_picker.h"

void test_initial_board_moves() {
    init_board();

    Move *white_moves = generate_moves_for_one_color(allPieces.whitePieces, true);
    CU_ASSERT_PTR_NOT_NULL(white_moves);

    Move *black_moves = generate_moves_for_one_color(allPieces.blackPieces, true);
    CU_ASSERT_PTR_NOT_NULL(black_moves);

    // Can pawns move forward
    for (int i = 0; i < 16; i++) {
        int row = i;
        if (row == 1) {
            row = 0;
        } else {
            row = row / 2;
        }

        if (i % 2 == 0) {
            CU_ASSERT_TRUE(white_moves[i].from_x == 6);
            CU_ASSERT_TRUE(white_moves[i].to_x == 5);
            CU_ASSERT_TRUE(white_moves[i].from_y == row);
            CU_ASSERT_TRUE(white_moves[i].to_y == row);
        } else if (i % 2 == 1) {
            CU_ASSERT_TRUE(white_moves[i].from_x == 6);
            CU_ASSERT_TRUE(white_moves[i].to_x == 4);
            CU_ASSERT_TRUE(white_moves[i].from_y == row);
            CU_ASSERT_TRUE(white_moves[i].to_y == row);
        }
    }

    for (int i = 0; i < 16; i++) {
        int row = i;
        if (row == 1) {
            row = 0;
        } else {
            row = row / 2;
        }

        if (i % 2 == 0) {
            CU_ASSERT_TRUE(black_moves[i].from_x == 1);
            CU_ASSERT_TRUE(black_moves[i].to_x == 2);
            CU_ASSERT_TRUE(black_moves[i].from_y == row);
            CU_ASSERT_TRUE(black_moves[i].to_y == row);
        } else if (i % 2 == 1) {
            CU_ASSERT_TRUE(black_moves[i].from_x == 1);
            CU_ASSERT_TRUE(black_moves[i].to_x == 3);
            CU_ASSERT_TRUE(black_moves[i].from_y == row);
            CU_ASSERT_TRUE(black_moves[i].to_y == row);
        }
    }

    // Can knights move. These are dependent on the order of the moves.
    CU_ASSERT_TRUE(white_moves[16].from_x == 7);
    CU_ASSERT_TRUE(white_moves[16].to_x == 5);
    CU_ASSERT_TRUE(white_moves[16].from_y == 1);
    CU_ASSERT_TRUE(white_moves[16].to_y == 2);

    CU_ASSERT_TRUE(white_moves[17].from_x == 7);
    CU_ASSERT_TRUE(white_moves[17].to_x == 5);
    CU_ASSERT_TRUE(white_moves[17].from_y == 1);
    CU_ASSERT_TRUE(white_moves[17].to_y == 0);

    CU_ASSERT_TRUE(white_moves[18].from_x == 7);
    CU_ASSERT_TRUE(white_moves[18].to_x == 5);
    CU_ASSERT_TRUE(white_moves[18].from_y == 6);
    CU_ASSERT_TRUE(white_moves[18].to_y == 7);

    CU_ASSERT_TRUE(white_moves[19].from_x == 7);
    CU_ASSERT_TRUE(white_moves[19].to_x == 5);
    CU_ASSERT_TRUE(white_moves[19].from_y == 6);
    CU_ASSERT_TRUE(white_moves[19].to_y == 5);

    CU_ASSERT_TRUE(black_moves[16].from_x == 0);
    CU_ASSERT_TRUE(black_moves[16].to_x == 2);
    CU_ASSERT_TRUE(black_moves[16].from_y == 1);
    CU_ASSERT_TRUE(black_moves[16].to_y == 2);

    CU_ASSERT_TRUE(black_moves[17].from_x == 0);
    CU_ASSERT_TRUE(black_moves[17].to_x == 2);
    CU_ASSERT_TRUE(black_moves[17].from_y == 1);
    CU_ASSERT_TRUE(black_moves[17].to_y == 0);

    CU_ASSERT_TRUE(black_moves[18].from_x == 0);
    CU_ASSERT_TRUE(black_moves[18].to_x == 2);
    CU_ASSERT_TRUE(black_moves[18].from_y == 6);
    CU_ASSERT_TRUE(black_moves[18].to_y == 7);

    CU_ASSERT_TRUE(black_moves[19].from_x == 0);
    CU_ASSERT_TRUE(black_moves[19].to_x == 2);
    CU_ASSERT_TRUE(black_moves[19].from_y == 6);
    CU_ASSERT_TRUE(black_moves[19].to_y == 5);

    CU_ASSERT_TRUE(white_moves[20].from_x == 0);
    CU_ASSERT_TRUE(white_moves[20].to_x == 0);
    CU_ASSERT_TRUE(white_moves[20].from_y == 0);
    CU_ASSERT_TRUE(white_moves[20].to_y == 0);

    CU_ASSERT_TRUE(black_moves[20].from_x == 0);
    CU_ASSERT_TRUE(black_moves[20].to_x == 0);
    CU_ASSERT_TRUE(black_moves[20].from_y == 0);
    CU_ASSERT_TRUE(black_moves[20].to_y == 0);

    free(white_moves);
    free(black_moves);
}

void test_checkmate() {
    OneColoursPieces *blackPieces = (OneColoursPieces *) malloc(sizeof(OneColoursPieces));
    OneColoursPieces *whitePieces = (OneColoursPieces *) malloc(sizeof(OneColoursPieces));

    allPieces.whitePieces = whitePieces;
    allPieces.blackPieces = blackPieces;

    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            board[x][y].piece = EMPTY;
            board[x][y].color = NONE;
            board[x][y].x_coord = x;
            board[x][y].y_coord = y;
        }
    }

    // Place White King on e4 (x = 4, y = 3)
    board[7][7].piece = KING;
    board[7][7].color = WHITE;
    allPieces.whitePieces->King = &board[7][7];

    // Place Black King on e7 (x = 4, y = 6)
    board[1][3].piece = KING;
    board[1][3].color = BLACK;
    allPieces.blackPieces->King = &board[1][3];

    // Place Black Queen on d6 (x = 3, y = 5)
    board[7][0].piece = ROOK;
    board[7][0].color = BLACK;
    allPieces.blackPieces->Rooks[0] = &board[7][0];

    board[5][6].piece = QUEEN;
    board[5][6].color = BLACK;
    allPieces.blackPieces->Queen = &board[5][6];

    // Place Black Knight on c5 (x = 2, y = 4)
    board[7][6].piece = PAWN;
    board[7][6].color = WHITE;
    allPieces.whitePieces->Pawns[0] = &board[7][6];

    print_board(&board);

    // Game loop
    Move *all_blacks_moves = generate_moves_for_one_color(allPieces.blackPieces, true);
    if (all_blacks_moves == NULL) {
        printf("Black has no moves left. Checkmate?\n");
    }
    Move *black_move = choose_move(all_blacks_moves);
    execute_move(*black_move, true);

    free(all_blacks_moves);

    printf("Blacks turn finished\n\n");
    Move *all_whites_moves = generate_moves_for_one_color(allPieces.whitePieces, true);

    printf("from_x: %d, from_y: %d, to_x: %d, to_y: %d\n", all_whites_moves->from_x, all_whites_moves->from_y, all_whites_moves->to_x, all_whites_moves->to_y);

    CU_ASSERT_TRUE(all_whites_moves->from_x == 0 &&
                   all_whites_moves->to_x == 0 &&
                   all_whites_moves->from_y == 0 &&
                   all_whites_moves->to_y == 0) // Checkmate
}

int main() {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("ChessIntegrationTest", 0, 0);

    CU_add_test(suite, "test_initial_board_moves", test_initial_board_moves);
    CU_add_test(suite, "test_checkmate", test_checkmate);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}