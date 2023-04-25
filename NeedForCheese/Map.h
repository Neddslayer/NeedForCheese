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
	Map(const char* filename)
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

            for (int j = 0; j < columns.size(); j++)
            {
                tiles.push_back(std::stoi(columns[j]));
            }
        }
	}
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

