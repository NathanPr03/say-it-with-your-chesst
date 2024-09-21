#include <printf.h>
#include <stdlib.h>
#include "board.h"

Square board[8][8];

AllPieces allPieces;

void init_board() {
    OneColoursPieces* blackPieces = (OneColoursPieces*) malloc(sizeof(OneColoursPieces));
    OneColoursPieces* whitePieces = (OneColoursPieces*) malloc(sizeof(OneColoursPieces));

    for (int i = 0; i < 8; i++) {
        board[1][i] = (Square) {PAWN, BLACK, 1, i};
        blackPieces->Pawns[i] = &board[1][i];
        board[6][i] = (Square) {PAWN, WHITE, 6, i};
        whitePieces->Pawns[i] = &board[6][i];
    }

    board[0][0] = (Square) {ROOK, BLACK, 0, 0};
    blackPieces->Rooks[0] = &board[0][0];
    board[0][1] = (Square) {KNIGHT, BLACK, 0, 1};
    blackPieces->Knights[0] = &board[0][1];
    board[0][2] = (Square) {BISHOP, BLACK, 0, 2};
    blackPieces->Bishops[0] = &board[0][2];
    board[0][3] = (Square) {QUEEN, BLACK, 0, 3};
    blackPieces->Queen = &board[0][3];
    board[0][4] = (Square) {KING, BLACK, 0, 4};
    blackPieces->King = &board[0][4];
    board[0][5] = (Square) {BISHOP, BLACK, 0, 5};
    blackPieces->Bishops[1] = &board[0][5];
    board[0][6] = (Square) {KNIGHT, BLACK, 0, 6};
    blackPieces->Knights[1] = &board[0][6];
    board[0][7] = (Square) {ROOK, BLACK, 0, 7};
    blackPieces->Rooks[1] = &board[0][7];

    board[7][0] = (Square) {ROOK, WHITE, 7, 0};
    whitePieces->Rooks[0] = &board[7][0];
    board[7][1] = (Square) {KNIGHT, WHITE, 7, 1};
    whitePieces->Knights[0] = &board[7][1];
    board[7][2] = (Square) {BISHOP, WHITE, 7, 2};
    whitePieces->Bishops[0] = &board[7][2];
    board[7][3] = (Square) {QUEEN, WHITE, 7, 3};
    whitePieces->Queen = &board[7][3];
    board[7][4] = (Square) {KING, WHITE, 7, 4};
    whitePieces->King = &board[7][4];
    board[7][5] = (Square) {BISHOP, WHITE, 7, 5};
    whitePieces->Bishops[1] = &board[7][5];
    board[7][6] = (Square) {KNIGHT, WHITE, 7, 6};
    whitePieces->Knights[1] = &board[7][6];
    board[7][7] = (Square) {ROOK, WHITE, 7, 7};
    whitePieces->Rooks[1] = &board[7][7];

    for (int i = 2; i < 6; i++) {
        for (int j = 0; j < 8; j++) {
            board[i][j] = (Square) {EMPTY, NONE, i, j};
        }
    }

    allPieces.whitePieces = whitePieces;
    allPieces.blackPieces = blackPieces;
}

void print_board(Square (*board_param)[8][8]) {
    printf("\n");
    if (board_param == NULL) {
        board_param = &board;
    }

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            char prefix[2] = "";
            if ((*board_param)[i][j].color == WHITE) {
                prefix[0] = 'W';
            } else if ((*board_param)[i][j].color == BLACK) {
                prefix[0] = 'B';
            }
            prefix[1] = '\0';

            switch ((*board_param)[i][j].piece) {
                case EMPTY:
                    printf("-");
                    break;
                case PAWN:
                    printf("%sP", prefix);
                    break;
                case KNIGHT:
                    printf("%sN", prefix);
                    break;
                case BISHOP:
                    printf("%sB", prefix);
                    break;
                case ROOK:
                    printf("%sR", prefix);
                    break;
                case QUEEN:
                    printf("%sQ", prefix);
                    break;
                case KING:
                    printf("%sK", prefix);
                    break;
            }
        }
        printf("\n");
    }
}