#include "Player.h"
#include "SDL2/SDL_image.h"

SDL_Texture* texture_box = { 0 };

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
        Player_Body->ApplyForceToCenter(b2Vec2(-5.0f, 0.0), true);
        direction = -1;
    }

    if (keyboard[SDL_SCANCODE_RIGHT])
    {
        Player_Body->ApplyForceToCenter(b2Vec2(5.0f, 0.0), true);
        direction = 1;
    }

    pos = Player_Body->GetPosition(); // Body = Body from box
    velo = Player_Body->GetLinearVelocity();
    angle = Player_Body->GetAngle();

    isGrounded = IsGrounded(Player_Body); // check if grounded (horray)
    sprinting = (keyboard[SDL_SCANCODE_LSHIFT] && (velo.x > 1.5f || velo.x < -1.5f) && state != 3);
    if (keyboard[SDL_SCANCODE_Z] && isGrounded) Player_Body->ApplyForceToCenter(b2Vec2(0.0, -50.0), true);
    
    if (velo.x > 2.0f && !sprinting) Player_Body->SetLinearVelocity(b2Vec2(2.0f, Player_Body->GetLinearVelocity().y));
    if (velo.x < -2.0f && !sprinting) Player_Body->SetLinearVelocity(b2Vec2(-2.0f, Player_Body->GetLinearVelocity().y));
    if (velo.x > 4.0f && sprinting) Player_Body->SetLinearVelocity(b2Vec2(6.0f, Player_Body->GetLinearVelocity().y));
    if (velo.x < -4.0f && sprinting) Player_Body->SetLinearVelocity(b2Vec2(-6.0f, Player_Body->GetLinearVelocity().y));

    box.x = ((SCALED_WIDTH / 2.0f) + pos.x) * MET2PIX - box.w / 2.0f;
    box.y = (((SCALED_HEIGHT / 2.0f) + pos.y) * MET2PIX - box.h / 2.0f) + MET2PIX / 20.0f;
    UpdateState();
}

void Player::UpdateState()
{
    if (!isGrounded) state = sprinting ? 4 : 3;
    if (isGrounded)
    {
        state = sprinting ? 2 : 1;
    }
    if (abs(velo.x) < 0.01 && abs(velo.y) < 0.01 && isGrounded) state = 0;
}

void Player::Draw(Camera2D camera)
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
        //animate run-jumping
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

    SDL_RenderCopyEx_Camera(camera, renderer, texture_box, NULL, &box, angle, NULL, SDL_FLIP_NONE);
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
            if (contactNormal.y == 1)
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