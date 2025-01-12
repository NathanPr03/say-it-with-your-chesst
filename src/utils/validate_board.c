#include "validate_board.h"

bool validate_board_state() {
    // For each non-empty square on the board
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            Square* curr = &board[x][y];
            if (curr->piece == EMPTY) continue;

            // Get the appropriate piece array (white or black)
            OneColoursPieces* pieces = (curr->color == WHITE) ?
                                       allPieces.whitePieces :
                                       allPieces.blackPieces;

            // Try to find this square in the appropriate piece array
            bool found = false;
            switch (curr->piece) {
                case PAWN:
                    for (int i = 0; i < 8; i++) {
                        if (pieces->Pawns[i] == curr) {
                            found = true;
                            break;
                        }
                    }
                    break;

                case KNIGHT:
                    if (pieces->Knights[0] == curr || pieces->Knights[1] == curr) {
                        found = true;
                    }
                    break;

                case BISHOP:
                    if (pieces->Bishops[0] == curr || pieces->Bishops[1] == curr) {
                        found = true;
                    }
                    break;

                case ROOK:
                    if (pieces->Rooks[0] == curr || pieces->Rooks[1] == curr) {
                        found = true;
                    }
                    break;

                case QUEEN:
                    if (pieces->Queen == curr) found = true;
                    break;

                case KING:
                    if (pieces->King == curr) found = true;
                    break;
            }

            // Also check promoted pieces array
            if (!found) {
                for (int i = 0; i < 8; i++) {
                    if (pieces->PromotedPieces[i] == curr) {
                        found = true;
                        break;
                    }
                }
            }

            if (!found) {
                printf("Piece at (%d,%d) not found in allPieces\n", x, y);
                return false;
            }
        }
    }
    return true;
}