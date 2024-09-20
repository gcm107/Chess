import random

class Piece:
    def __init__(self, color):
        self.color = color

    def __str__(self):
        return self.__class__.__name__[0] if self.color == 'white' else self.__class__.__name__[0].lower()

class King(Piece): pass
class Queen(Piece): pass
class Rook(Piece): pass
class Bishop(Piece): pass
class Knight(Piece): pass
class Pawn(Piece): pass

class Board:
    def __init__(self):
        self.board = self.create_board()

    def create_board(self):
        board = [[None for _ in range(8)] for _ in range(8)]
        # Place pieces for white
        board[0] = [Rook('white'), Knight('white'), Bishop('white'), Queen('white'), King('white'), Bishop('white'), Knight('white'), Rook('white')]
        board[1] = [Pawn('white') for _ in range(8)]
        # Place pieces for black
        board[6] = [Pawn('black') for _ in range(8)]
        board[7] = [Rook('black'), Knight('black'), Bishop('black'), Queen('black'), King('black'), Bishop('black'), Knight('black'), Rook('black')]
        return board

    def print_board(self):
        for row in self.board:
            print(" ".join(str(piece) if piece else '.' for piece in row))

    def move_piece(self, move):
        # This is a move parser and executor for standard chess notation
        # Assumes move is in the format '1.e4' or '2.Nc6'
        move = move.split('.')[1]  # Split and take the move part
        start_col = ord(move[0].lower()) - ord('a')
        start_row = 8 - int(move[1])
        end_col = ord(move[2].lower()) - ord('a')
        end_row = 8 - int(move[3])
        piece = self.board[start_row][start_col]
        self.board[start_row][start_col] = None
        self.board[end_row][end_col] = piece

    def generate_engine_move(self):
        # This is a very basic random move generator
        possible_moves = []
        for row in range(8):
            for col in range(8):
                piece = self.board[row][col]
                if piece and piece.color == 'black':
                    # Generate some random moves for the piece
                    for d_row in [-1, 1]:
                        for d_col in [-1, 1]:
                            new_row = row + d_row
                            new_col = col + d_col
                            if 0 <= new_row < 8 and 0 <= new_col < 8 and not self.board[new_row][new_col]:
                                possible_moves.append((row, col, new_row, new_col))
        if possible_moves:
            move = random.choice(possible_moves)
            self.board[move[2]][move[3]] = self.board[move[0]][move[1]]
            self.board[move[0]][move[1]] = None

if __name__ == "__main__":
    board = Board()
    while True:
        board.print_board()
        user_move = input("Enter your move (e.g., e2e4): ")
        board.move_piece(user_move)
        board.generate_engine_move()