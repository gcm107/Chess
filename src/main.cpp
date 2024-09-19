#include "ChessBoard.h"
#include <string>

void printMoves(const std::vector<Move>& moves) {
    for (const Move& move : moves) {
        std::cout << "Move from (" << move.fromRow << ", " << move.fromCol 
                  << ") to (" << move.toRow << ", " << move.toCol << ")\n";
    }
}

Move parseMove(const std::string& input) {
    int fromRow = 8 - (input[1] - '0');
    int fromCol = input[0] - 'a';
    int toRow = 8 - (input[3] - '0');
    int toCol = input[2] - 'a';
    return Move(fromRow, fromCol, toRow, toCol);
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
            // Make the move using setter methods
            board.setPieceAt(move.toRow, move.toCol, board.getPieceAt(move.fromRow, move.fromCol));
            board.setPieceAt(move.fromRow, move.fromCol, EMPTY);
            board.printBoard();
        } else {
            std::cout << "Invalid move. Try again.\n";
        }
    }

    return 0;
}