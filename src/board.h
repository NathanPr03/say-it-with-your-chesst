#ifndef SAY_IT_WITH_YOUR_CHESST_BOARD_H
#define SAY_IT_WITH_YOUR_CHESST_BOARD_H
#pragma once
typedef enum {
    EMPTY,
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
} Piece;

typedef enum {
    WHITE,
    BLACK,
    NONE
} Colour;

typedef struct {
    Piece piece;
    Colour color;
    int x_coord;
    int y_coord;
} Square;

typedef struct {
    Square* Pawns[8];
    Square* Knights[2];
    Square* Bishops[2];
    Square* Rooks[2];
    Square* Queen;
    Square* King;
}OneColoursPieces;

typedef struct {
    OneColoursPieces *whitePieces;
    OneColoursPieces *blackPieces;
} AllPieces;

extern AllPieces allPieces;
extern Square board[8][8];

void init_board();
void print_board(Square (*board_param)[8][8]);

#endif
