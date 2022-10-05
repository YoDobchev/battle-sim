#include <SDL2/SDL.h>
//#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_ttf.h>
#include <iostream>

const uint16_t SCREEN_WIDTH = 640;
const uint16_t SCREEN_HEIGHT = 640;

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

SDL_Event ev;

bool quit;

bool init() {
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << SDL_GetError() << std::endl;
        success = false;
    } else {
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
            std::cout << "LTF not enabled" << std::endl;
        }
        gWindow = SDL_CreateWindow("Jet Fighter", 650, 150, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            std::cout << SDL_GetError() << std::endl;
            success = false;
        } else {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (gRenderer == NULL) {
                std::cout << SDL_GetError() << std::endl;
                success = false;
            } else {
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                // if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
                //     std::cout << SDL_GetError() << std::endl;
                //     success = false;
                // }
                // if (TTF_Init() == -1) {
                //     std::cout << SDL_GetError() << std::endl;
                //     success = false;
                // }
            }
        }
    }

    return success;
}

void close() {
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;
    // IMG_Quit();
    // TTF_Quit();
    SDL_Quit();
}
int main(int argv, char** args) {
    if (!init()) {
        std::cout << "Failed to initialize!" << std::endl;
        return 0;
    }
    while (!quit) {
        while (SDL_PollEvent(&ev) != 0) {
            if (ev.type == SDL_QUIT) {
                quit = true;
            }
        }
        SDL_RenderClear(gRenderer);
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderPresent(gRenderer);
    }
    close();
    return 0;
}