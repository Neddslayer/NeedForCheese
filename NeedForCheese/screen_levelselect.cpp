#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_image.h"
#include "screens.h"
#include "utilities.h"
#include "world.h"
#include "box2d/box2d.h"
#include <map>
#include <iostream>
#include <vector>
#include "Player.h"
#include "tinyxml2.h"
using namespace std;

static int finishScreen;

b2World world = b2World(b2Vec2(0.0f, 9.81f));

float ground_x, ground_y = 0.0f;
b2Vec2 startpoint, endpoint;
b2BodyDef myGroundDef;
b2EdgeShape edgeShape;
SDL_Texture* background_sprite = { 0 };
b2FixtureDef edgeFixtureDef;

Player player = Player();

std::vector<int> split_string_to_ints(const char* str, const char* delimiter)
{
    std::vector<int> result;
    char* tmp = strdup(str);
    char* token = strtok(tmp, delimiter);
    while (token != nullptr)
    {
        result.push_back(atoi(token));
        token = strtok(nullptr, delimiter);
    }
    free(tmp);
    return result;
}

void load_map(const char* filename, SDL_Renderer* renderer)
{
    // Load the XML document and get the root map node
    tinyxml2::XMLDocument doc;
    doc.LoadFile(filename);
    tinyxml2::XMLElement* mapNode = doc.FirstChildElement("map");

    // Get the map dimensions and tile size
    int width, height, tileWidth, tileHeight;
    mapNode->QueryIntAttribute("width", &width);
    mapNode->QueryIntAttribute("height", &height);
    mapNode->QueryIntAttribute("tilewidth", &tileWidth);
    mapNode->QueryIntAttribute("tileheight", &tileHeight);

    // Load the tileset texture
    tinyxml2::XMLElement* tilesetNode = mapNode->FirstChildElement("tileset");
    const char* imagePath = tilesetNode->FirstChildElement("image")->Attribute("source");
    SDL_Texture* texture = IMG_LoadTexture(renderer, imagePath);
    int texture_width, texture_height;
    SDL_QueryTexture(texture, NULL, NULL, &texture_width, &texture_height);
    SDL_Rect srcrect = { 0, 0, tileWidth, tileHeight };

    // Iterate through the background layer and draw each tile
    for (tinyxml2::XMLElement* layerNode = mapNode->FirstChildElement("layer"); layerNode != nullptr; layerNode = layerNode->NextSiblingElement("layer"))
    {
        if (strcmp(layerNode->Attribute("name"), "background") == 0)
        {
            const char* data = layerNode->FirstChildElement("data")->GetText();
            std::vector<int> tiles = split_string_to_ints(data, ",");
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    int index = tiles[y * width + x];
                    if (index == 0)
                    {
                        continue;
                    }
                    int tilex = (index - 1) % (texture_width / tileWidth);
                    int tiley = (index - 1) / (texture_width / tileWidth);
                    SDL_Rect srcrect = { tilex * tileWidth, tiley * tileHeight, tileWidth, tileHeight };
                    SDL_Rect dstrect = { x * tileWidth, y * tileHeight, tileWidth, tileHeight };
                    SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
                }
            }
        }
    }
}



void InitLevelSelectScreen(void)
{
    // start ground point
    startpoint.x = -2.5f;
    startpoint.y = 1.5;

    // end ground point
    endpoint.x = 2.5;
    endpoint.y = 1.5;

    // LineGround
    myGroundDef.type = b2_staticBody;
    myGroundDef.position.Set(ground_x, ground_y); // set the starting position x and y cartesian
    myGroundDef.angle = 0;

    b2Body* groundLineBody = world.CreateBody(&myGroundDef);

    edgeShape.SetTwoSided(startpoint, endpoint);

    edgeFixtureDef.shape = &edgeShape;
    edgeFixtureDef.userData.pointer = (uintptr_t)GROUND;
    groundLineBody->CreateFixture(&edgeFixtureDef);

    //big balls -owen
    
    SDL_Surface* bgSurface = IMG_Load("resources/img/title_select_bg.png");
    background_sprite = SDL_CreateTextureFromSurface(renderer, bgSurface);
    SDL_FreeSurface(bgSurface);

    player.Initialize(&world, b2Vec2(0, -2.5), b2Vec2_zero);
    
}

void UpdateLevelSelectScreen(void)
{
    player.Update();
    world.Step(1.0f / 60.0f, 6.0f, 2.0f); // update
    //cout << "Position: { " << pos.x << ",  " << pos.y << " }" << endl << "Velocity: { " << velo.x << ", " << velo.y << " }" << endl;
    if (keyboard[SDL_SCANCODE_R])
    {
        player.Player_Body->SetTransform(b2Vec2_zero, 0);
        player.Player_Body->SetLinearVelocity(b2Vec2_zero);
    }
}

void DrawLevelSelectScreen(void)
{
    SDL_RenderCopy(renderer, background_sprite, NULL, NULL);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawLine(renderer, ((SCALED_WIDTH / 2.0f) + edgeShape.m_vertex1.x) * MET2PIX, ((SCALED_HEIGHT / 2.0f) + edgeShape.m_vertex1.y) * MET2PIX, ((SCALED_WIDTH / 2.0f) + edgeShape.m_vertex2.x) * MET2PIX, ((SCALED_HEIGHT / 2.0f) + edgeShape.m_vertex2.y) * MET2PIX);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    player.Draw();

}

void UnloadLevelSelectScreen(void)
{
    player.Unload();
    SDL_DestroyTexture(background_sprite);
}

int FinishLevelSelectScreen(void)
{
	return finishScreen;
}