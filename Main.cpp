#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <iomanip>

using namespace std;

class Cell {
public:
    bool isMine;
    bool isRevealed; // must be true to call the adjacent open cells fucntion we made in sfml as well
    int adjacentMines;

    Cell() : isMine(false), isRevealed(false), adjacentMines(0) {}
};

class Minesweeper {
private:
    int n;
    vector<vector<Cell>> board;
    vector<vector<bool>> revealed;
    bool isFirstMove;
    int cellsToReveal;

    const vector<pair<int, int>> directions = {
        {-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
        {0, 1}, {1, -1}, {1, 0}, {1, 1}
    };

    void placeMines(int safeX, int safeY) {
        srand(time(0));
        int totalMines = n * n / 6; // simple formula as per microsoft's minesweeper to decide number of the mines on the board based on size of grid
        int placedMines = 0; // we had fixed mines on sfml one but this is a counter so that we have current mines <= total mines

        while (placedMines < totalMines) {
            int x = rand() % n;
            int y = rand() % n;

            if ((x == safeX && y == safeY) || board[x][y].isMine) continue;

            board[x][y].isMine = true;
            placedMines++;

            for (auto [dx, dy] : directions) {
                int nx = x + dx, ny = y + dy;
                if (isValid(nx, ny) && !board[nx][ny].isMine) {
                    board[nx][ny].adjacentMines++;
                }
            }
        }
    }

    bool isValid(int x, int y) {
        return x >= 0 && x < n && y >= 0 && y < n;
    }

    void revealCell(int x, int y) {
        if (!isValid(x, y) || board[x][y].isRevealed) return;

        board[x][y].isRevealed = true;
        cellsToReveal--;

        // If no adjacent mines, reveal neighbors - empty cell behavior we made in the beginning of the minesweeper game on sfml
        if (board[x][y].adjacentMines == 0) {
            for (auto [dx, dy] : directions) {
                int nx = x + dx, ny = y + dy;
                revealCell(nx, ny);
            }
        }
    }

    void displayBoard(bool showMines = false) {
        cout << "\nCurrent Board:\n\n";
        cout << "   ";
        for (int i = 0; i < n; ++i) cout << setw(3) << i;
        cout << "\n";

        for (int i = 0; i < n; ++i) {
            cout << setw(3) << i;
            for (int j = 0; j < n; ++j) {
                if (board[i][j].isRevealed) {
                    if (board[i][j].isMine)
                        cout << "  *";
                    else if (board[i][j].adjacentMines > 0)
                        cout << setw(3) << board[i][j].adjacentMines;
                    else
                        cout << "   ";
                } else if (showMines && board[i][j].isMine) {
                    cout << "  *";
                } else {
                    cout << "  #";
                }
            }
            cout << "\n";
        }
    }

public:
    Minesweeper(int size) : n(size), isFirstMove(true) {
        board.resize(n, vector<Cell>(n));
        cellsToReveal = n * n;
    }

    void play() {
        while (true) {
            displayBoard();

            int x, y;
            cout << "\nEnter your move (x y): ";
            cin >> x >> y;

            if (!isValid(x, y) || board[x][y].isRevealed) {
                cout << "Invalid move! Try again.\n";
                continue;
            }

            if (isFirstMove) {
                placeMines(x, y);
                isFirstMove = false;
            }

            if (board[x][y].isMine) {
                displayBoard(true);
                cout << "\nBOOM! You hit a mine. Game Over!\n";
                cout << "\n==============================================\n";
                cout << "||                                          ||\n";
                cout << "||               YOU HIT A MINE             ||\n";
                cout << "||               Try Again ToT              ||\n";
                cout << "||           Made by - Rishi Saxena         ||\n";
                cout << "==============================================\n";
                break;
            }
            // we had this in sfml and it does same work here as well
            revealCell(x, y);

            if (cellsToReveal == n * n / 6) {
                displayBoard(true);
                cout << "\nCongratulations! You have cleared the board!\n";
                cout << "\n==============================================\n";
                cout << "||                                          ||\n";
                cout << "||               YOU WON!                   ||\n";
                cout << "||       Congratulations on clearing        ||\n";
                cout << "||              the board!                  ||\n";
                cout << "||         Made by - Rishi Saxena           ||\n";
                cout << "==============================================\n";
                break;
            }
        }
    }
};

int main() {

    // intro and rules for the game
    cout << "...............................................................\n";
    cout << "Welcome to Minesweeper!\n";
    cout << "...............................................................\n";
    cout << "RULES and HOW TO PLAY :\n";
    cout << "...............................................................\n";
    cout << "1. You click on a cell and if it is a mine game over.\n";
    cout << "2. You click on a proximity cell it shows distance in units from a cell with a mine\n";
    cout << "3. We dont have a Flagging system here so make your moves and remember to not click on a mine \n";
    cout << "...............................................................\n";
    cout << "Edge cases might have issues so please ping me if you find any!\n";
    cout << " \n";
    int boardSize;
    cout << "Enter board size (n >= 9): ";
    cin >> boardSize;

    if (boardSize < 9) {
        cout << "Board size must be at least 9.\n";
        return 1;
    }

    Minesweeper game(boardSize);
    game.play();
}
