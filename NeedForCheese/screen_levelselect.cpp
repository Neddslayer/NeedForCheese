#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_image.h"
#include "screens.h"
#include "utilities.h"
#include "box2d/box2d.h"
#include <map>
#include <iostream>
using namespace std;

static int finishScreen;

b2World world = b2World(b2Vec2(0.0f, 9.81f));

const int MET2PIX = 80; // 640 / 80 = 8

const int WIDTH = 1280;
const int HEIGHT = 720;

const int SCALED_WIDTH = WIDTH / MET2PIX; // 4 | 3
const int SCALED_HEIGHT = HEIGHT / MET2PIX;

//1 rad x 180/pi = 57,296deg
const float RAD2DEG = 180 / M_PI;

float ground_x, ground_y = 0.0f;
b2Vec2 startpoint, endpoint;
b2BodyDef myGroundDef;
// cartesian origin box
float x_box = 0;
float y_box = -2.5f;

// size of box
float w_box = 0.5;
float h_box = 0.75;
// Box
SDL_Rect box;
b2Body* Player;
b2EdgeShape edgeShape;
SDL_Texture* texture_box = { 0 };
SDL_Texture* background_sprite = { 0 };
b2Vec2 pos; // Body = Body from box
b2Vec2 velo;
float angle;
b2PolygonShape dynamicBox;
b2FixtureDef edgeFixtureDef;
b2BodyDef boxBodyDef;
b2FixtureDef fixtureDef;

bool isGrounded;

bool IsGrounded(b2Body* playerBody)
{
    bool grounded = false;

    for (b2ContactEdge* ce = playerBody->GetContactList(); ce; ce = ce->next)
    {
        b2Contact* c = ce->contact;
        if (c->IsTouching())
        {
            b2Vec2 contactNormal = c->GetManifold()->localNormal;
            if (contactNormal.y < -0.5f)
            {
                b2Fixture* fA = c->GetFixtureA();
                b2Fixture* fB = c->GetFixtureB();
                int userDataA = fA->GetUserData().pointer;
                int userDataB = fB->GetUserData().pointer;
                if (userDataA == 2 || userDataB == 2)
                {
                    grounded = true;
                    break;
                }
            }
        }
    }

    return grounded;
}

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
    edgeFixtureDef.userData.pointer = (uintptr_t)2;
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

    boxBodyDef.type = b2_dynamicBody;
    //boxBodyDef.angle = 45; // flips the whole thing -> 180 grad drehung
    boxBodyDef.angle = 0;
    boxBodyDef.fixedRotation = true;
    boxBodyDef.position.Set(x_box, y_box);

    Player = world.CreateBody(&boxBodyDef);
    Player->SetFixedRotation(true);

    dynamicBox.SetAsBox(w_box / 2.0f, h_box / 2.0f); // will be 0.5 x 0.5

    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1;
    fixtureDef.friction = 0.8f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.userData.pointer = (uintptr_t)1;
    Player->CreateFixture(&fixtureDef);

    // box: convert Metres back to Pixels for width and height
    box.w = w_box * MET2PIX;
    box.h = h_box * MET2PIX;
}

void UpdateLevelSelectScreen(void)
{
    if (keyboard[SDL_SCANCODE_LEFT])
    {
        Player->ApplyForceToCenter(b2Vec2(-5.0, 0.0), true);
    }
    if (keyboard[SDL_SCANCODE_RIGHT])
    {
        Player->ApplyForceToCenter(b2Vec2(5.0, 0.0), true);
    }
    if (keyboard[SDL_SCANCODE_Z] && isGrounded)
    {
        Player->ApplyForceToCenter(b2Vec2(0.0, -50.0), true);
    }
    world.Step(1.0f / 60.0f, 6.0f, 2.0f); // update
    pos = Player->GetPosition(); // Body = Body from box
    velo = Player->GetLinearVelocity();
    angle = Player->GetAngle();

    isGrounded = IsGrounded(Player); // check if grounded (horray)

    cout << "Position: { " << pos.x << ",  " << pos.y << " }" << endl << "Velocity: { " << velo.x << ", " << velo.y << " }" << endl;

    box.x = ((SCALED_WIDTH / 2.0f) + pos.x) * MET2PIX - box.w / 2.0f;
    box.y = (((SCALED_HEIGHT / 2.0f) + pos.y) * MET2PIX - box.h / 2.0f) + 2;

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