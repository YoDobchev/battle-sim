#ifndef GAME_H
#define GAME_H

const uint16_t SCREEN_WIDTH = 1920;
const uint16_t SCREEN_HEIGHT = 1080;

// dopulnitelna structura samo za teksturi
struct Texture {

    bool loadFromRenderedText( std::string textureText, SDL_Color textColor );

    bool loadSprite(std::string path);

    SDL_Texture* rTexture;

    int rWidth, rHeight;
};

struct Entity: public Texture {
    Entity();

    void render(SDL_Rect* clip = NULL);

    int getWidth();
    int getHeight();

    int posX, posY, deg;

};

struct Tile: public Entity {
    Tile();
    // 0 - grass 1 - flower 2 - dirt
    int tileType;
    bool buildable;
};

struct Unit: public Entity {
    Unit();

    void move(int speed);

    int health;
};

bool init();

bool loadMedia();

void close();

#endif