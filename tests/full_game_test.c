#include <CUnit/Basic.h>
#include <unistd.h>
#include <sys/signal.h>
#include "game/game.h"
#include "utils/prints.h"
#include "board.h"
#include "moves/execute_move.h"
#include "utils/random.h"
#include "minimax.h"
#include <sys/wait.h>
#include <time.h>
#include <signal.h>
#include <sys/fcntl.h>
#include "utils/persist_move.h"

void test_against_bad_bot() {
    struct RedirectedStdout redirectedStdout = redirect_stdout_to_pipe();
    play_game("bad-bot");

    CU_ASSERT_TRUE(assert_last_printed_line_is(redirectedStdout, "White wins! Black is CHECKMATED, after 9 moves"));
}


void test_against_good_bot() {
    struct RedirectedStdout redirectedStdout = redirect_stdout_to_pipe();
    play_game("good-bot");

    // TODO: This assertion is a bit flaky, as the engine changes the number of moves will change
    CU_ASSERT_TRUE(assert_last_printed_line_is(redirectedStdout, "Stalemate! Three fold repetition after 59 moves"));
}

void test_against_random_bot() {
    init_board();
    FILE* move_log = fopen("moves.txt", "w");
    if (!move_log) {
        CU_FAIL("Failed to open move log");
        return;
    }

    for(int i = 0; i < 150; i++) {
        Move* white_moves = generate_moves_for_one_color(allPieces.whitePieces, true, 2);
        if (!white_moves ||
            (white_moves[0].to_x == 0 && white_moves[0].from_x == 0 &&
             white_moves[0].to_y == 0 && white_moves[0].from_y == 0)) {
            if(is_king_in_check(0, 1)) {
                CU_PASS("BLACK checkmates WHITE");
                fclose(move_log);
                return;
            }
            CU_PASS("Stalemate");
            fclose(move_log);
            return;
        }

        int white_count = count_valid_moves(white_moves);
        unsigned int white_index = random_number(white_count - 1);
        Move white_choice = white_moves[white_index];

        write_move(move_log, white_choice);
        execute_move(&white_choice, false);

        Move* black_moves = generate_moves_for_one_color(allPieces.blackPieces, true, 2);
        if (!black_moves ||
            (black_moves[0].to_x == 0 && black_moves[0].from_x == 0 &&
             black_moves[0].to_y == 0 && black_moves[0].from_y == 0)) {
            if(is_king_in_check(1, 1)) {
                CU_PASS("WHITE checkmates BLACK");
                fclose(move_log);
                return;
            }
            CU_PASS("Stalemate");
            fclose(move_log);
            return;
        }
        int black_count = count_valid_moves(black_moves);
        unsigned int black_index = random_number(black_count - 1);

        Move black_choice = black_moves[black_index];
        write_move(move_log, black_choice);

        execute_move(&black_choice, false);
    }

    fclose(move_log);
    CU_PASS("Game ran without errors");
}

void replay_random_moves() {
    init_board();
    replay_moves_from_file("moves.txt");
}

int main() {
    CU_initialize_registry();

    CU_pSuite suite = CU_add_suite("MainFunctionTests", NULL, NULL);
    CU_add_test(suite, "test_against_bad_bot", test_against_bad_bot);
    CU_add_test(suite, "test_against_good_bot", test_against_good_bot);
    CU_add_test(suite, "test_against_random_bot", test_against_random_bot);
//    CU_add_test(suite, "replay_random_moves", replay_random_moves); // Uncomment this line to replay the most recent random moves game

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}