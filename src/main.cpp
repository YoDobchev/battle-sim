#include "include/bs/includes.h"

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

SDL_Event ev;

// ???
// SDL_Event k;
// //event za click na mishka
// SDL_Event mb;

TTF_Font* gFont = NULL;

Texture grass, flower, dirt, melee, yellow, barracks[4], allowedToBuildMask, forbiddenToBuildMask, transparentBarracks, flag;

Tile battlefield[150][54];

Tile goal;

Unit meleeClass;

SDL_Color gtcolor = { 0, 0, 0, 255 };

Entity buildingPlacementIndication, buildingBeingPlaced;

bool quit;

int cameraX = 0, mouseX, mouseY, mouseEvent, row, column;

bool isFullScreen = true;

enum mouseSelection {
    Default = 0,
    BuilderMode = 1,
};

enum barracks {
    barracksUpLeft = 0,
    barracksUpRigh = 2,
    barracksDownLeft = 1,
    barracksDownRight = 3,
};

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
    buildable = true; 
}

bool Tile::operator==(const Tile &t) const {
    return posX == t.posX && posY == t.posY;
}

bool Tile::operator<(const Tile &t) const {
    return std::tie(posX, posY) < std::tie(t.posX, t.posY);
}

bool operator!=(Tile a, Tile b) {
    return !(a == b);
}

Unit::Unit() {
    rWidth = 20;
    rHeight = 20;
}
// начи ако тряя обеснявам звънкайте
void Unit::move() { 
    if (!path.empty() && path.size() >= 1 && posX == path[0].posX && posY == path[0].posY) {
        int currentTile = 0;
        tileStandingOn = battlefield[path[0].posX / 20][path[0].posY / 20];
        for(short int i = -1; i < 2; i++) {
            for(short int j = -1; j < 2; j++) {
                currentTile++;
                if (battlefield[path[0].posX / 20 + j][path[0].posY / 20 + i] == battlefield[path[1].posX / 20][path[1].posY / 20]) {
                    velX = directions[currentTile].first;
                    velY = directions[currentTile].second;
                    if (path.size() > 1)
                        deg = std::atan2(velY, velX) * (180.0/3.141592653589793238463);
                }
            }
        }
        path.erase(path.begin());
        return;
    }
    posX += velX;
    posY += velY;
}

bool loadMedia() {
    // zarejdame samo like 15 teksturi znaesh kak e 🤩
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

    if (!barracks[0].loadSprite("src/media/png/barracks/barracksTopL.png")) {
        std::cout << "Failed to load texture!" << std::endl;
        return false;
    }

    if (!barracks[1].loadSprite("src/media/png/barracks/barracksBotL.png")) {
        std::cout << "Failed to load texture!" << std::endl;
        return false;
    }
    
    if (!barracks[2].loadSprite("src/media/png/barracks/barracksTopR.png")) {
        std::cout << "Failed to load texture!" << std::endl;
        return false;
    }
    
    if (!barracks[3].loadSprite("src/media/png/barracks/barracksBotR.png")) {
        std::cout << "Failed to load texture!" << std::endl;
        return false;
    }
    
    if (!allowedToBuildMask.loadSprite("src/media/png/allowedToBuildMask.png")) {
        std::cout << "Failed to load texture!" << std::endl;
        return false;
    }

    if (!forbiddenToBuildMask.loadSprite("src/media/png/forbiddenToBuildMask.png")) {
        std::cout << "Failed to load texture!" << std::endl;
        return false;
    }

    if (!transparentBarracks.loadSprite("src/media/png/transparent/barracks.png")) {
        std::cout << "Failed to load texture!" << std::endl;
        return false;
    }
    
    if (!flag.loadSprite("src/media/png/flag.png")) {
        std::cout << "Failed to load texture!" << std::endl;
        return false;
    }
    
    buildingPlacementIndication.rHeight = 40;
    buildingPlacementIndication.rWidth = 40;
    buildingBeingPlaced.rHeight = 40;
    buildingBeingPlaced.rWidth = 40;
    buildingBeingPlaced.rTexture = transparentBarracks.rTexture;


    // gFont = TTF_OpenFont( "src/media/ttf/CrimsonText-Italic.ttf", 28);
    // if( gFont == NULL ) {
    //     printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
    //     return false;
    // }

    // glupost za da raboti random num 🤷
    srand(time(NULL));
    // koordinatite koit se updatevat na vsqka interaciq na cikula za da gi razpolojim na vseki 20 po x & y
    int relativeX = -20, relativeY = -20;
    for (short int i = 0; i < 150; ++i) {
        relativeX += 20;
        relativeY = -20;
        for (short int j = 0; j < 54; ++j) {
            battlefield[i][j].walkable = true;
            relativeY += 20;
            battlefield[i][j].posX = relativeX;
            battlefield[i][j].posY = relativeY;
            battlefield[i][j].deg = 90 * (0 + (rand() % 4));
            int randomNum = 1 + (rand() % 100);
            if (randomNum <= 5) {
                battlefield[i][j].rTexture = flower.rTexture;
                battlefield[i][j].tileType = 1;
            } else if (randomNum > 5 && randomNum < 95) {
                battlefield[i][j].rTexture = grass.rTexture;
                battlefield[i][j].tileType = 0;
            } else if (randomNum >= 95) {
                battlefield[i][j].rTexture = yellow.rTexture;
                battlefield[i][j].tileType = 2;
            }
            if (i <= 3) {
                battlefield[i][j].deg = 0;
                battlefield[i][j].rTexture = dirt.rTexture;
                battlefield[i][j].tileType = 2;
            }
            if (j == 20 && i > 30 && i < 70) {
                battlefield[i][j].rTexture = melee.rTexture;
                battlefield[i][j].walkable = false;
            }
        }
    }

    meleeClass.rTexture = melee.rTexture;
    meleeClass.posX = 200;
    meleeClass.posY = 500;
    meleeClass.tileStandingOn = battlefield[meleeClass.posX / 20][meleeClass.posY / 20];

    return true;

}

bool checkIfBuildable() {
    //novata checkIfBuildable nqma nujda praim [1] vseki put kat minaame prez proverki
    return battlefield[row][column].buildable == true && battlefield[row + 1][column + 1].buildable == true;
}

void place(Texture textures[], bool building) {
    if (building) {
        int textureOrder = 0;
        for(int i = 0; i < 2; i++) {
            for(int j = 0; j < 2; j++) {
                battlefield[row + i][column + j].rTexture = textures[textureOrder].rTexture;
                textureOrder++;
                battlefield[row + i][column + j].deg = 0;
            }
        }
        // [1] tuka si e nali kato slojim blockhetata da ne moje da se stroi po i okolo tqh 
        // edge case
        for(short int i =- 1; i <= 2; i++) {
            for(short int j =- 1; j <= 2; j++) {
                battlefield[row + i][column + j].buildable = false;
            }
        }
        return;
    }
    battlefield[row][column].rTexture = flag.rTexture;
}

bool PriorityQueue::empty() const {
    return elements.empty();
} 

void PriorityQueue::put(Tile item, double priority) {
    elements.emplace(priority, item);
}

Tile PriorityQueue::get() {
    Tile best_item = elements.top().second;
    elements.pop();
    return best_item;
}

int heuristic(Tile a, Tile b) {
    return std::abs(a.posX - b.posX) + std::abs(a.posY - b.posY);
}

std::vector<Tile> getNeighbours(Tile* tile) {
    std::vector<Tile> neighbours;
    neighbours.reserve(8);
    int currentTileNeighbour = 0;
    for(short int i = -1; i < 2; i++) {
        for(short int j = -1; j < 2; j++) {
            currentTileNeighbour++;
            Tile currentTile = battlefield[tile->posX / 20 + i][tile->posY / 20 + j];
            if (currentTileNeighbour != 5 && currentTile.walkable)
                neighbours.emplace_back(currentTile);
        }
    }
    return neighbours;
}

void AStarSearch(Unit* unit) {
    std::unordered_map<Tile, Tile> cameFrom;
    std::unordered_map<Tile, double> costSoFar;
    PriorityQueue frontier;
    frontier.put(unit->tileStandingOn, 0);
    cameFrom[unit->tileStandingOn] = unit->tileStandingOn;
    costSoFar[unit->tileStandingOn] = 0;

    while (!frontier.empty()) {
        Tile current = frontier.get();
        
        if (current == battlefield[goal.posX / 20][goal.posY / 20]) {
            break;
        }
        for (Tile& nextTile: getNeighbours(&current)) {
            double newCost = costSoFar[current];
            if (costSoFar.find(nextTile) == costSoFar.end() || newCost < costSoFar[nextTile]) {
                costSoFar[nextTile] = newCost;
                double priority = newCost + heuristic(nextTile, battlefield[goal.posX / 20][goal.posY / 20]);
                // std::cout << "tile (" << current.posX << "-" << current.posY << ") newCost " << newCost << ", heuristic " << heuristic(nextTile, battlefield[goal.posX / 20][goal.posY / 20]) << ", priority " << priority << std::endl;
                frontier.put(nextTile, priority);
                cameFrom[nextTile] = current;
            }
        }
    }
    std::vector<Tile> newPath = reconstuctPath(cameFrom, &unit->tileStandingOn);
    if (unit->path.size() != 0)
        unit->path.resize(1);
    unit->path.insert(unit->path.end(), newPath.begin(), newPath.end());
}

std::vector<Tile> reconstuctPath(std::unordered_map<Tile, Tile> cameFrom, Tile* start) {
    std::vector<Tile> path;
    Tile current = goal;
    if (cameFrom.find(goal) == cameFrom.end()) {
        return path;
    }
    while (current != battlefield[start->posX / 20][start->posY / 20]) {
        path.push_back(current);
        current = cameFrom[current];
    }
    path.push_back(*start);
    std::reverse(path.begin(), path.end());
    // for (Tile& t: path) {
    //     battlefield[t.posX / 20][t.posY / 20].rTexture = dirt.rTexture;
    // }
    return path;
}

void close() {
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;
    // gFont=NULL;
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
        std::cout << "Failed to load media!" << std::endl;
        return 0;
    }

    while (!quit) {
        //kordinatite v grida na mishkata sega 😏🤨
        row = ((mouseX + cameraX) / 20);
        column = mouseY / 20;
        while (SDL_PollEvent(&ev) != 0) {
            if (ev.type == SDL_QUIT) {
                quit = true;
            }
            //map movement
            switch (ev.key.keysym.sym) {
                case SDLK_RIGHT:
                    if (cameraX <= 2000) {
                        cameraX += 20;
                        for (short int i = 0; i < 150; i++) {
                            for (short int j = 0; j < 54; j++) {
                                battlefield[i][j].posX -= 20;
                            }
                        }
                    }
                    break;
                case SDLK_LEFT:
                    if (cameraX >= 1) {
                        cameraX -= 20;
                        for (short int i = 0; i < 150; i++) {
                            for (short int j = 0; j < 54; j++) {
                                battlefield[i][j].posX += 20;
                            }
                        }
                    }
                    break;
            }
            SDL_GetMouseState(&mouseX, &mouseY);

            if (ev.type == SDL_MOUSEBUTTONDOWN) {
                if (checkIfBuildable() && battlefield[row][column].walkable) {
                    place(barracks, false);
                    goal.posX = row * 20;
                    goal.posY = column * 20;
                    // loadMedia();
                    AStarSearch(&meleeClass);
                }
            }
        }
        
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);
        for (short int i = 0; i < 150; ++i) {
            for (short int j = 0; j < 54; ++j) {
                battlefield[i][j].render();
            }
        }
        
        buildingPlacementIndication.posX = battlefield[row][column].posX;
        buildingPlacementIndication.posY = battlefield[row][column].posY;
        buildingBeingPlaced.posX = battlefield[row][column].posX;
        buildingBeingPlaced.posY = battlefield[row][column].posY;
        if (checkIfBuildable()) {
            //ako moje da se stroi nali inicializirame toBePlaced i texturata mu stava allowedToBuildMask
            buildingPlacementIndication.rTexture = allowedToBuildMask.rTexture;
        } else {
            buildingPlacementIndication.rTexture = forbiddenToBuildMask.rTexture;
            
        }
        //rendervame otgore na mapa 
        buildingBeingPlaced.render();
        buildingPlacementIndication.render();
        
        meleeClass.move();
        meleeClass.render();
        SDL_RenderPresent(gRenderer);
    }
    close();
    return 0;
}
