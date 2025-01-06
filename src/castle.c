#include <stddef.h>
#include "castle.h"
#include "game_history.h"

// These are dependant on the rooks been ordered "correctly" // TODO: Maybe change this?
const int QUEEN_SIDE_ROOK_INDEX = 0;
const int KING_SIDE_ROOK_INDEX = 1;


bool has_king_been_moved(Colour colour) {
    if (colour == WHITE) {
        return does_game_history_contain_any_moves_from_piece(*allPieces.whitePieces->King);
    } else {
        return does_game_history_contain_any_moves_from_piece(*allPieces.blackPieces->King);
    }
}

bool has_rook_been_moved(Square the_rook, Colour colour) {
    if (colour == WHITE) {
        if (the_rook.x_coord == 7 && the_rook.y_coord == 0) {
            return does_game_history_contain_any_moves_from_piece(*allPieces.whitePieces->Rooks[QUEEN_SIDE_ROOK_INDEX]);
        } else if (the_rook.x_coord == 7 && the_rook.y_coord == 7) {
            return does_game_history_contain_any_moves_from_piece(*allPieces.whitePieces->Rooks[KING_SIDE_ROOK_INDEX]);
        }
    } else {
        if (the_rook.x_coord == 0 && the_rook.y_coord == 0) {
            return does_game_history_contain_any_moves_from_piece(*allPieces.blackPieces->Rooks[QUEEN_SIDE_ROOK_INDEX]);
        } else if (the_rook.x_coord == 0 && the_rook.y_coord == 7) {
            return does_game_history_contain_any_moves_from_piece(*allPieces.blackPieces->Rooks[KING_SIDE_ROOK_INDEX]);
        }
    }

    return false;
}

bool is_castling_possible(Square the_rook, Colour colour){
    return !has_rook_been_moved(the_rook, colour) && !has_king_been_moved(colour); // TODO: Add more conditions
}

bool can_short_castle(Square king) {
    Colour colour = king.color;

    OneColoursPieces* one_colours_piece = colour == WHITE ? allPieces.whitePieces : allPieces.blackPieces;
    Square* the_rook = one_colours_piece->Rooks[KING_SIDE_ROOK_INDEX];

    if(the_rook == NULL){
        return false;
    }

    if(!is_castling_possible(*the_rook, colour)){
        return false;
    }

    if(colour == WHITE){
        return king.x_coord == 7 && king.y_coord == 4 // Is king in place
        && the_rook->x_coord == 7 && the_rook->y_coord == 7  // Is rook is in place -- Technically unnecessary as is_castling_possible->has_rook_been_moved
        && board[7][5].piece == EMPTY && board[7][6].piece == EMPTY; // Are squares between king and rook empty
    }else{
        return king.x_coord == 0 && king.y_coord == 4 // Is king in place
        && the_rook->x_coord == 0 && the_rook->x_coord == 7 // Is rook is in place -- Technically unnecessary as is_castling_possible->has_rook_been_moved
        && board[0][5].piece == EMPTY && board[0][6].piece == EMPTY; // Are squares between king and rook empty
    }
}

bool can_long_castle(Square king) {
    Colour colour = king.color;

    OneColoursPieces* one_colours_piece = colour == WHITE ? allPieces.whitePieces : allPieces.blackPieces;
    Square* the_rook = one_colours_piece->Rooks[QUEEN_SIDE_ROOK_INDEX];

    if(the_rook == NULL){
        return false;
    }

    if(!is_castling_possible(*the_rook, colour)){
        return false;
    }

    if(colour == WHITE){
        return king.x_coord == 7 && king.y_coord == 4 // Is king in place
        && the_rook->x_coord == 7 && the_rook->y_coord == 0 // Is rook is in place -- Technically unnecessary as is_castling_possible->has_rook_been_moved
        && board[7][1].piece == EMPTY && board[7][2].piece == EMPTY && board[7][3].piece == EMPTY; // Are squares between king and rook empty
    }else{
        return king.x_coord == 0 && king.y_coord == 4 // Is king in place
        && the_rook->x_coord == 0 && the_rook->y_coord == 0 // Is rook is in place -- Technically unnecessary as is_castling_possible->has_rook_been_moved
        && board[0][1].piece == EMPTY && board[0][2].piece == EMPTY && board[0][3].piece == EMPTY; // Are squares between king and rook empty
    }
}