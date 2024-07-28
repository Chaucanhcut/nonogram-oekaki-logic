#include <iostream>
#include <vector>

using namespace std;

const int EMPTY = 0;
const int FILLED = 1;

void displayGrid(const vector<vector<int>>& grid);
bool checkRow(const vector<int>& row, const vector<int>& numbers);
bool checkColumn(const vector<int>& column, const vector<int>& numbers);
bool isSolved(const vector<vector<int>>& grid, const vector<vector<int>>& rowNumbers, const vector<vector<int>>& colNumbers);

int main()
{
    // Initializing
    vector<vector<int>> grid = {
        {0, 0, 0, 0},
        {0, 0, 1, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0}
    };

    vector<vector<int>> rowNumbers = {
        {0},
        {1},
        {2},
        {0}
    };

    vector<vector<int>> colNumbers = {
        {0},
        {1},
        {2},
        {0}
    };

    // game loop
    while (!isSolved(grid, rowNumbers, colNumbers)) {
        // Display the grid
        displayGrid(grid);

        // User select and set value for a grid
        int row, col, value;
        cout << "Enter row, column, and value: ";
        cin >> row >> col >> value;

        // Update the grid
        grid[row][col] = value;

        // Check for valid move
        if (!checkRow(grid[row], rowNumbers[row]) || !checkColumn(grid[col], colNumbers[col])) {
            cout << "Invalid move!" << endl;
        }
    }
    cout << "Congratulations, you solved the puzzle!" << endl;
    return 0;
}

void displayGrid(const vector<vector<int>>& grid) {
    for (const auto& row : grid) {
        for (int cell : row) {
            cout << (cell == FILLED ? 'X' : ' ');
        }
        cout << endl;
    }
}

bool checkRow(const vector<int>& row, const vector<int>& numbers) {
    // Check if the row is valid according to the given numbers
    int numGroups = 0;
    int currentGroup = 0;
    for (int i = 0; i < row.size(); ++i) {
        if (row[i] == FILLED) {
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

bool isSolved(const vector<vector<int>>& grid, const vector<vector<int>>& rowNumbers, const vector<vector<int>>& colNumbers) {
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
        if (!checkRow(grid[i], rowNumbers[i]) || !checkColumn(grid[i], colNumbers[i])) {
            return false;
        }
    }

    return true;
}

