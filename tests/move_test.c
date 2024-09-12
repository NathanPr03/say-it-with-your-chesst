#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdlib.h>
#include "board.h"
#include "move.h"

void test_initial_board_moves() {
    // Initialize the board
    init_board();

    // Generate all legal moves for white
    Move* white_moves = generate_moves_for_one_color(allPieces.whitePieces, true);
    CU_ASSERT_PTR_NOT_NULL(white_moves);

    // Generate all legal moves for black
    Move* black_moves = generate_moves_for_one_color(allPieces.blackPieces, true);
    CU_ASSERT_PTR_NOT_NULL(black_moves);

    // Check that white moves are generated
    CU_ASSERT_TRUE(white_moves[0].from_x != 0 || white_moves[0].from_y != 0 || white_moves[0].to_x != 0 || white_moves[0].to_y != 0);

    // Check that black moves are generated
    CU_ASSERT_TRUE(black_moves[0].from_x != 0 || black_moves[0].from_y != 0 || black_moves[0].to_x != 0 || black_moves[0].to_y != 0);

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