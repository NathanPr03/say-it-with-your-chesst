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

add_executable(move_test tests/move_test.c src/board.c src/move.c src/move_picker.c src/postional_score_matrices.c src/promotion.c src/minimax.c src/pieces.c
        src/en_passant.c)
target_link_libraries(move_test ${CUNIT_LIB})

add_executable(promotion_test tests/promotion_test.c src/board.c src/move.c src/move_picker.c src/postional_score_matrices.c src/promotion.c src/minimax.c src/pieces.c
        src/en_passant.c)
target_link_libraries(promotion_test ${CUNIT_LIB})

add_executable(en_passant_test tests/en_passant_test.c src/board.c src/move.c src/move_picker.c src/postional_score_matrices.c src/promotion.c src/minimax.c src/pieces.c
        src/en_passant.c)
target_link_libraries(en_passant_test ${CUNIT_LIB})

# Create an umbrella target for all tests
add_custom_target(tests ALL
        DEPENDS move_test promotion_test en_passant_test
        COMMENT "Building all test executables"
)

enable_testing()
add_test(NAME IntegrationTests COMMAND tests)