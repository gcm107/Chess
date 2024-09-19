#include "ChessBoard.h"

// Function to print generated moves
void printMoves(const std::vector<Move>& moves) {
    for (const Move& move : moves) {
        std::cout << "Move from (" << move.fromRow << ", " << move.fromCol 
                  << ") to (" << move.toRow << ", " << move.toCol << ")\n";
    }
}

int main() {
    ChessBoard board;
    board.initializeBoard();
    board.printBoard();

    // Generate and print all legal moves
    std::vector<Move> moves = board.generateLegalMoves();
    std::cout << "All legal moves:\n";
    printMoves(moves);

    // Test specific piece moves
    std::cout << "\nTesting Knight Moves from Position (7, 1):\n";
    moves.clear(); // Clear previous moves
    board.generateKnightMoves(7, 1, moves);
    printMoves(moves);

    return 0;
}