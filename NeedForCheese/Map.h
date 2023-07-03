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
#include "SDL_Camera.h"
using namespace std;
using namespace tinyxml2;
class Map
{
public:
    // Initialize the map. Used when loading a new map
    Map(const char* filename, b2World* world);
    // Initialize a blank map. This will fail when calling anything else, so use an actual map!
    Map();
    // Draw the map from the tiles specified in the files. Will fail if the empty Map() constructor is called rather than the initialized form.
    void draw_map(SDL_Renderer* renderer, b2World* world, Camera2D camera);
    // Used while debugging the game. This does NOT need to be used in release builds!
    void GetPolygonShapeDimensions(const b2PolygonShape& shape, float& width, float& height);
private:
    SDL_Texture* texture; // The map tileset texture.
    int texture_width; // The tileset's texture width.
    int texture_height; // The tileset's texture height.

    vector<int> rendered_tiles; // A list of all of the tiles to be rendered in the map.
    vector<int> collision_tiles; // A list of all of the collision tiles to be created.
    int mapWidth; // The width (in tiles) of the map.
    int mapHeight; // The height (in tiles) of the map.
    int tileWidth; // A single tile's texture width.
    int tileHeight; // A single tile's texture height.

};
#endif // !MAP_H