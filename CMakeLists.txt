# Main executable
set(CMAKE_OSX_SYSROOT /Library/Developer/CommandLineTools/SDKs/MacOSX15.2.sdk) # Without this it tries to use the 15.1 SDK
cmake_minimum_required(VERSION 3.29)
project(say_it_with_your_chesst C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH})

add_library(say_it_with_your_chesst_lib
        src/board.c
        src/move.c
        src/move_picker.c
        src/postional_score_matrices.c
        src/minimax.c
        src/promotion.c
        src/pieces.c
        src/en_passant.c
        src/castle.c
        src/game/game_history.c
        src/game/game.c
        src/moves/execute_move.c
        src/utils/validate_board.c
        src/utils/random.c
        src/board/zobrist.c
)

target_include_directories(say_it_with_your_chesst_lib PUBLIC ${PROJECT_SOURCE_DIR}/src)

add_executable(say_it_with_your_chesst
        src/main.c
)

target_link_libraries(say_it_with_your_chesst say_it_with_your_chesst_lib)

# CUnit
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories("/opt/homebrew/opt/cunit/include")
link_directories("/opt/homebrew/opt/cunit/lib")

find_library(CUNIT_LIB cunit HINTS "/opt/homebrew/opt/cunit/lib")
enable_testing()

# Build all test executables
add_executable(move_test tests/move_test.c src/board.c src/move.c src/move_picker.c src/postional_score_matrices.c src/promotion.c src/minimax.c src/pieces.c
        src/en_passant.c src/castle.c src/game/game_history.c src/moves/execute_move.c src/utils/validate_board.c src/board/zobrist.c src/utils/random.c)
target_link_libraries(move_test ${CUNIT_LIB})

add_executable(promotion_test tests/promotion_test.c src/board.c src/move.c src/move_picker.c src/postional_score_matrices.c src/promotion.c src/minimax.c src/pieces.c
        src/en_passant.c src/castle.c src/game/game_history.c src/moves/execute_move.c src/utils/validate_board.c src/board/zobrist.c src/utils/random.c)
target_link_libraries(promotion_test ${CUNIT_LIB})

add_executable(en_passant_test tests/en_passant_test.c src/board.c src/move.c src/move_picker.c src/postional_score_matrices.c src/promotion.c src/minimax.c src/pieces.c
        src/en_passant.c src/castle.c src/game/game_history.c src/moves/execute_move.c src/utils/validate_board.c src/board/zobrist.c src/utils/random.c)
target_link_libraries(en_passant_test ${CUNIT_LIB})

add_executable(castle_test tests/castle_test.c src/board.c src/move.c src/move_picker.c src/postional_score_matrices.c src/promotion.c src/minimax.c src/pieces.c
        src/en_passant.c src/castle.c src/game/game_history.c src/moves/execute_move.c src/utils/validate_board.c src/board/zobrist.c src/utils/random.c)
target_link_libraries(castle_test ${CUNIT_LIB})

add_executable(full_game_test tests/full_game_test.c src/board.c src/move.c src/move_picker.c src/postional_score_matrices.c src/promotion.c src/minimax.c src/pieces.c
        src/en_passant.c src/castle.c src/game/game_history.c src/moves/execute_move.c src/utils/validate_board.c src/game/game.c tests/utils/prints.c src/utils/random.c
        src/board/zobrist.c tests/utils/persist_move.c)
target_link_libraries(full_game_test ${CUNIT_LIB})

# Register each executable as a test
add_test(NAME MoveTest COMMAND move_test)
add_test(NAME PromotionTest COMMAND promotion_test)
add_test(NAME EnPassantTest COMMAND en_passant_test)
add_test(NAME CastleTest COMMAND castle_test)
add_test(NAME FullGameTest COMMAND full_game_test)

# Create a custom target for running all tests
add_custom_target(tests
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
        COMMENT "Running all tests"
)
