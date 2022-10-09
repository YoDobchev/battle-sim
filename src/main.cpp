#include "include/bs/includes.h"

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

SDL_Event ev;

Texture grass, flower, dirt, melee, yellow;

Tile battlefield[96][54];

Unit meleeClass;

bool quit;

bool isFullScreen = true;

// void toggleFullScreen(SDL_Window* window, bool currentState)
// {
//     isFullScreen = !currentState;

//     SDL_SetWindowFullscreen(window, !currentState);
//     SDL_ShowCursor(currentState);
// }

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << SDL_GetError() << std::endl;
        return false;
    }
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
        std::cout << "LTF not enabled" << std::endl;
        return false;
    }
    gWindow = SDL_CreateWindow("Battle Simulator", 0, 30, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

Tile::Tile() {
    rWidth = 20;
    rHeight = 20;   
}

Unit::Unit() {
    rWidth = 20;
    rHeight = 20;   
}

void Unit::move(int speed) {
    posX += speed;
    posY += speed;
}

bool loadMedia() {
    // zarejdame samo 2te teksturi koit ni trqbvat (za da ne go praim za drugite 5000 i nesh)
    if (!grass.loadSprite("src/media/png/grass2.png")) {
        std::cout << "Failed to load texture!" << std::endl;
        return false;
    }

    if (!flower.loadSprite("src/media/png/flowers2.png")) {
        std::cout << "Failed to load texture!" << std::endl;
        return false;
    }

    if (!yellow.loadSprite("src/media/png/yellowfg.png")) {
        std::cout << "Failed to load texture!" << std::endl;
        return false;
    }

    if (!dirt.loadSprite("src/media/png/dirtb.jpg")) {
        std::cout << "Failed to load texture!" << std::endl;
        return false;
    }

    if (!melee.loadSprite("src/media/png/melee.png")) {
        std::cout << "Failed to load texture!" << std::endl;
        return false;
    }

    // glupost za da raboti random num 🤷
    srand(time(NULL));
    // koordinatite koit se updatevat na vsqka interaciq na cikula za da gi razpolojim na vseki 20 po x & y
    int relativeX = -20, relativeY = -20;
    for (short int i = 0; i < 96; ++i) {
        relativeX += 20;
        relativeY = -20;
        for (short int j = 0; j < 54; ++j) {
            relativeY += 20;
            battlefield[i][j].posX = relativeX;
            battlefield[i][j].posY = relativeY;
            battlefield[i][j].deg = 90 * (0 + (rand() / 3));
            int randomNum = 1 + (rand() % 100);
            if (randomNum <= 5) {
                battlefield[i][j].rTexture = flower.rTexture;
                battlefield[i][j].tileType = 1;
            } else if (randomNum > 5 && randomNum < 95) {
                battlefield[i][j].rTexture = grass.rTexture;
                battlefield[i][j].tileType = 0;
            } else if (randomNum >=95) {
                battlefield[i][j].rTexture = yellow.rTexture;
                battlefield[i][j].tileType = 2;
            }
            if (i <= 3) {
                battlefield[i][j].deg = 0;
                battlefield[i][j].rTexture = dirt.rTexture;
                battlefield[i][j].tileType = 2;
            }
        }
    }
    meleeClass.rTexture = melee.rTexture;
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
    // toggleFullScreen(gWindow, false);
    while (!quit) {
        while (SDL_PollEvent(&ev) != 0) {
            if (ev.type == SDL_QUIT) {
                quit = true;
            }
        }
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);
        for (short int i = 0; i < 96; ++i) {
            for (short int j = 0; j < 54; ++j){
                battlefield[i][j].render();
            }
        }
        // meleeClass.move(3);
        // meleeClass.render();
        SDL_RenderPresent(gRenderer);
    }
    close();
    return 0;
}
