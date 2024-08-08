#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

const int MAX_BAD_GUESSES = 3; // 3 lives 

// 3 states for each cell
const int EMPTY = 0;
const int FILLED = 1;
const int MARKED = -1;

void displayGrid(const vector<vector<int>>& grid, const vector<vector<int>>& headerRow, const vector<vector<int>>& sideColumn);
bool checkRow(const vector<int>& row, const vector<int>& numbers);
bool checkColumn(const vector<int>& column, const vector<int>& numbers);
bool isSolved(const vector<vector<int>>& grid, const vector<vector<int>>& headerRow, const vector<vector<int>>& sideColumn);

int main()
{
    // Initializing
    vector<vector<int>> grid = {
        {1, 1, 1, 0, 0},
        {0, 1, 1, 0, 0},
        {1, 0, 1, 1, 1},
        {0, 1, 1, 0, 0},
        {1, 1, 1, 0, 0}
    };
    
    //vector<vector<int>> headerRow = {
    //    {1, 1, 1},
    //    {2, 2},
    //    {5},
    //    {1},
    //    {1}
    //};

    vector<vector<int>> headerRow = {
        {1},
        {1, 2},
        {1, 2, 5, 1, 1}
    };

    vector<vector<int>> sideColumn = {
        {3},
        {2},
        {1, 3},
        {2},
        {3}
    };

    vector<vector<int>> playerGrid = {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}
    };

    // game loop
    while (!isSolved(grid, headerRow, sideColumn)) {
        // Display the grid
        displayGrid(playerGrid, headerRow, sideColumn);

        // User select and set value for a grid
        int row, col, value;
        cout << "Enter row, column, and value: ";
        cin >> row >> col >> value;

        // Update the grid
        playerGrid[row][col] = value;

        // Check for valid move
        if (!checkRow(playerGrid[row], headerRow[row]) || !checkColumn(playerGrid[col], sideColumn[col])) {
            cout << "Invalid move!" << endl;
            // lose 1 life
        }
    }
    cout << "Congratulations, you solved the puzzle!" << endl;
    return 0;
}

void displayGrid(const vector<vector<int>>& grid, const vector<vector<int>>& headerRow, const vector<vector<int>>& sideColumn) {
    // calculate grid size (or can add more inputs)
    int rows = (int)grid.size();
    int cols = (int)grid[0].size();

    // print header row
    //for (int i = 0; i < rows; ++i) {
    for (int i = 0; i < (int)headerRow.size(); ++i) {
        for (int num : headerRow[i]) {
            cout << setw(2) << setfill('|') << num;
        }
        //cout << "|";
        cout << endl;
    }
    cout << endl;

    // print side column
    for (int i = 0; i < cols; ++i) {
        for (int num : sideColumn[i]) {
            cout << setw(2) << num;
        }
        cout << " ";

        for (int j = 0; j < cols; ++j) {
            if (grid[i][j] == FILLED) {
                cout << (char)254u << "|"; // filled square
            }
            else if (grid[i][j] == MARKED) {
                cout << ".|";
            }
            else { // cell == EMPTY
                cout << " |";
            }
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

