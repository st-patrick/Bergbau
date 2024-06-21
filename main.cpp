#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include <vector>

// Screen dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Rectangle dimensions
const int CELL_PIXEL_SIZE = 50;

// Grid dimensions
const int GRID_COLS = SCREEN_WIDTH / CELL_PIXEL_SIZE;
const int GRID_ROWS = SCREEN_HEIGHT / CELL_PIXEL_SIZE;

// Cell states
enum CellState { PASSABLE, UNPASSABLE };

struct Player {
    struct Position {
		int row = 5;
		int column = 5;
	} position;
} player;


// Function to render text
void renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y) {
    SDL_Color textColor = { 0, 0, 0, 255 };
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Rect renderQuad = { x, y, textSurface->w, textSurface->h };
    SDL_RenderCopy(renderer, textTexture, nullptr, &renderQuad);

    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}

int main(int argc, char* args[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Load font
    TTF_Font* font = TTF_OpenFont("assets/This Cafe.ttf", 24);
    if (font == nullptr) {
        std::cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Create window
    SDL_Window* window = SDL_CreateWindow("SDL2 Grid with Text",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        TTF_CloseFont(font);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        TTF_CloseFont(font);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }


    // setup player


    // Set the initial position of the rectangle
    SDL_Rect rect = { player.position.column * CELL_PIXEL_SIZE, player.position.row * CELL_PIXEL_SIZE, CELL_PIXEL_SIZE, CELL_PIXEL_SIZE };

    // Initialize grid with some unpassable cells
    std::vector<std::vector<CellState>> grid(GRID_ROWS, std::vector<CellState>(GRID_COLS, PASSABLE));
    // Example: Making some cells unpassable
    grid[5][5] = UNPASSABLE;
    grid[4][7] = UNPASSABLE;
    grid[6][3] = UNPASSABLE;

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        // Handle events on the queue
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else if (e.type == SDL_KEYDOWN) {
                int newRow = player.position.row;
                int newColumn = player.position.column;
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                    case SDLK_w:
                        newRow -= 1;
                        break;
                    case SDLK_DOWN:
                    case SDLK_s:
                        newRow += 1;
                        break;
                    case SDLK_LEFT:
                    case SDLK_a:
                        newColumn -= 1;
                        break;
                    case SDLK_RIGHT:
                    case SDLK_d:
                        newColumn += 1;
                        break;
                }


                // Check bounds and cell state
                if (newColumn >= 0 && newColumn < GRID_COLS &&
                    newRow >= 0 && newRow < GRID_ROWS &&
                    grid[newRow][newColumn] == PASSABLE) {

                    player.position.row = newRow;
                    player.position.column = newColumn;

                    // convert grid coordinates to pixel coordinates
                    int fromGridX = player.position.column * CELL_PIXEL_SIZE;
                    int fromGridY = player.position.row * CELL_PIXEL_SIZE;

                    rect.x = fromGridX;
                    rect.y = fromGridY;
                }
            }
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Render the grid with cell numbers and unpassable cells
        for (int row = 0; row < GRID_ROWS; ++row) {
            for (int col = 0; col < GRID_COLS; ++col) {
                if (grid[row][col] == UNPASSABLE) {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 139, 255); // Dark blue
                    SDL_Rect cellRect = { col * CELL_PIXEL_SIZE, row * CELL_PIXEL_SIZE, CELL_PIXEL_SIZE, CELL_PIXEL_SIZE };
                    SDL_RenderFillRect(renderer, &cellRect);
                }
                else {
                    std::string cellNumber = std::to_string(row * GRID_COLS + col);
                    renderText(renderer, font, cellNumber, col * CELL_PIXEL_SIZE + 10, row * CELL_PIXEL_SIZE + 10);
                }
            }
        }

        // Set render color to black (rect will be black)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        // Render the rectangle
        SDL_RenderFillRect(renderer, &rect);

        // Update screen
        SDL_RenderPresent(renderer);
    }

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
