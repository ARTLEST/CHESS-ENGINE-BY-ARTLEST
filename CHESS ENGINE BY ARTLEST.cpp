// Chess Engine by ARTLEST
// A simple chess engine for beginners with basic rules enforcement, including correct moves, capturing, check, and checkmate.

// Including standard libraries for input/output, string handling, and vectors
#include <iostream>
#include <string>
#include <vector>
#include <cctype>

// Using namespace std to avoid prefixing std:: for beginners
using namespace std;

// Define the 8x8 chessboard as a 2D array of characters
// 'r' = rook, 'n' = knight, 'b' = bishop, 'q' = queen, 'k' = king, 'p' = pawn
// Uppercase for white pieces (e.g., 'R'), lowercase for black pieces (e.g., 'r')
char board[8][8];

// Tracks whose turn it is (true = white, false = black)
bool whiteTurn = true;

// Function to initialize the chessboard with starting positions
void initializeBoard() {
    // Set up black pieces on rows 0 and 1
    board[0][0] = 'r'; board[0][1] = 'n'; board[0][2] = 'b'; board[0][3] = 'q';
    board[0][4] = 'k'; board[0][5] = 'b'; board[0][6] = 'n'; board[0][7] = 'r';
    for (int col = 0; col < 8; col++) {
        board[1][col] = 'p'; // Black pawns on row 1
    }

    // Set up white pieces on rows 6 and 7
    board[7][0] = 'R'; board[7][1] = 'N'; board[7][2] = 'B'; board[7][3] = 'Q';
    board[7][4] = 'K'; board[7][5] = 'B'; board[7][6] = 'N'; board[7][7] = 'R';
    for (int col = 0; col < 8; col++) {
        board[6][col] = 'P'; // White pawns on row 6
    }

    // Empty squares in the middle (rows 2 to 5)
    for (int row = 2; row < 6; row++) {
        for (int col = 0; col < 8; col++) {
            board[row][col] = '.'; // '.' represents an empty square
        }
    }
}

// Function to display the chessboard
void displayBoard() {
    cout << "\nChessboard by ARTLEST\n";
    cout << "Turn: " << (whiteTurn ? "White" : "Black") << "\n";
    cout << "  a b c d e f g h\n"; // Column labels
    for (int row = 0; row < 8; row++) {
        cout << 8 - row << " "; // Row number (8 to 1)
        for (int col = 0; col < 8; col++) {
            cout << board[row][col] << " ";
        }
        cout << 8 - row << "\n"; // Row number on the right
    }
    cout << "  a b c d e f g h\n";
}

// Function to check if a position is within the board
bool isValidPosition(int row, int col) {
    return row >= 0 && row < 8 && col >= 0 && col < 8;
}

// Function to convert user input (e.g., "e2") to board coordinates
pair<int, int> parsePosition(string pos) {
    if (pos.length() != 2) return make_pair(-1, -1);
    char colChar = tolower(pos[0]);
    char rowChar = pos[1];
    int col = colChar - 'a';
    if (col < 0 || col > 7) return make_pair(-1, -1);
    if (rowChar < '1' || rowChar > '8') return make_pair(-1, -1);
    int row = 8 - (rowChar - '0'); // Convert '1' to 7, '2' to 6, ..., '8' to 0
    return make_pair(row, col);
}

// Function to get the color of a piece ('w' for white, 'b' for black)
char getPieceColor(char piece) {
    if (isupper(piece)) return 'w';
    if (islower(piece)) return 'b';
    return ' '; // Empty square
}

// Function to check if two pieces are of different colors
bool areDifferentColors(char piece1, char piece2) {
    if (piece1 == '.' || piece2 == '.') return false;
    return getPieceColor(piece1) != getPieceColor(piece2);
}

// Function to generate pseudo-legal moves for a piece at (row, col)
vector<pair<int, int>> getPseudoLegalMoves(int row, int col) {
    vector<pair<int, int>> moves;
    char piece = board[row][col];
    char color = getPieceColor(piece);
    if (color == ' ') return moves; // Empty square

    piece = tolower(piece); // Simplify by using lowercase

    if (piece == 'p') { // Pawn
        int direction = (color == 'w') ? -1 : 1; // White moves up, black moves down
        int startRow = (color == 'w') ? 6 : 1;
        // Move forward one square
        int toRow = row + direction;
        if (isValidPosition(toRow, col) && board[toRow][col] == '.') {
            moves.push_back(make_pair(toRow, col));
            // Move forward two squares from starting position
            if (row == startRow) {
                toRow = row + 2 * direction;
                if (board[toRow][col] == '.' && board[row + direction][col] == '.') {
                    moves.push_back(make_pair(toRow, col));
                }
            }
        }
        // Capture diagonally
        int captures[2] = {col - 1, col + 1};
        for (int c : captures) {
            if (isValidPosition(toRow, c) && board[toRow][c] != '.' && areDifferentColors(board[row][col], board[toRow][c])) {
                moves.push_back(make_pair(toRow, c));
            }
        }
    } else if (piece == 'n') { // Knight
        int offsets[8][2] = {{2,1},{2,-1},{-2,1},{-2,-1},{1,2},{1,-2},{-1,2},{-1,-2}};
        for (auto& offset : offsets) {
            int r = row + offset[0];
            int c = col + offset[1];
            if (isValidPosition(r, c) && (board[r][c] == '.' || areDifferentColors(board[row][col], board[r][c]))) {
                moves.push_back(make_pair(r, c));
            }
        }
    } else if (piece == 'b') { // Bishop
        int directions[4][2] = {{1,1},{1,-1},{-1,1},{-1,-1}};
        for (auto& dir : directions) {
            int r = row, c = col;
            while (true) {
                r += dir[0];
                c += dir[1];
                if (!isValidPosition(r, c)) break;
                if (board[r][c] == '.') {
                    moves.push_back(make_pair(r, c));
                } else {
                    if (areDifferentColors(board[row][col], board[r][c])) {
                        moves.push_back(make_pair(r, c));
                    }
                    break;
                }
            }
        }
    } else if (piece == 'r') { // Rook
        int directions[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
        for (auto& dir : directions) {
            int r = row, c = col;
            while (true) {
                r += dir[0];
                c += dir[1];
                if (!isValidPosition(r, c)) break;
                if (board[r][c] == '.') {
                    moves.push_back(make_pair(r, c));
                } else {
                    if (areDifferentColors(board[row][col], board[r][c])) {
                        moves.push_back(make_pair(r, c));
                    }
                    break;
                }
            }
        }
    } else if (piece == 'q') { // Queen
        // Combine bishop and rook moves
        int directions[8][2] = {{1,1},{1,-1},{-1,1},{-1,-1},{1,0},{-1,0},{0,1},{0,-1}};
        for (auto& dir : directions) {
            int r = row, c = col;
            while (true) {
                r += dir[0];
                c += dir[1];
                if (!isValidPosition(r, c)) break;
                if (board[r][c] == '.') {
                    moves.push_back(make_pair(r, c));
                } else {
                    if (areDifferentColors(board[row][col], board[r][c])) {
                        moves.push_back(make_pair(r, c));
                    }
                    break;
                }
            }
        }
    } else if (piece == 'k') { // King
        int offsets[8][2] = {{1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1}};
        for (auto& offset : offsets) {
            int r = row + offset[0];
            int c = col + offset[1];
            if (isValidPosition(r, c) && (board[r][c] == '.' || areDifferentColors(board[row][col], board[r][c]))) {
                moves.push_back(make_pair(r, c));
            }
        }
    }
    return moves;
}

// Function to find the king's position for a given color
pair<int, int> findKing(char color) {
    char king = (color == 'w') ? 'K' : 'k';
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (board[row][col] == king) {
                return make_pair(row, col);
            }
        }
    }
    return make_pair(-1, -1); // King not found (shouldn't happen)
}

// Function to check if a square is attacked by the opponent
bool isSquareAttacked(int row, int col, char attackerColor) {
    // Temporarily, we'll check if any opponent piece can move to this square
    char opponentColor = (attackerColor == 'w') ? 'b' : 'w';
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if (getPieceColor(board[r][c]) == attackerColor) {
                vector<pair<int, int>> moves = getPseudoLegalMoves(r, c);
                for (auto& move : moves) {
                    if (move.first == row && move.second == col) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

// Function to check if the king of the given color is in check
bool isKingInCheck(char color) {
    pair<int, int> kingPos = findKing(color);
    char opponentColor = (color == 'w') ? 'b' : 'w';
    return isSquareAttacked(kingPos.first, kingPos.second, opponentColor);
}

// Function to check if a move is legal (doesn't leave king in check)
bool isLegalMove(int fromRow, int fromCol, int toRow, int toCol) {
    // First, check if the move is pseudo-legal
    vector<pair<int, int>> moves = getPseudoLegalMoves(fromRow, fromCol);
    bool isPseudoLegal = false;
    for (auto& move : moves) {
        if (move.first == toRow && move.second == toCol) {
            isPseudoLegal = true;
            break;
        }
    }
    if (!isPseudoLegal) return false;

    // Temporarily make the move
    char movingPiece = board[fromRow][fromCol];
    char capturedPiece = board[toRow][toCol];
    board[toRow][toCol] = movingPiece;
    board[fromRow][fromCol] = '.';

    // Check if own king is in check
    char currentColor = getPieceColor(movingPiece);
    bool inCheck = isKingInCheck(currentColor);

    // Undo the move
    board[fromRow][fromCol] = movingPiece;
    board[toRow][toCol] = capturedPiece;

    return !inCheck;
}

// Function to check if the current player has any legal moves
bool hasLegalMoves(char color) {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (getPieceColor(board[row][col]) == color) {
                vector<pair<int, int>> moves = getPseudoLegalMoves(row, col);
                for (auto& move : moves) {
                    if (isLegalMove(row, col, move.first, move.second)) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

// Function to make a move and check for check/checkmate
void makeMove(int fromRow, int fromCol, int toRow, int toCol) {
    // Move the piece
    char movingPiece = board[fromRow][fromCol];
    board[toRow][toCol] = movingPiece;
    board[fromRow][fromCol] = '.';

    // Switch turns
    whiteTurn = !whiteTurn;

    // Check if the opponent is in check
    char opponentColor = (whiteTurn ? 'w' : 'b');
    if (isKingInCheck(opponentColor)) {
        cout << "Check!" << endl;
        if (!hasLegalMoves(opponentColor)) {
            cout << "Checkmate! " << (whiteTurn ? "Black" : "White") << " wins!" << endl;
            exit(0); // End the game
        }
    }
}

// Main function to run the chess engine
int main() {
    // Initialize the chessboard
    initializeBoard();

    while (true) {
        // Display the board
        displayBoard();

        // Get user input for move
        string from, to;
        cout << "Enter move (e.g., 'e2 e4') or 'quit': ";
        cin >> from;
        if (from == "quit") {
            cout << "Thanks for playing! - ARTLEST\n";
            break;
        }
        cin >> to;

        // Parse positions
        pair<int, int> fromPos = parsePosition(from);
        pair<int, int> toPos = parsePosition(to);
        if (fromPos.first == -1 || toPos.first == -1) {
            cout << "Invalid position format! Use 'e2' to 'e4'.\n";
            continue;
        }

        int fromRow = fromPos.first, fromCol = fromPos.second;
        int toRow = toPos.first, toCol = toPos.second;

        // Check if there's a piece at the "from" position
        if (board[fromRow][fromCol] == '.') {
            cout << "No piece at " << from << "!\n";
            continue;
        }

        // Check if the piece belongs to the current player
        char pieceColor = getPieceColor(board[fromRow][fromCol]);
        if ((whiteTurn && pieceColor != 'w') || (!whiteTurn && pieceColor != 'b')) {
            cout << "It's " << (whiteTurn ? "White's" : "Black's") << " turn!\n";
            continue;
        }

        // Check if the move is legal
        if (isLegalMove(fromRow, fromCol, toRow, toCol)) {
            makeMove(fromRow, fromCol, toRow, toCol);
        } else {
            cout << "Illegal move! Try again.\n";
        }
    }

    return 0;
}