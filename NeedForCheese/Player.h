#include "SDL2/SDL.h"
#include "box2d/box2d.h"
#include "utilities.h"
#include "world.h"

#ifndef PLAYER_H
#define PLAYER_H

class Player
{
public:
    // cartesian origin box
    float x_box;
    float y_box;

    // size of box
    float w_box = 0.5;
    float h_box = 0.75;
    // Box
    SDL_Rect box;
    b2Body* Player_Body;
    b2Vec2 pos; // Body = Body from box
    b2Vec2 velo;
    float angle;
    b2PolygonShape dynamicBox;
    b2BodyDef boxBodyDef;
    b2FixtureDef fixtureDef;

	// Initialize the player. This includes adding it into the world and giving it a starting position and velocity, usually for cutscene purposes.
    void Initialize(b2World* world, b2Vec2 position, b2Vec2 velocity);
	// Update the player. This includes applying velocity based on input, and detecting stuff like crouching.
	void Update();
	void Draw();
    void Unload();
private:

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
};

#endif // !PLAYER_H
