#include <math.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdlib.h>
#include "move.h"
#include "minimax.h"
#include "castle.h"

void test_castle_short() {
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

    board[7][4].piece = KING;
    board[7][4].color = WHITE;
    board[7][7].piece = ROOK;
    board[7][7].color = WHITE;

    board[0][4].piece = KING;
    board[0][4].color = BLACK;
    board[5][3].piece = KNIGHT;
    board[5][3].color = WHITE;

    allPieces.whitePieces->King = &board[7][4];
    allPieces.whitePieces->Rooks[1] = &board[7][7];
    allPieces.whitePieces->Knights[0] = &board[5][3];

    allPieces.blackPieces->King = &board[0][4];

    MinimaxResult whiteDecision = minimax(3, true, -INFINITY, INFINITY);
    Move *white_move = &whiteDecision.best_move;

    CU_ASSERT_TRUE(white_move->is_castling);

    execute_move(*white_move, true);
    previous_move = *white_move;

    CU_ASSERT_TRUE(board[7][4].piece == EMPTY);
    CU_ASSERT_TRUE(board[7][4].color == NONE);
    CU_ASSERT_TRUE(board[7][6].piece == KING);
    CU_ASSERT_TRUE(board[7][6].color == WHITE);
    CU_ASSERT_TRUE(board[7][5].piece == ROOK);
    CU_ASSERT_TRUE(board[7][5].color == WHITE);
}

void test_castle_long() {
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

    board[7][4].piece = KING;
    board[7][4].color = WHITE;
    board[7][0].piece = ROOK;
    board[7][0].color = WHITE;

    board[0][4].piece = KING;
    board[0][4].color = BLACK;
    board[5][3].piece = KNIGHT;
    board[5][3].color = WHITE;

    allPieces.whitePieces->King = &board[7][4];
    allPieces.whitePieces->Rooks[0] = &board[7][0];
    allPieces.whitePieces->Knights[0] = &board[5][3];

    allPieces.blackPieces->King = &board[0][4];

    MinimaxResult whiteDecision = minimax(3, true, -INFINITY, INFINITY);
    Move *white_move = &whiteDecision.best_move;

    CU_ASSERT_TRUE(white_move->is_castling);

    execute_move(*white_move, true);
    previous_move = *white_move;

    CU_ASSERT_TRUE(board[7][4].piece == EMPTY);
    CU_ASSERT_TRUE(board[7][4].color == NONE);
    CU_ASSERT_TRUE(board[7][2].piece == KING);
    CU_ASSERT_TRUE(board[7][2].color == WHITE);
    CU_ASSERT_TRUE(board[7][3].piece == ROOK);
    CU_ASSERT_TRUE(board[7][3].color == WHITE);
}

void test_cant_castle_when_king_has_been_checked()
{
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

    board[7][4].piece = KING;
    board[7][4].color = WHITE;
    board[7][0].piece = ROOK;
    board[7][0].color = WHITE;

    board[0][4].piece = KING;
    board[0][4].color = BLACK;
    board[5][3].piece = KNIGHT;
    board[5][3].color = WHITE;

    allPieces.whitePieces->King = &board[7][4];
    allPieces.whitePieces->Rooks[0] = &board[7][0];
    allPieces.whitePieces->Knights[0] = &board[5][3];

    allPieces.blackPieces->King = &board[0][4];

    mark_king_as_in_check(WHITE);
    MinimaxResult whiteDecision = minimax(3, true, -INFINITY, INFINITY);
    Move *white_move = &whiteDecision.best_move;
    execute_move(*white_move, true);
    previous_move = *white_move;

    CU_ASSERT_FALSE(white_move->is_castling);
}

int main() {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("CastleTest", 0, 0);

    CU_add_test(suite, "test_castle_short", test_castle_short);
    CU_add_test(suite, "test_castle_long", test_castle_long);
    CU_add_test(suite, "test_cant_castle_when_king_has_been_checked", test_cant_castle_when_king_has_been_checked);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return 0;
}