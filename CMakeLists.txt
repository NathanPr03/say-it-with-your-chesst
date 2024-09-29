# Main executable
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
        src/minimax.h
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

add_executable(tests
        tests/move_test.c
        src/board.c
        src/move.c
        src/move_picker.c
        src/postional_score_matrices.c
)

target_link_libraries(tests ${CUNIT_LIB})

enable_testing()
add_test(NAME IntegrationTests COMMAND tests)