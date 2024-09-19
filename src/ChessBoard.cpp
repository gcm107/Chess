#include "ChessBoard.h"

// Constructor initializes an empty board
ChessBoard::ChessBoard() {
    board.resize(8, std::vector<int>(8, EMPTY));
}

// Function to initialize the board with the starting position
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
}

// Function to print the board
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

// Get the piece at a specific position
Piece ChessBoard::getPieceAt(int row, int col) const {
    return static_cast<Piece>(board[row][col]);
}

// Set a piece at a specific position
void ChessBoard::setPieceAt(int row, int col, Piece piece) {
    board[row][col] = piece;
}

// Function to generate all legal moves
std::vector<Move> ChessBoard::generateLegalMoves() {
    std::vector<Move> moves;

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (board[row][col] != EMPTY) {
                std::vector<Move> pieceMoves;
                switch(board[row][col]) {
                    case WHITE_PAWN: case BLACK_PAWN:
                        generatePawnMoves(row, col, pieceMoves); break;
                    case WHITE_ROOK: case BLACK_ROOK:
                        generateRookMoves(row, col, pieceMoves); break;
                    case WHITE_KNIGHT: case BLACK_KNIGHT:
                        generateKnightMoves(row, col, pieceMoves); break;
                    case WHITE_BISHOP: case BLACK_BISHOP:
                        generateBishopMoves(row, col, pieceMoves); break;
                    case WHITE_QUEEN: case BLACK_QUEEN:
                        generateQueenMoves(row, col, pieceMoves); break;
                    case WHITE_KING: case BLACK_KING:
                        generateKingMoves(row, col, pieceMoves); break;
                }

                // Check legality of each move
                for (const Move& move : pieceMoves) {
                    // Temporarily make the move
                    int capturedPiece = board[move.toRow][move.toCol];
                    board[move.toRow][move.toCol] = board[move.fromRow][move.fromCol];
                    board[move.fromRow][move.fromCol] = EMPTY;

                    if (!isInCheck(board[move.toRow][move.toCol] > 0)) {
                        moves.push_back(move); // Legal move
                    }

                    // Undo the move
                    board[move.fromRow][move.fromCol] = board[move.toRow][move.toCol];
                    board[move.toRow][move.toCol] = capturedPiece;
                }
            }
        }
    }

    return moves;
}

// Function to check if a player is in check
bool ChessBoard::isInCheck(bool white) {
    // Find the king's position
    int kingRow = -1, kingCol = -1;
    int kingPiece = white ? WHITE_KING : BLACK_KING;

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j] == kingPiece) {
                kingRow = i;
                kingCol = j;
                break;
            }
        }
    }

    // Check if any opponent piece can attack the king's position
    std::vector<Move> opponentMoves;
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if ((white && board[row][col] < 0) || (!white && board[row][col] > 0)) {
                switch (abs(board[row][col])) {
                    case WHITE_PAWN: case BLACK_PAWN:
                        generatePawnMoves(row, col, opponentMoves); break;
                    case WHITE_ROOK: case BLACK_ROOK:
                        generateRookMoves(row, col, opponentMoves); break;
                    case WHITE_KNIGHT: case BLACK_KNIGHT:
                        generateKnightMoves(row, col, opponentMoves); break;
                    case WHITE_BISHOP: case BLACK_BISHOP:
                        generateBishopMoves(row, col, opponentMoves); break;
                    case WHITE_QUEEN: case BLACK_QUEEN:
                        generateQueenMoves(row, col, opponentMoves); break;
                    case WHITE_KING: case BLACK_KING:
                        generateKingMoves(row, col, opponentMoves); break;
                }
            }
        }
    }

    // Check if any move attacks the king's position
    for (const auto& move : opponentMoves) {
        if (move.toRow == kingRow && move.toCol == kingCol) {
            return true; // King is in check
        }
    }

    return false;
}

// Pawn moves
void ChessBoard::generatePawnMoves(int row, int col, std::vector<Move>& moves) {
    int direction = (board[row][col] == WHITE_PAWN) ? 1 : -1;

    // Single move forward
    if (board[row + direction][col] == EMPTY) {
        moves.push_back(Move(row, col, row + direction, col));
        
        // Double move forward if on initial row
        if ((board[row][col] == WHITE_PAWN && row == 1) || (board[row][col] == BLACK_PAWN && row == 6)) {
            if (board[row + 2 * direction][col] == EMPTY) {
                moves.push_back(Move(row, col, row + 2 * direction, col));
            }
        }
    }

    // Capturing diagonally
    if (col > 0 && board[row + direction][col - 1] != EMPTY && board[row][col] * board[row + direction][col - 1] < 0) {
        moves.push_back(Move(row, col, row + direction, col - 1));
    }
    if (col < 7 && board[row + direction][col + 1] != EMPTY && board[row][col] * board[row + direction][col + 1] < 0) {
        moves.push_back(Move(row, col, row + direction, col + 1));
    }

    
}

// Rook moves
void ChessBoard::generateRookMoves(int row, int col, std::vector<Move>& moves) {
    // Upwards movement
    for (int i = row + 1; i < 8; ++i) {
        if (board[i][col] == EMPTY) {
            moves.push_back(Move(row, col, i, col));
        } else {
            if (board[row][col] * board[i][col] < 0) // Capture opponent's piece
                moves.push_back(Move(row, col, i, col));
            break; // Blocked by a piece
        }
    }
    // Downwards movement
    for (int i = row - 1; i >= 0; --i) {
        if (board[i][col] == EMPTY) {
            moves.push_back(Move(row, col, i, col));
        } else {
            if (board[row][col] * board[i][col] < 0) // Capture opponent's piece
                moves.push_back(Move(row, col, i, col));
            break; // Blocked by a piece
        }
    }
    // Right movement
    for (int i = col + 1; i < 8; ++i) {
        if (board[row][i] == EMPTY) {
            moves.push_back(Move(row, col, row, i));
        } else {
            if (board[row][col] * board[row][i] < 0) // Capture opponent's piece
                moves.push_back(Move(row, col, row, i));
            break; // Blocked by a piece
        }
    }
    // Left movement
    for (int i = col - 1; i >= 0; --i) {
        if (board[row][i] == EMPTY) {
            moves.push_back(Move(row, col, row, i));
        } else {
            if (board[row][col] * board[row][i] < 0) // Capture opponent's piece
                moves.push_back(Move(row, col, row, i));
            break; // Blocked by a piece
        }
    }
}

// Knight moves
void ChessBoard::generateKnightMoves(int row, int col, std::vector<Move>& moves) {
    // Possible moves for a knight in terms of row and column changes
    const int knightMoves[8][2] = {
        {2, 1}, {2, -1}, {-2, 1}, {-2, -1}, 
        {1, 2}, {1, -2}, {-1, 2}, {-1, -2}
    };

    for (const auto& move : knightMoves) {
        int newRow = row + move[0];
        int newCol = col + move[1];
        if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) { // Check board boundaries
            if (board[newRow][newCol] == EMPTY || board[row][col] * board[newRow][newCol] < 0) {
                moves.push_back(Move(row, col, newRow, newCol));
            }
        }
    }
}

// Bishop moves
void ChessBoard::generateBishopMoves(int row, int col, std::vector<Move>& moves) {
    // Top-right diagonal
    for (int i = 1; row + i < 8 && col + i < 8; ++i) {
        if (board[row + i][col + i] == EMPTY) {
            moves.push_back(Move(row, col, row + i, col + i));
        } else {
            if (board[row][col] * board[row + i][col + i] < 0) // Capture opponent's piece
                moves.push_back(Move(row, col, row + i, col + i));
            break; // Blocked by a piece
        }
    }
    // Top-left diagonal
    for (int i = 1; row + i < 8 && col - i >= 0; ++i) {
        if (board[row + i][col - i] == EMPTY) {
            moves.push_back(Move(row, col, row + i, col - i));
        } else {
            if (board[row][col] * board[row + i][col - i] < 0) // Capture opponent's piece
                moves.push_back(Move(row, col, row + i, col - i));
            break; // Blocked by a piece
        }
    }
    // Bottom-right diagonal
    for (int i = 1; row - i >= 0 && col + i < 8; ++i) {
        if (board[row - i][col + i] == EMPTY) {
            moves.push_back(Move(row, col, row - i, col + i));
        } else {
            if (board[row][col] * board[row - i][col + i] < 0) // Capture opponent's piece
                moves.push_back(Move(row, col, row - i, col + i));
            break; // Blocked by a piece
        }
    }
    // Bottom-left diagonal
    for (int i = 1; row - i >= 0 && col - i >= 0; ++i) {
        if (board[row - i][col - i] == EMPTY) {
            moves.push_back(Move(row, col, row - i, col - i));
        } else {
            if (board[row][col] * board[row - i][col - i] < 0) // Capture opponent's piece
                moves.push_back(Move(row, col, row - i, col - i));
            break; // Blocked by a piece
        }
    }
}

// Queen moves
void ChessBoard::generateQueenMoves(int row, int col, std::vector<Move>& moves) {
    generateRookMoves(row, col, moves);  // Rook-like moves
    generateBishopMoves(row, col, moves);  // Bishop-like moves
}


// King Moves
void ChessBoard::generateKingMoves(int row, int col, std::vector<Move>& moves) {
    // Standard king moves
    const int kingMoves[8][2] = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}, 
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
    };

    for (const auto& move : kingMoves) {
        int newRow = row + move[0];
        int newCol = col + move[1];
        if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) { // Check board boundaries
            if (board[newRow][newCol] == EMPTY || board[row][col] * board[newRow][newCol] < 0) {
                moves.push_back(Move(row, col, newRow, newCol));
            }
        }
    }

    // Castling logic (simplified, need to check if castling rights are valid)
    if (board[row][col] == WHITE_KING && row == 0 && col == 4) {
        if (board[0][0] == WHITE_ROOK && board[0][1] == EMPTY && board[0][2] == EMPTY && board[0][3] == EMPTY) {
            moves.push_back(Move(0, 4, 0, 2)); // Queen-side castling
        }
        if (board[0][7] == WHITE_ROOK && board[0][5] == EMPTY && board[0][6] == EMPTY) {
            moves.push_back(Move(0, 4, 0, 6)); // King-side castling
        }
    }

    if (board[row][col] == BLACK_KING && row == 7 && col == 4) {
        if (board[7][0] == BLACK_ROOK && board[7][1] == EMPTY && board[7][2] == EMPTY && board[7][3] == EMPTY) {
            moves.push_back(Move(7, 4, 7, 2)); // Queen-side castling
        }
        if (board[7][7] == BLACK_ROOK && board[7][5] == EMPTY && board[7][6] == EMPTY) {
            moves.push_back(Move(7, 4, 7, 6)); // King-side castling
        }
    }
}