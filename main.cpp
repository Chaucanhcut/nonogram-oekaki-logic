#include <iostream>
#include <vector>

using namespace std;

// 3 states for each cell
const int EMPTY = 0;
const int FILLED = 1;
const int MARKED = -1;

// function prototypes -----------------------------------------
//void chooseLevel() - read from file 
void renderGame(const vector<vector<int>>& grid, const vector<vector<int>>& headerRow, const vector<vector<int>>& sideColumn, int badGuessCount);
bool contains(const vector<vector<int>> & OGgrid, const int row, const int col, const int value);
bool checkRow(const vector<int>& row, const vector<int>& numbers);
bool checkColumn(const vector<int>& column, const vector<int>& numbers);
bool isSolved(const vector<vector<int>>& grid, const vector<vector<int>>& headerRow, const vector<vector<int>>& sideColumn);

// main ------------------------------------------
int main()
{
    // Initializing
    int badGuessCount = 0;

    vector<vector<int>> playerGrid = {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}
    };

    vector<vector<int>> OGgrid = {
        {1, 1, 1, 0, 0},
        {0, 1, 1, 0, 0},
        {1, 0, 1, 1, 1},
        {0, 1, 1, 0, 0},
        {1, 1, 1, 0, 0}
    };

    vector<vector<int>> headerRow = {
        {1, 0, 0, 0 ,0},
        {1, 2, 0, 0, 0},
        {1, 2, 5, 1, 1}
    };

    vector<vector<int>> sideColumn = {
        {0, 3},
        {0, 2},
        {1, 3},
        {0, 2},
        {0, 3}
    };

    // game loop
    do {
        renderGame(playerGrid, headerRow, sideColumn, badGuessCount);
        
        // get player's guess: player select & set value for a cell
        int row, col, value;
        cout << endl << "Choose a row, column and value: ";
        cin >> row >> col >> value;

        if (contains(OGgrid, row, col, value)) {
            // Update player's grid
            playerGrid[row][col] = value;
        }
        else badGuessCount++;
    } while (badGuessCount < MAX_BAD_GUESSES && OGgrid != playerGrid);

    renderGame(playerGrid, headerRow, sideColumn, badGuessCount);
    if (badGuessCount != MAX_BAD_GUESSES) {
        cout << "Congratulations, you solved the puzzle!" << endl;
    }
    else {
        cout << "You lost! Good luck next time!" << endl;
    }
    return 0;
}

// define functions ------------------------------------------
//void chooseLevel() {}

void renderGame(const vector<vector<int>>& grid, const vector<vector<int>>& headerRow, const vector<vector<int>>& sideColumn, int badGuessCount) {
    const int PATCH_LINES = 30;
    for (int i = 0; i < PATCH_LINES; i++) cout << endl;

    cout << "You have " << 3 - badGuessCount << " lives left!" << endl << endl;
    
    // calculate grid size (or can add more inputs)
    int rows = (int)grid.size();
    int cols = (int)grid[0].size();

    // print header row
    for (int i = 0; i < (int)headerRow.size(); ++i) {
        // print space to header rows
        for (int num : sideColumn[0]) {
            cout << setw(2) << setfill(' ') << " ";
        }

        // print header rows
        for (int num : headerRow[i]) {
            if (num != 0) {
                cout << setw(2) << setfill('|') << num;
            }
            else {
                cout << setw(2) << setfill('|') << " ";
            }
        } 
        cout << endl;
    }

    // separate line
    cout << endl;

    // print side column and then player grid 
    for (int i = 0; i < cols; ++i) {
        // side column - print each row 
        for (int num : sideColumn[i]) {
            if (num != 0) {
                cout << setw(2) << setfill('|') << num;
            }
            else {
                cout << setw(2) << setfill('|') << " ";
            }
        }

        // print player grid
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j] == FILLED) {
                cout << "|" << (char)254u; // filled square
            }
            else if (grid[i][j] == MARKED) {
                cout << "|.";
            }
            else { // cell == EMPTY
                cout << "| ";
            }
        }
        cout << "|" << endl;
    }
}

bool contains(const vector<vector<int>>& OGgrid, const int row, const int col, const int value) {
    if (OGgrid[row][col] == value) return true;
    return false;
}

bool checkRow(const vector<int>& row, const vector<int>& numbers) {
    // Check if the row is valid according to the given numbers
    int numGroups = 0;
    int currentGroup = 0;
    for (int i = 0; i < row.size(); ++i) {
        if (row[i] == FILLED) {
            currentGroup++;
        } else { // row[i] is empty or marked
            if (currentGroup > 0) {
                numGroups++;
                if (numGroups > numbers.size() || currentGroup != numbers[numGroups - 1]) {
                    return false;
                }
                currentGroup = 0; // start counting a new number group
            }
        }
    }
    return numGroups == numbers.size() && currentGroup == numbers.back();
}

bool checkColumn(const vector<int>& column, const vector<int>& numbers) {
    // Check if the column is valid according to the given numbers
    int numGroups = 0;
    int currentGroup = 0;
    for (int i = 0; i < column.size(); ++i) {
        if (column[i] == FILLED) {
            currentGroup++;
        } else {
            if (currentGroup > 0) {
                numGroups++;
                if (numGroups > numbers.size() || currentGroup != numbers[numGroups - 1]) {
                    return false;
                }
                currentGroup = 0;
            }
        }
    }
    return numGroups == numbers.size() && currentGroup == numbers.back();
}

bool isSolved(const vector<vector<int>>& grid, const vector<vector<int>>& headerRow, const vector<vector<int>>& sideColumn) {
    // Check if the grid is completely filled and valid
    for (const auto& row : grid) {
        for (int cell : row) {
            if (cell == EMPTY) {
                return false;
            }
        }
    }

    // Check if all rows and columns are valid
    for (int i = 0; i < grid.size(); ++i) {
        if (!checkRow(grid[i], headerRow[i]) || !checkColumn(grid[i], sideColumn[i])) {
            return false;
        }
    }

    return true;
}

