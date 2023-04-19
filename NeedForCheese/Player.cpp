#include "Player.h"

// cartesian origin box
float x_box = 0;
float y_box = -2.5f;

// size of box
float w_box = 0.5;
float h_box = 0.75;
// Box
SDL_Rect box;
b2Body* Player;
b2Vec2 pos; // Body = Body from box
b2Vec2 velo;
float angle;
b2PolygonShape dynamicBox;
b2BodyDef boxBodyDef;
b2FixtureDef fixtureDef;

void Player::Initialize(b2World* world, b2Vec2 position, b2Vec2 velocity)
{
    boxBodyDef.type = b2_dynamicBody;
    //boxBodyDef.angle = 45; // flips the whole thing -> 180 grad drehung
    boxBodyDef.angle = 0;
    boxBodyDef.fixedRotation = true;
    boxBodyDef.position.Set(x_box, y_box);

    Player = world->CreateBody(&boxBodyDef);
    Player->SetFixedRotation(true);

    dynamicBox.SetAsBox(w_box / 2.0f, h_box / 2.0f); // will be 0.5 x 0.5

    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1;
    fixtureDef.friction = 0.8f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.userData.pointer = (uintptr_t)PLAYER;
    Player->CreateFixture(&fixtureDef);

    // box: convert Metres back to Pixels for width and height
    box.w = w_box * MET2PIX;
    box.h = h_box * MET2PIX;
}

void Player::Update()
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
    pos = Player->GetPosition(); // Body = Body from box
    velo = Player->GetLinearVelocity();
    angle = Player->GetAngle();
    isGrounded = IsGrounded(Player); // check if grounded (horray)
    box.x = ((SCALED_WIDTH / 2.0f) + pos.x) * MET2PIX - box.w / 2.0f;
    box.y = (((SCALED_HEIGHT / 2.0f) + pos.y) * MET2PIX - box.h / 2.0f) + 2;
}

void Player::Draw()
{

}