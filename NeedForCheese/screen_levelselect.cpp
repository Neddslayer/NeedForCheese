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
#include "Map.h"
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
Map level = Map();

void InitLevelSelectScreen(void)
{
    //big balls -owen
    
    SDL_Surface* bgSurface = IMG_Load("resources/img/title_select_bg.png");
    background_sprite = SDL_CreateTextureFromSurface(renderer, bgSurface);
    SDL_FreeSurface(bgSurface);

    player.Initialize(&world, b2Vec2(0, 0), b2Vec2_zero);

    level = Map("resources/map/timmy.xml", &world);
    
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
    level.draw_map(renderer, &world);
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