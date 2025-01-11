#include "move.h"
#include "board.h"
#include "move_picker.h"
#include "promotion.h"
#include "en_passant.h"
#include "pieces.h"
#include "castle.h"
#include "game_history.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

const int MAX_POTENTIAL_TOTAL_MOVES_PER_COLOR = 129;

Move previous_move = {0, 0, 0, 0, 0, false, EMPTY};

/**
 * Updates the Pieces struct for a particular colour. This keeps the Pieces struct in sync with the board array after a move is made. .
 * @return A pointer to the square that was taken, NULL if no square was taken. This can be used to undo a move.
 */
Square** update_piece_pointer(Square* from, Square* to, Colour colour) {
    OneColoursPieces *pieces;
    if(colour == WHITE) {
        pieces = allPieces.whitePieces;
    } else if (colour == BLACK) {
        pieces = allPieces.blackPieces;
    } else if (colour == NONE) {
        return NULL;
    }

    int x_coord = from->x_coord;
    int y_coord = from->y_coord;

    for(int i = 0; i < 8; i++) {
        // TODO: We should really extract the pawn once here, but I cant figure out the pointers
        if(pieces->Pawns[i] != NULL && pieces->Pawns[i]->x_coord == x_coord && pieces->Pawns[i]->y_coord == y_coord) {
            pieces->Pawns[i] = to;
            return &pieces->Pawns[i];
        }
    }

    for(int i = 0; i < 2; i++) {
        if(pieces->Rooks[i] != NULL && pieces->Rooks[i]->x_coord == x_coord && pieces->Rooks[i]->y_coord == y_coord) {
            pieces->Rooks[i] = to;
            return &pieces->Rooks[i];
        }
    }

    for(int i = 0; i < 2; i++) {
        if(pieces->Knights[i] != NULL && pieces->Knights[i]->x_coord == x_coord && pieces->Knights[i]->y_coord == y_coord) {
            pieces->Knights[i] = to;
            return &pieces->Knights[i];
        }
    }

    for(int i = 0; i < 2; i++) {
        if(pieces->Bishops[i] != NULL && pieces->Bishops[i]->x_coord == x_coord && pieces->Bishops[i]->y_coord == y_coord) {
            pieces->Bishops[i] = to;
            return &pieces->Bishops[i];
        }
    }

    if(pieces->Queen != NULL && pieces->Queen->x_coord == x_coord && pieces->Queen->y_coord == y_coord) {
        pieces->Queen = to;
        return &pieces->Queen;
    }

    if(pieces->King != NULL && pieces->King->x_coord == x_coord && pieces->King->y_coord == y_coord) {
        if (to == NULL) {
            printf("King is null\n");
        }

        if (to->piece != KING) {
            printf("King is not a king\n");
        }

        pieces->King = to;
        return &pieces->King;
    }

    for (int i = 0; i < 8; i++) {
        if(pieces->PromotedPieces[i] != NULL && pieces->PromotedPieces[i]->x_coord == x_coord && pieces->PromotedPieces[i]->y_coord == y_coord) {
            pieces->PromotedPieces[i] = to;
            return &pieces->PromotedPieces[i];
        }
    }

    return NULL;
}

/**
 * @return Square** A pointer to the square that was taken, NULL if no square was taken
 */
Square** execute_move(Move move, bool commit) {
    Square *from = &board[move.from_x][move.from_y];
    Square *to = &board[move.to_x][move.to_y];

    if(commit) {
        add_move_to_game_history(&move, *from, *to);
    }

    Square* old = &(Square){to->piece, to->color, to->x_coord, to->y_coord};

    to->piece = from->piece;
    to->color = from->color;
    from->piece = EMPTY;
    from->color = NONE;

    update_piece_pointer(from, to, to->color);

    // If we are promoting we handle updating the old pawn pointer in `promote_pawn_to_other_piece`
    if(move.is_promotion) {
        promote_pawn_to_other_piece(to, move.promotion_piece);

        return (Square **) from;
    }

    if(move.is_en_passant) {
        Square* previously_moved_pawn = &board[previous_move.to_x][previous_move.to_y];

        Square** en_passantee = update_piece_pointer(previously_moved_pawn, NULL, previously_moved_pawn->color);

        board[previous_move.to_x][previous_move.to_y].piece = EMPTY;
        board[previous_move.to_x][previous_move.to_y].color = NONE;

        return en_passantee;
    }

    if(move.is_castling) {
        Square** rook_thats_moving = (Square**) {EMPTY, NONE, -1, -1};

        if(move.to_y == 2) {
            // Long castle
            Square* rook = &board[move.to_x][0];
            Square* rook_to = &board[move.to_x][3];

            rook_to->piece = rook->piece;
            rook_to->color = rook->color;
            rook->piece = EMPTY;
            rook->color = NONE;

            rook_thats_moving = update_piece_pointer(rook, rook_to, rook_to->color);
        } else {
            // Short castle
            Square* rook = &board[move.to_x][7];
            Square* rook_to = &board[move.to_x][5];

            rook_to->piece = rook->piece;
            rook_to->color = rook->color;
            rook->piece = EMPTY;
            rook->color = NONE;

            rook_thats_moving = update_piece_pointer(rook, rook_to, rook_to->color);
        }

        return rook_thats_moving;
    }

    Square** all_pieces = update_piece_pointer(old, NULL, old->color);
    if (!commit) {
        if(all_pieces == NULL) {
        }else {
            return all_pieces;
        }
    }

    to = NULL;

    return NULL;
}

/**
 * Used to see if king would be moving into check. These moves are executed, checked then undone.
 */
bool is_king_in_check_after_move(Move move, Colour colour, int depth) {
    depth--;
    if (depth == 0) {
        // Presume white wants to move
        // We check if moving a white piece means a black piece puts the king in check
        // To see if the black piece puts the white king in check we generate all black moves
        // Now we have to check if moving a black piece puts the black king in check. But we don't care if it does.
        // Even if the black piece putting the white king under check is pinned to the black king, the white king is still in check.
        return false;
    }

    OneColoursPieces *otherColoursPieces;
    if (colour == WHITE) {
        otherColoursPieces = allPieces.blackPieces;
    } else if (colour == BLACK) {
        otherColoursPieces = allPieces.whitePieces;
    }

    if (otherColoursPieces == NULL) {
        printf("otherColoursPieces not set");
    }

    if (move.to_x == otherColoursPieces->King->x_coord && move.to_y == otherColoursPieces->King->y_coord) {
        // Slight bastardisation of this function.
        // The colour whose moving king isn't in check, but the move is trying to take the other colours piece which is invalid.
        return true;
    }

    Square previous_square_val = board[move.to_x][move.to_y];
    Square *previous_square = &board[move.to_x][move.to_y];

    Square **just_taken_square = execute_move(move, false);
    Move two_moves_ago = previous_move;
    previous_move = move; // Set this global variable for en passant

    bool is_check = is_king_in_check(colour, depth);

    // Undo move
    execute_move((Move) {move.to_x, move.to_y, move.from_x, move.from_y}, false);
    board[move.to_x][move.to_y] = previous_square_val;
    previous_move = two_moves_ago;

    // Promotion requires a more complicated undo. This is because it's moving two different piece types.
    if (move.is_promotion) {
        OneColoursPieces **pieces = (board[move.from_x][move.from_y].color == WHITE) ?
                                    &allPieces.whitePieces : &allPieces.blackPieces;

        board[move.from_x][move.from_y].piece = PAWN;

        int pawn_index = find_next_empty_piece_index(board[move.from_x][move.from_y].color, PAWN);
        (*pieces)->Pawns[pawn_index] = &board[move.from_x][move.from_y];

        int promoted_piece_index = find_piece_index_by_coordinate(move.from_x, move.from_y, EMPTY, true);
        (*pieces)->PromotedPieces[promoted_piece_index] = NULL;
    } else if(move.is_en_passant) {
        // An en passant moves the pawn to the square behind the taken pawn. This will be -1 for white and +1 for black
        int en_passant_offset = (colour == WHITE) ? 1 : -1;
        Square* the_moved_to_square = &board[move.to_x+en_passant_offset][move.to_y];
        Colour opposite_colour = colour == WHITE ? BLACK : WHITE;
        the_moved_to_square->color = opposite_colour;
        the_moved_to_square->piece = PAWN;

        *just_taken_square = the_moved_to_square;
    } else if(move.is_castling) {
        // Long castle
        if(move.to_y == 2) {
            Square* where_rook_should_be = &board[move.to_x][0];
            Square* moved_rook = &board[move.to_x][3];

            where_rook_should_be->piece = moved_rook->piece;
            where_rook_should_be->color = moved_rook->color;

            moved_rook->piece = EMPTY;
            moved_rook->color = NONE;

            *just_taken_square = where_rook_should_be;
        }else if(move.to_y == 6) { // Short castle
            Square* where_rook_should_be = &board[move.to_x][7];
            Square* moved_rook = &board[move.to_x][5];

            where_rook_should_be->piece = moved_rook->piece;
            where_rook_should_be->color = moved_rook->color;

            moved_rook->piece = EMPTY;
            moved_rook->color = NONE;

            *just_taken_square = where_rook_should_be;
        }
    } else if (just_taken_square != NULL) {
        *just_taken_square = previous_square;
    }

    return is_check;
}

/**
 * @return Move array of size 27, either populated or all 0s. Populated entries are contiguous, not fragmented.
 */
Move* generate_legal_moves_for_cell(Square *square, int depth) {
    Move* moves = (Move*) calloc(MAX_POTENTIAL_MOVES_FOR_ONE_PIECE, sizeof(Move));
    int index = 0;
    Piece piece = square->piece;
    Colour colour = square->color;
    int x = square->x_coord;
    int y = square->y_coord;

    if (piece == EMPTY) {
        printf("Piece is empty\n");
        return NULL;
    }

    if (piece == PAWN) {
        if (colour == WHITE) {
            // Move forward
            if(x > 1 && board[x-1][y].piece == EMPTY) {
                Move* move = &(Move) {x, y, x-1, y};
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    calculate_move_score(move);
                    moves[index] = *move;
                    index++;
                }
            }

            // Move forward two
            if(x == 6 && board[x-2][y].piece == EMPTY && board[x-1][y].piece == EMPTY) {
                Move* move = &(Move) {x, y, x-2, y};
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    calculate_move_score(move);
                    moves[index] = *move;
                    index++;
                }
            }

            // Take to left
            if (x > 1 && y > 0 && board[x-1][y-1].color == BLACK) {
                Move* move = &(Move) {x, y, x-1, y-1};
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    calculate_move_score(move);
                    moves[index] = *move;
                    index++;
                }
            }

            // Take to right
            if (x > 1 && y < 7 && board[x-1][y+1].color == BLACK) {
                Move* move = &(Move) {x, y, x-1, y+1};
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    calculate_move_score(move);
                    moves[index] = *move;
                    index++;
                }
            }

            // Promote
            if(x == 1 && board[0][y].piece == EMPTY) {
                Move* move = &(Move) {x, y, 0, y, true};
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    for (int i=0; i<4; i++)
                    {
                        Piece promotion_piece = retrieve_promotion_piece(i);
                        move->is_promotion = true;
                        move->promotion_piece = promotion_piece;
                        calculate_move_score(move);
                        moves[index] = *move;
                        index++;
                    }
                }
            }

            if(x == 1 && board[0][y-1].color == BLACK) {
                Move* move = &(Move) {x, y, 0, y-1};
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    for (int i=0; i<4; i++)
                    {
                        Piece promotion_piece = retrieve_promotion_piece(i);
                        move->is_promotion = true;
                        move->promotion_piece = promotion_piece;
                        calculate_move_score(move);
                        moves[index] = *move;
                        index++;
                    }
                }
            }

            if(x == 1 && board[0][y+1].color == BLACK) {
                Move* move = &(Move) {x, y, 0, y+1};
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    for (int i=0; i<4; i++)
                    {
                        Piece promotion_piece = retrieve_promotion_piece(i);
                        move->is_promotion = true;
                        move->promotion_piece = promotion_piece;
                        calculate_move_score(move);
                        moves[index] = *move;
                        index++;
                    }
                }
            }

            // En passant
            if(is_move_en_passantable(previous_move)) {
                // Don't have to check if the piece is a pawn here as it's done in `is_move_en_passantable`
                if(y > 0 && board[x][y-1].color == BLACK && x == previous_move.to_x && y-1 == previous_move.to_y) {
                    Move* move = &(Move) {x, y, x-1, y-1};
                    move->is_en_passant = true;

                    if(!is_king_in_check_after_move(*move, colour, depth)){
                        calculate_move_score(move);
                        moves[index] = *move;
                        index++;
                    }
                }

                if(y < 7 && board[x][y+1].color == BLACK && x == previous_move.to_x && y+1 == previous_move.to_y) {
                    Move* move = &(Move) {x, y, x-1, y+1};
                    move->is_en_passant = true;

                    if(!is_king_in_check_after_move(*move, colour, depth)){
                        calculate_move_score(move);
                        moves[index] = *move;
                        index++;
                    }
                }
            }
        } else if(colour == BLACK) {
            // Move forward
            if(x < 7 && board[x+1][y].piece == EMPTY) {
                Move* move = &((Move) {x, y, x+1, y});
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    calculate_move_score(move);
                    moves[index] = *move;
                    index++;
                }
            }

            // Move forward two
            if(x == 1 && board[x+2][y].piece == EMPTY && board[x+1][y].piece == EMPTY) {
                Move* move = &((Move) {x, y, x+2, y});
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    calculate_move_score(move);
                    moves[index] = *move;
                    index++;
                }
            }

            // Take to right
            if (x < 7 && y > 0 && board[x+1][y-1].color == WHITE) {
                Move* move = &((Move) {x, y, x+1, y-1});
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    calculate_move_score(move);
                    moves[index] = *move;
                    index++;
                }
            }

            // Take to left
            if (x < 7 && y < 7 && board[x+1][y+1].color == WHITE) {
                Move* move = &(Move) {x, y, x+1, y+1};
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    calculate_move_score(move);
                    moves[index] = *move;
                    index++;
                }
            }

            // Promotion
            if(x == 6 && board[7][y].piece == EMPTY) {
                Move* move = &(Move) {x, y, 7, y};
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    for (int i=0; i<4; i++)
                    {
                        Piece promotion_piece = retrieve_promotion_piece(i);
                        move->is_promotion = true;
                        move->promotion_piece = promotion_piece;
                        calculate_move_score(move);
                        moves[index] = *move;
                        index++;
                    }
                }
            }

            if(x == 6 && board[7][y-1].color == BLACK) {
                Move* move = &(Move) {x, y, 7, y-1};
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    for (int i=0; i<4; i++)
                    {
                        Piece promotion_piece = retrieve_promotion_piece(i);
                        move->is_promotion = true;
                        move->promotion_piece = promotion_piece;
                        calculate_move_score(move);
                        moves[index] = *move;
                        index++;
                    }
                }
            }

            if(x == 6 && board[7][y+1].color == BLACK) {
                Move* move = &(Move) {x, y, 7, y+1};
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    for (int i=0; i<4; i++)
                    {
                        Piece promotion_piece = retrieve_promotion_piece(i);
                        move->is_promotion = true;
                        move->promotion_piece = promotion_piece;
                        calculate_move_score(move);
                        moves[index] = *move;
                        index++;
                    }
                }
            }

            // En passant
             if(is_move_en_passantable(previous_move)) {
                // Don't have to check if the piece is a pawn here as it's done in `is_move_en_passantable`
                if(y > 0 && board[x][y-1].color == WHITE && x == previous_move.to_x && y-1 == previous_move.to_y) {
                    Move* move = &(Move) {x, y, x+1, y-1};
                    move->is_en_passant = true;

                    if(!is_king_in_check_after_move(*move, colour, depth)){
                        calculate_move_score(move);
                        moves[index] = *move;
                        index++;
                    }
                }

                if(y < 7 && board[x][y+1].color == WHITE && x == previous_move.to_x && y+1 == previous_move.to_y) {
                    Move* move = &(Move) {x, y, x+1, y+1};
                    move->is_en_passant = true;

                    if(!is_king_in_check_after_move(*move, colour, depth)){
                        calculate_move_score(move);
                        moves[index] = *move;
                        index++;
                    }
                }
            }
        }
    } else if(piece == BISHOP) {
        // Move right diagonally
        for (int i = 1; i < 8; i++) {
            if(x+i > 7 || y+i > 7) {
                break;
            }
            if (board[x+i][y+i].piece == EMPTY) {
                Move* move = &(Move) {x, y, x+i, y+i};
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    calculate_move_score(move);
                    moves[index] = *move;
                    index++;
                }
            } else if (board[x+i][y+i].color != colour) {
                Move* move = &(Move) {x, y, x+i, y+i};
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    calculate_move_score(move);
                    moves[index] = *move;
                    index++;
                }
                break;
            } else {
                break;
            }
        }

        // Move left diagonally
        for (int i = 1; i < 8; i++) {
            if(x-i < 0) {
                continue;
            }else if(y+i > 7) {
                break;
            }

            if (board[x-i][y+i].piece == EMPTY) {
                Move* move = &(Move) {x, y, x-i, y+i};
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    calculate_move_score(move);
                    moves[index] = *move;
                    index++;
                }
            } else if (board[x-i][y+i].color != colour) {
                Move* move = &(Move) {x, y, x-i, y+i};
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    calculate_move_score(move);
                    moves[index] = *move;
                    index++;
                }
                break;
            } else {
                break;
            }
        }

        // Move back left diagonally
        for (int i = 1; i < 8; i++) {
            if(x-i < 0 || y-i < 0) {
                continue;
            }

            if (board[x-i][y-i].piece == EMPTY) {
                Move* move = &(Move) {x, y, x-i, y-i};
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    calculate_move_score(move);
                    moves[index] = *move;
                    index++;
                }
            } else if (board[x-i][y-i].color != colour) {
                Move* move = &(Move) {x, y, x-i, y-i};
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    calculate_move_score(move);
                    moves[index] = *move;
                    index++;
                }
                break;
            } else {
                break;
            }
        }

        // Move back right diagonally
        for (int i = 1; i < 8; i++) {
            if(x+i > 7) {
                break;
            }else if(y-i < 0) {
                continue;
            }

            if (board[x+i][y-i].piece == EMPTY) {
                Move* move = &(Move) {x, y, x+i, y-i};
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    calculate_move_score(move);
                    moves[index] = *move;
                    index++;
                }
            } else if (board[x+i][y-i].color != colour) {
                Move* move = &(Move) {x, y, x+i, y-i};
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    calculate_move_score(move);
                    moves[index] = *move;
                    index++;
                }
                break;
            } else {
                break;
            }
        }
    } else if (piece == ROOK) {
        // i starts at one here as we need a differential else we will try to move to the square we are on

        // Move east
        for (int i = 1; i < 8; i++) {
            if(y+i > 7) {
                break;
            }

            if (board[x][y+i].piece == EMPTY) {
                Move* move = &(Move) {x, y, x, y+i};
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    calculate_move_score(move);
                    moves[index] = *move;
                    index++;
                }
            } else if (board[x][y+i].color != colour) {
                Move* move = &(Move) {x, y, x, y+i};
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    calculate_move_score(move);
                    moves[index] = *move;
                    index++;
                }
                break;
            } else {
                break;
            }
        }

        // Move west
        for (int i = 1; i < 8; i++) {
            if(y-i < 0) {
                break;
            }

            if (board[x][y-i].piece == EMPTY) {
                Move* move = &(Move) {x, y, x, y-i};
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    calculate_move_score(move);
                    moves[index] = *move;
                    index++;
                }
            } else if (board[x][y-i].color != colour) {
                Move* move = &(Move) {x, y, x, y-i};
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    calculate_move_score(move);
                    moves[index] = *move;
                    index++;
                }
                break;
            } else {
                break;
            }
        }

        // Move north
        for (int i = 1; i < 8; i++) {
            if(x-i < 0) {
                break;
            }

            if (board[x-i][y].piece == EMPTY) {
                Move* move = &(Move) {x, y, x-i, y};
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    calculate_move_score(move);
                    moves[index] = *move;
                    index++;
                }
            } else if (board[x-i][y].color != colour) {
                Move* move = &(Move) {x, y, x-i, y};
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    calculate_move_score(move);
                    moves[index] = *move;
                    index++;
                }
                break;
            } else {
                break;
            }
        }

        // Move south
        for (int i = 1; i < 8; i++) {
            if(x+i > 7) {
                break;
            }

            if (board[x+i][y].piece == EMPTY) {
                Move* move = &(Move) {x, y, x+i, y};
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    calculate_move_score(move);
                    moves[index] = *move;
                    index++;
                }
            } else if (board[x+i][y].color != colour) {
                Move* move = &(Move) {x, y, x+i, y};
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    calculate_move_score(move);
                    moves[index] = *move;
                    index++;
                }
                break;
            } else {
                break;
            }
        }

    } else if(piece == KNIGHT) {
        // Move up and right (white inverse)
        if (x < 6 && y < 7 && (board[x+2][y+1].piece == EMPTY || board[x+2][y+1].color != colour)) {
            Move* move = &(Move) {x, y, x+2, y+1};
            if(!is_king_in_check_after_move(*move, colour, depth)){
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            }
        }

        // Move up and left (white inverse)
        if (x < 6 && y > 0 && (board[x+2][y-1].piece == EMPTY || board[x+2][y-1].color != colour)) {
            Move *move = &(Move) {x, y, x + 2, y - 1};
            if (!is_king_in_check_after_move(*move, colour, depth)) {
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            }
        }

        // Move down and right (white inverse)
        if (x > 1 && y < 7 && (board[x-2][y+1].piece == EMPTY || board[x-2][y+1].color != colour)) {
            Move* move = &(Move) {x, y, x-2, y+1};
            if(!is_king_in_check_after_move(*move, colour, depth)){
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            }
        }

        // Move down and left (white inverse)
        if (x > 1 && y > 0 && (board[x-2][y-1].piece == EMPTY || board[x-2][y-1].color != colour)) {
            Move* move = &(Move) {x, y, x-2, y-1};
            if(!is_king_in_check_after_move(*move, colour, depth)){
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            }
        }

        // Move right and up (white inverse)
        if (x < 7 && y < 6 && (board[x+1][y+2].piece == EMPTY || board[x+1][y+2].color != colour)) {
            Move* move = &(Move) {x, y, x+1, y+2};
            if(!is_king_in_check_after_move(*move, colour, depth)){
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            }
        }

        // Move right and up (white inverse)
        if (x < 7 && y > 1 && (board[x+1][y-2].piece == EMPTY || board[x+1][y-2].color != colour)) {
            Move* move = &(Move) {x, y, x+1, y-2};
            if(!is_king_in_check_after_move(*move, colour, depth)){
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            }
        }

        // Move right and down (white inverse)
        if (x > 0 && y < 6 && (board[x-1][y+2].piece == EMPTY || board[x-1][y+2].color != colour)) {
            Move* move = &(Move) {x, y, x-1, y+2};
            if(!is_king_in_check_after_move(*move, colour, depth)){
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            }
        }

        // Move left and down (white inverse)
        if (x > 0 && y > 1 && (board[x-1][y-2].piece == EMPTY || board[x-1][y-2].color != colour)) {
            Move* move = &(Move) {x, y, x-1, y-2};
            if(!is_king_in_check_after_move(*move, colour, depth)){
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            }
        }

        // Move left and up (white inverse)
        if (x < 7 && y > 1 && (board[x+1][y-2].piece == EMPTY || board[x+1][y-2].color != colour)) {
            Move* move = &(Move) {x, y, x+1, y-2};
            if(!is_king_in_check_after_move(*move, colour, depth)){
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            }
        }
    } else if(piece == QUEEN) {
        // Copy paste of Bishop + Rook logic TODO: Extract
        // Rook below
        // Move right (left for black)
        // Move right (left for black)
        for (int i = 1; i < 8; i++) {
            if(y+i > 7) {
                break;
            }

            if (board[x][y+i].piece == EMPTY) {
                Move* move = &(Move) {x, y, x, y+i};
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    calculate_move_score(move);
                    moves[index] = *move;
                    index++;
                }
            } else if (board[x][y+i].color != colour) {
                Move* move = &(Move) {x, y, x, y+i};
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    calculate_move_score(move);
                    moves[index] = *move;
                    index++;
                }
                break;
            } else {
                break;
            }
        }

        // Move left (right for black)
        for (int i = 1; i < 8; i++) {
            if(y-i < 0) {
                break;
            }

            if (board[x][y-i].piece == EMPTY) {
                Move* move = &(Move) {x, y, x, y-i};
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    calculate_move_score(move);
                    moves[index] = *move;
                    index++;
                }
            } else if (board[x][y-i].color != colour) {
                Move* move = &(Move) {x, y, x, y-i};
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    calculate_move_score(move);
                    moves[index] = *move;
                    index++;
                }
                break;
            } else {
                break;
            }
        }

        // Move up (down for black)
        for (int i = 1; i < 8; i++) {
            if(x-i < 0) {
                continue;
            }

            if (board[x-i][y].piece == EMPTY) {
                Move* move = &(Move) {x, y, x-i, y};
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    calculate_move_score(move);
                    moves[index] = *move;
                    index++;
                }
            } else if (board[x-i][y].color != colour) {
                Move* move = &(Move) {x, y, x-i, y};
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    calculate_move_score(move);
                    moves[index] = *move;
                    index++;
                }
                break;
            } else {
                break;
            }
        }

        // Move down (up for black)
        for (int i = 1; i < 8; i++) {
            if(x+i > 7) {
                break;
            }

            if (board[x+i][y].piece == EMPTY) {
                Move* move = &(Move) {x, y, x+i, y};
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    calculate_move_score(move);
                    moves[index] = *move;
                    index++;
                }
            } else if (board[x+i][y].color != colour) {
                Move* move = &(Move) {x, y, x+i, y};
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    calculate_move_score(move);
                    moves[index] = *move;
                    index++;
                }
                break;
            } else {
                break;
            }
        }

        // Bishop below
        // Move right diagonally
        // Move right diagonally
        for (int i = 1; i < 8; i++) {
            if(x+i > 7 || y+i > 7) {
                break;
            }
            if (board[x+i][y+i].piece == EMPTY) {
                Move* move = &(Move) {x, y, x+i, y+i};
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    calculate_move_score(move);
                    moves[index] = *move;
                    index++;
                }
            } else if (board[x+i][y+i].color != colour) {
                Move* move = &(Move) {x, y, x+i, y+i};
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    calculate_move_score(move);
                    moves[index] = *move;
                    index++;
                }
                break;
            } else {
                break;
            }
        }

        // Move left diagonally
        for (int i = 1; i < 8; i++) {
            if(x-i < 0) {
                continue;
            }else if(y+i > 7) {
                break;
            }

            if (board[x-i][y+i].piece == EMPTY) {
                Move* move = &(Move) {x, y, x-i, y+i};
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    calculate_move_score(move);
                    moves[index] = *move;
                    index++;
                }
            } else if (board[x-i][y+i].color != colour) {
                Move* move = &(Move) {x, y, x-i, y+i};
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    calculate_move_score(move);
                    moves[index] = *move;
                    index++;
                }
                break;
            } else {
                break;
            }
        }

        // Move back left diagonally
        for (int i = 1; i < 8; i++) {
            if(x-i < 0 || y-i < 0) {
                continue;
            }

            if (board[x-i][y-i].piece == EMPTY) {
                Move* move = &(Move) {x, y, x-i, y-i};
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    calculate_move_score(move);
                    moves[index] = *move;
                    index++;
                }
            } else if (board[x-i][y-i].color != colour) {
                Move* move = &(Move) {x, y, x-i, y-i};
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    calculate_move_score(move);
                    moves[index] = *move;
                    index++;
                }
                break;
            } else {
                break;
            }
        }

        // Move back right diagonally
        for (int i = 1; i < 8; i++) {
            if(x+i > 7) {
                break;
            }else if(y-i < 0) {
                continue;
            }

            if (board[x+i][y-i].piece == EMPTY) {
                Move* move = &(Move) {x, y, x+i, y-i};
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    calculate_move_score(move);
                    moves[index] = *move;
                    index++;
                }
            } else if (board[x+i][y-i].color != colour) {
                Move* move = &(Move) {x, y, x+i, y-i};
                if(!is_king_in_check_after_move(*move, colour, depth)){
                    calculate_move_score(move);
                    moves[index] = *move;
                    index++;
                }
                break;
            } else {
                break;
            }
        }
    } else if(piece == KING) {
        // Move up
        if (x > 0 && (board[x-1][y].piece == EMPTY || board[x-1][y].color != colour)) {
            Move potential_move = (Move) {x, y, x-1, y};
            if(!is_king_in_check_after_move(potential_move, colour, depth)) {
                Move* move = &potential_move;
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            }
        }

        // Move down
        if (x < 7 && (board[x+1][y].piece == EMPTY || board[x+1][y].color != colour)) {
            Move potential_move = (Move) {x, y, x+1, y};

            if(!is_king_in_check_after_move(potential_move, colour, depth)) {
                Move* move = &potential_move;
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            }
        }

        // Move left
        if (y > 0 && (board[x][y-1].piece == EMPTY || board[x][y-1].color != colour)) {
            Move potential_move = (Move) {x, y, x, y-1};

            if(!is_king_in_check_after_move(potential_move, colour, depth)) {
                Move* move = &potential_move;
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            }
        }

        // Move right
        if (y < 7 && (board[x][y+1].piece == EMPTY || board[x][y+1].color != colour)) {
            Move potential_move = (Move) {x, y, x, y+1};

            if(!is_king_in_check_after_move(potential_move, colour, depth)) {
                Move* move = &potential_move;
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            }
        }

        // Move up and right
        if (x > 0 && y < 7 && (board[x-1][y+1].piece == EMPTY || board[x-1][y+1].color != colour)) {
            Move potential_move = (Move) {x, y, x-1, y+1};

            if(!is_king_in_check_after_move(potential_move, colour, depth)) {
                Move* move = &potential_move;
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            }
        }

        // Move up and left
        if (x > 0 && y > 0 && (board[x-1][y-1].piece == EMPTY || board[x-1][y-1].color != colour)) {
            Move potential_move = (Move) {x, y, x-1, y-1};

            if(!is_king_in_check_after_move(potential_move, colour, depth)) {
                Move* move = &potential_move;
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            }
        }

        // Move down and right
        if (x < 7 && y < 7 && (board[x+1][y+1].piece == EMPTY || board[x+1][y+1].color != colour)) {
            Move potential_move = (Move) {x, y, x+1, y+1};

            if(!is_king_in_check_after_move(potential_move, colour, depth)) {
                Move* move = &potential_move;
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            }
        }

        // Move down and left
        if (y > 0 && x < 7 && (board[x+1][y-1].piece == EMPTY || board[x+1][y-1].color != colour)) {
            Move potential_move = (Move) {x, y, x+1, y-1};
            if(!is_king_in_check_after_move(potential_move, colour, depth)) {
                Move* move = &potential_move;
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            }
        }

        // Castling
        if(can_long_castle(*square)) {
            // Don't need separate y coords for black and white here, since the y coord is the same for both
            Move* move = &(Move) {x, y, x, y-2};
            move->is_castling = true;
            calculate_move_score(move);
            moves[index] = *move;
            index++;
        }

        if(can_short_castle(*square)) {
            Move* move = &(Move) {x, y, x, y+2};
            move->is_castling = true;
            calculate_move_score(move);
            moves[index] = *move;
            index++;
        }
    }

    return moves;
}

void merge_arrays_for_pieces(Move* the_moves, Move* some_moves, int* total_moves_added) {
    if(some_moves == NULL) {
        printf("some_moves is NULL, should this happen \n");
        return;
    }

    for(int j = 0; j < MAX_POTENTIAL_MOVES_FOR_ONE_PIECE; j++) {
        Move* a_move = &some_moves[j];
        if (some_moves[j].from_x == 0 && some_moves[j].from_y == 0 &&
            some_moves[j].to_x == 0 && some_moves[j].to_y == 0) {
            break; // Skip empty moves
        }
        the_moves[*total_moves_added] = *a_move;
        (*total_moves_added)++;
    }
}

/**
 * @param depth used to stop infinite recursion when making sure a potential move doesnt put the king in check
 * @return Array of moves, limited to 129. Not fragmented, one null value will be the end of the array.
 */
Move* generate_moves_for_one_color(OneColoursPieces* aColoursPieces, bool include_king, int depth) {
    Move *moves = (Move*) calloc(MAX_POTENTIAL_TOTAL_MOVES_PER_COLOR, sizeof(Move));

    int total_moves_added = 0;

    if(include_king) {
        Square* my_king = aColoursPieces->King;
        if(my_king == NULL) {
            printf("King is NULL\n");
            return NULL;
        }
        Move* kings_moves = generate_legal_moves_for_cell(my_king, depth);
        merge_arrays_for_pieces(moves, kings_moves, &total_moves_added);

        free(kings_moves);
    }

    for(int i = 0; i < 8; i++) {
        Square* pawn = aColoursPieces->Pawns[i];
        if(pawn == NULL) {
            continue;
        }

        Move* some_moves = generate_legal_moves_for_cell(pawn, depth);
        if(some_moves == NULL) {
            printf("Failed to generate legal moves for pawn\n");
            free(some_moves);
            return NULL;
        }

        merge_arrays_for_pieces(moves, some_moves, &total_moves_added);

        free(some_moves);
    }


    for(int i = 0; i < 2; i++) {
        Square* rook = aColoursPieces->Rooks[i];
        if(rook == NULL) {
            continue;
        }

        Move* some_moves = generate_legal_moves_for_cell(rook, depth);
        merge_arrays_for_pieces(moves, some_moves, &total_moves_added);

        free(some_moves);
    }

    for(int i = 0; i < 2; i++) {
        Square* knight = aColoursPieces->Knights[i];
        if(knight == NULL) {
            continue;
        }

        Move* some_moves = generate_legal_moves_for_cell(knight, depth);
        merge_arrays_for_pieces(moves, some_moves, &total_moves_added);

        free(some_moves);
    }

    for(int i = 0; i < 2; i++) {
        Square* bishop = aColoursPieces->Bishops[i];
        if(bishop == NULL) {
            continue;
        }

        Move* some_moves = generate_legal_moves_for_cell(bishop, depth);
        merge_arrays_for_pieces(moves, some_moves, &total_moves_added);

        free(some_moves);
    }

    Square* queen = aColoursPieces->Queen;
    if(queen != NULL) {
        Move* some_moves = generate_legal_moves_for_cell(queen, depth);
        merge_arrays_for_pieces(moves, some_moves, &total_moves_added);
        free(some_moves);
    }

    for(int i=0; i<8; i++){
        Square* promoted_piece = aColoursPieces->PromotedPieces[i];

        if(promoted_piece == NULL ||
        (promoted_piece->x_coord == 0 && promoted_piece->y_coord == 0
        && promoted_piece->color == NONE && promoted_piece-> piece == EMPTY)
        ) {
            continue;
        }

        Move* some_moves = generate_legal_moves_for_cell(promoted_piece, depth);
        merge_arrays_for_pieces(moves, some_moves, &total_moves_added);
        free(some_moves);
    }

    return moves;
}

// TODO: Move to utils
int are_coordinates_within1(int x1, int y1, int x2, int y2) {
    return (abs(x1 - x2) <= 1) && (abs(y1 - y2) <= 1);
}

/**
 * @param depth used to stop infinite recursion when making sure a potential move doesnt put the king in check
 */
bool is_king_in_check(Colour colour, int depth) {
    Move* all_legal_moves;
    Square* king;
    Square* opponentsKing;

    if(colour == WHITE) {
        all_legal_moves = generate_moves_for_one_color(allPieces.blackPieces, false, depth);
        king = allPieces.whitePieces->King;
        opponentsKing = allPieces.blackPieces->King;
    }else {
        all_legal_moves = generate_moves_for_one_color(allPieces.whitePieces, false, depth);
        king = allPieces.blackPieces->King;
        opponentsKing = allPieces.whitePieces->King;
    }

    if(all_legal_moves == NULL) {
        printf("Failed to generate all legal moves\n");
        free(all_legal_moves);
        return false;
    }

    int kings_x = king->x_coord;
    int kings_y = king->y_coord;

    // Is the other king putting our king in check
    if(are_coordinates_within1(kings_x, kings_y, opponentsKing->x_coord, opponentsKing->y_coord)){
        free(all_legal_moves);
        return true;
    }

    // TODO: If we instead moved this check inside `generate_all_legal_moves` we could avoid this loop
    for (int i = 0; i < MAX_POTENTIAL_TOTAL_MOVES_PER_COLOR; i++) {
        Move* move = &all_legal_moves[i];
        if(move == NULL || (move->from_x == 0 && move->from_y == 0 && move->to_x == 0 && move->to_y == 0)) {
            free(all_legal_moves);
            return false;
        }
        if (move->to_x == king->x_coord && move->to_y == king->y_coord) {
            free(all_legal_moves);
            return true;
        }
    }
    free(all_legal_moves);

    return false;
}