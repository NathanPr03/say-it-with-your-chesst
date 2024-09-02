#include "move.h"
#include "board.h"
#include <printf.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

const int MAX_POTENTIAL_MOVES_FOR_ONE_PIECE = 27;
const int MAX_POTENTIAL_TOTAL_MOVES_PER_COLOR = 129;

Square* find_piece(int x_coord, int y_coord, Colour colour) {
    OneColoursPieces *pieces;
    if(colour == WHITE) {
        pieces = allPieces.whitePieces;
    } else {
        pieces = allPieces.blackPieces;
    }

    for(int i = 0; i < 8; i++) {
        Square* pawn = pieces->Pawns[i];
        if(pawn != NULL && pawn->x_coord == x_coord && pawn->y_coord == y_coord) {
            return pawn;
        }
    }

    for(int i = 0; i < 2; i++) {
        Square* rook = pieces->Rooks[i];
        if(rook != NULL && rook->x_coord == x_coord && rook->y_coord == y_coord) {
            return rook;
        }
    }

    for(int i = 0; i < 2; i++) {
        Square* knight = pieces->Knights[i];
        if(knight != NULL && knight->x_coord == x_coord && knight->y_coord == y_coord) {
            return knight;
        }
    }

    for(int i = 0; i < 2; i++) {
        Square* bishop = pieces->Bishops[i];
        if(bishop != NULL && bishop->x_coord == x_coord && bishop->y_coord == y_coord) {
            return bishop;
        }
    }

    Square* queen = pieces->Queen;
    if(queen != NULL && queen->x_coord == x_coord && queen->y_coord == y_coord) {
        return queen;
    }

    Square* king = pieces->King;
    if(king != NULL && king->x_coord == x_coord && king->y_coord == y_coord) {
        return king;
    }
    return NULL;
}

void execute_move(Move move, bool commit) {
    Square* from = &board[move.from_x][move.from_y];
    Square* to = &board[move.to_x][move.to_y];

    if(to->piece != EMPTY && commit) {
        Square *piece = find_piece(move.to_x, move.to_y, to->color);
        if(piece == NULL) {
            printf("Failed to find piece to remove\n");
            return;
        }

        piece = NULL;
    }

    to->piece = from->piece;
    to->color = from->color;
    from->piece = EMPTY;
    from->color = NONE;
}

bool ensure_king_is_not_in_check_after_move(Move move, Colour colour) {
    execute_move(move, false);
    bool is_check = is_king_in_check(colour);

    // Undo move
    Square* from = &board[move.to_x][move.to_y];
    Square* to = &board[move.from_x][move.from_y];

    from->piece = to->piece;
    from->color = to->color;
    to->piece = EMPTY;
    to->color = NONE;

    return !is_check;
}

/**
 * @return Move array of size 27, either populated or all 0s. Populated entries are not fragmented.
 */
Move* generate_legal_moves_for_cell(Square *square) {
    Move* moves = (Move*) malloc(MAX_POTENTIAL_MOVES_FOR_ONE_PIECE * sizeof(Move));

    Piece piece = square->piece;
    Colour colour = square->color;
    int x = square->x_coord;
    int y = square->y_coord;

    if (piece == EMPTY) {
        return NULL;
    }

    if (piece == PAWN) {
        //TODO: How do we prevent out of bounds?
        if (colour == WHITE) {
            // Move forward
            if(x > 1 && board[x-1][y].piece == EMPTY) {
                moves[0] = (Move) {x, y, x-1, y};
            }

            // Take to left
            if (x > 1 && y > 1 && board[x-1][y-1].color == BLACK) {
                moves[1] = (Move) {x, y, x-1, y+1};
            }

            // Take to right
            if (x > 1 && y < 7 && board[x-1][y+1].color == BLACK) {
                moves[2] = (Move) {x, y, x-1, y+1};
            }

            // Promote TODO: Implement promotion
            if(x == 6 && (board[0][y].piece == EMPTY || board[0][y-1].piece == EMPTY) || board[0][y+1].piece == EMPTY){
                moves[3] = (Move) {x, y, 99999, 99999};
            }
        } else if(colour == BLACK) {
            // Move forward
            if(x < 7 && board[x+1][y].piece == EMPTY) {
                moves[0] = (Move) {x, y, x+1, y};
            }

            // Take to right
            if (x < 7 && y > 1 && board[x+1][y-1].color == WHITE) {
                moves[1] = (Move) {x, y, x+1, y-1};
            }

            // Take to left
            if (x < 7 && y < 7 && board[x+1][y+1].color == WHITE) {
                moves[2] = (Move) {x, y, x+1, y+1};
            }

            // Promote TODO: Implement promotion
            if(x == 1 && (board[0][y].piece == EMPTY || board[7][y+1].piece == EMPTY) || board[7][y+1].piece == EMPTY){
                moves[3] = (Move) {x, y, 99999, 99999};
            }
        }
    } else if(piece == BISHOP) {
        // Move right diagonally
        for (int i = 1; i < 8; i++) {
            if(x+i > 7 || y+i > 7) {
                break;
            }
            if (board[x+i][y+i].piece == EMPTY) {
                moves[i-1] = (Move) {x, y, x+i, y+i};
            } else if (board[x+i][y+i].color != colour) {
                moves[i-1] = (Move) {x, y, x+i, y+i};
                break;
            } else {
                break;
            }
        }

        // Move left diagonally
        for (int i = 1; i < 8; i++) {
            if(x-i < 0 || y+i > 7) {
                break;
            }

            if (board[x-i][y+i].piece == EMPTY) {
                moves[i-1] = (Move) {x, y, x-i, y+i};
            } else if (board[x-i][y+i].color != colour) {
                moves[i-1] = (Move) {x, y, x-i, y+i};
                break;
            } else {
                break;
            }
        }

        // Move back left diagonally
        for (int i = 1; i < 8; i++) {
            if(x-i < 0 || y-i < 0) {
                break;
            }

            if (board[x-i][y-i].piece == EMPTY) {
                moves[i-1] = (Move) {x, y, x-i, y-i};
            } else if (board[x-i][y-i].color != colour) {
                moves[i-1] = (Move) {x, y, x-i, y-i};
                break;
            } else {
                break;
            }
        }

        // Move back right diagonally
        for (int i = 1; i < 8; i++) {
            if(x+i > 7 || y-i < 0) {
                break;
            }

            if (board[x+i][y-i].piece == EMPTY) {
                moves[i-1] = (Move) {x, y, x+i, y-i};
            } else if (board[x+i][y-i].color != colour) {
                moves[i-1] = (Move) {x, y, x+i, y-i};
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
                moves[i-1] = (Move) {x, y, x, y+i};
            } else if (board[x][y+i].color != colour) {
                moves[i-1] = (Move) {x, y, x, y+i};
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
                moves[i-1] = (Move) {x, y, x, y-i};
            } else if (board[x][y-i].color != colour) {
                moves[i-1] = (Move) {x, y, x, y-i};
                break;
            } else {
                break;
            }
        }

        // Move up (down for black)
        for (int i = 1; i < 8; i++) {
            if(x-i < 0) {
                break;
            }

            if (board[x-i][y].piece == EMPTY) {
                moves[i-1] = (Move) {x, y, x-i, y};
            } else if (board[x-i][y].color != colour) {
                moves[i-1] = (Move) {x, y, x-i, y};
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
                moves[i-1] = (Move) {x, y, x+i, y};
            } else if (board[x+i][y].color != colour) {
                moves[i-1] = (Move) {x, y, x+i, y};
                break;
            } else {
                break;
            }
        }
    } else if(piece == KNIGHT) {
        // Move up and right (white inverse)
        if (x < 6 && y < 7 && board[x+2][y+1].piece == EMPTY || board[x+2][y+1].color != colour) {
            moves[0] = (Move) {x, y, x+2, y+1};
        }

        // Move up and left (white inverse)
        if (x < 6 && y > 0 && board[x+2][y-1].piece == EMPTY || board[x+2][y-1].color != colour) {
            moves[1] = (Move) {x, y, x+2, y-1};
        }

        // Move down and right (white inverse)
        if (x > 1 && y < 7 && board[x-2][y+1].piece == EMPTY || board[x-2][y+1].color != colour) {
            moves[2] = (Move) {x, y, x-2, y+1};
        }

        // Move down and left (white inverse)
        if (x > 1 && y > 0 && board[x-2][y-1].piece == EMPTY || board[x-2][y-1].color != colour) {
            moves[3] = (Move) {x, y, x-2, y-1};
        }

        // Move right and up (white inverse)
        if (x < 7 && y < 6 &&board[x+1][y+2].piece == EMPTY || board[x+1][y+2].color != colour) {
            moves[4] = (Move) {x, y, x+1, y+2};
        }

        // Move right and up (white inverse)
        if (x < 7 && y > 6 && board[x+1][y-2].piece == EMPTY || board[x+1][y-2].color != colour) {
            moves[5] = (Move) {x, y, x+1, y-2};
        }

        // Move right and down (white inverse)
        if (x > 0 && y < 6 && board[x-1][y+2].piece == EMPTY || board[x-1][y+2].color != colour) {
            moves[6] = (Move) {x, y, x-1, y+2};
        }

        // Move left and down (white inverse)
        if (x > 0 && y > 1 && board[x-1][y-2].piece == EMPTY || board[x-1][y-2].color != colour) {
            moves[7] = (Move) {x, y, x-1, y-2};
        }

        // Move left and up (white inverse)
        if (x < 7 && y > 1 && board[x+1][y-2].piece == EMPTY || board[x+1][y-2].color != colour) {
            moves[8] = (Move) {x, y, x+1, y+2};
        }
    } else if(piece == QUEEN) {
        // Copy paste of Bishop + Rook logic TODO: Extract
        // Rook below
        // Move right (left for black)
        for (int i = 1; i < 8; i++) {
            if(y+i > 7) {
                break;
            }

            if (board[x][y+i].piece == EMPTY) {
                moves[i-1] = (Move) {x, y, x, y+i};
            } else if (board[x][y+i].color != colour) {
                moves[i-1] = (Move) {x, y, x, y+i};
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
                moves[i-1] = (Move) {x, y, x, y-i};
            } else if (board[x][y-i].color != colour) {
                moves[i-1] = (Move) {x, y, x, y-i};
                break;
            } else {
                break;
            }
        }

        // Move up (down for black)
        for (int i = 1; i < 8; i++) {
            if(x-i < 0) {
                break;
            }

            if (board[x-i][y].piece == EMPTY) {
                moves[i-1] = (Move) {x, y, x-i, y};
            } else if (board[x-i][y].color != colour) {
                moves[i-1] = (Move) {x, y, x-i, y};
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
                moves[i-1] = (Move) {x, y, x+i, y};
            } else if (board[x+i][y].color != colour) {
                moves[i-1] = (Move) {x, y, x+i, y};
                break;
            } else {
                break;
            }
        }

        // Bishop below
        // Move right diagonally
        for (int i = 1; i < 8; i++) {
            if(x+i > 7 || y+i > 7) {
                break;
            }
            if (board[x+i][y+i].piece == EMPTY) {
                moves[i-1] = (Move) {x, y, x+i, y+i};
            } else if (board[x+i][y+i].color != colour) {
                moves[i-1] = (Move) {x, y, x+i, y+i};
                break;
            } else {
                break;
            }
        }

        // Move left diagonally
        for (int i = 1; i < 8; i++) {
            if(x-i < 0 || y+i > 7) {
                break;
            }

            if (board[x-i][y+i].piece == EMPTY) {
                moves[i-1] = (Move) {x, y, x-i, y+i};
            } else if (board[x-i][y+i].color != colour) {
                moves[i-1] = (Move) {x, y, x-i, y+i};
                break;
            } else {
                break;
            }
        }

        // Move back left diagonally
        for (int i = 1; i < 8; i++) {
            if(x-i < 0 || y-i < 0) {
                break;
            }

            if (board[x-i][y-i].piece == EMPTY) {
                moves[i-1] = (Move) {x, y, x-i, y-i};
            } else if (board[x-i][y-i].color != colour) {
                moves[i-1] = (Move) {x, y, x-i, y-i};
                break;
            } else {
                break;
            }
        }

        // Move back right diagonally
        for (int i = 1; i < 8; i++) {
            if(x+i > 7 || y-i < 0) {
                break;
            }

            if (board[x+i][y-i].piece == EMPTY) {
                moves[i-1] = (Move) {x, y, x+i, y-i};
            } else if (board[x+i][y-i].color != colour) {
                moves[i-1] = (Move) {x, y, x+i, y-i};
                break;
            } else {
                break;
            }
        }
    } else if(piece == KING) {
        // TODO: This will result in fragmented array of null moves. As well as rook, bishop, queen and knight
        // Move up
        if (x > 1 && (board[x-1][y].piece == EMPTY || board[x-1][y].color != colour)) {
            Move potential_move = (Move) {x, y, x-1, y};
            if(!ensure_king_is_not_in_check_after_move(potential_move, colour)) {
                moves[0] = potential_move;
            }
        }

        // Move down
        if (x < 7 && (board[x+1][y].piece == EMPTY || board[x+1][y].color != colour)) {
            Move potential_move = (Move) {x, y, x+1, y};

            if(!ensure_king_is_not_in_check_after_move(potential_move, colour)) {
                moves[1] = potential_move;
            }
        }

        // Move left
        if (y > 1 && (board[x][y-1].piece == EMPTY || board[x][y-1].color != colour)) {
            Move potential_move = (Move) {x, y, x, y-1};

            if(!ensure_king_is_not_in_check_after_move(potential_move, colour)) {
                moves[2] = potential_move;
            }
        }

        // Move right
        if (y < 7 && (board[x][y+1].piece == EMPTY || board[x][y+1].color != colour)) {
            Move potential_move = (Move) {x, y, x, y+1};

            if(!ensure_king_is_not_in_check_after_move(potential_move, colour)) {
                moves[3] = potential_move;
            }
        }

        // Move up and right
        if (x > 1 && y < 7 && (board[x-1][y+1].piece == EMPTY || board[x-1][y+1].color != colour)) {
            Move potential_move = (Move) {x, y, x-1, y+1};

            if(!ensure_king_is_not_in_check_after_move(potential_move, colour)) {
                moves[4] = potential_move;
            }
        }

        // Move up and left
        if (x > 1 && y > 1 && (board[x-1][y-1].piece == EMPTY || board[x-1][y-1].color != colour)) {
            Move potential_move = (Move) {x, y, x-1, y-1};

            if(!ensure_king_is_not_in_check_after_move(potential_move, colour)) {
                moves[5] = potential_move;
            }
        }

        // Move down and right
        if (x < 7 && y < 7 && (board[x+1][y+1].piece == EMPTY || board[x+1][y+1].color != colour)) {
            Move potential_move = (Move) {x, y, x+1, y+1};

            if(!ensure_king_is_not_in_check_after_move(potential_move, colour)) {
                moves[6] = potential_move;
            }
        }

        // Move down and left
        if (y > 1 && x < 7 && (board[x+1][y-1].piece == EMPTY || board[x+1][y-1].color != colour)) {
            Move potential_move = (Move) {x, y, x+1, y-1};
            if(!ensure_king_is_not_in_check_after_move(potential_move, colour)) {
                moves[6] = potential_move;
            }
        }
    }

    return moves;
}

void merge_arrays_for_pieces(Move* the_moves, Move* some_moves, int* total_moves_added) {
    for(int j = 0; j < MAX_POTENTIAL_MOVES_FOR_ONE_PIECE; j++) { //TODO: Should extract all this logic
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
    Move *moves = (Move*) malloc(MAX_POTENTIAL_TOTAL_MOVES_PER_COLOR * sizeof(Move));
    int total_moves_added = 0;
    for(int i = 0; i < 8; i++) {
        Square* pawn = aColoursPieces->Pawns[i];
        if(pawn == NULL) {
            continue;
        }

        Move* some_moves = generate_legal_moves_for_cell(pawn);
        merge_arrays_for_pieces(moves, some_moves, &total_moves_added);

        free(some_moves);
    }

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

    Move* some_moves = generate_legal_moves_for_cell(aColoursPieces->Queen);
    merge_arrays_for_pieces(moves, some_moves, &total_moves_added);

    free(some_moves);

    if(include_king) {
        Move* kings_moves = generate_legal_moves_for_cell(aColoursPieces->King);
        merge_arrays_for_pieces(moves, kings_moves, &total_moves_added);
        free(kings_moves);
    }

    return moves;
}

Move* generate_all_legal_moves() {
    Move* moves = (Move*) malloc(MAX_POTENTIAL_TOTAL_MOVES_PER_COLOR * sizeof(Move));
    int total_moves_added = 0;

    Move* whites_moves = generate_moves_for_one_color(allPieces.blackPieces, true);
    Move* blacks_moves = generate_moves_for_one_color(allPieces.whitePieces, true);

    for(int j = 0; j < MAX_POTENTIAL_TOTAL_MOVES_PER_COLOR; j++) {
        Move* a_move = &whites_moves[j];
        if(a_move == NULL) {
            continue;
        }
        moves[total_moves_added] = *a_move;
        total_moves_added++;
    }

    free(whites_moves);

    for(int j = 0; j < MAX_POTENTIAL_TOTAL_MOVES_PER_COLOR; j++) {
        Move* a_move = &blacks_moves[j];
        if(a_move == NULL) {
            continue;
        }
        moves[total_moves_added] = *a_move;
        total_moves_added++;
    }

    free(blacks_moves);

    return moves;
}

int are_coordinates_within1(int x1, int y1, int x2, int y2) {
    return (abs(x1 - x2) <= 1) && (abs(y1 - y2) <= 1);
}

bool is_king_in_check(Colour colour) {
    Move* all_legal_moves;
    Square* king;
    Square* opponentsKing;

    if(colour == WHITE) {
        all_legal_moves = generate_moves_for_one_color(allPieces.blackPieces, false);
        king = allPieces.whitePieces->King;
        opponentsKing = allPieces.blackPieces->King;
    }else {
        all_legal_moves = generate_moves_for_one_color(allPieces.whitePieces, false);
        king = allPieces.blackPieces->King;
        opponentsKing = allPieces.whitePieces->King;
    }

    if(all_legal_moves == NULL) {
        printf("Failed to generate all legal moves\n");
        free(all_legal_moves);
        return false;
    }

    int kings_x =king->x_coord;
    int kings_y = king->y_coord;

    // Is the other king putting our king in check
    if(are_coordinates_within1(kings_x, kings_y, opponentsKing->x_coord, opponentsKing->y_coord)){
        free(all_legal_moves);
        return true;
    }

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
    free(all_legal_moves);
    return false;
}