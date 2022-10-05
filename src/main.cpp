#include "include/bs/includes.h"

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

SDL_Event ev;

Tile grass, flower;

bool quit;

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << SDL_GetError() << std::endl;
        return false;
    }
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
        std::cout << "LTF not enabled" << std::endl;
        return false;
    }
    gWindow = SDL_CreateWindow("Battle Simulator", 350, 200, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL) {
        std::cout << SDL_GetError() << std::endl;
        return false;
    }
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (gRenderer == NULL) {
        std::cout << SDL_GetError() << std::endl;
        return false;
    }
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cout << SDL_GetError() << std::endl;
        return false;
    }
    // if (TTF_Init() == -1) {
    //     std::cout << SDL_GetError() << std::endl;
    //     return false;
    // }
    return true;
}

Entity::Entity() {
    posX = 0;
    posY = 0;
    deg = 0;
}
Tile::Tile(int tileType) {
    srand(time(0));
    int chance = rand();
    std::cout << chance << std::endl;
    if (chance % 2 == 0) {
        tileType = 1;
    } else {
        tileType = 0;
    }
}

bool Entity::loadSprite(std::string path) {
    SDL_Texture* finalTexture = NULL;
    SDL_Surface* loadFromSurface = IMG_Load(path.c_str());
    if (loadFromSurface == NULL) {
        std::cout << SDL_GetError() << std::endl;
        return false;
    }
    finalTexture = SDL_CreateTextureFromSurface(gRenderer, loadFromSurface);
    if (finalTexture == NULL) {
        std::cout << SDL_GetError() << std::endl;
        return false;
    }
    rWidth = loadFromSurface->w;
    rHeight = loadFromSurface->h;
    SDL_FreeSurface(loadFromSurface);
    rTexture = finalTexture;
    return rTexture != NULL;   
}


void Entity::render(SDL_Rect* clip) {
    SDL_Rect renderRect = {posX, posY, rWidth, rHeight};
    if (clip != NULL) {
        renderRect.w = clip->w;
        renderRect.h = clip->h;
    }
    SDL_RenderCopyEx(gRenderer, rTexture, clip, &renderRect, deg, NULL, SDL_FLIP_NONE);
}

bool loadMedia() {
    grass.loadSprite("src/media/png/grass.png");
    flower.loadSprite("src/media/png/flower.png");
    return true;
}

void close() {
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;
    IMG_Quit();
    // TTF_Quit();
    SDL_Quit();
}

int main(int argv, char** args) {
    if (!init()) {
        std::cout << "Failed to initialize!" << std::endl;
        return 0;
    }
    if (!loadMedia()) {
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
