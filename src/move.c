#include "move.h"
#include "move_picker.h"
#include "board.h"
#include <printf.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

const int MAX_POTENTIAL_MOVES_FOR_ONE_PIECE = 27;
const int MAX_POTENTIAL_TOTAL_MOVES_PER_COLOR = 129;

void update_piece_pointer(Square* from, Square* to, Colour colour) {
    OneColoursPieces *pieces;
    if(colour == WHITE) {
        pieces = allPieces.whitePieces;
    } else {
        pieces = allPieces.blackPieces;
    }

    int x_coord = from->x_coord;
    int y_coord = from->y_coord;

    for(int i = 0; i < 8; i++) {
        // TODO: We should really extract the pawn once here, but I cant figure out the pointers
        if(pieces->Pawns[i] != NULL && pieces->Pawns[i]->x_coord == x_coord && pieces->Pawns[i]->y_coord == y_coord) {
            pieces->Pawns[i] = to;
        }
    }

    for(int i = 0; i < 2; i++) {
        if(pieces->Rooks[i] != NULL && pieces->Rooks[i]->x_coord == x_coord && pieces->Rooks[i]->y_coord == y_coord) {
            pieces->Rooks[i] = to;
        }
    }

    for(int i = 0; i < 2; i++) {
        if(pieces->Knights[i] != NULL && pieces->Knights[i]->x_coord == x_coord && pieces->Knights[i]->y_coord == y_coord) {
            pieces->Knights[i] = to;
        }
    }

    for(int i = 0; i < 2; i++) {
        if(pieces->Bishops[i] != NULL && pieces->Bishops[i]->x_coord == x_coord && pieces->Bishops[i]->y_coord == y_coord) {
            pieces->Bishops[i] = to;
        }
    }

    if(pieces->Queen != NULL && pieces->Queen->x_coord == x_coord && pieces->Queen->y_coord == y_coord) {
        pieces->Queen = to;
    }

    if(pieces->King != NULL && pieces->King->x_coord == x_coord && pieces->King->y_coord == y_coord) {
        pieces->King = to;
    }
}

void execute_move(Move move, bool commit) {
    Square *from = &board[move.from_x][move.from_y];
    Square *to = &board[move.to_x][move.to_y];

    to->piece = from->piece;
    to->color = from->color;
    from->piece = EMPTY;
    from->color = NONE;

    if (commit) {
        update_piece_pointer(from, to, to->color);
    }

    int hi = 2;
}

bool is_king_in_check_after_move(Move move, Colour colour) {
    execute_move(move, false);
    bool is_check = is_king_in_check(colour);

    // Undo move
    Square* from = &board[move.to_x][move.to_y];
    Square* to = &board[move.from_x][move.from_y];

    to->piece = from->piece;
    to->color = from->color;
    from->piece = EMPTY;
    from->color = NONE;

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
        return NULL;
    }

    if (piece == PAWN) {
        // TODO: Implement en passant
        if (colour == WHITE) {
            // Move forward
            if(x > 1 && board[x-1][y].piece == EMPTY) {
                int move_score = calculate_move_score((Move *) &board[x - 1][y]);
                moves[index] = (Move) {x, y, x-1, y, move_score};

                index++;
            }

            // Move forward two
            if(x == 6 && board[x-2][y].piece == EMPTY) {
                int move_score = calculate_move_score((Move *) &board[x - 2][y]);
                moves[index] = (Move) {x, y, x-2, y};
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
                int move_score = calculate_move_score((Move *) &board[x - 1][y - 1]);
                moves[index] = (Move) {x, y, x-1, -+1};
                index++;
            }

            // Take to right
            if (x > 1 && y < 7 && board[x-1][y+1].color == BLACK) {
                int move_score = calculate_move_score((Move *) &board[x - 1][y + 1]);
                moves[index] = (Move) {x, y, x-1, y+1};
                index++;
            }

            // Promote TODO: Implement promotion
            if(x == 1 && (board[0][y].piece == EMPTY || board[0][y-1].piece == EMPTY || board[0][y+1].piece == EMPTY)) {
                int move_score = 9999999;
                moves[index] = (Move) {x, y, 99999, 99999};
                index++;
            }
        } else if(colour == BLACK) {
            // Move forward
            if(x < 7 && board[x+1][y].piece == EMPTY) {
                int move_score = calculate_move_score((Move *) &board[x + 1][y]);
                moves[index] = (Move) {x, y, x+1, y};
                index++;
            }

            // Move forward two
            if(x == 1 && board[x+2][y].piece == EMPTY) {
                int move_score = calculate_move_score((Move *) &board[x + 2][y]);
                moves[index] = (Move) {x, y, x+2, y};
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
                int move_score = calculate_move_score((Move *) &board[x + 1][y - 1]);
                moves[index] = (Move) {x, y, x+1, y-1};
                index++;
            }

            // Take to left
            if (x < 7 && y < 7 && board[x+1][y+1].color == WHITE) {
                int move_score = calculate_move_score((Move *) &board[x + 1][y + 1]);
                moves[index] = (Move) {x, y, x+1, y+1};
                index++;
            }

            // Promote TODO: Implement promotion
            if(x == 6 && (board[7][y].piece == EMPTY || board[7][y-1].piece == EMPTY || board[7][y+1].piece == EMPTY)){
                int move_score = 9999999;
                moves[index] = (Move) {x, y, 99999, 99999};
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
                int move_score = calculate_move_score((Move *) &board[x + i][y + i]);
                moves[index] = (Move) {x, y, x+i, y+i, move_score};
                index++;
            } else if (board[x+i][y+i].color != colour) {
                int move_score = calculate_move_score((Move *) &board[x + i][y + i]);
                moves[index] = (Move) {x, y, x+i, y+i, move_score};
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
                int move_score = calculate_move_score((Move *) &board[x - i][y + i]);
                moves[index] = (Move) {x, y, x-i, y+i, move_score};
                index++;
            } else if (board[x-i][y+i].color != colour) {
                int move_score = calculate_move_score((Move *) &board[x - i][y + i]);
                moves[index] = (Move) {x, y, x-i, y+i, move_score};
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
                int move_score = calculate_move_score((Move *) &board[x - i][y - i]);
                moves[index] = (Move) {x, y, x-i, y-i, move_score};
                index++;
            } else if (board[x-i][y-i].color != colour) {
                int move_score = calculate_move_score((Move *) &board[x - i][y - i]);
                moves[index] = (Move) {x, y, x-i, y-i, move_score};
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
                int move_score = calculate_move_score((Move *) &board[x + i][y - i]);
                moves[i-1] = (Move) {x, y, x+i, y-i, move_score};
            } else if (board[x+i][y-i].color != colour) {
                int move_score = calculate_move_score((Move *) &board[x + i][y - i]);
                moves[i-1] = (Move) {x, y, x+i, y-i, move_score};
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
                int move_score = calculate_move_score((Move *) &board[x][y + i]);
                moves[index] = (Move) {x, y, x, y+i, move_score};
                index++;
            } else if (board[x][y+i].color != colour) {
                int move_score = calculate_move_score((Move *) &board[x][y + i]);
                moves[index] = (Move) {x, y, x, y+i, move_score};
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
                int move_score = calculate_move_score((Move *) &board[x][y - i]);
                moves[index] = (Move) {x, y, x, y-i, move_score};
                index++;
            } else if (board[x][y-i].color != colour) {
                int move_score = calculate_move_score((Move *) &board[x][y - i]);
                moves[index] = (Move) {x, y, x, y-i, move_score};
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
                int move_score = calculate_move_score((Move *) &board[x - i][y]);
                moves[index] = (Move) {x, y, x-i, y, move_score};
                index++;
            } else if (board[x-i][y].color != colour) {
                int move_score = calculate_move_score((Move *) &board[x - i][y]);
                moves[index] = (Move) {x, y, x-i, y, move_score};
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
                int move_score = calculate_move_score((Move *) &board[x + i][y]);
                moves[index] = (Move) {x, y, x+i, y, move_score};
                index++;
            } else if (board[x+i][y].color != colour) {
                int move_score = calculate_move_score((Move *) &board[x + i][y]);
                moves[index] = (Move) {x, y, x+i, y, move_score};
                index++;
                break;
            } else {
                break;
            }
        }
    } else if(piece == KNIGHT) {
        // Move up and right (white inverse)
        if (x < 6 && y < 7 && (board[x+2][y+1].piece == EMPTY || board[x+2][y+1].color != colour)) {
            int move_score = calculate_move_score((Move *) &board[x + 2][y + 1]);
            moves[index] = (Move) {x, y, x+2, y+1, move_score};
            index++;
        }

        // Move up and left (white inverse)
        if (x < 6 && y > 0 && (board[x+2][y-1].piece == EMPTY || board[x+2][y-1].color != colour)) {
            int move_score = calculate_move_score((Move *) &board[x + 2][y - 1]);
            moves[index] = (Move) {x, y, x+2, y-1, move_score};
            index++;
        }

        // Move down and right (white inverse)
        if (x > 1 && y < 7 && (board[x-2][y+1].piece == EMPTY || board[x-2][y+1].color != colour)) {
            int move_score = calculate_move_score((Move *) &board[x - 2][y + 1]);
            moves[index] = (Move) {x, y, x-2, y+1, move_score};
            index++;
        }

        // Move down and left (white inverse)
        if (x > 1 && y > 0 && (board[x-2][y-1].piece == EMPTY || board[x-2][y-1].color != colour)) {
            int move_score = calculate_move_score((Move *) &board[x - 2][y - 1]);
            moves[index] = (Move) {x, y, x-2, y-1, move_score};
            index++;
        }

        // Move right and up (white inverse)
        if (x < 7 && y < 6 && (board[x+1][y+2].piece == EMPTY || board[x+1][y+2].color != colour)) {
            int move_score = calculate_move_score((Move *) &board[x + 1][y + 2]);
            moves[index] = (Move) {x, y, x+1, y+2, move_score};
            index++;
        }

        // Move right and up (white inverse)
        if (x < 7 && y > 6 && (board[x+1][y-2].piece == EMPTY || board[x+1][y-2].color != colour)) {
            int move_score = calculate_move_score((Move *) &board[x + 1][y - 2]);
            moves[index] = (Move) {x, y, x+1, y-2, move_score};
            index++;
        }

        // Move right and down (white inverse)
        if (x > 0 && y < 6 && (board[x-1][y+2].piece == EMPTY || board[x-1][y+2].color != colour)) {
            int move_score = calculate_move_score((Move *) &board[x - 1][y + 2]);
            moves[index] = (Move) {x, y, x-1, y+2, move_score};
            index++;
        }

        // Move left and down (white inverse)
        if (x > 0 && y > 1 && (board[x-1][y-2].piece == EMPTY || board[x-1][y-2].color != colour)) {
            int move_score = calculate_move_score((Move *) &board[x - 1][y - 2]);
            moves[index] = (Move) {x, y, x-1, y-2, move_score};
            index++;
        }

        // Move left and up (white inverse)
        if (x < 7 && y > 1 && (board[x+1][y-2].piece == EMPTY || board[x+1][y-2].color != colour)) {
            int move_score = calculate_move_score((Move *) &board[x + 1][y + 2]);
            moves[index] = (Move) {x, y, x+1, y+2, move_score};
            index++;
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
                int move_score = calculate_move_score((Move *) &board[x][y + i]);
                moves[index] = (Move) {x, y, x, y+i, move_score};
                index++;
            } else if (board[x][y+i].color != colour) {
                int move_score = calculate_move_score((Move *) &board[x][y + i]);
                moves[index] = (Move) {x, y, x, y+i, move_score};
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
                int move_score = calculate_move_score((Move *) &board[x][y - i]);
                moves[index] = (Move) {x, y, x, y-i, move_score};
                index++;
            } else if (board[x][y-i].color != colour) {
                int move_score = calculate_move_score((Move *) &board[x][y - i]);
                moves[index] = (Move) {x, y, x, y-i, move_score};
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
                int move_score = calculate_move_score((Move *) &board[x - i][y]);
                moves[index] = (Move) {x, y, x-i, y, move_score};
                index++;
            } else if (board[x-i][y].color != colour) {
                int move_score = calculate_move_score((Move *) &board[x - i][y]);
                moves[index] = (Move) {x, y, x-i, y, move_score};
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
                int move_score = calculate_move_score((Move *) &board[x + i][y]);
                moves[index] = (Move) {x, y, x+i, y, move_score};
                index++;
            } else if (board[x+i][y].color != colour) {
                int move_score = calculate_move_score((Move *) &board[x + i][y]);
                moves[index] = (Move) {x, y, x+i, y, move_score};
                index++;
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
                int move_score = calculate_move_score((Move *) &board[x + i][y + i]);
                moves[index] = (Move) {x, y, x+i, y+i, move_score};
                index++;
            } else if (board[x+i][y+i].color != colour) {
                int move_score = calculate_move_score((Move *) &board[x + i][y + i]);
                moves[index] = (Move) {x, y, x+i, y+i, move_score};
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
                int move_score = calculate_move_score((Move *) &board[x - i][y + i]);
                moves[index] = (Move) {x, y, x-i, y+i, move_score};
                index++;
            } else if (board[x-i][y+i].color != colour) {
                int move_score = calculate_move_score((Move *) &board[x - i][y + i]);
                moves[index] = (Move) {x, y, x-i, y+i, move_score};
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
                int move_score = calculate_move_score((Move *) &board[x - i][y - i]);
                moves[index] = (Move) {x, y, x-i, y-i, move_score};
                index++;
            } else if (board[x-i][y-i].color != colour) {
                int move_score = calculate_move_score((Move *) &board[x - i][y - i]);
                moves[index] = (Move) {x, y, x-i, y-i, move_score};
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
                int move_score = calculate_move_score((Move *) &board[x + i][y - i]);
                moves[i-1] = (Move) {x, y, x+i, y-i, move_score};
            } else if (board[x+i][y-i].color != colour) {
                int move_score = calculate_move_score((Move *) &board[x + i][y - i]);
                moves[i-1] = (Move) {x, y, x+i, y-i, move_score};
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
                int move_score = calculate_move_score((Move *) &board[x][y + i]);
                moves[index] = (Move) {x, y, x, y+i, move_score};
                index++;
            } else if (board[x][y+i].color != colour) {
                int move_score = calculate_move_score((Move *) &board[x][y + i]);
                moves[index] = (Move) {x, y, x, y+i, move_score};
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
                int move_score = calculate_move_score((Move *) &board[x][y - i]);
                moves[index] = (Move) {x, y, x, y-i, move_score};
                index++;
            } else if (board[x][y-i].color != colour) {
                int move_score = calculate_move_score((Move *) &board[x][y - i]);
                moves[index] = (Move) {x, y, x, y-i, move_score};
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
                int move_score = calculate_move_score((Move *) &board[x - i][y]);
                moves[index] = (Move) {x, y, x-i, y, move_score};
                index++;
            } else if (board[x-i][y].color != colour) {
                int move_score = calculate_move_score((Move *) &board[x - i][y]);
                moves[index] = (Move) {x, y, x-i, y, move_score};
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
                int move_score = calculate_move_score((Move *) &board[x + i][y]);
                moves[index] = (Move) {x, y, x+i, y, move_score};
                index++;
            } else if (board[x+i][y].color != colour) {
                int move_score = calculate_move_score((Move *) &board[x + i][y]);
                moves[index] = (Move) {x, y, x+i, y, move_score};
                index++;
                break;
            } else {
                break;
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
    Move *moves = (Move*) malloc(MAX_POTENTIAL_TOTAL_MOVES_PER_COLOR * sizeof(Move));
    int total_moves_added = 0;
    for(int i = 0; i < 8; i++) {
        Square* pawn = aColoursPieces->Pawns[i];
        if(pawn->piece == EMPTY && pawn->color == NONE) {
            continue;
        }

        Move* some_moves = generate_legal_moves_for_cell(pawn);
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

    Move* whites_moves = generate_moves_for_one_color(allPieces.whitePieces, true);
    Move* blacks_moves = generate_moves_for_one_color(allPieces.blackPieces, true);

    for(int j = 0; j < MAX_POTENTIAL_TOTAL_MOVES_PER_COLOR; j++) {
        Move* a_move = &whites_moves[j];

        if (a_move->from_x == 0 && a_move->from_y == 0 &&
            a_move->to_x == 0 && a_move->to_y == 0) {
            break; // Skip empty moves
        }
        moves[total_moves_added] = *a_move;
        total_moves_added++;
    }

    free(whites_moves);

    for(int j = 0; j < MAX_POTENTIAL_TOTAL_MOVES_PER_COLOR; j++) {
        Move* a_move = &blacks_moves[j];

        if (a_move->from_x == 0 && a_move->from_y == 0 &&
                a_move->to_x == 0 && a_move->to_y == 0) {
            break; // Skip empty moves
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