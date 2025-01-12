#include <math.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdlib.h>
#include "move.h"
#include "minimax.h"
#include "game_history.h"
#include "moves/execute_move.h"

void test_promotion_by_moving_forward() {
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

    board[7][7] = (Square) {KING, BLACK, 7, 7};
    board[0][0] = (Square) {KING, WHITE, 0, 0};

    board[1][6] = (Square) {PAWN, WHITE, 1, 6}; // White pawn one square away

    allPieces.whitePieces->King = &board[0][0];
    allPieces.blackPieces->King = &board[7][7];
    allPieces.whitePieces->Pawns[0] = &board[1][6];

    MinimaxResult meeneymax = minimax(1, true, -INFINITY, INFINITY);
    Move *white_move = &meeneymax.best_move;

    execute_move_new(white_move, false);

    CU_ASSERT_TRUE(board[1][6].piece == EMPTY);
    CU_ASSERT_TRUE(board[1][6].color == NONE);

    CU_ASSERT_TRUE(board[0][6].color == WHITE);
    CU_ASSERT_TRUE(board[0][6].piece == QUEEN);

    CU_ASSERT_TRUE(allPieces.whitePieces->PromotedPieces[0] == &board[0][6]);
}

void test_promotion_by_taking_to_the_left() {
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

    execute_move_new(white_move, false);

    CU_ASSERT_TRUE(board[1][5].piece == EMPTY);
    CU_ASSERT_TRUE(board[1][5].color == NONE);


    CU_ASSERT_TRUE(board[0][4].color == WHITE);
    CU_ASSERT_TRUE(board[0][4].piece == QUEEN);

    CU_ASSERT_TRUE(allPieces.whitePieces->PromotedPieces[0] == &board[0][4]);
}

void test_promotion_by_taking_to_the_right() {
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

    board[7][7] = (Square) {KING, BLACK, 7, 7};
    board[0][0] = (Square) {KING, WHITE, 0, 0};

    // Set up promotion by taking to the right
    board[1][5] = (Square) {PAWN, WHITE, 1, 5}; // White pawn
    board[0][6] = (Square) {ROOK, BLACK, 0, 6}; // Black rook (capturable to the right)

    allPieces.whitePieces->King = &board[0][0];
    allPieces.blackPieces->King = &board[7][7];
    allPieces.whitePieces->Pawns[0] = &board[1][5];
    allPieces.blackPieces->Rooks[0] = &board[0][6];

    MinimaxResult meeneymax = minimax(1, true, -INFINITY, INFINITY);
    Move *white_move = &meeneymax.best_move;

    execute_move_new(white_move, false);

    CU_ASSERT_TRUE(board[1][5].piece == EMPTY);
    CU_ASSERT_TRUE(board[1][5].color == NONE);


    CU_ASSERT_TRUE(board[0][6].color == WHITE);
    CU_ASSERT_TRUE(board[0][6].piece == QUEEN);

    CU_ASSERT_TRUE(allPieces.whitePieces->PromotedPieces[0] == &board[0][6]);
}

void test_promotion_to_knight() {
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

    board[1][2].piece = KING;
    board[1][2].color = BLACK;

    board[7][7].piece = KING;
    board[7][7].color = WHITE;

    board[1][0].piece = PAWN;
    board[1][0].color = WHITE;

    // Surround black king with white pawns, except (0,1) which is occupied by the white pawn
    board[0][2].piece = PAWN; board[0][2].color = WHITE;
    board[0][3].piece = PAWN; board[0][3].color = WHITE;
    board[1][1].piece = PAWN; board[1][1].color = WHITE;
    board[1][3].piece = PAWN; board[1][3].color = WHITE;
    board[2][1].piece = PAWN; board[2][1].color = WHITE;
    board[2][3].piece = PAWN; board[2][3].color = WHITE;
    board[3][2].piece = QUEEN; board[3][2].color = WHITE;
    board[0][1].piece = KNIGHT; board[0][1].color = WHITE;
    board[0][4].piece = ROOK; board[0][4].color = WHITE;
    board[2][0].piece = BISHOP; board[2][0].color = WHITE;
    board[1][4].piece = ROOK; board[1][4].color = WHITE;

    // Place a white knight at (2,2) to attack (0,1), ensuring black king cannot move there after promotion
    board[2][2].piece = KNIGHT;
    board[2][2].color = WHITE;

    allPieces.whitePieces->King = &board[7][7];
    allPieces.blackPieces->King = &board[1][2];
    allPieces.whitePieces->Pawns[0] = &board[1][0];
    allPieces.whitePieces->Pawns[1] = &board[0][2];
    allPieces.whitePieces->Pawns[2] = &board[0][3];
    allPieces.whitePieces->Pawns[3] = &board[1][1];
    allPieces.whitePieces->Pawns[4] = &board[1][3];
    allPieces.whitePieces->Pawns[5] = &board[2][1];
    allPieces.whitePieces->Pawns[6] = &board[2][3];
    allPieces.whitePieces->Knights[0] = &board[2][2];
    allPieces.whitePieces->Knights[1] = &board[0][1];
    allPieces.whitePieces->Queen = &board[3][2];
    allPieces.whitePieces->Rooks[0] = &board[0][4];
    allPieces.whitePieces->Rooks[1] = &board[1][4];
    allPieces.whitePieces->Bishops[0] = &board[2][0];

    MinimaxResult meeneymax = minimax(3, true, -INFINITY, INFINITY);
    Move *white_move = &meeneymax.best_move;

    execute_move_new(white_move, false);

    CU_ASSERT_TRUE(board[1][0].piece == EMPTY);
    CU_ASSERT_TRUE(board[1][0].color == NONE);


    CU_ASSERT_TRUE(board[0][0].color == WHITE);
    CU_ASSERT_TRUE(board[0][0].piece == KNIGHT);

    CU_ASSERT_TRUE(allPieces.whitePieces->PromotedPieces[0] == &board[0][0]);
}

bool helper_is_any_move_coming_from_a_promoted_piece(Move* moves, int num_moves) {
    for(int i=0; i<num_moves; i++) {
        Move the_move = moves[i];

        // This is naive and brittle, but for the current test case we will only have 1 promoted piece
        Square* promoted_piece = allPieces.whitePieces->PromotedPieces[0];

        // This condition is also brittle - but if everything else is working correctly this is sufficient
        if(the_move.from_x == promoted_piece->x_coord && the_move.from_y == promoted_piece->y_coord) {
            return true;
        }
    }

    return false;
}

void test_promotion_then_generate_moves_for_promoted_piece() {
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

    board[7][7] = (Square) {KING, BLACK, 7, 7};
    board[0][0] = (Square) {KING, WHITE, 0, 0};

    board[1][6] = (Square) {PAWN, WHITE, 1, 6}; // White pawn one square away

    allPieces.whitePieces->King = &board[0][0];
    allPieces.blackPieces->King = &board[7][7];
    allPieces.whitePieces->Pawns[0] = &board[1][6];

    MinimaxResult meeneymax = minimax(1, true, -INFINITY, INFINITY);
    Move *white_move = &meeneymax.best_move;

    execute_move_new(white_move, false);

    CU_ASSERT_TRUE(board[1][6].piece == EMPTY);
    CU_ASSERT_TRUE(board[1][6].color == NONE);

    CU_ASSERT_TRUE(board[0][6].color == WHITE);
    CU_ASSERT_TRUE(board[0][6].piece == QUEEN);

    CU_ASSERT_TRUE(allPieces.whitePieces->PromotedPieces[0] == &board[0][6]);

    Move* moves = generate_moves_for_one_color(allPieces.whitePieces, true, 2);
    int num_moves = count_valid_moves(moves);

    CU_ASSERT_TRUE(helper_is_any_move_coming_from_a_promoted_piece(moves, num_moves));
}

int main() {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("PromotionTest", 0, 0);

    CU_add_test(suite, "test_promotion_by_moving_forward", test_promotion_by_moving_forward);
    CU_add_test(suite, "test_promotion_by_taking_to_the_left", test_promotion_by_taking_to_the_left);
    CU_add_test(suite, "test_promotion_by_taking_to_the_right", test_promotion_by_taking_to_the_right);
    CU_add_test(suite, "test_promotion_to_knight", test_promotion_to_knight);
    CU_add_test(suite, "test_promotion_then_generate_moves_for_promoted_piece", test_promotion_then_generate_moves_for_promoted_piece);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}
