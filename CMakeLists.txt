cmake_minimum_required(VERSION 3.29)
project(say_it_with_your_chesst C)

set(CMAKE_C_STANDARD 11)

add_executable(say_it_with_your_chesst
        main.c
        board.c
        board.h
        move.c
        move.h
        move_picker.c
        move_picker.h)


