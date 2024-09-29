#include <printf.h>
#include <stdlib.h>
#include "board.h"
#include "postional_score_matrices.h"

Square board[8][8];

AllPieces allPieces;

void init_board() {
    OneColoursPieces* blackPieces = (OneColoursPieces*) calloc(1, sizeof(OneColoursPieces));
    OneColoursPieces* whitePieces = (OneColoursPieces*) calloc(1, sizeof(OneColoursPieces));

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
    board[0][5] = (Square) {BISHOP, BLACK, 0,5};
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
            Square square = (*board_param)[i][j];
            if(square.color == BLACK) {
                switch (square.piece) {
                    case EMPTY:
                        printf("-");
                        break;
                    case PAWN:
                        printf("\u2659");
                        break;
                    case KNIGHT:
                        printf("\u2658");
                        break;
                    case BISHOP:
                        printf("\u2657");
                        break;
                    case ROOK:
                        printf("\u2656");
                        break;
                    case QUEEN:
                        printf("\u2655");
                        break;
                    case KING:
                        printf("\u2654");
                        break;
                }
            } else {
                switch (square.piece) {
                    case EMPTY:
                        printf("-");
                        break;
                    case PAWN:
                        printf("\u265F");
                        break;
                    case KNIGHT:
                        printf("\u265E");
                        break;
                    case BISHOP:
                        printf("\u265D");
                        break;
                    case ROOK:
                        printf("\u265C");
                        break;
                    case QUEEN:
                        printf("\u265B");
                        break;
                    case KING:
                        printf("\u265A");
                        break;
                }
            }

        }
        printf("\n");
    }
}

double calculate_board_score(Square (*board_param)[8][8]) {
    double score = 0;
    double white_score = 0;
    double black_score = 0;
    OneColoursPieces *pieces;
    double (*pawn_position_score)[8];
    double (*knight_position_score)[8];
    double (*bishop_position_score)[8];
    double (*rook_position_score)[8];
    double (*queen_position_score)[8];
    double (*king_position_score)[8];

    if(board_param == NULL) {
        board_param = &board;
    }


    for (int j = 0; j < 2; ++j) {
        if (j == 0) {
            pieces = allPieces.whitePieces;
            pawn_position_score = white_pawn_position_score;
            knight_position_score = white_knight_position_score;
            bishop_position_score = white_bishop_position_score;
            rook_position_score = white_rook_position_score;
            queen_position_score = white_queen_position_score;
            king_position_score = white_king_position_score;
        } else {
            pieces = allPieces.blackPieces;
            pawn_position_score = black_pawn_position_score;
            knight_position_score = black_knight_position_score;
            bishop_position_score = black_bishop_position_score;
            rook_position_score = black_rook_position_score;
            queen_position_score = black_queen_position_score;
            king_position_score = black_king_position_score;
        }

        score = 0;
        for (int i = 0; i < 8; i++) {
            // TODO: We should really extract the pawn once here, but I cant figure out the pointers
            if (pieces->Pawns[i] != NULL) {
                score += 1;
                score += pawn_position_score[pieces->Pawns[i]->x_coord][pieces->Pawns[i]->y_coord];
            }
        }

        for (int i = 0; i < 2; i++) {
            if (pieces->Rooks[i] != NULL) {
                score += 5;
                score += rook_position_score[pieces->Rooks[i]->x_coord][pieces->Rooks[i]->y_coord];
            }
        }

        for (int i = 0; i < 2; i++) {
            if (pieces->Knights[i] != NULL) {
                score += 3;
                score += knight_position_score[pieces->Knights[i]->x_coord][pieces->Knights[i]->y_coord];
            }
        }

        for (int i = 0; i < 2; i++) {
            if (pieces->Bishops[i] != NULL) {
                score += 3;
                score += bishop_position_score[pieces->Bishops[i]->x_coord][pieces->Bishops[i]->y_coord];
            }
        }

        if (pieces->Queen != NULL) {
            score += 9;
            score += queen_position_score[pieces->Queen->x_coord][pieces->Queen->y_coord];
        }

        if (pieces->King != NULL) {
            score += 100;
            score += king_position_score[pieces->King->x_coord][pieces->King->y_coord];
        }

        if (j == 0) {
            white_score = score;
        } else {
            black_score = score;
        }
    }

    return white_score - black_score;
}