#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdlib.h>
#include "board.h"
#include "move.h"

void test_initial_board_moves() {
    init_board();

    Move* white_moves = generate_moves_for_one_color(allPieces.whitePieces, true);
    CU_ASSERT_PTR_NOT_NULL(white_moves);

    Move* black_moves = generate_moves_for_one_color(allPieces.blackPieces, true);
    CU_ASSERT_PTR_NOT_NULL(black_moves);

    // Can pawns move forward
    for (int i = 0; i < 16; i++) {
        int row = i;
        if(row == 1) {
            row = 0;
        }else{
            row = row / 2;
        }

        if(i % 2 == 0) {
            CU_ASSERT_TRUE(white_moves[i].from_x == 6);
            CU_ASSERT_TRUE(white_moves[i].to_x == 5);
            CU_ASSERT_TRUE(white_moves[i].from_y == row);
            CU_ASSERT_TRUE(white_moves[i].to_y == row);
        }else if(i % 2 == 1) {
            CU_ASSERT_TRUE(white_moves[i].from_x == 6);
            CU_ASSERT_TRUE(white_moves[i].to_x == 4);
            CU_ASSERT_TRUE(white_moves[i].from_y == row);
            CU_ASSERT_TRUE(white_moves[i].to_y == row);
        }
    }

    for (int i = 0; i < 16; i++) {
        int row = i;
        if(row == 1) {
            row = 0;
        }else{
            row = row / 2;
        }

        if(i % 2 == 0) {
            CU_ASSERT_TRUE(black_moves[i].from_x == 1);
            CU_ASSERT_TRUE(black_moves[i].to_x == 2);
            CU_ASSERT_TRUE(black_moves[i].from_y == row);
            CU_ASSERT_TRUE(black_moves[i].to_y == row);
        }else if(i % 2 == 1) {
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

int main() {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("ChessIntegrationTest", 0, 0);

    CU_add_test(suite, "test_initial_board_moves", test_initial_board_moves);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}