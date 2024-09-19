#include "ChessBoard.h"

// Constructor initializes an empty board
ChessBoard::ChessBoard() : currentPlayer(WHITE) {
    board.resize(8, std::vector<int>(8, EMPTY));
}

// Function to initialize the board with the starting position
void ChessBoard::initializeBoard() {
    // Initialize white pieces (uppercase)
    board[7][0] = board[7][7] = WHITE_ROOK;
    board[7][1] = board[7][6] = WHITE_KNIGHT;
    board[7][2] = board[7][5] = WHITE_BISHOP;
    board[7][3] = WHITE_QUEEN;
    board[7][4] = WHITE_KING;
    for (int i = 0; i < 8; i++) board[6][i] = WHITE_PAWN;

    // Initialize black pieces (lowercase)
    board[0][0] = board[0][7] = BLACK_ROOK;
    board[0][1] = board[0][6] = BLACK_KNIGHT;
    board[0][2] = board[0][5] = BLACK_BISHOP;
    board[0][3] = BLACK_QUEEN;
    board[0][4] = BLACK_KING;
    for (int i = 0; i < 8; i++) board[1][i] = BLACK_PAWN;
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

// Function to generate all legal moves for the current player
std::vector<Move> ChessBoard::generateLegalMoves(bool isWhite) {
    std::vector<Move> moves;
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (board[row][col] != EMPTY && (isWhite == (board[row][col] > 0))) {
                std::vector<Move> pieceMoves;
                switch(abs(board[row][col])) {
                    case WHITE_PAWN:
                        generatePawnMoves(row, col, pieceMoves); break;
                    case WHITE_ROOK:
                        generateRookMoves(row, col, pieceMoves); break;
                    case WHITE_KNIGHT:
                        generateKnightMoves(row, col, pieceMoves); break;
                    case WHITE_BISHOP:
                        generateBishopMoves(row, col, pieceMoves); break;
                    case WHITE_QUEEN:
                        generateQueenMoves(row, col, pieceMoves); break;
                    case WHITE_KING:
                        generateKingMoves(row, col, pieceMoves); break;
                }
                moves.insert(moves.end(), pieceMoves.begin(), pieceMoves.end());
            }
        }
    }
    return moves;
}

// Add a function to convert algebraic notation to board coordinates
Move ChessBoard::algebraicToMove(const std::string& algebraic) {
    int fromCol = algebraic[0] - 'a';
    int fromRow = 8 - (algebraic[1] - '0');
    int toCol = algebraic[2] - 'a';
    int toRow = 8 - (algebraic[3] - '0');
    return Move(fromRow, fromCol, toRow, toCol);
}

// Function to evaluate the board state and return a score
int ChessBoard::evaluateBoard() {
    int score = 0;
    const int pieceValues[7] = {0, 10, 50, 30, 30, 90, 900}; // Pawn, Rook, Knight, Bishop, Queen, King

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; col++) {
            int piece = board[row][col];
            if (piece != EMPTY) {
                int pieceValue = pieceValues[abs(piece)];
                if (piece > 0) {
                    score += pieceValue; // White pieces add to score
                } else {
                    score -= pieceValue; // Black pieces subtract from score
                }
            }
        }
    }

    return score;
}

// Generate moves for a pawn
void ChessBoard::generatePawnMoves(int row, int col, std::vector<Move>& moves) {
    int direction = (board[row][col] > 0) ? -1 : 1; // White moves up, Black moves down
    int startRow = (board[row][col] > 0) ? 6 : 1;  // Starting row for pawns

    // Single move forward
    if (row + direction >= 0 && row + direction < 8 && board[row + direction][col] == EMPTY) {
        moves.push_back(Move(row, col, row + direction, col));
        // Double move forward
        if (row == startRow && board[row + 2 * direction][col] == EMPTY) {
            moves.push_back(Move(row, col, row + 2 * direction, col));
        }
    }

    // Captures
    if (col > 0 && row + direction >= 0 && row + direction < 8 && board[row + direction][col - 1] * board[row][col] < 0) {
        moves.push_back(Move(row, col, row + direction, col - 1));
    }
    if (col < 7 && row + direction >= 0 && row + direction < 8 && board[row + direction][col + 1] * board[row][col] < 0) {
        moves.push_back(Move(row, col, row + direction, col + 1));
    }
}

// Generate moves for a rook
void ChessBoard::generateRookMoves(int row, int col, std::vector<Move>& moves) {
    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    for (auto& dir : directions) {
        for (int i = 1; i < 8; ++i) {
            int newRow = row + i * dir[0];
            int newCol = col + i * dir[1];
            if (newRow < 0 || newRow >= 8 || newCol < 0 || newCol >= 8) break;
            if (board[newRow][newCol] == EMPTY) {
                moves.push_back(Move(row, col, newRow, newCol));
            } else {
                if (board[newRow][newCol] * board[row][col] < 0) {
                    moves.push_back(Move(row, col, newRow, newCol)); // Capture
                }
                break;
            }
        }
    }
}

// Generate moves for a knight
void ChessBoard::generateKnightMoves(int row, int col, std::vector<Move>& moves) {
    int knightMoves[8][2] = {{-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}, {2, -1}, {2, 1}};
    for (auto& move : knightMoves) {
        int newRow = row + move[0];
        int newCol = col + move[1];
        if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8 && board[newRow][newCol] * board[row][col] <= 0) {
            moves.push_back(Move(row, col, newRow, newCol));
        }
    }
}

// Generate moves for a bishop
void ChessBoard::generateBishopMoves(int row, int col, std::vector<Move>& moves) {
    int directions[4][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
    for (auto& dir : directions) {
        for (int i = 1; i < 8; ++i) {
            int newRow = row + i * dir[0];
            int newCol = col + i * dir[1];
            if (newRow < 0 || newRow >= 8 || newCol < 0 || newCol >= 8) break;
            if (board[newRow][newCol] == EMPTY) {
                moves.push_back(Move(row, col, newRow, newCol));
            } else {
                if (board[newRow][newCol] * board[row][col] < 0) {
                    moves.push_back(Move(row, col, newRow, newCol)); // Capture
                }
                break;
            }
        }
    }
}

// Generate moves for a queen
void ChessBoard::generateQueenMoves(int row, int col, std::vector<Move>& moves) {
    generateRookMoves(row, col, moves);
    generateBishopMoves(row, col, moves);
}

// Generate moves for a king
void ChessBoard::generateKingMoves(int row, int col, std::vector<Move>& moves) {
    int kingMoves[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};
    for (auto& move : kingMoves) {
        int newRow = row + move[0];
        int newCol = col + move[1];
        if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8 && board[newRow][newCol] * board[row][col] <= 0) {
            moves.push_back(Move(row, col, newRow, newCol));
        }
    }
}

// Print all generated legal moves for debugging
void ChessBoard::printLegalMoves() {
    std::vector<Move> moves = generateLegalMoves(currentPlayer == WHITE);
    std::cout << "Generated " << moves.size() << " legal moves:\n";
    for (const Move& move : moves) {
        std::cout << "Move from (" << move.fromRow << ", " << move.fromCol 
                  << ") to (" << move.toRow << ", " << move.toCol << ")\n";
    }
}

bool ChessBoard::makeMove(const Move& move) {
    std::vector<Move> legalMoves = generateLegalMoves(currentPlayer == WHITE);
    
    for (const auto& legalMove : legalMoves) {
        if (move.fromRow == legalMove.fromRow && move.fromCol == legalMove.fromCol &&
            move.toRow == legalMove.toRow && move.toCol == legalMove.toCol) {
            // Make the move
            Piece piece = getPieceAt(move.fromRow, move.fromCol);
            setPieceAt(move.toRow, move.toCol, piece);
            setPieceAt(move.fromRow, move.fromCol, EMPTY);
            currentPlayer = (currentPlayer == WHITE) ? BLACK : WHITE;
            
            // Debug output
            std::cout << "Move made: " << move.toString() << std::endl;
            std::cout << "Piece moved: " << piece << std::endl;
            
            return true;
        }
    }
    
    // Debug output for invalid move
    std::cout << "Invalid move attempted: " << move.toString() << std::endl;
    
    return false;
}

// Make a move using algebraic notation
bool ChessBoard::makeMove(const std::string& algebraic) {
    Move move = algebraicToMove(algebraic);
    return makeMove(move);
}

// Undo a move on the board
void ChessBoard::undoMove(const Move& move, Piece capturedPiece) {
    Piece piece = getPieceAt(move.toRow, move.toCol);
    setPieceAt(move.fromRow, move.fromCol, piece);
    setPieceAt(move.toRow, move.toCol, capturedPiece);
    currentPlayer = (currentPlayer == WHITE) ? BLACK : WHITE;
}

// Minimax algorithm for AI
int ChessBoard::minimax(int depth, bool isMaximizingPlayer) {
    if (depth == 0) {
        return evaluateBoard();
    }

    std::vector<Move> moves = generateLegalMoves(isMaximizingPlayer);
    
    if (moves.empty()) {
        return isMaximizingPlayer ? -10000 : 10000;
    }

    int bestValue = isMaximizingPlayer ? -10000 : 10000;
    for (const Move& move : moves) {
        ChessBoard tempBoard = *this; // Create a copy of the current board
        tempBoard.makeMove(move);
        int eval = tempBoard.minimax(depth - 1, !isMaximizingPlayer);
        
        if (isMaximizingPlayer) {
            bestValue = std::max(bestValue, eval);
        } else {
            bestValue = std::min(bestValue, eval);
        }
    }
    return bestValue;
}