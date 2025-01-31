#include <stdint.h>
#include "zobrist.h"
#include "move.h"
#include "utils/random.h"

#define BOARD_SIZE 8
#define NUM_COLOURS 3 // white, black, none
#define NUM_PIECE_TYPES 7 // pawn, knight, bishop, rook, queen, king, empty
#define NUM_PIECE_TYPE_AND_COLOUR_COMBINATIONS NUM_COLOURS * NUM_PIECE_TYPES

typedef uint64_t ZobristKey;

ZobristKey zobrist_table[BOARD_SIZE][BOARD_SIZE][NUM_PIECE_TYPE_AND_COLOUR_COMBINATIONS];
ZobristKey current_board_hash = 0;

unsigned int get_zobrist_index(Piece piece, Colour color) {
    return (piece * NUM_COLOURS) + color;
}

void init_zobrist_table() {
    // Generate random 64-bit values for each square, piece type combination
    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            for (int piece = EMPTY; piece <= KING; piece++) {
                for (Colour color = WHITE; color <= NONE; color++) {
                    unsigned int index = get_zobrist_index(piece, color);
                    zobrist_table[x][y][index] = generate_64_bit_hash_value();
                }
            }
        }
    }

    current_board_hash = 0;
    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            Square square = board[x][y];
            if (square.piece != EMPTY) {
                unsigned int index = get_zobrist_index(square.piece, square.color);
                current_board_hash ^= zobrist_table[x][y][index];
            }
        }
    }
}

void update_zobrist_hash(Move* move, Square* from_square, Square* to_square) {
    // Remove the piece from its old position
    if (from_square->piece != EMPTY) {
        unsigned int from_index = get_zobrist_index(from_square->piece, from_square->color);
        current_board_hash ^= zobrist_table[move->from_x][move->from_y][from_index];
    }

    // Add the piece to its new position
    if (to_square->piece != EMPTY) {
        unsigned int to_index = get_zobrist_index(to_square->piece, to_square->color);
        current_board_hash ^= zobrist_table[move->to_x][move->to_y][to_index];
    }
}
