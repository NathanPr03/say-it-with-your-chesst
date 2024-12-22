#ifndef SAY_IT_WITH_YOUR_CHESST_PIECES_H
#define SAY_IT_WITH_YOUR_CHESST_PIECES_H
#pragma once
    int find_next_empty_piece_index(Colour colour, Piece piece_type);

    int find_piece_index_by_coordinate(int x, int y, Piece piece_type, bool is_promoted_piece);
#endif
