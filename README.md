# Chess Engine -- (Under Development)

This is a simple chess engine implemented in C++. It allows you to play against an AI opponent using a command-line interface.

## Prerequisites

- C++ compiler (GCC or Clang recommended)
- Make (optional, but recommended for easier compilation)

## Compilation

### Manual Compilation (Cale recommends this.)

If you don't have Make, you can compile the program manually using the following command:

`g++ -std=c++11 -Iinclude -o bin/chess src/main.cpp src/ChessBoard.cpp`


Make sure you're in the root directory of the project when running this command.

### Using Make

If you have Make installed, you can use the following command to compile the program:

`make`

This will create an executable named `chess` in the `bin` directory.

## Running the Program

After compilation, you can run the program using:

`./bin/chess`


## How to Play

1. The game starts with the initial chess board setup.
2. You play as White, and the AI plays as Black.
3. Enter your moves in algebraic notation (e.g., "e2e4" to move a piece from e2 to e4).
4. The AI will respond with its move.
5. Continue playing until the game ends.
6. Type "exit" at any time to quit the game.

## Game Controls

- Enter moves in the format "fromSquaretoSquare" (e.g., "e2e4", "g1f3")
- Type "exit" to quit the game

Enjoy playing chess against the AI!