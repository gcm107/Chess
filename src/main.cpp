#include "ChessBoard.h"
#include <string>

// Print all generated moves
void printMoves(const std::vector<Move>& moves) {
    for (const Move& move : moves) {
        std::cout << "Move from (" << move.fromRow << ", " << move.fromCol 
                  << ") to (" << move.toRow << ", " << move.toCol << ")\n";
    }
}

// Parse player input in algebraic notation
Move parseMove(const std::string& input) {
    int fromRow = 8 - (input[1] - '0');
    int fromCol = input[0] - 'a';
    int toRow = 8 - (input[3] - '0');
    int toCol = input[2] - 'a';
    return Move(fromRow, fromCol, toRow, toCol);
}

// Get the best move using Minimax
Move getBestMove(ChessBoard& board, int depth) {
    std::vector<Move> moves = board.generateLegalMoves();
    int bestValue = -10000;
    Move bestMove = moves[0];

    for (const Move& move : moves) {
        board.makeMove(move);
        int moveValue = board.minimax(depth - 1, false); // AI is maximizing
        board.undoMove(move);

        if (moveValue > bestValue) {
            bestValue = moveValue;
            bestMove = move;
        }
    }

    return bestMove;
}

int main() {
    ChessBoard board;
    board.initializeBoard();
    board.printBoard();

    while (true) {
        // Player's move
        std::cout << "Enter your move (e.g., e2e4): ";
        std::string input;
        std::cin >> input;

        if (input == "exit") break; // Exit condition

        Move move = parseMove(input);
        std::vector<Move> legalMoves = board.generateLegalMoves();

        // Check if the move is legal
        bool isValid = false;
        for (const Move& m : legalMoves) {
            if (m.fromRow == move.fromRow && m.fromCol == move.fromCol &&
                m.toRow == move.toRow && m.toCol == move.toCol) {
                isValid = true;
                break;
            }
        }

        if (isValid) {
            // Make the player's move
            board.makeMove(move);
            board.printBoard();

            // AI's move
            std::cout << "AI is thinking...\n";
            Move aiMove = getBestMove(board, 3); // Depth 3 for Minimax
            board.makeMove(aiMove);
            board.printBoard();
        } else {
            std::cout << "Invalid move. Try again.\n";
        }
    }

    return 0;
}