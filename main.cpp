#include <SDL.h>
#include <iostream>

// Screen dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Rectangle dimensions
const int RECT_WIDTH = 50;
const int RECT_HEIGHT = 50;

// Speed of the rectangle
const int RECT_SPEED = 5;

int main(int argc, char* args[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create window
    SDL_Window* window = SDL_CreateWindow("SDL2 Rectangle Move",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Set the initial position of the rectangle
    SDL_Rect rect = { SCREEN_WIDTH / 2 - RECT_WIDTH / 2, SCREEN_HEIGHT / 2 - RECT_HEIGHT / 2, RECT_WIDTH, RECT_HEIGHT };

    bool quit = false;
    SDL_Event e;

    // Key state array
    const Uint8* currentKeyStates = SDL_GetKeyboardState(nullptr);

    while (!quit) {
        // Handle events on the queue
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Update rectangle position based on key states
        if (currentKeyStates[SDL_SCANCODE_UP] || currentKeyStates[SDL_SCANCODE_W]) {
            rect.y -= RECT_SPEED;
        }
        if (currentKeyStates[SDL_SCANCODE_DOWN] || currentKeyStates[SDL_SCANCODE_S]) {
            rect.y += RECT_SPEED;
        }
        if (currentKeyStates[SDL_SCANCODE_LEFT] || currentKeyStates[SDL_SCANCODE_A]) {
            rect.x -= RECT_SPEED;
        }
        if (currentKeyStates[SDL_SCANCODE_RIGHT] || currentKeyStates[SDL_SCANCODE_D]) {
            rect.x += RECT_SPEED;
        }

        // Ensure the rectangle stays within bounds
        if (rect.x < 0) rect.x = 0;
        if (rect.y < 0) rect.y = 0;
        if (rect.x + RECT_WIDTH > SCREEN_WIDTH) rect.x = SCREEN_WIDTH - RECT_WIDTH;
        if (rect.y + RECT_HEIGHT > SCREEN_HEIGHT) rect.y = SCREEN_HEIGHT - RECT_HEIGHT;

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Set render color to black (rect will be black)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        // Render the rectangle
        SDL_RenderFillRect(renderer, &rect);

        // Update screen
        SDL_RenderPresent(renderer);

        // Delay to control frame rate
        SDL_Delay(16); // Approximately 60 frames per second
    }

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
