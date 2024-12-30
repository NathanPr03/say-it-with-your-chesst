#include "castle.h"

const int QUEEN_SIDE_ROOK_INDEX = 0;
const int KING_SIDE_ROOK_INDEX = 1;

bool has_white_king_been_checked;
bool has_black_king_been_checked;

bool has_king_been_moved(Colour colour) {
    // TODO: Implement
    return false;
}

bool has_rook_been_moved(Piece the_rook, Colour colour) {
    // TODO: Implement
    return false;
}

bool has_king_been_in_check(Colour colour) {
    return colour == WHITE ? has_white_king_been_checked : has_black_king_been_checked;
}

void mark_king_as_in_check(Colour colour) {
    if (colour == WHITE) {
        has_white_king_been_checked = true;
    } else {
        has_black_king_been_checked = true;
    }
}

bool is_castling_possible(Colour colour, Piece the_rook){
    return !has_king_been_in_check(colour) && !has_king_been_moved(colour); // TODO: Add more conditions
}

bool can_short_castle(Square king) {
    Colour colour = king.color;

    OneColoursPieces* one_colours_piece = colour == WHITE ? allPieces.whitePieces : allPieces.blackPieces;
    Square* the_rook = one_colours_piece->Rooks[KING_SIDE_ROOK_INDEX];

    if(is_castling_possible(colour, the_rook->piece)){
        return false;
    }

    if(colour == WHITE){
        return king.x_coord == 7 && king.y_coord == 4 // Is king in place
        && the_rook->x_coord == 7 && the_rook->y_coord == 7  // Is rook is in place
        && board[7][5].piece == EMPTY && board[7][6].piece == EMPTY; // Are squares between king and rook empty
    }else{
        return king.x_coord == 0 && king.y_coord == 4 // Is king in place
        && the_rook->x_coord == 0 && the_rook->x_coord == 7 // Is rook is in place
        && board[0][5].piece == EMPTY && board[0][6].piece == EMPTY; // Are squares between king and rook empty
    }
}

bool can_long_castle(Square king) {
    Colour colour = king.color;

    OneColoursPieces* one_colours_piece = colour == WHITE ? allPieces.whitePieces : allPieces.blackPieces;
    Square* the_rook = one_colours_piece->Rooks[QUEEN_SIDE_ROOK_INDEX];

    if(is_castling_possible(colour, the_rook->piece)){
        return false;
    }

    if(colour == WHITE){
        return king.x_coord == 7 && king.y_coord == 4 // Is king in place
        && the_rook->x_coord == 7 && the_rook->y_coord == 0 // Is rook is in place
        && board[7][1].piece == EMPTY && board[7][2].piece == EMPTY && board[7][3].piece == EMPTY; // Are squares between king and rook empty
    }else{
        return king.x_coord == 0 && king.y_coord == 4 // Is king in place
        && the_rook->x_coord == 0 && the_rook->y_coord == 0 // Is rook is in place
        && board[0][1].piece == EMPTY && board[0][2].piece == EMPTY && board[0][3].piece == EMPTY; // Are squares between king and rook empty
    }
}