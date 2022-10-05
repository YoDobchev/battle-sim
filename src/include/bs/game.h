#ifndef GAME_H
#define GAME_H

const uint16_t SCREEN_WIDTH = 1920;
const uint16_t SCREEN_HEIGHT = 1080;

struct Entity {
    Entity();

    bool loadSprite(std::string path);

    // virtual void free();

    void render(SDL_Rect* clip = NULL);

    int getWidth();
    int getHeight();

    int rWidth, rHeight, posX, posY, deg;

    SDL_Texture* rTexture;
};

struct Tile: public Entity {
    Tile(int tileType = 0);
    // 0 - grass 1 - flower
    int tileType;
};

#endif