#include <stddef.h>
#include "castle.h"
#include "game_history.h"
#include "minimax.h"

// These are dependant on the rooks been ordered "correctly" // TODO: Maybe change this?
const int QUEEN_SIDE_ROOK_INDEX = 0;
const int KING_SIDE_ROOK_INDEX = 1;

const int NUMBER_OF_SQUARES_IN_LONG_CASTLE = 3;
const int NUMBER_OF_SQUARES_IN_SHORT_CASTLE = 4;

bool is_opposing_piece_looking_at_square(Move* opposite_colours_moves, int num_opposite_colours_moves, Square square) {
    for(int i = 0; i < num_opposite_colours_moves; i++) {
        Move move = opposite_colours_moves[i];
        if(move.to_x == square.x_coord && move.to_y == square.y_coord) {
            return true;
        }
    }

    return false;
}

static bool is_opposing_piece_looking_at_any_of_these_squares(
        Colour colour,
        Square squares[],
        int num_squares,
        Move* opposite_colours_moves,
        int num_opposite_colours_moves
        ) {
    // TODO: opposing_colour_pieces or opponents_king could be passed in as arguments
    OneColoursPieces* opposing_colour_pieces =
            (colour == WHITE) ? allPieces.blackPieces : allPieces.whitePieces;

    Square *opponents_king = opposing_colour_pieces->King;

    for(int i = 0; i < num_squares; i++) {
        Square the_square = squares[i];
        if(is_opposing_piece_looking_at_square(opposite_colours_moves, num_opposite_colours_moves, the_square)) {
            return true;
        }

        // Here we check if the other king is blocking check separately to avoid infinite recursion
        if(are_coordinates_within1(the_square.x_coord, the_square.y_coord, opponents_king->x_coord, opponents_king->y_coord)){
            return true;
        }
    }
    return false;
}

bool is_opposing_piece_looking_at_a_long_castle_square(Colour colour) {
    OneColoursPieces* opposing_colour_pieces =
            (colour == WHITE) ? allPieces.blackPieces : allPieces.whitePieces;

    int row = (colour == WHITE) ? 7 : 0;

    Square long_castle_squares[3] = {
            { EMPTY, colour, row, 2 },
            { EMPTY, colour, row, 3 },
            { KING,  colour, row, 4 }
    };

    Move* opposing_colours_moves = generate_moves_for_one_color(opposing_colour_pieces, false, 1);
    int num_opposite_colours_moves = count_valid_moves(opposing_colours_moves);

    return is_opposing_piece_looking_at_any_of_these_squares(
            colour,
            long_castle_squares,
            NUMBER_OF_SQUARES_IN_LONG_CASTLE,
            opposing_colours_moves,
            num_opposite_colours_moves
    );
}

bool is_opposing_piece_looking_at_a_short_castle_square(Colour colour) {
    OneColoursPieces* opposing_colour_pieces =
            (colour == WHITE) ? allPieces.blackPieces : allPieces.whitePieces;

    int row = (colour == WHITE) ? 7 : 0;

    Square short_castle_squares[4] = {
            { KING, colour, row, 4 },
            { EMPTY, colour, row, 5 },
            { EMPTY, colour, row, 6 },
            { ROOK,  colour, row, 7 }
    };

    Move* opposing_colours_moves = generate_moves_for_one_color(opposing_colour_pieces, false, 1);
    int num_opposite_colours_moves = count_valid_moves(opposing_colours_moves);

    return is_opposing_piece_looking_at_any_of_these_squares(
        colour,
        short_castle_squares,
        NUMBER_OF_SQUARES_IN_SHORT_CASTLE,
        opposing_colours_moves,
        num_opposite_colours_moves
    );
}

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

bool has_rook_or_king_been_moved(Square the_rook, Colour colour){
    // Order of these is very important! A castle is marked as a king move in the game history
    // This means after a castle, the kingside rook can move to the queenside square
    // This will cause a null pointer in `has_rook_been_moved` as we will check the wrong index of the allPiece.xColour->Rooks array
    return has_king_been_moved(colour) || has_rook_been_moved(the_rook, colour);
}

bool can_short_castle(Square king) {
    Colour colour = king.color;

    OneColoursPieces* one_colours_piece = colour == WHITE ? allPieces.whitePieces : allPieces.blackPieces;
    Square* the_rook = one_colours_piece->Rooks[KING_SIDE_ROOK_INDEX];

    if(the_rook == NULL){
        return false;
    }

    if(has_rook_or_king_been_moved(*the_rook, colour)){
        return false;
    }

    if(is_opposing_piece_looking_at_a_short_castle_square(colour)){
        return false;
    }

    if(colour == WHITE){
        return king.x_coord == 7 && king.y_coord == 4 // Is king in place
        && the_rook->x_coord == 7 && the_rook->y_coord == 7  // Is rook is in place -- Technically unnecessary as has_rook_or_king_been_moved->has_rook_been_moved
        && board[7][5].piece == EMPTY && board[7][6].piece == EMPTY; // Are squares between king and rook empty
    }else{
        return king.x_coord == 0 && king.y_coord == 4 // Is king in place
        && the_rook->x_coord == 0 && the_rook->x_coord == 7 // Is rook is in place -- Technically unnecessary as has_rook_or_king_been_moved->has_rook_been_moved
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

    if(has_rook_or_king_been_moved(*the_rook, colour)){
        return false;
    }

    if(is_opposing_piece_looking_at_a_long_castle_square(colour)){
        return false;
    }

    if(colour == WHITE){
        return king.x_coord == 7 && king.y_coord == 4 // Is king in place
        && the_rook->x_coord == 7 && the_rook->y_coord == 0 // Is rook is in place -- Technically unnecessary as has_rook_or_king_been_moved->has_rook_been_moved
        && board[7][1].piece == EMPTY && board[7][2].piece == EMPTY && board[7][3].piece == EMPTY; // Are squares between king and rook empty
    }else{
        return king.x_coord == 0 && king.y_coord == 4 // Is king in place
        && the_rook->x_coord == 0 && the_rook->y_coord == 0 // Is rook is in place -- Technically unnecessary as has_rook_or_king_been_moved->has_rook_been_moved
        && board[0][1].piece == EMPTY && board[0][2].piece == EMPTY && board[0][3].piece == EMPTY; // Are squares between king and rook empty
    }
}


