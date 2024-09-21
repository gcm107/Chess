#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <vector>
#include <string>
#include <iostream>

enum Piece {
    EMPTY = 0,
    WHITE_PAWN = 1, WHITE_ROOK = 2, WHITE_KNIGHT = 3, WHITE_BISHOP = 4, WHITE_QUEEN = 5, WHITE_KING = 6,
    BLACK_PAWN = -1, BLACK_ROOK = -2, BLACK_KNIGHT = -3, BLACK_BISHOP = -4, BLACK_QUEEN = -5, BLACK_KING = -6
};

enum Player {
    WHITE,
    BLACK
};

struct Move {
    int fromRow, fromCol, toRow, toCol;
    Move(int fr, int fc, int tr, int tc) : fromRow(fr), fromCol(fc), toRow(tr), toCol(tc) {}
    std::string toString() const {
        return std::string(1, 'a' + fromCol) + std::to_string(8 - fromRow) +
               std::string(1, 'a' + toCol) + std::to_string(8 - toRow);
    }
};

class ChessBoard {
public:
    ChessBoard();
    void initializeBoard();
    void printBoard();
    Player getCurrentPlayer() const { return currentPlayer; }
    std::vector<Move> generateLegalMoves(bool isWhite);
    Piece getPieceAt(int row, int col) const;
    bool makeMove(const Move& move);
    bool makeMove(const std::string& algebraic);
    void undoMove(const Move& move, Piece capturedPiece);
    int minimax(int depth, bool isMaximizingPlayer);
    int evaluateBoard();
    void printLegalMoves();

private:
    std::vector<std::vector<int>> board;
    Player currentPlayer;
    

    // Helper functions
    void setPieceAt(int row, int col, Piece piece);
    Move algebraicToMove(const std::string& algebraic);

    // Move generation helper functions
    void generatePawnMoves(int row, int col, std::vector<Move>& moves);
    void generateRookMoves(int row, int col, std::vector<Move>& moves);
    void generateKnightMoves(int row, int col, std::vector<Move>& moves);
    void generateBishopMoves(int row, int col, std::vector<Move>& moves);
    void generateQueenMoves(int row, int col, std::vector<Move>& moves);
    void generateKingMoves(int row, int col, std::vector<Move>& moves);
};

#endif // CHESSBOARD_H