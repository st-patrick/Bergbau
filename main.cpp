#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>

// Screen dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Rectangle dimensions
const int RECT_SIZE = 50;

// Grid dimensions
const int GRID_COLS = SCREEN_WIDTH / RECT_SIZE;
const int GRID_ROWS = SCREEN_HEIGHT / RECT_SIZE;

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

    // Set the initial position of the rectangle
    SDL_Rect rect = { SCREEN_WIDTH / 2 - RECT_SIZE / 2, SCREEN_HEIGHT / 2 - RECT_SIZE / 2, RECT_SIZE, RECT_SIZE };

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        // Handle events on the queue
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                case SDLK_UP:
                case SDLK_w:
                    if (rect.y > 0) rect.y -= RECT_SIZE;
                    break;
                case SDLK_DOWN:
                case SDLK_s:
                    if (rect.y < SCREEN_HEIGHT - RECT_SIZE) rect.y += RECT_SIZE;
                    break;
                case SDLK_LEFT:
                case SDLK_a:
                    if (rect.x > 0) rect.x -= RECT_SIZE;
                    break;
                case SDLK_RIGHT:
                case SDLK_d:
                    if (rect.x < SCREEN_WIDTH - RECT_SIZE) rect.x += RECT_SIZE;
                    break;
                }
            }
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Render the grid with cell numbers
        for (int row = 0; row < GRID_ROWS; ++row) {
            for (int col = 0; col < GRID_COLS; ++col) {
                std::string cellNumber = std::to_string(row * GRID_COLS + col);
                renderText(renderer, font, cellNumber, col * RECT_SIZE + 10, row * RECT_SIZE + 10);
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
