#include <math.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdlib.h>
#include "move.h"
#include "minimax.h"

void test_en_passant() {
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

    // Kings
    board[0][0].piece = KING;
    board[0][0].color = BLACK;
    board[7][7].piece = KING;
    board[7][7].color = WHITE;

    // Pawns set up for en passant
    // White pawn on (6,3), Black pawn on (4,4)
    board[6][3].piece = PAWN;
    board[6][3].color = WHITE;
    board[4][4].piece = PAWN;
    board[4][4].color = BLACK;

    // Assign piece pointers
    allPieces.blackPieces->King = &board[0][0];
    allPieces.whitePieces->King = &board[7][7];
    allPieces.whitePieces->Pawns[0] = &board[6][3];
    allPieces.blackPieces->Pawns[0] = &board[4][4];

    // White moves first: pawn forward two squares
    MinimaxResult whiteFirst = minimax(2, true, -INFINITY, INFINITY);
    Move *white_move = &whiteFirst.best_move;
    Move white_move_expected = {6, 3, 4, 3};
    execute_move(white_move_expected, true);

    CU_ASSERT_TRUE(board[6][3].piece == EMPTY);
    CU_ASSERT_TRUE(board[6][3].color == NONE);
    CU_ASSERT_TRUE(board[4][3].piece == PAWN);
    CU_ASSERT_TRUE(board[4][3].color == WHITE);

    // Black responds with en passant
    MinimaxResult blackResp = minimax(2, false, -INFINITY, INFINITY);
    Move *black_move = &blackResp.best_move;

    //TODO: WHy doesnt black choose en passant?
    Move black_move_expected = {4, 4, 5, 3};
    black_move_expected.is_en_passant = true;
    execute_move(black_move_expected, false);

    // Black pawn should move to (5,3), capturing the white pawn that jumped
    CU_ASSERT_TRUE(board[4][4].piece == EMPTY);
    CU_ASSERT_TRUE(board[4][4].color == NONE);
    CU_ASSERT_TRUE(board[4][3].piece == EMPTY);
    CU_ASSERT_TRUE(board[4][3].color == NONE);
    CU_ASSERT_TRUE(board[5][4].piece == PAWN);
    CU_ASSERT_TRUE(board[5][4].color == BLACK);
}

int main() {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("EnPassantTest", 0, 0);

    CU_add_test(suite, "test_en_passant", test_en_passant);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}