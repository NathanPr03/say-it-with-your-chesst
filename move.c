#include "move.h"
#include "board.h"
#include <printf.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

const int MAX_POTENTIAL_MOVES_FOR_ONE_PIECE = 27;
const int MAX_POTENTIAL_TOTAL_MOVES_PER_COLOR = 129;
const int MAX_POTENTIAL_TOTAL_MOVES_PER_COLOR_MINUS_KING = 121;

const int TOTAL_PIECES_PER_COLOR = 16;

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
            if(board[x+1][y].piece == EMPTY) {
                moves[0] = (Move) {x, y, x+1, y};
            }

            // Take to left
            if (board[x+1][y+1].color == BLACK) {
                moves[1] = (Move) {x, y, x+1, y+1};
            }

            // Take to right
            if (board[x-1][y-1].color == BLACK) {
                moves[2] = (Move) {x, y, x+1, y-1};
            }

            // Promote TODO: Implement promotion
            if(x == 6 && (board[0][y].piece == EMPTY || board[0][y-1].piece == EMPTY) || board[0][y+1].piece == EMPTY){
                moves[3] = (Move) {x, y, 99999, 99999};
            }
        } else if(colour == BLACK) {
            // Move forward
            if(board[x+1][y].piece == EMPTY) {
                moves[0] = (Move) {x, y, x-1, y};
            }

            // Take to left
            if (board[x-1][y-1].color == BLACK) {
                moves[1] = (Move) {x, y, x-1, y-1};
            }

            // Take to right
            if (board[x-1][y+1].color == BLACK) {
                moves[2] = (Move) {x, y, x-1, y+1};
            }

            // Promote TODO: Implement promotion
            if(x == 1 && (board[0][y].piece == EMPTY || board[0][y-1].piece == EMPTY) || board[0][y+1].piece == EMPTY){
                moves[3] = (Move) {x, y, 99999, 99999};
            }
        }
    } if(piece == BISHOP) {
        // Move right diagonally
        for (int i = 1; i < 8; i++) {
            if (board[x+i][y+i].piece == EMPTY) {
                moves[i] = (Move) {x, y, x+i, y+i};
            } else if (board[x+i][y+i].color != colour) {
                moves[i] = (Move) {x, y, x+i, y+i};
                break;
            } else {
                break;
            }
        }
        // Move left diagonally
        for (int i = 1; i < 8; i++) {
            if (board[x-i][y+i].piece == EMPTY) {
                moves[i] = (Move) {x, y, x-i, y+i};
            } else if (board[x-i][y+i].color != colour) {
                moves[i] = (Move) {x, y, x-i, y+i};
                break;
            } else {
                break;
            }
        }

        // Move back left diagonally
        for (int i = 1; i < 8; i++) {
            if (board[x-i][y-i].piece == EMPTY) {
                moves[i] = (Move) {x, y, x-i, y-i};
            } else if (board[x-i][y-i].color != colour) {
                moves[i] = (Move) {x, y, x-i, y-i};
                break;
            } else {
                break;
            }
        }

        // Move back right diagonally
        for (int i = 1; i < 8; i++) {
            if (board[x+i][y-i].piece == EMPTY) {
                moves[i] = (Move) {x, y, x+i, y-i};
            } else if (board[x+i][y-i].color != colour) {
                moves[i] = (Move) {x, y, x+i, y-i};
                break;
            } else {
                break;
            }
        }
    } if (piece == ROOK) {
        // Move right
        for (int i = 1; i < 8; i++) {
            if (board[x][y+i].piece == EMPTY) {
                moves[i] = (Move) {x, y, x, y+i};
            } else if (board[x][y+i].color != colour) {
                moves[i] = (Move) {x, y, x, y+i};
                break;
            } else {
                break;
            }
        }

        // Move left
        for (int i = 1; i < 8; i++) {
            if (board[x][y-i].piece == EMPTY) {
                moves[i] = (Move) {x, y, x, y-i};
            } else if (board[x][y-i].color != colour) {
                moves[i] = (Move) {x, y, x, y-i};
                break;
            } else {
                break;
            }
        }

        // Move up
        for (int i = 1; i < 8; i++) {
            if (board[x-i][y].piece == EMPTY) {
                moves[i] = (Move) {x, y, x-i, y};
            } else if (board[x-i][y].color != colour) {
                moves[i] = (Move) {x, y, x-i, y};
                break;
            } else {
                break;
            }
        }

        // Move down
        for (int i = 1; i < 8; i++) {
            if (board[x+i][y].piece == EMPTY) {
                moves[i] = (Move) {x, y, x+i, y};
            } else if (board[x+i][y].color != colour) {
                moves[i] = (Move) {x, y, x+i, y};
                break;
            } else {
                break;
            }
        }
    }if(piece == KNIGHT) {
        // Move up and right
        if (board[x+2][y+1].piece == EMPTY || board[x+2][y+1].color != colour) {
            moves[0] = (Move) {x, y, x+2, y+1};
        }

        // Move up and left
        if (board[x+2][y-1].piece == EMPTY || board[x+2][y-1].color != colour) {
            moves[1] = (Move) {x, y, x+2, y-1};
        }

        // Move down and right
        if (board[x-2][y+1].piece == EMPTY || board[x-2][y+1].color != colour) {
            moves[2] = (Move) {x, y, x-2, y+1};
        }

        // Move down and left
        if (board[x-2][y-1].piece == EMPTY || board[x-2][y-1].color != colour) {
            moves[3] = (Move) {x, y, x-2, y-1};
        }

        // Move right and up
        if (board[x+1][y+2].piece == EMPTY || board[x+1][y+2].color != colour) {
            moves[4] = (Move) {x, y, x+1, y+2};
        }

        // Move right and up
        if (board[x+1][y-2].piece == EMPTY || board[x+1][y-2].color != colour) {
            moves[5] = (Move) {x, y, x+1, y-2};
        }

        // Move right and down
        if (board[x-1][y+2].piece == EMPTY || board[x-1][y+2].color != colour) {
            moves[6] = (Move) {x, y, x-1, y+2};
        }

        // Move left and down
        if (board[x-1][y-2].piece == EMPTY || board[x-1][y-2].color != colour) {
            moves[7] = (Move) {x, y, x-1, y-2};
        }

        // Move left and up
        if (board[x+1][y-2].piece == EMPTY || board[x+1][y-2].color != colour) {
            moves[8] = (Move) {x, y, x+1, y+2};
        }
    } if(piece == QUEEN) {
        // Copy paste of Bishop + Rook logic TODO: Extract
        // Rook below
        // Move right
        for (int i = 1; i < 8; i++) {
            if (board[x][y+i].piece == EMPTY) {
                moves[i] = (Move) {x, y, x, y+i};
            } else if (board[x][y+i].color != colour) {
                moves[i] = (Move) {x, y, x, y+i};
                break;
            } else {
                break;
            }
        }

        // Move left
        for (int i = 1; i < 8; i++) {
            if (board[x][y-i].piece == EMPTY) {
                moves[i] = (Move) {x, y, x, y-i};
            } else if (board[x][y-i].color != colour) {
                moves[i] = (Move) {x, y, x, y-i};
                break;
            } else {
                break;
            }
        }

        // Move up
        for (int i = 1; i < 8; i++) {
            if (board[x-i][y].piece == EMPTY) {
                moves[i] = (Move) {x, y, x-i, y};
            } else if (board[x-i][y].color != colour) {
                moves[i] = (Move) {x, y, x-i, y};
                break;
            } else {
                break;
            }
        }

        // Move down
        for (int i = 1; i < 8; i++) {
            if (board[x+i][y].piece == EMPTY) {
                moves[i] = (Move) {x, y, x+i, y};
            } else if (board[x+i][y].color != colour) {
                moves[i] = (Move) {x, y, x+i, y};
                break;
            } else {
                break;
            }
        }

        // Bishop below
        // Move right diagonally
        for (int i = 1; i < 8; i++) {
            if (board[x+i][y+i].piece == EMPTY) {
                moves[i] = (Move) {x, y, x+i, y+i};
            } else if (board[x+i][y+i].color != colour) {
                moves[i] = (Move) {x, y, x+i, y+i};
                break;
            } else {
                break;
            }
        }
        // Move left diagonally
        for (int i = 1; i < 8; i++) {
            if (board[x-i][y+i].piece == EMPTY) {
                moves[i] = (Move) {x, y, x-i, y+i};
            } else if (board[x-i][y+i].color != colour) {
                moves[i] = (Move) {x, y, x-i, y+i};
                break;
            } else {
                break;
            }
        }

        // Move back left diagonally
        for (int i = 1; i < 8; i++) {
            if (board[x-i][y-i].piece == EMPTY) {
                moves[i] = (Move) {x, y, x-i, y-i};
            } else if (board[x-i][y-i].color != colour) {
                moves[i] = (Move) {x, y, x-i, y-i};
                break;
            } else {
                break;
            }
        }

        // Move back right diagonally
        for (int i = 1; i < 8; i++) {
            if (board[x+i][y-i].piece == EMPTY) {
                moves[i] = (Move) {x, y, x+i, y-i};
            } else if (board[x+i][y-i].color != colour) {
                moves[i] = (Move) {x, y, x+i, y-i};
                break;
            } else {
                break;
            }
        }
    } if(piece == KING) {
        // Move up
        if (board[x-1][y].piece == EMPTY || board[x-1][y].color != colour) {
            moves[0] = (Move) {x, y, x-1, y};
        }

        // Move down
        if (board[x+1][y].piece == EMPTY || board[x+1][y].color != colour) {
            moves[1] = (Move) {x, y, x+1, y};
        }

        // Move left
        if (board[x][y-1].piece == EMPTY || board[x][y-1].color != colour) {
            moves[2] = (Move) {x, y, x, y-1};
        }

        // Move right
        if (board[x][y+1].piece == EMPTY || board[x][y+1].color != colour) {
            moves[3] = (Move) {x, y, x, y+1};
        }

        // Move up and right
        if (board[x-1][y+1].piece == EMPTY || board[x-1][y+1].color != colour) {
            moves[4] = (Move) {x, y, x-1, y+1};
        }

        // Move up and left
        if (board[x-1][y-1].piece == EMPTY || board[x-1][y-1].color != colour) {
            moves[5] = (Move) {x, y, x-1, y-1};
        }

        // Move down and right
        if (board[x+1][y+1].piece == EMPTY || board[x+1][y+1].color != colour) {
            moves[6] = (Move) {x, y, x+1, y+1};
        }

        // Move down and left
        if (board[x+1][y-1].piece == EMPTY || board[x+1][y-1].color != colour) {
            moves[7] = (Move) {x, y, x+1, y-1};
        }
    }

    return moves;
}

bool is_king_moving_into_check(Move move, Colour colour) {
    Square* king;
    Move* moves[MAX_POTENTIAL_TOTAL_MOVES_PER_COLOR];
    if(colour == WHITE) {
        king = allPieces.whitePieces->King;

    } else {
        king = allPieces.blackPieces->King;
    }

    if(move.to_x == king->x_coord && move.to_y == king->y_coord) {
        return true;
    }

    return false;
}

/**
 * @return Array of moves, limited to 129
 */
Move* generate_moves_for_one_color(OneColoursPieces aColoursPieces){
    Move *moves;
    int total_moves_added = 0;
    for(int i = 0; i < 4; i++) {
        Square* pawn = allPieces.blackPieces->Pawns[i];
        if(pawn == NULL) {
            continue;
        }

        Move* some_moves = generate_legal_moves_for_cell(pawn);
        for(int j = 0; j < MAX_POTENTIAL_MOVES_FOR_ONE_PIECE; j++) { //TODO: Should extract all this logic
            Move* a_move = &some_moves[j];
            if(a_move == NULL) {
                continue;
            }
            moves[total_moves_added] = a_move;
            total_moves_added++;
        }
    }

    for(int i = 0; i < 2; i++) {
        Square* rook = allPieces.blackPieces->Rooks[i];
        if(rook == NULL) {
            continue;
        }

        Move* some_moves = generate_legal_moves_for_cell(rook);
        for(int j = 0; j < MAX_POTENTIAL_MOVES_FOR_ONE_PIECE; j++) {
            Move* a_move = &some_moves[j];
            if(a_move == NULL) {
                continue;
            }
            moves[total_moves_added] = a_move;
            total_moves_added++;
        }
    }

    for(int i = 0; i < 2; i++) {
        Square* knight = allPieces.blackPieces->Knights[i];
        if(knight == NULL) {
            continue;
        }

        Move* some_moves = generate_legal_moves_for_cell(knight);
        for(int j = 0; j < MAX_POTENTIAL_MOVES_FOR_ONE_PIECE; j++) {
            Move* a_move = &some_moves[j];
            if(a_move == NULL) {
                continue;
            }
            moves[total_moves_added] = a_move;
            total_moves_added++;
        }
    }

    for(int i = 0; i < 2; i++) {
        Square* bishop = allPieces.blackPieces->Bishops[i];
        if(bishop == NULL) {
            continue;
        }

        Move* some_moves = generate_legal_moves_for_cell(bishop);
        for(int j = 0; j < MAX_POTENTIAL_MOVES_FOR_ONE_PIECE; j++) {
            Move* a_move = &some_moves[j];
            if(a_move == NULL) {
                continue;
            }
            moves[total_moves_added] = a_move;
            total_moves_added++;
        }
    }

    Move* some_moves = generate_legal_moves_for_cell(allPieces.blackPieces->Queen);
    for(int j = 0; j < MAX_POTENTIAL_MOVES_FOR_ONE_PIECE; j++) {
        Move* a_move = &some_moves[j];
        if(a_move == NULL) {
            continue;
        }
        moves[total_moves_added] = a_move;
        total_moves_added++;
    }

    // TODO: Commented this line out to stop infinite recusrion
    // This will need to be re-added or the program will move a king into be checked by another king
    // Move* some_moves = generate_legal_moves_for_cell(allPieces.blackPieces->King);
//    for(int j = 0; j < MAX_POTENTIAL_MOVES_FOR_ONE_PIECE; j++) {
//        Move* a_move = &some_moves[j];
//        if(a_move == NULL) {
//            continue;
//        }
//        moves[total_moves_added] = a_move;
//        total_moves_added++;
//    }

    return moves;
}

Move* generate_all_legal_moves(Colour colour) {
    Move* moves = (Move*) malloc(MAX_POTENTIAL_TOTAL_MOVES_PER_COLOR * sizeof(Move));
    int total_moves_added = 0;

    Move* whites_moves = generate_moves_for_one_color(*allPieces.blackPieces);
    Move* blacks_moves = generate_moves_for_one_color(*allPieces.whitePieces);

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

bool is_king_in_check(Colour colour) {
    Move* all_legal_moves;
    Square* king;
    if(colour == WHITE) {
        all_legal_moves = generate_all_legal_moves(BLACK);
        king = allPieces.whitePieces->King;
    }else {
        all_legal_moves = generate_all_legal_moves(WHITE);
        king = allPieces.blackPieces->King;
    }

    if(all_legal_moves == NULL) {
        printf("Failed to generate all legal moves\n");
        free(all_legal_moves);
        return false;
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