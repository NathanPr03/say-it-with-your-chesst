#include <stdio.h>
#include <stdlib.h>
#include "move.h"
#include "moves/execute_move.h"

void write_move(FILE* file, Move m) {
    fprintf(file, "%d %d %d %d %.15g %d %d %d %d\n",
            m.from_x,
            m.from_y,
            m.to_x,
            m.to_y,
            m.score,
            m.is_promotion,
            m.promotion_piece,
            m.is_en_passant,
            m.is_castling
    );
}

void replay_moves_from_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        return;
    }
    while (!feof(file)) {
        Move move_to_execute;
        if (fscanf(
                file,
                "%d %d %d %d %lf %d %d %d %d",
                &move_to_execute.from_x,
                &move_to_execute.from_y,
                &move_to_execute.to_x,
                &move_to_execute.to_y,
                &move_to_execute.score,
                (int*)&move_to_execute.is_promotion,
                (int*)&move_to_execute.promotion_piece,
                (int*)&move_to_execute.is_en_passant,
                (int*)&move_to_execute.is_castling
        ) == 9) {
            execute_move(&move_to_execute, false);
        } else {
            break;
        }
    }
    fclose(file);
}
