#include "Map.h"
#include "utilities.h"
#include "world.h"
#include "tinyxml2.h"
#include "SDL2/SDL.h"
#include "box2d/box2d.h"
#include <string>
#include <iostream>
#include "SDL_Camera.h"
#include "debug.h"
using namespace std;

Map::Map(const char* filename, b2World* world)
{
    XMLDocument level;
    level.LoadFile((string("resources/data/map/") + string(filename) + string(".xml")).c_str());


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
    XMLElement* collisionElement = mapElement->FirstChildElement("layer")->NextSiblingElement("layer")->FirstChildElement("data");
    if (dataElement == nullptr || collisionElement == nullptr)
    {
        printf("Error loading map: no layer data found\n");
    }
    const char* csvData = dataElement->GetText();
    const char* collisionData = collisionElement->GetText();
    
    if (csvData == nullptr || collisionData == nullptr)
    {
        printf("Error loading map: no layer data found\n");
    }

    string csvString(csvData);
    string collisionString(collisionData);
    vector<string> rows = split_string(csvString, ",\n");
    vector<string> collisions = split_string(collisionData, ",\n");

    for (int i = 0; i < rows.size(); i++)
    {
        vector<string> columns = split_string(rows[i], ",");
        
        for (int j = 0; j < columns.size(); j++)
        {
            rendered_tiles.push_back(stoi(columns[j]));
        }
    }

    for (int i = 0; i < collisions.size(); i++)
    {
        vector<string> columns = split_string(collisions[i], ",");

        for (int j = 0; j < columns.size(); j++)
        {
            collision_tiles.push_back(stoi(columns[j]));
        }
    }
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    b2Body* body = world->CreateBody(&bodyDef);

    if (old_collision_generation) // this should NOT be used, it is a lot more taxing, as it creates bodies for each RENDERED tile, not the optimized collision tile.
    {
        for (int y = 0; y < mapHeight; y++)
        {
            for (int x = 0; x < mapWidth; x++)
            {
                int tileIndex = y * mapWidth + x;
                int tile = rendered_tiles[tileIndex];
                if (tile != 0)
                {
                    if (tile > 400)
                    {
                        bodyDef.position.Set((x * (tileWidth * PIX2MET)) - (tileWidth * PIX2MET) * 12.5f,
                            (y * (tileHeight * PIX2MET)) - (tileHeight * PIX2MET) - (tileWidth * PIX2MET) * 5.55f);
                        bodyDef.angle = 0;
                        body = world->CreateBody(&bodyDef);
                        body->GetUserData().pointer = (uintptr_t)GROUND;
                        b2FixtureDef edgeFixtureDef;
                        b2EdgeShape edgeShape;
                        edgeShape.SetOneSided(b2Vec2(-0.5f, -0.25f), b2Vec2(-0.25f, -0.25f), b2Vec2(0.25f, -0.25f), b2Vec2(0.5f, -0.25f));
                        edgeFixtureDef.shape = &edgeShape;
                        edgeFixtureDef.userData.pointer = (uintptr_t)GROUND;
                        body->CreateFixture(&edgeFixtureDef);

                        tileBodies.push_back(body);
                    }
                    else
                    {
                        // Create static body at center of current tile
                        bodyDef.position.Set((x * (tileWidth * PIX2MET)) - (tileWidth * PIX2MET) * 12.5f,
                            (y * (tileHeight * PIX2MET)) - (tileHeight * PIX2MET) - (tileWidth * PIX2MET) * 5.55f);
                        body = world->CreateBody(&bodyDef);
                        body->GetUserData().pointer = (uintptr_t)GROUND;

                        // Create fixture for current tile
                        b2PolygonShape shape;
                        b2FixtureDef tileFixtureDef;
                        shape.SetAsBox((tileWidth * PIX2MET) / 2.0f, (tileHeight * PIX2MET) / 2.0f);
                        tileFixtureDef.shape = &shape;
                        body->CreateFixture(&tileFixtureDef);

                        tileBodies.push_back(body);
                    }
                }
            }
        }
    }
    else
    {
        for (int y = 0; y < mapHeight; y++)
        {
            for (int x = 0; x < mapWidth; x++)
            {
                int tileIndex = y * mapWidth + x;
                int tile = collision_tiles[tileIndex];
                if (tile != 0)
                    createTileCollision(x, y, tile, world);
            }
        }
    }
}

Map::Map(){}

void Map::draw_map(SDL_Renderer* renderer, b2World* world, Camera2D camera)
{
    int texture_width, texture_height;
    SDL_QueryTexture(texture, NULL, NULL, &texture_width, &texture_height);
    
    for (int i = 0; i < mapHeight; i++)
    {
        for (int j = 0; j < mapWidth; j++)
        {
            int tile = rendered_tiles[static_cast<vector<int, allocator<int>>::size_type>(i) * mapWidth + j];

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
            SDL_Rect dstrect = { j * tileWidth * 3.0 * SCALE, i * tileHeight * 3.0 * SCALE, tileWidth * 3.0 * SCALE, tileHeight * 3.0 * SCALE };

            //SDL_RenderFillRect(renderer, &dstrect);

            // Render the tile to the screen
            SDL_RenderCopy_Camera(camera, renderer, texture, &srcrect, &dstrect);
        }
    }
    
    if (showHitboxes)
    {
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
                    b2EdgeShape* edgeShape = (b2EdgeShape*)shape;
                    b2Vec2 v1 = edgeShape->m_vertex1;
                    b2Vec2 v2 = edgeShape->m_vertex2;
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                    SDL_RenderDrawLine_Camera(camera, renderer, ((SCALED_WIDTH / 2.0f) + v1.x + position.x) * MET2PIX, ((SCALED_HEIGHT / 2.0f) + v1.y + position.y) * MET2PIX, ((SCALED_WIDTH / 2.0f) + v2.x + position.x) * MET2PIX, ((SCALED_HEIGHT / 2.0f) + v2.y + position.y) * MET2PIX);
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
                        SDL_RenderDrawLine_Camera(camera, renderer, (((SCALED_WIDTH / 2.0f) + v1.x) * MET2PIX) - width / 2.0f, (((SCALED_WIDTH / 2.0f) + v1.y) * MET2PIX) - height / 2.0f - MET2PIX * 2.9775f, (((SCALED_WIDTH / 2.0f) + v2.x) * MET2PIX) - width / 2.0f, (((SCALED_WIDTH / 2.0f) + v2.y) * MET2PIX) - height / 2.0f - MET2PIX * 2.9775f);
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

void Map::createTileCollision(int x, int y, int tile, b2World* world)
{
    switch (tile)
    {
    case 626: // UP
    {
        createUpCollision(x, y, world);
        break;
    }
    case 627: // UP-RIGHT
    {
        createUpCollision(x, y, world);
        createRightCollision(x, y, world);
        break;
    }
    case 628: // UP-LEFT
    {
        createUpCollision(x, y, world);
        createLeftCollision(x, y, world);
        break;
    }
    case 629: // RIGHT
    {
        createRightCollision(x, y, world);
        break;
    }
    case 630: // LEFT
    {
        createLeftCollision(x, y, world);
        break;
    }
    case 638: // UP-SEMISOLID
    {
        b2BodyDef bodyDef;
        bodyDef.type = b2_staticBody;
        b2Body* body = world->CreateBody(&bodyDef);
        bodyDef.position.Set((x * (tileWidth * PIX2MET)) - (tileWidth * PIX2MET) * 12.5f,
            (y * (tileHeight * PIX2MET)) - (tileHeight * PIX2MET) - (tileWidth * PIX2MET) * 5.55f);
        bodyDef.angle = 0;
        body = world->CreateBody(&bodyDef);
        body->GetUserData().pointer = (uintptr_t)GROUND;
        b2FixtureDef edgeFixtureDef;
        b2EdgeShape edgeShape;
        edgeShape.SetOneSided(b2Vec2(-0.5f, -0.25f), b2Vec2(-0.25f, -0.25f), b2Vec2(0.25f, -0.25f), b2Vec2(0.5f, -0.25f));
        edgeFixtureDef.shape = &edgeShape;
        edgeFixtureDef.userData.pointer = (uintptr_t)GROUND;
        body->CreateFixture(&edgeFixtureDef);

        tileBodies.push_back(body);
        break;
    }
    default:
        break;
    }
    
}
void Map::createUpCollision(int x, int y, b2World* world)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    b2Body* body = world->CreateBody(&bodyDef);
    bodyDef.position.Set((x * (tileWidth * PIX2MET)) - (tileWidth * PIX2MET) * 12.5f,
        (y * (tileHeight * PIX2MET)) - (tileHeight * PIX2MET) - (tileWidth * PIX2MET) * 5.55f);
    bodyDef.angle = 0;
    body = world->CreateBody(&bodyDef);
    body->GetUserData().pointer = (uintptr_t)GROUND;
    b2FixtureDef edgeFixtureDef;
    b2EdgeShape edgeShape;
    edgeShape.SetTwoSided(b2Vec2(-0.25f, -0.25f), b2Vec2(0.25f, -0.25f));
    edgeFixtureDef.shape = &edgeShape;
    edgeFixtureDef.userData.pointer = (uintptr_t)GROUND;
    body->CreateFixture(&edgeFixtureDef);

    tileBodies.push_back(body);
}
void Map::createLeftCollision(int x, int y, b2World* world)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    b2Body* body = world->CreateBody(&bodyDef);
    bodyDef.position.Set((x * (tileWidth * PIX2MET)) - (tileWidth * PIX2MET) * 12.5f,
        (y * (tileHeight * PIX2MET)) - (tileHeight * PIX2MET) - (tileWidth * PIX2MET) * 5.55f);
    bodyDef.angle = 0;
    body = world->CreateBody(&bodyDef);
    body->GetUserData().pointer = (uintptr_t)GROUND;
    b2FixtureDef edgeFixtureDef;
    b2EdgeShape edgeShape;
    edgeShape.SetTwoSided(b2Vec2(-0.25f, -0.25f), b2Vec2(-0.25f, 0.25f));
    edgeFixtureDef.shape = &edgeShape;
    edgeFixtureDef.userData.pointer = (uintptr_t)GROUND;
    body->CreateFixture(&edgeFixtureDef);

    tileBodies.push_back(body);
}
void Map::createDownCollision(int x, int y, b2World* world)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    b2Body* body = world->CreateBody(&bodyDef);
    bodyDef.position.Set((x * (tileWidth * PIX2MET)) - (tileWidth * PIX2MET) * 12.5f,
        (y * (tileHeight * PIX2MET)) - (tileHeight * PIX2MET) - (tileWidth * PIX2MET) * 5.55f);
    bodyDef.angle = 0;
    body = world->CreateBody(&bodyDef);
    body->GetUserData().pointer = (uintptr_t)GROUND;
    b2FixtureDef edgeFixtureDef;
    b2EdgeShape edgeShape;
    edgeShape.SetTwoSided(b2Vec2(-0.25f, 0.25f), b2Vec2(0.25f, 0.25f));
    edgeFixtureDef.shape = &edgeShape;
    edgeFixtureDef.userData.pointer = (uintptr_t)GROUND;
    body->CreateFixture(&edgeFixtureDef);

    tileBodies.push_back(body);
}
void Map::createRightCollision(int x, int y, b2World* world)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    b2Body* body = world->CreateBody(&bodyDef);
    bodyDef.position.Set((x * (tileWidth * PIX2MET)) - (tileWidth * PIX2MET) * 12.5f,
        (y * (tileHeight * PIX2MET)) - (tileHeight * PIX2MET) - (tileWidth * PIX2MET) * 5.55f);
    bodyDef.angle = 0;
    body = world->CreateBody(&bodyDef);
    body->GetUserData().pointer = (uintptr_t)GROUND;
    b2FixtureDef edgeFixtureDef;
    b2EdgeShape edgeShape;
    edgeShape.SetTwoSided(b2Vec2(0.25f, -0.25f), b2Vec2(0.25f, 0.25f));
    edgeFixtureDef.shape = &edgeShape;
    edgeFixtureDef.userData.pointer = (uintptr_t)GROUND;
    body->CreateFixture(&edgeFixtureDef);

    tileBodies.push_back(body);
}