#include "move.h"
#include "board.h"
#include "move_picker.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

const int MAX_POTENTIAL_TOTAL_MOVES_PER_COLOR = 129;

Square** update_piece_pointer(Square* from, Square* to, Colour colour) {
    OneColoursPieces *pieces;
    if(colour == WHITE) {
        pieces = allPieces.whitePieces;
    } else if (colour == BLACK) {
        pieces = allPieces.blackPieces;
    } else if (colour == NONE) {
        printf("Passed in no colour, returning\n");
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
        pieces->King = to;
        return &pieces->King;
    }

    return NULL;
}

Square** execute_move(Move move, bool commit) {
    Square *from = &board[move.from_x][move.from_y];
    Square *to = &board[move.to_x][move.to_y];

    // Log all from and to properties
    printf("From properties: x: %d, y: %d, piece: %d, color: %d\n", from->x_coord, from->y_coord, from->piece, from->color);
    printf("To properties: x: %d, y: %d, piece: %d, color: %d\n", to->x_coord, to->y_coord, to->piece, to->color);

    Square* old = &(Square){to->piece, to->color, to->x_coord, to->y_coord};
    to->piece = from->piece;
    to->color = from->color;
    from->piece = EMPTY;
    from->color = NONE;

    update_piece_pointer(from, to, to->color);

    Square** all_pieces = update_piece_pointer(old, NULL, old->color);

    if (!commit) {
        if(all_pieces == NULL) {
            printf("Last square taken was empty\n");
        }else {
            return all_pieces;
        }
    }

    to = NULL;

    return NULL;
}

// Used to see if king would be moving into check. These moves are executed, checked then undone.
bool is_king_in_check_after_move(Move move, Colour colour) {
    Square previous_square_val = board[move.to_x][move.to_y];
    Square* previous_square = &board[move.to_x][move.to_y];
    Square** just_taken_square = execute_move(move, false);
    bool is_check = is_king_in_check(colour);

    // Undo move
    execute_move((Move) {move.to_x, move.to_y, move.from_x, move.from_y}, false);
    board[move.to_x][move.to_y] = previous_square_val;

    if(just_taken_square != NULL) {
        *just_taken_square = previous_square;
    }

    if(is_check) {
        printf("King would be in check after move\n");
    }

    return is_check;
}

/**
 * @return Move array of size 27, either populated or all 0s. Populated entries are not fragmented.
 */
Move* generate_legal_moves_for_cell(Square *square) {
    Move* moves = (Move*) malloc(MAX_POTENTIAL_MOVES_FOR_ONE_PIECE * sizeof(Move));
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
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            }

            // Move forward two
            if(x == 6 && board[x-2][y].piece == EMPTY) {
                Move* move = &(Move) {x, y, x-2, y};
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            }

            // Take to left
            if (x > 1 && y > 1 && board[x-1][y-1].color == BLACK) {
                Move* move = &(Move) {x, y, x-1, y-1};
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            }

            // Take to right
            if (x > 1 && y < 7 && board[x-1][y+1].color == BLACK) {
                Move* move = &(Move) {x, y, x-1, y+1};
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            }

            // Promote TODO: Implement promotion
            if(x == 1 && (board[0][y].piece == EMPTY || board[0][y-1].piece == EMPTY || board[0][y+1].piece == EMPTY)) {
                Move* move = &(Move) {x, y, 99999, 99999};
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            }
        } else if(colour == BLACK) {
            // Move forward
            if(x < 7 && board[x+1][y].piece == EMPTY) {
                Move* move = &((Move) {x, y, x+1, y});
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            }

            // Move forward two
            if(x == 1 && board[x+2][y].piece == EMPTY) {
                Move* move = &((Move) {x, y, x+2, y});
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            }

            // Take to right
            if (x < 7 && y > 1 && board[x+1][y-1].color == WHITE) {
                Move* move = &((Move) {x, y, x+1, y-1});
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            }

            // Take to left
            if (x < 7 && y < 7 && board[x+1][y+1].color == WHITE) {
                Move* move = &(Move) {x, y, x+1, y+1};
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            }

            // Promote TODO: Implement promotion
            if(x == 6 && (board[7][y].piece == EMPTY || board[7][y-1].piece == EMPTY || board[7][y+1].piece == EMPTY)) {
                Move* move = &(Move) {x, y, 99999, 99999};
                calculate_move_score(move);
                moves[index] = *move;
                index++;
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
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            } else if (board[x+i][y+i].color != colour) {
                Move* move = &(Move) {x, y, x+i, y+i};
                calculate_move_score(move);
                moves[index] = *move;
                index++;
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
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            } else if (board[x-i][y+i].color != colour) {
                Move* move = &(Move) {x, y, x-i, y+i};
                calculate_move_score(move);
                moves[index] = *move;
                index++;
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
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            } else if (board[x-i][y-i].color != colour) {
                Move* move = &(Move) {x, y, x-i, y-i};
                calculate_move_score(move);
                moves[index] = *move;
                index++;
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
                calculate_move_score(move);
                moves[index] = *move;
            } else if (board[x+i][y-i].color != colour) {
                Move* move = &(Move) {x, y, x+i, y-i};
                calculate_move_score(move);
                moves[index] = *move;
                break;
            } else {
                break;
            }
        }
    } else if (piece == ROOK) {
        // Move right (left for black)
        for (int i = 1; i < 8; i++) {
            if(y+i > 7) {
                break;
            }

            if (board[x][y+i].piece == EMPTY) {
                Move* move = &(Move) {x, y, x, y+i};
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            } else if (board[x][y+i].color != colour) {
                Move* move = &(Move) {x, y, x, y+i};
                calculate_move_score(move);
                moves[index] = *move;
                index++;
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
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            } else if (board[x][y-i].color != colour) {
                Move* move = &(Move) {x, y, x, y-i};
                calculate_move_score(move);
                moves[index] = *move;
                index++;
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
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            } else if (board[x-i][y].color != colour) {
                Move* move = &(Move) {x, y, x-i, y};
                calculate_move_score(move);
                moves[index] = *move;
                index++;
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
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            } else if (board[x+i][y].color != colour) {
                Move* move = &(Move) {x, y, x+i, y};
                calculate_move_score(move);
                moves[index] = *move;
                index++;
                break;
            } else {
                break;
            }
        }

    } else if(piece == KNIGHT) {
        // Move up and right (white inverse)
        if (x < 6 && y < 7 && (board[x+2][y+1].piece == EMPTY || board[x+2][y+1].color != colour)) {
            Move* move = &(Move) {x, y, x+2, y+1};
            calculate_move_score(move);
            moves[index] = *move;
            index++;
        }

        // Move up and left (white inverse)
        if (x < 6 && y > 0 && (board[x+2][y-1].piece == EMPTY || board[x+2][y-1].color != colour)) {
            Move* move = &(Move) {x, y, x+2, y-1};
            calculate_move_score(move);
            moves[index] = *move;
            index++;
        }

        // Move down and right (white inverse)
        if (x > 1 && y < 7 && (board[x-2][y+1].piece == EMPTY || board[x-2][y+1].color != colour)) {
            Move* move = &(Move) {x, y, x-2, y+1};
            calculate_move_score(move);
            moves[index] = *move;
            index++;
        }

        // Move down and left (white inverse)
        if (x > 1 && y > 0 && (board[x-2][y-1].piece == EMPTY || board[x-2][y-1].color != colour)) {
            Move* move = &(Move) {x, y, x-2, y-1};
            calculate_move_score(move);
            moves[index] = *move;
            index++;
        }

        // Move right and up (white inverse)
        if (x < 7 && y < 6 && (board[x+1][y+2].piece == EMPTY || board[x+1][y+2].color != colour)) {
            Move* move = &(Move) {x, y, x+1, y+2};
            calculate_move_score(move);
            moves[index] = *move;
            index++;
        }

        // Move right and up (white inverse)
        if (x < 7 && y > 6 && (board[x+1][y-2].piece == EMPTY || board[x+1][y-2].color != colour)) {
            Move* move = &(Move) {x, y, x+1, y-2};
            calculate_move_score(move);
            moves[index] = *move;
            index++;
        }

        // Move right and down (white inverse)
        if (x > 0 && y < 6 && (board[x-1][y+2].piece == EMPTY || board[x-1][y+2].color != colour)) {
            Move* move = &(Move) {x, y, x-1, y+2};
            calculate_move_score(move);
            moves[index] = *move;
            index++;
        }

        // Move left and down (white inverse)
        if (x > 0 && y > 1 && (board[x-1][y-2].piece == EMPTY || board[x-1][y-2].color != colour)) {
            Move* move = &(Move) {x, y, x-1, y-2};
            calculate_move_score(move);
            moves[index] = *move;
            index++;
        }

        // Move left and up (white inverse)
        if (x < 7 && y > 1 && (board[x+1][y-2].piece == EMPTY || board[x+1][y-2].color != colour)) {
            Move* move = &(Move) {x, y, x+1, y+2};
            calculate_move_score(move);
            moves[index] = *move;
            index++;
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
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            } else if (board[x][y+i].color != colour) {
                Move* move = &(Move) {x, y, x, y+i};
                calculate_move_score(move);
                moves[index] = *move;
                index++;
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
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            } else if (board[x][y-i].color != colour) {
                Move* move = &(Move) {x, y, x, y-i};
                calculate_move_score(move);
                moves[index] = *move;
                index++;
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
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            } else if (board[x-i][y].color != colour) {
                Move* move = &(Move) {x, y, x-i, y};
                calculate_move_score(move);
                moves[index] = *move;
                index++;
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
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            } else if (board[x+i][y].color != colour) {
                Move* move = &(Move) {x, y, x+i, y};
                calculate_move_score(move);
                moves[index] = *move;
                index++;
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
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            } else if (board[x+i][y+i].color != colour) {
                Move* move = &(Move) {x, y, x+i, y+i};
                calculate_move_score(move);
                moves[index] = *move;
                index++;
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
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            } else if (board[x-i][y+i].color != colour) {
                Move* move = &(Move) {x, y, x-i, y+i};
                calculate_move_score(move);
                moves[index] = *move;
                index++;
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
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            } else if (board[x-i][y-i].color != colour) {
                Move* move = &(Move) {x, y, x-i, y-i};
                calculate_move_score(move);
                moves[index] = *move;
                index++;
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
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            } else if (board[x+i][y-i].color != colour) {
                Move* move = &(Move) {x, y, x+i, y-i};
                calculate_move_score(move);
                moves[index] = *move;
                index++;
                break;
            } else {
                break;
            }
        }
    } else if(piece == KING) {
        // Move up
        if (x > 1 && (board[x-1][y].piece == EMPTY || board[x-1][y].color != colour)) {
            Move potential_move = (Move) {x, y, x-1, y};
            if(!is_king_in_check_after_move(potential_move, colour)) {
                Move* move = &potential_move;
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            }
        }

        // Move down
        if (x < 7 && (board[x+1][y].piece == EMPTY || board[x+1][y].color != colour)) {
            Move potential_move = (Move) {x, y, x+1, y};

            if(!is_king_in_check_after_move(potential_move, colour)) {
                Move* move = &potential_move;
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            }
        }

        // Move left
        if (y > 1 && (board[x][y-1].piece == EMPTY || board[x][y-1].color != colour)) {
            Move potential_move = (Move) {x, y, x, y-1};

            if(!is_king_in_check_after_move(potential_move, colour)) {
                Move* move = &potential_move;
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            }
        }

        // Move right
        if (y < 7 && (board[x][y+1].piece == EMPTY || board[x][y+1].color != colour)) {
            Move potential_move = (Move) {x, y, x, y+1};

            if(!is_king_in_check_after_move(potential_move, colour)) {
                Move* move = &potential_move;
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            }
        }

        // Move up and right
        if (x > 1 && y < 7 && (board[x-1][y+1].piece == EMPTY || board[x-1][y+1].color != colour)) {
            Move potential_move = (Move) {x, y, x-1, y+1};

            if(!is_king_in_check_after_move(potential_move, colour)) {
                Move* move = &potential_move;
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            }
        }

        // Move up and left
        if (x > 1 && y > 1 && (board[x-1][y-1].piece == EMPTY || board[x-1][y-1].color != colour)) {
            Move potential_move = (Move) {x, y, x-1, y-1};

            if(!is_king_in_check_after_move(potential_move, colour)) {
                Move* move = &potential_move;
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            }
        }

        // Move down and right
        if (x < 7 && y < 7 && (board[x+1][y+1].piece == EMPTY || board[x+1][y+1].color != colour)) {
            Move potential_move = (Move) {x, y, x+1, y+1};

            if(!is_king_in_check_after_move(potential_move, colour)) {
                Move* move = &potential_move;
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            }
        }

        // Move down and left
        if (y > 1 && x < 7 && (board[x+1][y-1].piece == EMPTY || board[x+1][y-1].color != colour)) {
            Move potential_move = (Move) {x, y, x+1, y-1};
            if(!is_king_in_check_after_move(potential_move, colour)) {
                Move* move = &potential_move;
                calculate_move_score(move);
                moves[index] = *move;
                index++;
            }
        }
    }

    return moves;
}

void merge_arrays_for_pieces(Move* the_moves, Move* some_moves, int* total_moves_added) {
    if(some_moves == NULL) {
        printf("some_moves in NULL, should this happen \n");
        return;
    }
    // TODO: HERE i=14
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
 * @return Array of moves, limited to 129. Not fragmented, one null value will be the end of the array.
 */
Move* generate_moves_for_one_color(OneColoursPieces* aColoursPieces, bool include_king) {
    Move *moves = (Move*) calloc(MAX_POTENTIAL_TOTAL_MOVES_PER_COLOR, sizeof(Move));

    int total_moves_added = 0;

    if(aColoursPieces->King == allPieces.blackPieces->King) {
        printf("Generating moves for black \n");
    } else {
        printf("Generating moves for white \n");
    }

    printf("\nFirst move is: %d, %d, %d, %d\n\n", moves[0].from_x, moves[0].from_y, moves[0].to_x, moves[0].to_y);

    if(include_king) {
        Square* my_king = aColoursPieces->King;
        if(my_king == NULL) {
            printf("King is NULL\n");
            return NULL;
        }
        Move* kings_moves = generate_legal_moves_for_cell(my_king);
        merge_arrays_for_pieces(moves, kings_moves, &total_moves_added);
        printf("Kings first move: %d, %d, %d, %d\n", kings_moves[0].from_x, kings_moves[0].from_y, kings_moves[0].to_x, kings_moves[0].to_y);

        free(kings_moves);
    }

    if(include_king && is_king_in_check(aColoursPieces->King->color)) {
        printf("King is in check, only generating moves for king\n");
        return moves;
    }

    for(int i = 0; i < 8; i++) {
        Square* pawn = aColoursPieces->Pawns[i];
        if(pawn == NULL) {
            continue;
        }
        printf("Generating legal moves for pawn\n");

        Move* some_moves = generate_legal_moves_for_cell(pawn);
        if(some_moves == NULL) {
            printf("Failed to generate legal moves for pawn\n");
            free(some_moves);
            return NULL;
        }

        // Print first move
        printf("First pawn move: %d, %d, %d, %d\n", some_moves[0].from_x, some_moves[0].from_y, some_moves[0].to_x, some_moves[0].to_y);
        merge_arrays_for_pieces(moves, some_moves, &total_moves_added);

        free(some_moves);
    }

    printf("past pawns\n");
    for(int i = 0; i < 2; i++) {
        Square* rook = aColoursPieces->Rooks[i];
        if(rook == NULL) {
            continue;
        }

        Move* some_moves = generate_legal_moves_for_cell(rook);
        merge_arrays_for_pieces(moves, some_moves, &total_moves_added);

        free(some_moves);
    }

    for(int i = 0; i < 2; i++) {
        Square* knight = aColoursPieces->Knights[i];
        if(knight == NULL) {
            continue;
        }

        Move* some_moves = generate_legal_moves_for_cell(knight);
        merge_arrays_for_pieces(moves, some_moves, &total_moves_added);

        free(some_moves);
    }

    for(int i = 0; i < 2; i++) {
        Square* bishop = aColoursPieces->Bishops[i];
        if(bishop == NULL) {
            continue;
        }

        Move* some_moves = generate_legal_moves_for_cell(bishop);
        merge_arrays_for_pieces(moves, some_moves, &total_moves_added);

        free(some_moves);
    }

    Square* queen = aColoursPieces->Queen;
    if(queen != NULL) {
        Move* some_moves = generate_legal_moves_for_cell(queen);
        merge_arrays_for_pieces(moves, some_moves, &total_moves_added);
        free(some_moves);
    }

    if(aColoursPieces->King == allPieces.blackPieces->King) {
        printf("Finished generating moves for black \n");
    } else {
        printf("Finished moves for white \n");
    }
    return moves;
}

//Move* generate_all_legal_moves() {
//    Move* moves = (Move*) malloc(MAX_POTENTIAL_TOTAL_MOVES_PER_COLOR * sizeof(Move));
//    int total_moves_added = 0;
//
//    Move* whites_moves = generate_moves_for_one_color(allPieces.whitePieces, true);
//    Move* blacks_moves = generate_moves_for_one_color(allPieces.blackPieces, true);
//
//    for(int j = 0; j < MAX_POTENTIAL_TOTAL_MOVES_PER_COLOR; j++) {
//        Move* a_move = &whites_moves[j];
//
//        if (a_move->from_x == 0 && a_move->from_y == 0 &&
//            a_move->to_x == 0 && a_move->to_y == 0) {
//            break; // Skip empty moves
//        }
//        moves[total_moves_added] = *a_move;
//        total_moves_added++;
//    }
//
//    free(whites_moves);
//
//    for(int j = 0; j < MAX_POTENTIAL_TOTAL_MOVES_PER_COLOR; j++) {
//        Move* a_move = &blacks_moves[j];
//
//        if (a_move->from_x == 0 && a_move->from_y == 0 &&
//                a_move->to_x == 0 && a_move->to_y == 0) {
//            break; // Skip empty moves
//        }
//        moves[total_moves_added] = *a_move;
//        total_moves_added++;
//    }
//
//    free(blacks_moves);
//
//    return moves;
//}

int are_coordinates_within1(int x1, int y1, int x2, int y2) {
    return (abs(x1 - x2) <= 1) && (abs(y1 - y2) <= 1);
}


bool is_king_in_check(Colour colour) { //TODO: Error in this func
    printf("Started checking if kings in check\n");
    Move* all_legal_moves;
    Square* king;
    Square* opponentsKing;

    if(colour == WHITE) {
        printf("Getting all legal moves\n");
        all_legal_moves = generate_moves_for_one_color(allPieces.blackPieces, false);
        printf("Got all legal moves\n");
        king = allPieces.whitePieces->King;
        opponentsKing = allPieces.blackPieces->King;
    }else {
        all_legal_moves = generate_moves_for_one_color(allPieces.whitePieces, false);
        king = allPieces.blackPieces->King;
        opponentsKing = allPieces.whitePieces->King;
    }

    printf("Generated moves and assigned\n");

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

    printf("Checked if other king is putting king in check\n");
    // TODO: If we instead moved this check inside `generate_all_legal_moves` we could avoid this loop
    for (int i = 0; i < MAX_POTENTIAL_TOTAL_MOVES_PER_COLOR; i++) {
        Move* move = &all_legal_moves[i];
        if(move == NULL) {
            free(all_legal_moves);
            return false;
        }
        if (move->to_x == king->x_coord && move->to_y == king->y_coord) {
            free(all_legal_moves);
            return true;
        }
    }
    printf("Checked if any other pieces are putting king in check\n");
    free(all_legal_moves);

    printf("Finished checking if kings in check\n");
    return false;
}