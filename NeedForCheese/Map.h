#ifndef MAP_H
#define MAP_H
#include <map>
#include <iostream>
#include <vector>
#include <string>
#include "utilities.h"
#include "tinyxml2.h"
#include "SDL2/SDL_image.h"
#include "box2d/box2d.h"
using namespace std;
using namespace tinyxml2;
class Map
{
public:
    Map(const char* filename, b2World world);
    Map();
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
#endif // !MAP_H