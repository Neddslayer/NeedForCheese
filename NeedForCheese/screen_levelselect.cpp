#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_image.h"
#include "screens.h"
#include "utilities.h"
#include "world.h"
#include "box2d/box2d.h"
#include <map>
#include <chrono>
#include <iostream>
#include <vector>
#include "Player.h"
#include "tinyxml2.h"
#include "Map.h"
#include "SDL_Camera.h"
#include "SDL_FontCache.h"
#include "debug.h"
using namespace std;

static int finishScreen;
float elapsedTime = 0;

b2World world = b2World(b2Vec2(0.0f, 19.84f));

float ground_x, ground_y = 0.0f;
b2Vec2 startpoint, endpoint;
b2BodyDef myGroundDef;
b2EdgeShape edgeShape;
SDL_Texture* background_sprite = { 0 };
b2FixtureDef edgeFixtureDef;

Player player = Player();
Map level = Map();
Camera2D camera = Camera2D(Vector2d(-1, 0), Vector2d(), 0.0f, Vector2d(1.0f,1.0f), { 0.0f * MET2PIX, 300.0f, 2.0f * MET2PIX, 640.0f });

static FC_Font* font;

void InitLevelSelectScreen(void)
{
    //big balls -owen
    
    SDL_Surface* bgSurface = IMG_Load("resources/img/temp-bg.png");
    background_sprite = SDL_CreateTextureFromSurface(renderer, bgSurface);
    SDL_FreeSurface(bgSurface);

    player.Initialize(&world, b2Vec2(1, 0.085), b2Vec2_zero);

    level = Map("phil", &world);
    
    font = FC_CreateFont();
    FC_LoadFont(font, renderer, "resources/font/RobotoSlab.ttf", 20 * SCALE, FC_MakeColor(0, 0, 0, 255), TTF_STYLE_NORMAL);

    camera.bounds = { 0.0f, 2.0f * MET2PIX, 1.6667f * MET2PIX, 4.0f * MET2PIX };
}

float Lerp(float startValue, float endValue, float t)
{
    t = t < 0 ? 0 : t > 1 ? 1 : t; // clamp t between 0 and 1
    return startValue + (endValue - startValue) * t * t * (3 - 2 * t);
}

void UpdateLevelSelectScreen(void)
{
    player.Update();
    world.Step(1.0f / 60.0f, 8.0f, 4.0f); // update
    
    if (keyboard[SDL_SCANCODE_R])
    {
        player.Player_Body->SetTransform(b2Vec2(1, 0.085), 0);
        player.Player_Body->SetLinearVelocity(b2Vec2_zero);
    }
    auto now = std::chrono::high_resolution_clock::now();
    auto timeSinceStart = now.time_since_epoch();
    float currentTime = std::chrono::duration_cast<std::chrono::duration<float>>(timeSinceStart).count();
    float deltaTime = currentTime - elapsedTime;
    elapsedTime = currentTime;

    // Calculate the interpolation factor
    const float InterpolationDuration = 0.05f;
    float interpolationFactor = deltaTime / InterpolationDuration;
    if (interpolationFactor > 1)
    {
        interpolationFactor = 1;
    }

    // Calculate the interpolated camera position
    //float interpolatedCameraX = Lerp(camera.target.x, SDL_clamp(player.pos.x * MET2PIX, camera.bounds.x, camera.bounds.w), interpolationFactor);
    //float interpolatedCameraY = Lerp(camera.target.y, SDL_clamp(player.pos.y * -MET2PIX + 50, camera.bounds.y, camera.bounds.h), interpolationFactor);
    float interpolatedCameraX = Lerp(camera.target.x, player.pos.x * MET2PIX, interpolationFactor);
    float interpolatedCameraY = Lerp(camera.target.y, player.pos.y * -MET2PIX + 50, interpolationFactor);
    camera.target = Vector2d(interpolatedCameraX, interpolatedCameraY);
}

void DrawLevelSelectScreen(void)
{
    SDL_RenderCopy(renderer, background_sprite, NULL, NULL);

    level.draw_map(renderer, &world, camera);
    player.Draw(camera);
    if (development_mode)
    {
        FC_Draw(font, renderer, 10, 10 * SCALE, to_string(player.pos.x).c_str());
        FC_Draw(font, renderer, 10, 40 * SCALE, to_string(player.pos.y).c_str());
        FC_Draw(font, renderer, 10, 70 * SCALE, to_string(player.velo.x).c_str());
        FC_Draw(font, renderer, 10, 100 * SCALE, to_string(player.velo.y).c_str());
        FC_Draw(font, renderer, 10, 130 * SCALE, to_string(player.state).c_str());
    }
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