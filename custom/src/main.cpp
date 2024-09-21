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
    std::vector<Move> moves = board.generateLegalMoves(board.getCurrentPlayer() == WHITE);
    if (moves.empty()) {
        return Move(-1, -1, -1, -1);
    }

    int bestValue = -10000;
    Move bestMove = moves[0];

    for (const Move& move : moves) {
        ChessBoard tempBoard = board; // Create a copy of the board
        tempBoard.makeMove(move);
        int moveValue = tempBoard.minimax(depth - 1, false);

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
        std::cout << "Enter your move (e.g., e2e4): ";
        std::string input;
        std::cin >> input;

        if (input == "exit") break; // Exit condition

        Move move = parseMove(input);
        std::vector<Move> legalMoves = board.generateLegalMoves(board.getCurrentPlayer() == WHITE);

        board.printLegalMoves(); // Print all legal moves for debugging

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
            board.makeMove(move);
            board.printBoard();

            // AI's move
            std::cout << "AI is thinking...\n";
            Move aiMove = getBestMove(board, 3); // Depth 3 for Minimax
            if (aiMove.fromRow != -1) {
                std::cout << "AI's move: " << aiMove.toString() << std::endl;
                board.makeMove(aiMove);
                board.printBoard();
            } else {
                std::cout << "AI has no legal moves. Game over.\n";
                break;
            }
        } else {
            std::cout << "Invalid move. Try again.\n";
        }
    }

    return 0;
}