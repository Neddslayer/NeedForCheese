#include "Player.h"
#include "SDL2/SDL_image.h"

SDL_Texture* texture_box = { 0 };

void Player::Initialize(b2World* world, b2Vec2 position, b2Vec2 velocity)
{
    boxBodyDef.type = b2_dynamicBody;
    //boxBodyDef.angle = 45; // flips the whole thing -> 180 grad drehung
    boxBodyDef.angle = 0;
    boxBodyDef.fixedRotation = true;
    boxBodyDef.position.Set(position.x, position.y);

    Player_Body = world->CreateBody(&boxBodyDef);
    Player_Body->SetFixedRotation(true);
    Player_Body->SetLinearVelocity(velocity);

    dynamicBox.SetAsBox(w_box / 2.0f, h_box / 2.0f); // will be 0.5 x 0.5

    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1;
    fixtureDef.friction = 0.8f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.userData.pointer = (uintptr_t)PLAYER;
    Player_Body->CreateFixture(&fixtureDef);

    // box: convert Metres back to Pixels for width and height
    box.w = w_box * MET2PIX;
    box.h = h_box * MET2PIX;

    SDL_Surface* tmp_sprites;
    tmp_sprites = IMG_Load("resources/img/checker.png");
    texture_box = SDL_CreateTextureFromSurface(renderer, tmp_sprites);
    SDL_FreeSurface(tmp_sprites);
    direction = 1;
    state = 0;
}

void Player::Update()
{
    if (keyboard[SDL_SCANCODE_LEFT])
    {
        Player_Body->ApplyForceToCenter(b2Vec2(-5.0, 0.0), true);
        direction = -1;
        state = 1;
    }
    if (keyboard[SDL_SCANCODE_RIGHT])
    {
        Player_Body->ApplyForceToCenter(b2Vec2(5.0, 0.0), true);
        direction = 1;
        state = 1;
    }
    if (keyboard[SDL_SCANCODE_Z] && isGrounded)
    {
        Player_Body->ApplyForceToCenter(b2Vec2(0.0, -35.0), true);
        state = 3;
    }
    pos = Player_Body->GetPosition(); // Body = Body from box
    velo = Player_Body->GetLinearVelocity();
    angle = Player_Body->GetAngle();
    isGrounded = IsGrounded(Player_Body); // check if grounded (horray)
    box.x = ((SCALED_WIDTH / 2.0f) + pos.x) * MET2PIX - box.w / 2.0f;
    box.y = (((SCALED_HEIGHT / 2.0f) + pos.y) * MET2PIX - box.h / 2.0f) + MET2PIX / 40.0f;
    if (-0.2 < velo.x < 0.2 && -0.2 < velo.y < 0.2) state = 0;
    std::cout << pos.x << " " << pos.y << endl;
}

void Player::Draw()
{
    switch (state)
    {
    case 0:
        // animate idle
        break;
    case 1:
        // animate walking
        break;
    case 2:
        // animate running
        break;
    case 3:
        // animate jumping
        break;
    case 4:
        //animate falling
        break;
    case 5:
        // animate attack
        break;
    case 6:
        // animate block
        break;
    default:
        // animate idle
        break;
    }

    SDL_RenderCopyEx(renderer, texture_box, NULL, &box, angle, NULL, SDL_FLIP_NONE);
}

void Player::Unload()
{
    SDL_DestroyTexture(texture_box);
}

bool Player::IsGrounded(b2Body* playerBody)
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
                int userDataA = fA->GetBody()->GetUserData().pointer;
                int userDataB = fB->GetBody()->GetUserData().pointer;
                if (userDataA == GROUND || userDataB == GROUND)
                {
                    grounded = true;
                    break;
                }
            }
        }
    }

    return grounded;
}