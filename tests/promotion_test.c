#include <math.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdlib.h>
#include "move.h"
#include "minimax.h"

void test_promotion_by_moving_forward() {
    OneColoursPieces* blackPieces = (OneColoursPieces*) calloc(1, sizeof(OneColoursPieces));
    OneColoursPieces* whitePieces = (OneColoursPieces*) calloc(1, sizeof(OneColoursPieces));

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

    board[7][7] = (Square) {KING, BLACK, 7, 7};
    board[0][0] = (Square) {KING, WHITE, 0, 0};

    board[1][6] = (Square) {PAWN, WHITE, 1, 6}; // White pawn one square away

    allPieces.whitePieces->King = &board[0][0];
    allPieces.blackPieces->King = &board[7][7];
    allPieces.whitePieces->Pawns[0] = &board[1][6];

    MinimaxResult meeneymax = minimax(1, true, -INFINITY, INFINITY);
    Move *white_move = &meeneymax.best_move;

    execute_move(*white_move, true);

    CU_ASSERT_TRUE(board[1][6].piece == EMPTY);
    CU_ASSERT_TRUE(board[1][6].color == NONE);

    CU_ASSERT_TRUE(board[0][6].color == WHITE);
    CU_ASSERT_TRUE(board[0][6].piece == QUEEN);

    CU_ASSERT_TRUE(allPieces.whitePieces->PromotedPieces[0] == &board[0][6]);
}

void test_promotion_by_taking_to_the_left() {
    OneColoursPieces* blackPieces = (OneColoursPieces*) calloc(1, sizeof(OneColoursPieces));
    OneColoursPieces* whitePieces = (OneColoursPieces*) calloc(1, sizeof(OneColoursPieces));

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

    board[7][7] = (Square) {KING, BLACK, 7, 7};
    board[0][0] = (Square) {KING, WHITE, 0, 0};

    // Set up promotion by taking to the left
    board[1][5] = (Square) {PAWN, WHITE, 1, 5}; // White pawn
    board[0][4] = (Square) {ROOK, BLACK, 0, 4}; // Black rook (capturable to the left)

    allPieces.whitePieces->King = &board[0][0];
    allPieces.blackPieces->King = &board[7][7];
    allPieces.whitePieces->Pawns[0] = &board[1][5];
    allPieces.blackPieces->Rooks[0] = &board[0][4];

    MinimaxResult meeneymax = minimax(1, true, -INFINITY, INFINITY);
    Move *white_move = &meeneymax.best_move;

    execute_move(*white_move, true);

    CU_ASSERT_TRUE(board[1][5].piece == EMPTY);
    CU_ASSERT_TRUE(board[1][5].color == NONE);


    CU_ASSERT_TRUE(board[0][4].color == WHITE);
    CU_ASSERT_TRUE(board[0][4].piece == QUEEN);

    CU_ASSERT_TRUE(allPieces.whitePieces->PromotedPieces[0] == &board[0][4]);
}

void test_promotion_by_taking_to_the_right() {
    OneColoursPieces* blackPieces = (OneColoursPieces*) calloc(1, sizeof(OneColoursPieces));
    OneColoursPieces* whitePieces = (OneColoursPieces*) calloc(1, sizeof(OneColoursPieces));

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

    board[7][7] = (Square) {KING, BLACK, 7, 7};
    board[0][0] = (Square) {KING, WHITE, 0, 0};

    // Set up promotion by taking to the left
    board[1][5] = (Square) {PAWN, WHITE, 1, 5}; // White pawn
    board[0][4] = (Square) {ROOK, BLACK, 0, 6}; // Black rook (capturable to the left)

    allPieces.whitePieces->King = &board[0][0];
    allPieces.blackPieces->King = &board[7][7];
    allPieces.whitePieces->Pawns[0] = &board[1][5];
    allPieces.blackPieces->Rooks[0] = &board[0][6];

    MinimaxResult meeneymax = minimax(1, true, -INFINITY, INFINITY);
    Move *white_move = &meeneymax.best_move;

    execute_move(*white_move, true);

    CU_ASSERT_TRUE(board[1][5].piece == EMPTY);
    CU_ASSERT_TRUE(board[1][5].color == NONE);


    CU_ASSERT_TRUE(board[0][6].color == WHITE);
    CU_ASSERT_TRUE(board[0][6].piece == QUEEN);

    CU_ASSERT_TRUE(allPieces.whitePieces->PromotedPieces[0] == &board[0][6]);
}

int main() {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("PromotionTest", 0, 0);

    CU_add_test(suite, "test_promotion_by_moving_forward", test_promotion_by_moving_forward);
    CU_add_test(suite, "test_promotion_by_taking_to_the_left", test_promotion_by_taking_to_the_left);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}
