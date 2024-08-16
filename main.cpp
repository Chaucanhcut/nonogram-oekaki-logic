#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

using namespace std;

const int MAX_BAD_GUESSES = 3; // 3 lives 

// 3 states for each cell
const int EMPTY = 0;
const int FILLED = 1;
const int MARKED = -1;

// size of OGgrid
int grid_rows;
int grid_cols;

// function prototypes -----------------------------------------
void playNonogram();
//vector<vector<int>> chooseLevel(); 
void get_og_grid_size();
vector <vector<int>> create_playerGrid(const vector<vector<int>>& OGgrid);
void renderGame(const vector<vector<int>>& grid, const vector<vector<int>>& headerRow, const vector<vector<int>>& sideColumn, int badGuessCount);
bool contains(const vector<vector<int>> & OGgrid, const int row, const int col, const int value);

// main ------------------------------------------
int main()
{
    playNonogram();
    cout << endl << "Try again?" << endl;
    int ans;
    cin >> ans;

    if (ans == 0) {
        cout << "See you next time!" << endl;
        return 0;
    }
    else {
        do {
            playNonogram();
            cout << endl << "Try again?" << endl;
            cin >> ans;
        } while (ans == 1);
    }
    return 0;
}

// define functions ------------------------------------------

void playNonogram() {
    // Initializing
    int badGuessCount = 0;
    get_og_grid_size();
        
    vector<vector<int>> OGgrid = {
        {1, 1, 1, 0, 0, 0, 0},
        {0, 1, 1, 0, 0, 0, 0},
        {1, 0, 1, 1, 1, 0, 0},
        {0, 1, 1, 0, 0, 0, 0},
        {1, 1, 1, 0, 0, 0, 0}
    };

    vector<vector<int>> headerRow = {
        {1, 0, 0, 0 ,0, 0, 0},
        {1, 2, 0, 0, 0, 0, 0},
        {1, 2, 5, 1, 1, 0, 0}
    };

    vector<vector<int>> sideColumn = {
        {0, 0, 0, 3},
        {0, 0, 0, 2},
        {0, 0, 1, 3},
        {0, 0, 0, 2},
        {0, 0, 0, 3}
    };

    vector<vector<int>> playerGrid = create_playerGrid(OGgrid); // grid to display all 3 cell states
    vector<vector<int>> checkGrid = playerGrid; // grid to compare with OGgrid, only has 2 cell states: 0 or 1

    // game loop
    do {
        renderGame(playerGrid, headerRow, sideColumn, badGuessCount);

        // get player's guess: player select & set value for a cell
        int row, col, value;
        cout << endl << "Choose row, column and set value: ";
        cin >> row >> col >> value;

        if (contains(OGgrid, row, col, value)) {
            // Update player's grid
            playerGrid[row][col] = value;
            if (value == -1) {
                checkGrid[row][col] = 0;
            }
            else {
                checkGrid[row][col] = value;
            }
        }
        else badGuessCount++;
    } while (badGuessCount < MAX_BAD_GUESSES && OGgrid != checkGrid);

    // show final game result
    renderGame(playerGrid, headerRow, sideColumn, badGuessCount);
    cout << endl;
    if (badGuessCount != MAX_BAD_GUESSES) {
        cout << "Congratulations, you solved the puzzle!" << endl;
    }
    else {
        cout << "You lost! Good luck next time!" << endl;
    }
}

/*

vector<vector<int>> chooseLevel() {

}
*/

void get_og_grid_size() {
    grid_rows = 5;
    grid_cols = 7;
}

vector <vector<int>> create_playerGrid(const vector<vector<int>>& OGgrid) {
    vector<vector<int>> playerGrid = OGgrid;
    for (auto i = 0; i < playerGrid.size(); i++) {
        for (auto j = 0; j < playerGrid[i].size(); j++) {
            playerGrid[i][j] = 0;
        }
    }
    return playerGrid;
}

void renderGame(const vector<vector<int>>& grid, const vector<vector<int>>& headerRow, const vector<vector<int>>& sideColumn, int badGuessCount) {
    const int PATCH_LINES = 30;
    for (int i = 0; i < PATCH_LINES; i++) cout << endl;

    cout << "You have " << 3 - badGuessCount << " lives left!" << endl << endl;
    
    // print header row
    // headerRow.size() == ceil(grid_rows * 0.5)
    for (int i = 0; i < (int)ceil(grid_rows * 0.5); ++i) {
        // print space to header rows
        for (int j = 0; j < (int)sideColumn[0].size(); j++) {
            cout << setw(2) << setfill(' ') << " ";
        }

        // print header rows
        for (int k = 0; k < (int)headerRow[i].size(); k++) {
            if (headerRow[i][k] != 0) {
                cout << setw(2) << setfill('|') << headerRow[i][k];
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
    for (int i = 0; i < grid_rows; ++i) {
        // side column - print each row
        for (int j = 0; j < (int)ceil(grid_cols * 0.5); j++) {
            if (sideColumn[i][j] != 0) {
                cout << setw(2) << setfill(' ') << sideColumn[i][j];
            }
            else {
                cout << setw(2) << setfill(' ') << " ";
            }
        }

        // print player grid
        for (int j = 0; j < grid_cols; ++j) {
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
    if (value == 0 || value == 1) {
        if (OGgrid[row][col] == value) return true;
        else return false;
    } 
    else if (value == -1) {
        if (OGgrid[row][col] == 0) return true;
        else return false;
    }
    return false;
}

