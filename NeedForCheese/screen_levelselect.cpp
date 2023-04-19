#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_image.h"
#include "screens.h"
#include "utilities.h"
#include "world.h"
#include "box2d/box2d.h"
#include <map>
#include <iostream>
#include "Player.h"
using namespace std;

static int finishScreen;

b2World world = b2World(b2Vec2(0.0f, 9.81f));

float ground_x, ground_y = 0.0f;
b2Vec2 startpoint, endpoint;
b2BodyDef myGroundDef;
b2EdgeShape edgeShape;
SDL_Texture* texture_box = { 0 };
SDL_Texture* background_sprite = { 0 };
b2FixtureDef edgeFixtureDef;

Player player = Player();


void InitLevelSelectScreen(void)
{
    // start ground point
    startpoint.x = -2.5f;
    startpoint.y = 0;

    // end ground point
    endpoint.x = 2.5;
    endpoint.y = 0;

    // LineGround
    myGroundDef.type = b2_staticBody;
    myGroundDef.position.Set(ground_x, ground_y); // set the starting position x and y cartesian
    myGroundDef.angle = 0;

    b2Body* groundLineBody = world.CreateBody(&myGroundDef);

    edgeShape.SetTwoSided(startpoint, endpoint);

    edgeFixtureDef.shape = &edgeShape;
    edgeFixtureDef.userData.pointer = (uintptr_t)GROUND;
    groundLineBody->CreateFixture(&edgeFixtureDef);

    SDL_Surface* tmp_sprites;
    tmp_sprites = IMG_Load("resources/img/checker.png");
    // balls are my favorite 🤓👆
    texture_box = SDL_CreateTextureFromSurface(renderer, tmp_sprites);
    SDL_FreeSurface(tmp_sprites);
    //big balls -owen
    
    SDL_Surface* bgSurface = IMG_Load("resources/img/title_select_bg.png");
    background_sprite = SDL_CreateTextureFromSurface(renderer, bgSurface);
    SDL_FreeSurface(bgSurface);

    player.Initialize();
    
}

void UpdateLevelSelectScreen(void)
{
    world.Step(1.0f / 60.0f, 6.0f, 2.0f); // update
    //cout << "Position: { " << pos.x << ",  " << pos.y << " }" << endl << "Velocity: { " << velo.x << ", " << velo.y << " }" << endl;
}

void DrawLevelSelectScreen(void)
{
    SDL_RenderCopy(renderer, background_sprite, NULL, NULL);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawLine(renderer, ((SCALED_WIDTH / 2.0f) + edgeShape.m_vertex1.x) * MET2PIX, ((SCALED_HEIGHT / 2.0f) + edgeShape.m_vertex1.y) * MET2PIX, ((SCALED_WIDTH / 2.0f) + edgeShape.m_vertex2.x) * MET2PIX, ((SCALED_HEIGHT / 2.0f) + edgeShape.m_vertex2.y) * MET2PIX);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_RenderCopyEx(renderer, texture_box, NULL, &box, angle, NULL, SDL_FLIP_NONE);

}

void UnloadLevelSelectScreen(void)
{
    SDL_DestroyTexture(background_sprite);
    SDL_DestroyTexture(texture_box);
}

int FinishLevelSelectScreen(void)
{
	return finishScreen;
}