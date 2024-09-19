#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <iostream>
#include <vector>

// Enum for pieces
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

// Structure to represent a move
struct Move {
    int fromRow, fromCol;
    int toRow, toCol;
    Piece promotion; // For pawn promotion, if any

    Move(int fr, int fc, int tr, int tc, Piece promo = EMPTY)
        : fromRow(fr), fromCol(fc), toRow(tr), toCol(tc), promotion(promo) {}
};

class ChessBoard {
public: // Make move generation functions public
    ChessBoard();
    void initializeBoard();
    void printBoard();
    
    // Function to generate all legal moves for the current board state
    std::vector<Move> generateLegalMoves();

    // Function to check if a player is in check
    bool isInCheck(bool white); 

    // Helper functions for move generation
    void generatePawnMoves(int row, int col, std::vector<Move>& moves);
    void generateRookMoves(int row, int col, std::vector<Move>& moves);
    void generateKnightMoves(int row, int col, std::vector<Move>& moves);
    void generateBishopMoves(int row, int col, std::vector<Move>& moves);
    void generateQueenMoves(int row, int col, std::vector<Move>& moves);
    void generateKingMoves(int row, int col, std::vector<Move>& moves);

private:
    std::vector<std::vector<int> > board; // Add space between '>' characters
};

#endif