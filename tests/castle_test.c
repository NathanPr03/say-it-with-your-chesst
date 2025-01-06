#include <math.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdlib.h>
#include "move.h"
#include "minimax.h"
#include "game_history.h"

void test_castle_short() {
    create_game_history();
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
    create_game_history();
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

void test_cant_castle_short_if_king_moved() {
    create_game_history();
    OneColoursPieces* blackPieces = (OneColoursPieces*) calloc(1, sizeof(OneColoursPieces));
    OneColoursPieces* whitePieces = (OneColoursPieces*) calloc(1, sizeof(OneColoursPieces));
    allPieces.whitePieces = whitePieces;
    allPieces.blackPieces = blackPieces;

    // Clear board
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            board[x][y].piece = EMPTY;
            board[x][y].color = NONE;
            board[x][y].x_coord = x;
            board[x][y].y_coord = y;
        }
    }

    // Place white king and rook to allow short castle - except we'll mark the king as having moved.
    board[7][4].piece = KING;
    board[7][4].color = WHITE;
    board[7][7].piece = ROOK;
    board[7][7].color = WHITE;

    // Place black king somewhere
    board[0][4].piece = KING;
    board[0][4].color = BLACK;

    // A white knight just to avoid engine thinking it's forced to do something else
    board[5][3].piece = KNIGHT;
    board[5][3].color = WHITE;

    allPieces.whitePieces->King = &board[7][4];
    allPieces.whitePieces->Rooks[1] = &board[7][7];
    allPieces.whitePieces->Knights[0] = &board[5][3];

    allPieces.blackPieces->King = &board[0][4];

    // Make a dummy king move to mark it "moved"
    Move king_move_away;
    king_move_away.from_x = 7;
    king_move_away.from_y = 4;
    king_move_away.to_x   = 7;
    king_move_away.to_y   = 3;
    king_move_away.is_castling = false;
    king_move_away.promotion_piece = EMPTY;
    king_move_away.is_promotion = false;
    king_move_away.is_en_passant = false;

    execute_move(king_move_away, true);

    // Move king back, ensuring the engine has recorded it as moved
    Move king_move_back;
    king_move_back.from_x = 7;
    king_move_back.from_y = 3;
    king_move_back.to_x   = 7;
    king_move_back.to_y   = 4;
    king_move_back.is_castling = false;
    king_move_back.promotion_piece = EMPTY;
    king_move_back.is_promotion = false;
    king_move_back.is_en_passant = false;

    execute_move(king_move_back, true);

    // Now attempt to castle via minimax
    MinimaxResult whiteDecision = minimax(3, true, -INFINITY, INFINITY);
    Move* white_move = &whiteDecision.best_move;

    CU_ASSERT_FALSE(white_move->is_castling);
}

void test_cant_castle_short_if_rook_moved() {
    create_game_history();
    OneColoursPieces* blackPieces = (OneColoursPieces*) calloc(1, sizeof(OneColoursPieces));
    OneColoursPieces* whitePieces = (OneColoursPieces*) calloc(1, sizeof(OneColoursPieces));
    allPieces.whitePieces = whitePieces;
    allPieces.blackPieces = blackPieces;

    // Clear board
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            board[x][y].piece = EMPTY;
            board[x][y].color = NONE;
            board[x][y].x_coord = x;
            board[x][y].y_coord = y;
        }
    }

    // Place white king and rook to allow short castle - except we'll mark the rook as having moved.
    board[7][4].piece = KING;
    board[7][4].color = WHITE;
    board[7][7].piece = ROOK;
    board[7][7].color = WHITE;

    // Place black king somewhere
    board[0][4].piece = KING;
    board[0][4].color = BLACK;

    // A white knight just to avoid engine ignoring the move
    board[5][3].piece = KNIGHT;
    board[5][3].color = WHITE;

    allPieces.whitePieces->King    = &board[7][4];
    allPieces.whitePieces->Rooks[1] = &board[7][7];
    allPieces.whitePieces->Knights[0] = &board[5][3];

    allPieces.blackPieces->King = &board[0][4];

    // Dummy rook move to mark it "moved"
    Move rook_move_away;
    rook_move_away.from_x = 7;
    rook_move_away.from_y = 7;
    rook_move_away.to_x   = 7;
    rook_move_away.to_y   = 6;
    rook_move_away.is_castling = false;
    rook_move_away.promotion_piece = EMPTY;
    rook_move_away.is_promotion = false;
    rook_move_away.is_en_passant = false;
    execute_move(rook_move_away, true);

    // Move rook back
    Move rook_move_back;
    rook_move_back.from_x = 7;
    rook_move_back.from_y = 6;
    rook_move_back.to_x   = 7;
    rook_move_back.to_y   = 7;
    rook_move_back.is_castling = false;
    rook_move_back.promotion_piece = EMPTY;
    rook_move_back.is_promotion = false;
    rook_move_back.is_en_passant = false;
    execute_move(rook_move_back, true);

    // Attempt to castle
    MinimaxResult whiteDecision = minimax(3, true, -INFINITY, INFINITY);
    Move* white_move = &whiteDecision.best_move;

    CU_ASSERT_FALSE(white_move->is_castling);
}

void test_cant_castle_long_if_king_moved() {
    create_game_history();
    OneColoursPieces* blackPieces = (OneColoursPieces*) calloc(1, sizeof(OneColoursPieces));
    OneColoursPieces* whitePieces = (OneColoursPieces*) calloc(1, sizeof(OneColoursPieces));
    allPieces.whitePieces = whitePieces;
    allPieces.blackPieces = blackPieces;

    // Clear board
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            board[x][y].piece = EMPTY;
            board[x][y].color = NONE;
            board[x][y].x_coord = x;
            board[x][y].y_coord = y;
        }
    }

    // White king and rook for long castle, but king will be "moved"
    board[7][4].piece = KING;
    board[7][4].color = WHITE;
    board[7][0].piece = ROOK;
    board[7][0].color = WHITE;

    // Black king
    board[0][4].piece = KING;
    board[0][4].color = BLACK;

    // Knight so engine has some alternative
    board[5][3].piece = KNIGHT;
    board[5][3].color = WHITE;

    allPieces.whitePieces->King      = &board[7][4];
    allPieces.whitePieces->Rooks[0]  = &board[7][0];
    allPieces.whitePieces->Knights[0] = &board[5][3];

    allPieces.blackPieces->King = &board[0][4];

    // Mark king as moved
    Move king_move_away;
    king_move_away.from_x = 7;
    king_move_away.from_y = 4;
    king_move_away.to_x   = 7;
    king_move_away.to_y   = 3;
    king_move_away.is_castling = false;
    king_move_away.promotion_piece = EMPTY;
    king_move_away.is_promotion = false;
    king_move_away.is_en_passant = false;
    execute_move(king_move_away, true);

    Move king_move_back;
    king_move_back.from_x = 7;
    king_move_back.from_y = 3;
    king_move_back.to_x   = 7;
    king_move_back.to_y   = 4;
    king_move_back.is_castling = false;
    king_move_away.promotion_piece = EMPTY;
    king_move_away.is_promotion = false;
    king_move_away.is_en_passant = false;
    execute_move(king_move_back, true);

    // Attempt castle
    MinimaxResult whiteDecision = minimax(3, true, -INFINITY, INFINITY);
    Move* white_move = &whiteDecision.best_move;

    CU_ASSERT_FALSE(white_move->is_castling);
}

void test_cant_castle_long_if_rook_moved() {
    create_game_history();
    OneColoursPieces* blackPieces = (OneColoursPieces*) calloc(1, sizeof(OneColoursPieces));
    OneColoursPieces* whitePieces = (OneColoursPieces*) calloc(1, sizeof(OneColoursPieces));
    allPieces.whitePieces = whitePieces;
    allPieces.blackPieces = blackPieces;

    // Clear board
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            board[x][y].piece = EMPTY;
            board[x][y].color = NONE;
            board[x][y].x_coord = x;
            board[x][y].y_coord = y;
        }
    }

    // White king and rook for long castle, but we'll mark the rook as moved
    board[7][4].piece = KING;
    board[7][4].color = WHITE;
    board[7][0].piece = ROOK;
    board[7][0].color = WHITE;

    // Black king
    board[0][4].piece = KING;
    board[0][4].color = BLACK;

    // White knight
    board[5][3].piece = KNIGHT;
    board[5][3].color = WHITE;

    allPieces.whitePieces->King      = &board[7][4];
    allPieces.whitePieces->Rooks[0]  = &board[7][0];
    allPieces.whitePieces->Knights[0] = &board[5][3];

    allPieces.blackPieces->King = &board[0][4];

    // Move rook away
    Move rook_move_away;
    rook_move_away.from_x = 7;
    rook_move_away.from_y = 0;
    rook_move_away.to_x   = 7;
    rook_move_away.to_y   = 1;
    rook_move_away.is_castling = false;
    rook_move_away.promotion_piece = EMPTY;
    rook_move_away.is_promotion = false;
    rook_move_away.is_en_passant = false;
    execute_move(rook_move_away, true);

    // Move rook back
    Move rook_move_back;
    rook_move_back.from_x = 7;
    rook_move_back.from_y = 1;
    rook_move_back.to_x   = 7;
    rook_move_back.to_y   = 0;
    rook_move_back.is_castling = false;
    rook_move_back.promotion_piece = EMPTY;
    rook_move_back.is_promotion = false;
    rook_move_back.is_en_passant = false;
    execute_move(rook_move_back, true);

    // Attempt castle
    MinimaxResult whiteDecision = minimax(3, true, -INFINITY, INFINITY);
    Move* white_move = &whiteDecision.best_move;

    CU_ASSERT_FALSE(white_move->is_castling);
}

int main() {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("CastleTest", 0, 0);

    CU_add_test(suite, "test_castle_short", test_castle_short);
    CU_add_test(suite, "test_castle_long", test_castle_long);
    CU_add_test(suite, "test_cant_castle_short_if_king_moved", test_cant_castle_short_if_king_moved);
    CU_add_test(suite, "test_cant_castle_short_if_rook_moved", test_cant_castle_short_if_rook_moved);
    CU_add_test(suite, "test_cant_castle_long_if_king_moved", test_cant_castle_long_if_king_moved);
    CU_add_test(suite, "test_cant_castle_long_if_rook_moved", test_cant_castle_long_if_rook_moved);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return 0;
}