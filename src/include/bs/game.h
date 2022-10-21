#ifndef GAME_H
#define GAME_H

const uint16_t SCREEN_WIDTH = 1920;
const uint16_t SCREEN_HEIGHT = 1080;

// как да се движи за x, y при всички посоки (виж unit::move функ)
std::unordered_map<int, std::pair<int, int>> directions = {
	{
		1,
		{ -2, -2 }
	},
	{
		2,
		{ 0, -2 }
	},
	{
		3,
		{ 2, -2 }
	},
	{
		4,
		{ -2, 0 }
	},
	{
		6,
		{ 2, 0 }
	},
	{
		7,
		{ -2, 2 }
	},
	{
		8,
		{ 0, 2 }
	},
	{
		9,
		{ 2, 2 }
	}
};

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
    bool buildable, walkable;

    // проверка дали два Tile обекта са еднакви
    bool operator==(const Tile &t) const;
    // сравнение на Tile обекти
    bool operator<(const Tile &t) const;
};

// понеже unordered_map иска ключа да се хашва и ако ключа е Tile, не знае как. Тук му казваме как
namespace std {
    template <> 
    struct hash<Tile> {
        std::size_t operator()(const Tile& key) const {
            // return std::hash<int>()(tile.x ^ (tile.y << 16));
            return std::hash<int>()(key.posX) ^ std::hash<int>()(key.posY << 19);
        }
    };
}
// дали два Tile обекта са различни
bool operator!=(Tile a, Tile b);

struct Unit: public Entity {
    Unit();

    void move();

    std::vector<Tile> path;

    Tile tileStandingOn;

    int health, velX, velY, goalLocS;
};

// като опашка за разкриване на полетата на картата (тези по-близо до крайната дестинация се откриват първи)
struct PriorityQueue {
    typedef std::pair<double, Tile> PQElement;
    std::priority_queue<PQElement, std::vector<PQElement>, std::greater<PQElement>> elements;

    bool empty() const;

    void put(Tile item, double priority);

    Tile get();
};

bool checkIfBuildable();

void place(Texture textures[], bool building);

// разтояние между два Tile обекта
int heuristic(Tile a, Tile b);

// връща съседите на един Tile обект
std::vector<Tile> getNeighbours(Tile* tile);

void AStarSearch();

std::vector<Tile> reconstuctPath(std::unordered_map<Tile, Tile> cameFrom, Tile* start);

bool init();

bool loadMedia();

void close();

#endif