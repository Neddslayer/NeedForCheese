#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_image.h"
#include "screens.h"
#include "utilities.h"
#include "box2d/box2d.h"
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
float h_box = 0.5;
// Box
SDL_Rect box;
b2Body* Body;
b2EdgeShape edgeShape;
SDL_Texture* texture_box = { 0 };
SDL_Texture* background_sprite = { 0 };
b2Vec2 pos; // Body = Body from box
float angle;
b2PolygonShape dynamicBox;
b2FixtureDef edgeFixtureDef;
b2BodyDef boxBodyDef;
b2FixtureDef fixtureDef;

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
    groundLineBody->CreateFixture(&edgeFixtureDef);

    SDL_Surface* tmp_sprites;
    tmp_sprites = IMG_Load("resources/img/checker.png");
    // balls are my favorite 🤓👆
    texture_box = SDL_CreateTextureFromSurface(renderer, tmp_sprites);
    SDL_FreeSurface(tmp_sprites);
    //big cum balls
    
    SDL_Surface* bgSurface = IMG_Load("resources/img/title_select_bg.png");
    background_sprite = SDL_CreateTextureFromSurface(renderer, bgSurface);
    SDL_FreeSurface(bgSurface);

    boxBodyDef.type = b2_dynamicBody;
    //boxBodyDef.angle = 45; // flips the whole thing -> 180 grad drehung
    boxBodyDef.angle = 0;
    boxBodyDef.fixedRotation = false;
    boxBodyDef.position.Set(x_box, y_box);

    Body = world.CreateBody(&boxBodyDef);
    Body->SetFixedRotation(false);

    dynamicBox.SetAsBox(w_box / 2.0f, h_box / 2.0f); // will be 0.5 x 0.5

    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.5f;
    Body->CreateFixture(&fixtureDef);

    // box: convert Metres back to Pixels for width and height
    box.w = w_box * MET2PIX;
    box.h = h_box * MET2PIX;
}
void UpdateLevelSelectScreen(void)
{
    if (keyboard[SDL_SCANCODE_A])
    {
        Body->ApplyForceToCenter(b2Vec2(-2.0, 0.0), true);
    }
    if (keyboard[SDL_SCANCODE_D])
    {
        Body->ApplyForceToCenter(b2Vec2(2.0, 0.0), true);
    }
    world.Step(1.0f / 60.0f, 6.0f, 2.0f); // update
    pos = Body->GetPosition(); // Body = Body from box
    angle = Body->GetAngle();
    box.x = ((SCALED_WIDTH / 2.0f) + pos.x) * MET2PIX - box.w / 2.0f;
    box.y = (((SCALED_HEIGHT / 2.0f) + pos.y) * MET2PIX - box.h / 2.0f) + 2;

    cout << "X of box:" << box.x << endl;
    cout << "Y of box:" << box.y << endl;
    cout << "Angle of box: " << angle << endl;

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
    delete &world;
}
int FinishLevelSelectScreen(void)
{
	return finishScreen;
}
/*
* 
b2World* world;

const int MET2PIX = 80; // 640 / 80 = 8

const int WIDTH = 640;
const int HEIGHT = 480;

const int SCALED_WIDTH = WIDTH / MET2PIX; // 4 | 3
const int SCALED_HEIGHT = HEIGHT / MET2PIX;

//1 rad × 180/π = 57,296°
const float RAD2DEG = 180 / M_PI;

using namespace std;

int main()
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("FirstGame", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    world = new b2World(b2Vec2(0.0f, 9.81f));
    
    // cartesian origin
    float ground_x = 0.0f;
    float ground_y = 0.0f;
    
    // start ground point
    b2Vec2 startpoint;
    startpoint.x = -2.5f;
    startpoint.y = 0;
    
    // end ground point
    b2Vec2 endpoint;
    endpoint.x = 2.5;
    endpoint.y = 0;
    
    // LineGround
    b2BodyDef myGroundDef;
    myGroundDef.type = b2_staticBody;
    myGroundDef.position.Set(ground_x, ground_y); // set the starting position x and y cartesian
    myGroundDef.angle = 0;
    
    b2Body* groundLineBody = world->CreateBody(&myGroundDef);
    
    b2EdgeShape edgeShape;
    edgeShape.Set( startpoint, endpoint ); // length -> coordinate vector from to vector
    
    b2FixtureDef edgeFixtureDef;
    edgeFixtureDef.shape = &edgeShape;
    groundLineBody->CreateFixture(&edgeFixtureDef);
    
    SDL_Surface* tmp_sprites;
    tmp_sprites = IMG_Load("assets/box.png");
    if(!tmp_sprites)
        return EXIT_FAILURE;
    
    SDL_Texture* texture_box = SDL_CreateTextureFromSurface(renderer, tmp_sprites);
    SDL_FreeSurface(tmp_sprites);
    
    // cartesian origin box
    float x_box = 0;
    float y_box = -2.5f;
    
    // size of box
    float w_box = 0.5;
    float h_box = 0.5;
    
    // Box
    SDL_Rect box;
    b2Body* Body;

    b2BodyDef boxBodyDef;
    boxBodyDef.type = b2_dynamicBody;
    boxBodyDef.angle = 45; // flips the whole thing -> 180 grad drehung
    //boxBodyDef.angle = 0;
    boxBodyDef.position.Set(x_box, y_box);
    
    Body = world->CreateBody(&boxBodyDef);
    
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(w_box / 2.0f, h_box / 2.0f); // will be 0.5 x 0.5
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.5f;
    Body->CreateFixture(&fixtureDef);
    
    // box: convert Metres back to Pixels for width and height
    box.w = w_box * MET2PIX;
    box.h = h_box * MET2PIX;
    
    bool close_game = false;
    SDL_Event event;
    
    // The game Loop
    while(close_game != true)
    {
        b2Vec2 pos = Body->GetPosition(); // Body = Body from box
        float angle = Body->GetAngle();
        
        // RAD2Degree
        angle *= RAD2DEG;
        
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
                close_game = true;
            
            else if(event.key.keysym.sym == SDLK_ESCAPE)
                close_game = true;
        }
        
        box.x = ((SCALED_WIDTH / 2.0f) + pos.x) * MET2PIX - box.w / 2;
        box.y = ((SCALED_HEIGHT / 2.0f) + pos.y) * MET2PIX - box.h / 2;
        
        cout << "X of box:" << setprecision(20) << box.x << endl;
        cout << "Y of box:" << setprecision(20) << box.y << endl;
        
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 0);
        
        // Draw ground platform
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 0);
        SDL_RenderDrawLine(renderer, ((SCALED_WIDTH / 2.0f) + edgeShape.m_vertex1.x) * MET2PIX, ((SCALED_HEIGHT / 2.0f) + edgeShape.m_vertex1.y) * MET2PIX, ((SCALED_WIDTH / 2.0f) + edgeShape.m_vertex2.x) * MET2PIX, ((SCALED_HEIGHT / 2.0f) + edgeShape.m_vertex2.y) * MET2PIX);
        
        // Draw our Box angle 45
        // Body->SetFixedRotation(true); - sets no rotation at all
        SDL_RenderCopyEx(renderer, texture_box, NULL, &box, angle, NULL, SDL_FLIP_NONE);
        
        // Draw box angle 45
        //Body->SetAngularVelocity(10.0f);
        //Body->SetFixedRotation(true);
        //SDL_RenderDrawRect(renderer, &box);
        //SDL_RenderFillRect(renderer, &box);
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderPresent(renderer);
        
        world->Step(1.0f / 60.0f, 6.0f, 2.0f); // update
        
    }
    
    delete world;
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
*/