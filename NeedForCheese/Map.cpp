#include "Map.h"
#include "utilities.h"
#include "tinyxml2.h"
#include "SDL2/SDL.h"

Map::Map(const char* filename)
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

    std::string csvString(csvData);
    std::vector<std::string> rows = split_string(csvString, "\n");

    for (int i = 0; i < rows.size(); i++)
    {
        std::vector<std::string> columns = split_string(rows[i], ",");
        printf(string(columns).c_str());
        for (int j = 0; j < columns.size(); j++)
        {
            tiles.push_back(std::stoi(columns[j]));
        }
    }
}

Map::Map()
{
}

void Map::draw_map(SDL_Renderer* renderer)
{
    int texture_width, texture_height;
    SDL_QueryTexture(texture, NULL, NULL, &texture_width, &texture_height);

    for (int i = 0; i < mapHeight; i++)
    {
        for (int j = 0; j < mapWidth; j++)
        {
            int tile = tiles[static_cast<std::vector<int, std::allocator<int>>::size_type>(i) * mapWidth + j];

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
            SDL_Rect dstrect = { j * tileWidth, i * tileHeight, tileWidth, tileHeight };

            // Render the tile to the screen
            SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
        }
    }
}

vector<string> split_string(const string& str, const char* delim)
{
    vector<string> tokens;
    string token;
    size_t start = 0, end = 0;
    while ((end = str.find(delim, start)) != string::npos)
    {
        token = str.substr(start, end - start);
        start = end + 1;
        tokens.push_back(token);
    }
    tokens.push_back(str.substr(start));
    return tokens;
}
