#include <iostream>
#include <vector>

using namespace std;

class TicTacToe {
public:
    TicTacToe();
    void playGame();

private:
    void displayBoard();
    bool makeMove();
    bool checkWin();
    bool checkDraw();
    void switchPlayers();
    void resetGame();

    char currentPlayer;
    vector<vector<char>> board;
};

TicTacToe::TicTacToe() : currentPlayer('X'), board(3, vector<char>(3, ' ')) {}

void TicTacToe::displayBoard() {
    cout << "  0 1 2\n";
    for (int i = 0; i < 3; ++i) {
        cout << i << " ";
        for (int j = 0; j < 3; ++j) {
            cout << board[i][j] << " ";
        }
        cout << "\n";
    }
}

bool TicTacToe::makeMove() {
    int row, col;
    cout << "Player " << currentPlayer << ", enter your move (row and column): ";
    cin >> row >> col;

    if (row < 0 || row >= 3 || col < 0 || col >= 3 || board[row][col] != ' ') {
        cout << "Invalid move. Try again.\n";
        return false;
    }

    board[row][col] = currentPlayer;
    return true;
}

bool TicTacToe::checkWin() {
    // Check rows and columns
    for (int i = 0; i < 3; ++i) {
        if ((board[i][0] == currentPlayer && board[i][1] == currentPlayer && board[i][2] == currentPlayer) ||
            (board[0][i] == currentPlayer && board[1][i] == currentPlayer && board[2][i] == currentPlayer)) {
            return true;
        }
    }

    // Check diagonals
    if ((board[0][0] == currentPlayer && board[1][1] == currentPlayer && board[2][2] == currentPlayer) ||
        (board[0][2] == currentPlayer && board[1][1] == currentPlayer && board[2][0] == currentPlayer)) {
        return true;
    }

    return false;
}

bool TicTacToe::checkDraw() {
    for (const auto &row : board) {
        for (char cell : row) {
            if (cell == ' ') {
                return false;
            }
        }
    }
    return true;
}

void TicTacToe::switchPlayers() {
    currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
}

void TicTacToe::resetGame() {
    currentPlayer = 'X';
    board = vector<vector<char>>(3, vector<char>(3, ' '));
}

void TicTacToe::playGame() {
    while (true) {
        displayBoard();

        if (makeMove()) {
            if (checkWin()) {
                displayBoard();
                cout << "Player " << currentPlayer << " wins!\n";
                break;
            } else if (checkDraw()) {
                displayBoard();
                cout << "It's a draw!\n";
                break;
            }

            switchPlayers();
        }
    }

    char playAgain;
    cout << "Do you want to play again? (y/n): ";
    cin >> playAgain;

    if (playAgain == 'y' || playAgain == 'Y') {
        resetGame();
        playGame();
    }
}

int main() {
    TicTacToe game;
    game.playGame();

    return 0;
}