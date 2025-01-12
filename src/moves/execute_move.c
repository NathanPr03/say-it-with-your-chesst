#include <stdbool.h>
#include <stdio.h>
#include "execute_move.h"
#include "game_history.h"
#include "promotion.h"
#include "pieces.h"

void handle_special_moves(Move *move, Square* to, Square* from) {
    if(move->is_promotion){
        promote_pawn_to_other_piece(to, move->promotion_piece);
        game_history->moves[game_history->length-1].ref_to_all_pieces_ptr = (Square **) from; //TODO: BAD!!
    }else if(move->is_en_passant){
        Move last_executed_move = get_move_before_most_recent_move().move;
        Square* previously_moved_pawn = &board[last_executed_move.to_x][last_executed_move.to_y];
        Square** en_passanted_pawn_ref = update_piece_pointer(previously_moved_pawn, NULL, previously_moved_pawn->color);

        board[last_executed_move.to_x][last_executed_move.to_y].piece = EMPTY;
        board[last_executed_move.to_x][last_executed_move.to_y].color = NONE;

        game_history->moves[game_history->length-1].ref_to_all_pieces_ptr = en_passanted_pawn_ref; //TODO: BAD!!
    }else if(move->is_castling){
        int rook_from_y = move->to_y == 2 ? 0 : 7; // rook_from_y is 0 for long castle, 7 for short castle
        int rook_to_y = move->to_y == 2 ? 3 : 5; // rook_to_y is 3 for long castle, 5 for short castle

        Square* rook = &board[move->to_x][rook_from_y];
        Square* rook_to = &board[move->to_x][rook_to_y];

        rook_to->piece = rook->piece;
        rook_to->color = rook->color;
        rook->piece = EMPTY;
        rook->color = NONE;

        Square** ref_for_all_pieces_ptr = update_piece_pointer(rook, rook_to, rook_to->color);
        game_history->moves[game_history->length-1].ref_to_all_pieces_ptr = ref_for_all_pieces_ptr; //TODO: BAD!!
    }
}

void execute_move_new(Move* move, bool is_undoing_move) {
    Square* from = &board[move->from_x][move->from_y];
    Square* to = &board[move->to_x][move->to_y];

    Square** ref_for_all_pieces_ptr = update_piece_pointer(to, NULL, to->color);

    if(!is_undoing_move) {
        add_move_to_game_history(*move, *from, *to, ref_for_all_pieces_ptr);
    }

    to->color = from->color;
    to->piece = from->piece;

    from->color = NONE;
    from->piece = EMPTY;

    update_piece_pointer(from, to, to->color);

    if(!is_undoing_move) {
        // Undoing special moves is handled in `handle_undoing_special_moves`
        handle_special_moves(move, to, from);
    }
}

bool handle_undoing_special_moves(Move* move, Square** ref_to_all_pieces_ptr) {
    bool was_it_a_special_move = false;
    if(move->is_promotion) {
        OneColoursPieces **pieces = (board[move->from_x][move->from_y].color == WHITE) ?
                                    &allPieces.whitePieces : &allPieces.blackPieces;

        board[move->from_x][move->from_y].piece = PAWN;

        int pawn_index = find_next_empty_piece_index(board[move->from_x][move->from_y].color, PAWN);
        (*pieces)->Pawns[pawn_index] = &board[move->from_x][move->from_y];

        int promoted_piece_index = find_piece_index_by_coordinate(move->from_x, move->from_y, true);
        (*pieces)->PromotedPieces[promoted_piece_index] = NULL;

        was_it_a_special_move = true;
    }else if(move->is_en_passant) {
        // An en passant moves the pawn to the square behind the taken pawn. This will be -1 for white and +1 for black
        Colour colour_of_previous_move = board[move->from_x][move->from_y].color;
        int en_passant_offset = (colour_of_previous_move == WHITE) ? 1 : -1;

        // This is not the square that was moved to, but the square of the opposing pawn that was taken
        Square* the_pawn_that_was_taken = &board[move->to_x+en_passant_offset][move->to_y];
        Colour opposite_colour = colour_of_previous_move == WHITE ? BLACK : WHITE;

        the_pawn_that_was_taken->color = opposite_colour;
        the_pawn_that_was_taken->piece = PAWN;

        *ref_to_all_pieces_ptr = the_pawn_that_was_taken;

        was_it_a_special_move = true;
    }else if(move->is_castling) {
        int where_rook_should_be_y = move->to_y == 2 ? 0 : 7; // rook_from_y is 0 for long castle, 7 for short castle
        int where_rook_moved_to_y = move->to_y == 2 ? 3 : 5; // rook_to_y is 3 for long castle, 5 for short castle

        Square* where_rook_should_be = &board[move->to_x][where_rook_should_be_y];
        Square* moved_rook = &board[move->to_x][where_rook_moved_to_y];

        where_rook_should_be->piece = moved_rook->piece;
        where_rook_should_be->color = moved_rook->color;

        moved_rook->piece = EMPTY;
        moved_rook->color = NONE;

        *ref_to_all_pieces_ptr = where_rook_should_be;

        was_it_a_special_move = true;
    }

    return was_it_a_special_move;
}

void undo_move(Move* move) {
    Move* reverse_move = &(Move){
        move->to_x,
        move->to_y,
        move->from_x,
        move->from_y,
        move->score, move->is_promotion,
        move->promotion_piece,
        move->is_en_passant,
        move->is_castling
    };

    execute_move_new(reverse_move, true);

    GameHistoryMove last_move = get_most_recent_move();
    Square the_taken_square = last_move.to_square;

    board[the_taken_square.x_coord][the_taken_square.y_coord] = the_taken_square;

    // Update the allPieces struct to reset the piece that was taken
    Square** taken_all_pieces_piece = last_move.ref_to_all_pieces_ptr;

    bool was_it_a_special_move = handle_undoing_special_moves(move, taken_all_pieces_piece);

    if(!was_it_a_special_move && taken_all_pieces_piece != NULL) {
        *taken_all_pieces_piece = &board[the_taken_square.x_coord][the_taken_square.y_coord];
    }

    pop_most_recent_move();
}
