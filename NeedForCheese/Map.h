#include <map>
#include <iostream>
#include <vector>
#include <string>
#include "utilities.h"
#include "tinyxml2.h"
#include "SDL2/SDL_image.h"
using namespace std;
using namespace tinyxml2;
class Map
{
public:
    Map(const char* filename);
    void draw_map(SDL_Renderer* renderer);
private:
    SDL_Texture* texture;
    int texture_width;
    int texture_height;

    vector<int> tiles;
    int mapWidth;
    int mapHeight;
    int tileWidth;
    int tileHeight;

};

