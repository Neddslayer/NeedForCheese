#include "Map.h"
#include "utilities.h"
#include "world.h"
#include "tinyxml2.h"
#include "SDL2/SDL.h"
#include "box2d/box2d.h"

Map::Map(const char* filename, b2World* world)
{
    XMLDocument level;
    level.LoadFile(filename);

    if (level.Error())
    {
        printf("Error loading map: %s\n", level.ErrorName());
    }

    XMLElement* mapElement = level.FirstChildElement("map");
    if (mapElement == nullptr)
    {
        printf("Error loading map: no map element found\n");
    }

    mapWidth = mapElement->IntAttribute("width");
    mapHeight = mapElement->IntAttribute("height");

    XMLElement* tilesetElement = level.FirstChildElement("map")->FirstChildElement("tileset");
    if (tilesetElement == nullptr)
    {
        printf("Error loading map: no tileset element found\n");
    }

    tileWidth = tilesetElement->IntAttribute("tilewidth");
    tileHeight = tilesetElement->IntAttribute("tileheight");

    const char* imageSource = tilesetElement->FirstChildElement("image")->Attribute("source");

    SDL_Surface* surface = IMG_Load(imageSource);
    if (surface == nullptr)
    {
        printf("Error loading texture: %s\n", IMG_GetError());
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr)
    {
        printf("Error creating texture: %s\n", SDL_GetError());
    }

    texture_width = surface->w;
    texture_height = surface->h;

    SDL_FreeSurface(surface);

    XMLElement* dataElement = mapElement->FirstChildElement("layer")->FirstChildElement("data");
    if (dataElement == nullptr)
    {
        printf("Error loading map: no layer data found\n");
    }
    const char* csvData = dataElement->GetText();
    
    if (csvData == nullptr)
    {
        printf("Error loading map: no layer data found\n");
    }

    string csvString(csvData);
    vector<string> rows = split_string(csvString, "\n");

    for (int i = 0; i < rows.size(); i++)
    {
        vector<string> columns = split_string(rows[i], ",");
        
        for (int j = 0; j < columns.size(); j++)
        {
            tiles.push_back(stoi(columns[j]));
        }
    }
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    b2Body* body = world->CreateBody(&bodyDef);

    for (int y = 0; y < mapHeight; y++)
    {
        for (int x = 0; x < mapWidth; x++)
        {
            int tileIndex = y * mapWidth + x;
            int tile = tiles[tileIndex];
            if (tile != 0)
            {
                // Create static body at center of current tile
                bodyDef.position.Set(x * tileWidth + tileWidth / 2.0f,
                    y * tileHeight + tileHeight / 2.0f);
                std::cout << bodyDef.position.x << " " << bodyDef.position.y << " " << y << endl;
                body = world->CreateBody(&bodyDef);

                // Create fixture for current tile
                b2PolygonShape shape;
                shape.SetAsBox(tileWidth / 2.0f, tileHeight / 2.0f);
                body->CreateFixture(&shape, 0.0f);
            }
        }
    }
}

Map::Map(){}

void Map::draw_map(SDL_Renderer* renderer)
{
    int texture_width, texture_height;
    SDL_QueryTexture(texture, NULL, NULL, &texture_width, &texture_height);

    for (int i = 0; i < mapHeight; i++)
    {
        for (int j = 0; j < mapWidth; j++)
        {
            int tile = tiles[static_cast<vector<int, allocator<int>>::size_type>(i) * mapWidth + j];

            // If the tile is 0, it's an empty tile so we skip it
            if (tile == 0)
            {
                continue;
            }

            // Calculate the texture coordinates for the tile
            int tileRow = (tile - 1) / (texture_width / tileWidth);
            int tileCol = (tile - 1) % (texture_width / tileWidth);
            int x = tileCol * tileWidth;
            int y = tileRow * tileHeight;

            // Set up the source and destination rectangles for rendering
            SDL_Rect srcrect = { x, y, tileWidth, tileHeight };
            SDL_Rect dstrect = { ((SCALED_WIDTH / 2.0f) + (j * tileWidth)) * MET2PIX, ((SCALED_WIDTH / 2.0f) + (i * tileHeight)) * MET2PIX, tileWidth * MET2PIX, tileHeight * MET2PIX};

            // Render the tile to the screen
            SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
        }
    }
}

vector<string> split_string(const string& str, const char* delimiter)
{
    vector<string> substrings;
    string substring;
    const int delimiterLength = strlen(delimiter);

    for (size_t i = 0; i < str.size();)
    {
        if (str.substr(i, delimiterLength) == delimiter)
        {
            substrings.push_back(substring);
            substring.clear();
            i += delimiterLength;
        }
        else
        {
            substring += str[i];
            ++i;
        }
    }

    substrings.push_back(substring);
    return substrings;
}
