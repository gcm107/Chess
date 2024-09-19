#include "ChessBoard.h"

ChessBoard::ChessBoard() {
    board.resize(8, std::vector<int>(8, EMPTY));
}

void ChessBoard::initializeBoard() {
    // Initialize white pieces
    board[0][0] = board[0][7] = WHITE_ROOK;
    board[0][1] = board[0][6] = WHITE_KNIGHT;
    board[0][2] = board[0][5] = WHITE_BISHOP;
    board[0][3] = WHITE_QUEEN;
    board[0][4] = WHITE_KING;
    for (int i = 0; i < 8; i++) board[1][i] = WHITE_PAWN;

    // Initialize black pieces
    board[7][0] = board[7][7] = BLACK_ROOK;
    board[7][1] = board[7][6] = BLACK_KNIGHT;
    board[7][2] = board[7][5] = BLACK_BISHOP;
    board[7][3] = BLACK_QUEEN;
    board[7][4] = BLACK_KING;
    for (int i = 0; i < 8; i++) board[6][i] = BLACK_PAWN;

    // Empty squares are already set in the constructor
}

void ChessBoard::printBoard() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            switch(board[i][j]) {
                case EMPTY: std::cout << ". "; break;
                case WHITE_PAWN: std::cout << "P "; break;
                case WHITE_ROOK: std::cout << "R "; break;
                case WHITE_KNIGHT: std::cout << "N "; break;
                case WHITE_BISHOP: std::cout << "B "; break;
                case WHITE_QUEEN: std::cout << "Q "; break;
                case WHITE_KING: std::cout << "K "; break;
                case BLACK_PAWN: std::cout << "p "; break;
                case BLACK_ROOK: std::cout << "r "; break;
                case BLACK_KNIGHT: std::cout << "n "; break;
                case BLACK_BISHOP: std::cout << "b "; break;
                case BLACK_QUEEN: std::cout << "q "; break;
                case BLACK_KING: std::cout << "k "; break;
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
