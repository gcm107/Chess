#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <iostream>
#include <vector>

// Enum to represent different pieces
enum Piece {
    EMPTY = 0,
    WHITE_PAWN = 1,
    WHITE_ROOK = 2,
    WHITE_KNIGHT = 3,
    WHITE_BISHOP = 4,
    WHITE_QUEEN = 5,
    WHITE_KING = 6,
    BLACK_PAWN = -1,
    BLACK_ROOK = -2,
    BLACK_KNIGHT = -3,
    BLACK_BISHOP = -4,
    BLACK_QUEEN = -5,
    BLACK_KING = -6
};

// ChessBoard class definition
class ChessBoard {
public:
    ChessBoard();
    void initializeBoard();
    void printBoard();

private:
    std::vector<std::vector<int>> board; // 2D vector to represent the chessboard
};

#endif
