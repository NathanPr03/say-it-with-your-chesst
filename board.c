#include <printf.h>
#include "board.h"

Square board[8][8];

void init_board() {
    for (int i = 0; i < 8; i++) {
        board[1][i] = (Square) {PAWN, BLACK, 1, i};
        board[6][i] = (Square) {PAWN, WHITE, 6, i};
    }

    board[0][0] = (Square) {ROOK, BLACK, 0, 0};
    board[0][1] = (Square) {KNIGHT, BLACK, 0, 1};
    board[0][2] = (Square) {BISHOP, BLACK, 0, 2};
    board[0][3] = (Square) {QUEEN, BLACK, 0, 3};
    board[0][4] = (Square) {KING, BLACK, 0, 4};
    board[0][5] = (Square) {BISHOP, BLACK, 0,5};
    board[0][6] = (Square) {KNIGHT, BLACK, 0, 6};
    board[0][7] = (Square) {ROOK, BLACK, 0, 7};

    board[7][0] = (Square) {ROOK, WHITE, 7, 0};
    board[7][1] = (Square) {KNIGHT, WHITE, 7, 1};
    board[7][2] = (Square) {BISHOP, WHITE, 7, 2};
    board[7][3] = (Square) {QUEEN, WHITE, 7, 3};
    board[7][4] = (Square) {KING, WHITE, 7, 4};
    board[7][5] = (Square) {BISHOP, WHITE, 7, 5};
    board[7][6] = (Square) {KNIGHT, WHITE, 7, 6};
    board[7][7] = (Square) {ROOK, WHITE, 7, 7};

    for (int i = 2; i < 6; i++) {
        for (int j = 0; j < 8; j++) {
            board[i][j] = (Square) {EMPTY, NONE, i, j};
        }
    }
}

void print_board(Square (*board_param)[8][8]) {
    if (board_param == NULL) {
        board_param = &board;
    }

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            switch ((*board_param)[i][j].piece) {
                case EMPTY:
                    printf("-");
                    break;
                case PAWN:
                    printf("P");
                    break;
                case KNIGHT:
                    printf("N");
                    break;
                case BISHOP:
                    printf("B");
                    break;
                case ROOK:
                    printf("R");
                    break;
                case QUEEN:
                    printf("Q");
                    break;
                case KING:
                    printf("K");
                    break;
            }
        }
        printf("\n");
    }
}
