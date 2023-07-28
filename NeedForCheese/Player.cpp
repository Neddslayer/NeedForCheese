#include "Player.h"
#include <iostream>
#include <fstream>
#include "SDL2/SDL_image.h"

SDL_Texture* texture_box = { 0 };
using json = nlohmann::json;

// Initialize the player. This includes adding it into the world and giving it a starting position and velocity, usually for cutscene purposes.
void Player::Initialize(b2World* world, b2Vec2 position, b2Vec2 velocity)
{
    boxBodyDef.type = b2_dynamicBody;
    boxBodyDef.angle = 0;
    boxBodyDef.fixedRotation = true;
    boxBodyDef.position.Set(position.x, position.y);

    Player_Body = world->CreateBody(&boxBodyDef);
    Player_Body->SetFixedRotation(true);
    Player_Body->SetLinearVelocity(velocity);

    dynamicBox.SetAsBox(w_box / 2.0f, h_box / 2.0f);

    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1;
    fixtureDef.friction = 0.5f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.userData.pointer = (uintptr_t)PLAYER;
    Player_Body->CreateFixture(&fixtureDef);

    Player_Body->SetBullet(true);

    // box: convert Meters back to Pixels for width and height
    box.w = w_box * MET2PIX;
    box.h = h_box * MET2PIX;

    cout << SCALE << endl;

    SDL_Surface* char_sprite;
    switch (playerType)
    {
    case Player::PLACEMAN:
        char_sprite = IMG_Load("resources/img/player/player_placeman.png");
        break;
    case Player::POGST:
        char_sprite = IMG_Load("resources/img/player/player_placeman.png");
        break;
    case Player::RUDYKIDS:
        char_sprite = IMG_Load("resources/img/player/player_placeman.png");
        break;
    case Player::NEDDSLAYER:
        char_sprite = IMG_Load("resources/img/player/player_placeman.png");
        break;
    case Player::KAUTION:
        char_sprite = IMG_Load("resources/img/player/player_placeman.png");
        break;
    default:
        char_sprite = IMG_Load("resources/img/player/player_placeman.png");
        break;
    }
    texture_box = SDL_CreateTextureFromSurface(renderer, char_sprite);
    SDL_FreeSurface(char_sprite);
    direction = 1;
    state = 0;
    std::ifstream f("resources/data/player/player_placeman.json");
    animationData = json::parse(f);
    animationSpeed = animationData["speed"];
}

// Update the player. This includes applying velocity based on input, and detecting stuff like crouching.
void Player::Update()
{
    if (keyboard[SDL_SCANCODE_LEFT]) // Moving left
    {
        Player_Body->ApplyForceToCenter(b2Vec2(-5.0f, 0.0), true);
        direction = -1;
    }

    if (keyboard[SDL_SCANCODE_RIGHT]) // Moving right
    {
        Player_Body->ApplyForceToCenter(b2Vec2(5.0f, 0.0), true);
        direction = 1;
    }

    pos = Player_Body->GetPosition(); // Get the player's rigidbody position.
    velo = Player_Body->GetLinearVelocity(); // Get the player's rigidbody velocity.

    isGrounded = IsGrounded(Player_Body); // Check if the player is grounded

    sprinting = (keyboard[SDL_SCANCODE_LSHIFT] && (velo.x > 1.5f || velo.x < -1.5f) && state != 3); // Sprinting check.
    if (keyboard[SDL_SCANCODE_Z] && isGrounded) Player_Body->ApplyForceToCenter(b2Vec2(0.0, -80.0), true); // If the player is on the ground, jump.
    
    // ---------------SPEED CAP---------------
    if (velo.x > 2.0f && !sprinting) Player_Body->SetLinearVelocity(b2Vec2(2.0f, Player_Body->GetLinearVelocity().y));
    if (velo.x < -2.0f && !sprinting) Player_Body->SetLinearVelocity(b2Vec2(-2.0f, Player_Body->GetLinearVelocity().y));
    if (velo.x > 5.0f && sprinting) Player_Body->SetLinearVelocity(b2Vec2(5.0f, Player_Body->GetLinearVelocity().y));
    if (velo.x < -5.0f && sprinting) Player_Body->SetLinearVelocity(b2Vec2(-5.0f, Player_Body->GetLinearVelocity().y));
    // ---------------------------------------

    box.x = ((SCALED_WIDTH / 2.0f) + pos.x) * MET2PIX - box.w / 2.0f;
    box.y = (((SCALED_HEIGHT / 2.0f) + pos.y) * MET2PIX - box.h / 2.0f) + MET2PIX / 30.0f;
    // Set the previous state, in case the state changes and we need to play a new animation.
    prevState = state;

    // Finally, update the player's animation state.
    UpdateState();
}

// Update the player's animation state.
void Player::UpdateState()
{
    if (!isGrounded) state = sprinting ? 4 : 3;
    if (isGrounded)
    {
        state = sprinting ? 2 : 1;
    }
    if (abs(velo.x) < 0.01 && abs(velo.y) < 0.01 && isGrounded) state = 0;
}

// Draw the player.
void Player::Draw(Camera2D camera)
{
    json indices = getCurrentAnimationIndices();
    json offset = getCurrentAnimationOffset();

    SDL_Rect sprite = { indices[0], indices[1], indices[2], indices[3] };

    SDL_Rect position = { box.x + offset[0], box.y + offset[1], box.w, box.h };

    SDL_RenderCopyEx_Camera(camera, renderer, texture_box, &sprite, &position, -0.00, NULL, direction == 1 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

// Unload the player. Shouldn't be used often, only when switching levels!
void Player::Unload()
{
    SDL_DestroyTexture(texture_box);
}

bool Player::IsGrounded(b2Body* playerBody)
{
    for (b2ContactEdge* ce = playerBody->GetContactList(); ce; ce = ce->next)
    {
        b2Contact* c = ce->contact;
        if (c->IsTouching())
        {
            b2Vec2 contactNormal = c->GetManifold()->localNormal;
            b2Fixture* fA = c->GetFixtureA();
            b2Fixture* fB = c->GetFixtureB();
            int userDataA = fA->GetBody()->GetUserData().pointer;
            int userDataB = fB->GetBody()->GetUserData().pointer;
            if (contactNormal.y == 1)
            {
                return userDataA == GROUND || userDataB == GROUND;
            }
            else if (contactNormal.y == -1)
            {
                return userDataA == GROUND || userDataB == GROUND && (fA->GetUserData().pointer == SEMISOLID || fB->GetUserData().pointer == SEMISOLID);
                
            }
        }
    }

    return false;
}

json Player::getCurrentAnimationIndices()
{
    switch (state)
    {
    case 0:
        setAnimationIndex("idle");
        return animationData["animations"]["idle"]["indices"][animationIndex];
        break;
    case 1:
        setAnimationIndex("walk");
        return animationData["animations"]["walk"]["indices"][animationIndex];
        break;
    case 2:
        setAnimationIndex("walk");
        return animationData["animations"]["walk"]["indices"][animationIndex];
        break;
    case 3:
        setAnimationIndex("jump");
        return animationData["animations"]["jump"]["indices"][animationIndex];
        break;
    case 4:
        setAnimationIndex("jump");
        return animationData["animations"]["jump"]["indices"][animationIndex];
        break;
    default:
        setAnimationIndex("idle");
        return animationData["animations"]["idle"]["indices"][animationIndex];
        break;
    }
}

json Player::getCurrentAnimationOffset()
{
    switch (state)
    {
    case 0:
        setAnimationIndex("idle");
        return animationData["animations"]["idle"]["offset"][animationIndex];
        break;
    case 1:
        setAnimationIndex("walk");
        return animationData["animations"]["walk"]["offset"][animationIndex];
        break;
    case 2:
        setAnimationIndex("walk");
        return animationData["animations"]["walk"]["offset"][animationIndex];
        break;
    case 3:
        setAnimationIndex("jump");
        return animationData["animations"]["jump"]["offset"][animationIndex];
        break;
    case 4:
        setAnimationIndex("jump");
        return animationData["animations"]["jump"]["offset"][animationIndex];
        break;
    default:
        setAnimationIndex("idle");
        return animationData["animations"]["idle"]["offset"][animationIndex];
        break;
    }
}

void Player::setAnimationIndex(const char* animation)
{
    animationTimer++;
    if (animationTimer > animationSpeed || prevState != state)
    {
        animationTimer = 0;
        animationIndex++;
        int size = animationData["animations"][animation]["indices"].size();
        if (animationIndex > (size - 1))
        {
            animationIndex = 0;
        }
    }
}