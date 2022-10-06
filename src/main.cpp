#include "include/bs/includes.h"

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

SDL_Event ev;

Texture grass, flower;

Tile battlefield[96][54];

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
    rWidth = 0;
    rHeight = 0;
}

bool Texture::loadSprite(std::string path) {
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
    // zarejdame samo 2te teksturi koit ni trqbvat (za da ne go praim za drugite 5000 i nesh)
    if (!grass.loadSprite("src/media/png/grass.png")) {
        std::cout << "Failed to load texture!" << std::endl;
        return false;
    } 
    if (!flower.loadSprite("src/media/png/flower.png")) {
        std::cout << "Failed to load texture!" << std::endl;
        return false;
    }
    // glupost za da raboti random num 🤷
    srand(time(NULL));
    // koordinatite koit se updatevat na vsqka interaciq na cikula za da gi razpolojim na vseki 20 po x & y
    int relativeX = -20, relativeY = -20;
    for (short int i = 0; i < 96; ++i) {
        relativeX += 20;
        relativeY = 0;
        for (short int j = 0; j < 54; ++j) {
            relativeY += 20;
            battlefield[i][j].posX = relativeX;
            battlefield[i][j].posY = relativeY;
            battlefield[i][j].rWidth = 20;
            battlefield[i][j].rHeight = 20;
            int randomNum = 1 + (rand() % 100);
            if (randomNum < 5) {
                battlefield[i][j].rTexture = flower.rTexture;
            } else {
                battlefield[i][j].rTexture = grass.rTexture;
            }
        }
    }
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
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);
        for (short int i = 0; i < 96; ++i) {
            for (short int j = 0; j < 54; ++j) {
                battlefield[i][j].render();
            }
        }
        SDL_RenderPresent(gRenderer);
    }
    close();
    return 0;
}
