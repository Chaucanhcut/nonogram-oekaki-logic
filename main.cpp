#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <string>
#include <cctype>
// SDL and standard IO
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

using namespace std;

//SDL-------------------------------------------------------------------
// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int RENDERER_DRAW_COLOR = 0xFF;

//The window we'll be rendering to
SDL_Window* window = NULL;
//The surface contained by the window
SDL_Surface* screenSurface = NULL;

SDL_Renderer* ren = NULL;
Mix_Music* gMusic = NULL;

//SDL functions----------------------------------------------------------------
bool initSDL();
bool loadMedia();

//GAME LOGIC-------------------------------------------------------------------
const int MAX_BAD_GUESSES = 3; // 3 lives 

// 3 states for each cell
const int EMPTY = 0;
const int FILLED = 1;
const int MARKED = -1;

// size of OGgrid
int grid_rows;
int grid_cols;

// vectors to save map - OGgrid, headerRow, side Column
vector<vector<int>> OGgrid;
vector<vector<int>> headerRow;
vector<vector<int>> sideColumn;

// function prototypes -----------------------------------------
void playNonogram();
vector<vector<int>> slicing(vector<vector<int>>& arr, int x, int y);
void chooseLevel(const char* fileName); 
void get_og_grid_size(int row, int col);
vector <vector<int>> create_playerGrid(const vector<vector<int>>& OGgrid);
void renderGame(const vector<vector<int>>& grid, const vector<vector<int>>& headerRow, const vector<vector<int>>& sideColumn, int badGuessCount);
bool contains(const vector<vector<int>> & OGgrid, const int row, const int col, const int value);

// main ------------------------------------------
int main(int argc, char* args[])
{
    // Start SDL & create window
    if (!initSDL()) {
        cout << "Failed to initialize!\n" << endl;
        return -1;
    }
    else {
        //Load media
        if (!loadMedia()) {
            cout << "Failed to load media!\n" << endl;
            return -1;
        }
        Mix_PlayMusic(gMusic, -1);
    }
    
    SDL_Delay(10000);

    //Destroy window - void close()
    SDL_DestroyWindow(window);
    window = NULL;

    //Quit SDL subsystems
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();

    /// CONSOLE PLAY ---------------------------------------
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
/// SDL functions
bool initSDL() {
    // init flag
    bool success = true;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        cout << "SDL could not initialize! SDL_Error: %s\n" << SDL_GetError() << endl;
        //success = false;
    }
    else {
        //Create window
        window = SDL_CreateWindow("NONOGRAM", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL) {
            cout << "Window could not be created! SDL_Error: %s\n" << SDL_GetError() << endl;
            //success = false;
        }
        else {
            // //Get window surface
            // screenSurface = SDL_GetWindowSurface(window);
            // 
            // //Fill the surface white
            // SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
            // 
            // //Update the surface
            // SDL_UpdateWindowSurface(window);
            // 
            // //Hack to get window to stay up
            // SDL_Event e; 
            // bool quit = false; 
            // while (quit == false) { 
            //     while (SDL_PollEvent(&e)) { 
            //         if (e.type == SDL_QUIT) quit = true; 
            //     } 
            // }

             //Create renderer for window
            ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (ren == NULL) {
                cout << "Renderer could not be created! SDL Error: %s\n" << SDL_GetError() << endl;
                //success = false;
            }
            else {
                //Initialize renderer color
                SDL_SetRenderDrawColor(ren, RENDERER_DRAW_COLOR, RENDERER_DRAW_COLOR, RENDERER_DRAW_COLOR, RENDERER_DRAW_COLOR);

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    cout << "SDL_image could not initialize! SDL_image Error: %s\n" << IMG_GetError() << endl;
                    //success = false;
                }

                //Initialize SDL_mixer
                if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
                    cout << "SDL_mixer could not initialize! SDL_mixer Error: %s\n" << Mix_GetError() << endl;
                    //success = false;
                }

                //Initialize SDL_ttf
                if (TTF_Init() == -1) {
                    cout << "SDL_ttf could not initialize! SDL_ttf Error: %s\n" << TTF_GetError() << endl;
                    //success = false;
                }
            }
        }
    }

    return success;
}

bool loadMedia() {
    // flag
    bool success = true;
    
    // load images
    // TO-DO

    // load music
    gMusic = Mix_LoadMUS("resource/audio/beat.mp3");
    if (gMusic == NULL) {
        cout << "Failed to load background music! SDL_mixer Error: %s\n" << Mix_GetError() << endl;
        success = false;
    }
    return success;
}

/// game logic functions
void playNonogram() {
    // Initializing
    int badGuessCount = 0;

    cout << "Choose difficulty: E - M - H: ";
    char level;
    cin >> level;
    level = toupper(level);

    switch (level) {
    case 'E':
        get_og_grid_size(5, 5);
        chooseLevel("resource/maps/1E_map.txt");
        break;

    case 'M':
        get_og_grid_size(7, 9);
        chooseLevel("resource/maps/1M_map.txt");
        break;

    case 'H':
        get_og_grid_size(9, 9);
        chooseLevel("resource/maps/1H_map.txt");
        break;
    }
    

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

vector<vector<int>> slicing(vector<vector<int>>& arr, int x, int y) {
    // Starting and Ending iterators: slice from x to y
    auto start = arr.begin() + x;
    auto end = arr.begin() + y + 1;

    // To store the sliced vector
    vector<vector<int>> result(y - x + 1);

    // Copy vector using copy function()
    copy(start, end, result.begin());

    // Return the final sliced vector
    return result;
}

void chooseLevel(const char* fileName) {
    vector<vector<int>> fileGrid;
    ifstream inFile(fileName);
    if (inFile.is_open()) {
        string line;
        while (getline(inFile, line)) {
            vector<int> row_data;

            for (char& c : line) {
                if (c != ' ') {
                    // convert whole file from string to int values 
                    // then add each row to 2D vector
                    row_data.push_back(c - '0');
                }
            }
            fileGrid.push_back(row_data);
        }
        inFile.close();
    }
    else {
        cout << "Unable to open file!";
    }

    // TO - DO: Split file grid to 3 
    OGgrid = slicing(fileGrid, 0, grid_rows - 1);
    headerRow = slicing(fileGrid, grid_rows + 1, grid_rows + (int)ceil(grid_rows * 0.5));
    sideColumn = slicing(fileGrid, grid_rows + (int)ceil(grid_rows * 0.5) + 2, (int)fileGrid.size() - 1);
}

void get_og_grid_size(int row, int col) {
    grid_rows = row;
    grid_cols = col;
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

