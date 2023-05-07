#include "Map.h"
#include "utilities.h"
#include "world.h"
#include "tinyxml2.h"
#include "SDL2/SDL.h"
#include "box2d/box2d.h"
#include <string>
#include <iostream>
using namespace std;

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
                //bodyDef.position.Set((x / static_cast<float>(MET2PIX)) + ((tileWidth / static_cast<float>(MET2PIX)) / 2.0f),
                //    (y / static_cast<float>(MET2PIX)) + ((tileHeight / static_cast<float>(MET2PIX)) / 2.0f));
                bodyDef.position.Set((x * (tileWidth * PIX2MET)) - (tileWidth * PIX2MET) * 10.0f,
                    (y * (tileHeight * PIX2MET)) - (tileHeight * PIX2MET) / 2.0f);
                body = world->CreateBody(&bodyDef);

                // Create fixture for current tile
                b2PolygonShape shape;
                b2FixtureDef tileFixtureDef;
                shape.SetAsBox((tileWidth * PIX2MET) / 2.0f, (tileHeight * PIX2MET) / 2.0f);
                tileFixtureDef.shape = &shape;
                body->GetUserData().pointer = GROUND;
                body->CreateFixture(&tileFixtureDef);

                std::cout << bodyDef.position.x << " " << bodyDef.position.y << endl;
            }
        }
    }
}

Map::Map(){}

void Map::draw_map(SDL_Renderer* renderer, b2World* world)
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
            //SDL_Rect dstrect = { ((SCALED_WIDTH / 2.0f) + (j * tileWidth)) * MET2PIX, ((SCALED_WIDTH / 2.0f) + (i * tileHeight)) * MET2PIX, tileWidth * MET2PIX, tileHeight * MET2PIX};
            SDL_Rect dstrect = { j * tileWidth, i * tileHeight, tileWidth, tileHeight };

            //SDL_RenderFillRect(renderer, &dstrect);

            // Render the tile to the screen
            SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
        }
    }
    
    for (b2Body* body = world->GetBodyList(); body; body = body->GetNext())
    {
        // Get the body position and angle
        b2Vec2 position = body->GetPosition();
        float angle = body->GetAngle();

        // Iterate over all fixtures in the body
        for (b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext())
        {
            // Get the fixture shape and type
            b2Shape* shape = fixture->GetShape();
            b2Shape::Type type = shape->GetType();

            // Set the rendering color
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

            // Draw the shape based on its type
            if (type == b2Shape::e_edge)
            {
                b2EdgeShape* edgeShape = static_cast<b2EdgeShape*>(shape);
                b2Vec2 v1 = edgeShape->m_vertex1;
                b2Vec2 v2 = edgeShape->m_vertex2;
                SDL_RenderDrawLine(renderer, ((SCALED_WIDTH / 2.0f) + v1.x) * MET2PIX, ((SCALED_WIDTH / 2.0f) + v1.y) * MET2PIX, ((SCALED_WIDTH / 2.0f) + v2.x) * MET2PIX, ((SCALED_WIDTH / 2.0f) + v2.y) * MET2PIX);
            }
            else if (type == b2Shape::e_polygon)
            {
                b2PolygonShape* polygonShape = static_cast<b2PolygonShape*>(shape);
                b2PolygonShape copy;
                copy.Set(polygonShape->m_vertices, polygonShape->m_count);
                int count = polygonShape->m_count;
                float width = 0;
                float height = 0;
                GetPolygonShapeDimensions(copy, width, height);
                for (int i = 0; i < count; i++)
                {
                    b2Vec2 v1 = body->GetWorldPoint(polygonShape->m_vertices[i]);
                    b2Vec2 v2 = body->GetWorldPoint(polygonShape->m_vertices[(i + 1) % count]);
                    SDL_RenderDrawLine(renderer, (((SCALED_WIDTH / 2.0f) + v1.x) * MET2PIX) - width / 2.0f, (((SCALED_WIDTH / 2.0f) + v1.y) * MET2PIX) - height / 2.0f - MET2PIX * 4, (((SCALED_WIDTH / 2.0f) + v2.x) * MET2PIX) - width / 2.0f, (((SCALED_WIDTH / 2.0f) + v2.y) * MET2PIX) - height / 2.0f - MET2PIX * 4);
                }
            }
            else if (type == b2Shape::e_circle)
            {
                b2CircleShape* circleShape = static_cast<b2CircleShape*>(shape);
                b2Vec2 center = body->GetWorldPoint(circleShape->m_p);
                float radius = circleShape->m_radius * SCALED_WIDTH;
                SDL_Rect rect = { (int)(center.x - radius), (int)(center.y - radius), (int)(2 * radius), (int)(2 * radius) };
                SDL_RenderDrawRect(renderer, &rect);
            }
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
void Map::GetPolygonShapeDimensions(const b2PolygonShape& shape, float& width, float& height)
{
    const b2Vec2* vertices = shape.m_vertices;
    int32 count = shape.m_count;
    float minX = vertices[0].x, maxX = vertices[0].x;
    float minY = vertices[0].y, maxY = vertices[0].y;

    // Loop through all the vertices to find the min and max x and y values
    for (int32 i = 1; i < count; ++i)
    {
        b2Vec2 vertex = vertices[i];
        if (vertex.x < minX) minX = vertex.x;
        if (vertex.x > maxX) maxX = vertex.x;
        if (vertex.y < minY) minY = vertex.y;
        if (vertex.y > maxY) maxY = vertex.y;
    }

    // Calculate the width and height based on the min and max values
    width = maxX - minX;
    height = maxY - minY;
}