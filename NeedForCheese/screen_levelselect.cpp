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
#include "SDL_Camera.h"
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
Camera2D camera = Camera2D(Vector2(-1, 0), Vector2(), 0.0f, Vector2(1.0f,1.0f));

void InitLevelSelectScreen(void)
{
    //big balls -owen
    
    SDL_Surface* bgSurface = IMG_Load("resources/img/title_select_bg.png");
    background_sprite = SDL_CreateTextureFromSurface(renderer, bgSurface);
    SDL_FreeSurface(bgSurface);

    player.Initialize(&world, b2Vec2_zero, b2Vec2_zero);

    level = Map("resources/map/test.xml", &world);
    
}

void UpdateLevelSelectScreen(void)
{
    player.Update();
    world.Step(1.0f / 60.0f, 6.0f, 2.0f); // update
    
    if (keyboard[SDL_SCANCODE_R])
    {
        player.Player_Body->SetTransform(b2Vec2_zero, 0);
        player.Player_Body->SetLinearVelocity(b2Vec2_zero);
    }
    if (keyboard[SDL_SCANCODE_D])
    {
        camera.target += Vector2(5);
    }
    if (keyboard[SDL_SCANCODE_A])
    {
        camera.target -= Vector2(5);
    }
    if (keyboard[SDL_SCANCODE_S])
    {
        camera.target -= Vector2(0, 5);
    }
    if (keyboard[SDL_SCANCODE_W])
    {
        camera.target += Vector2(0, 5);
    }
}

void DrawLevelSelectScreen(void)
{
    SDL_RenderCopy(renderer, background_sprite, NULL, NULL);

    level.draw_map(renderer, &world, camera);
    player.Draw(camera);
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