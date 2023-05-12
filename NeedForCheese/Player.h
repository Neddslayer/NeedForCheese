#ifndef PLAYER_H
#define PLAYER_H
#include "SDL2/SDL.h"
#include "box2d/box2d.h"
#include "utilities.h"
#include "world.h"
class Player
{
public:
    // cartesian origin box
    float x_box; // Player body starting X
    float y_box; // Player body starting Y

    float w_box = 0.5; // Player body half-width
    float h_box = 0.75; // Player body half-height
    SDL_Rect box; // Character sprite box
    b2Body* Player_Body; // Player rigidbody. Used the most for movement.
    b2Vec2 pos; // Player position
    b2Vec2 velo; // Player velocity
    float angle; // Player angle. Currently unused as of now.
    b2PolygonShape dynamicBox; // Player polygon shape. Used when initializing to set hitbox.
    b2BodyDef boxBodyDef; // Player body definition. Used while initializing.
    b2FixtureDef fixtureDef; // Player fixture definition. Used while initializing.

    int state; // Player animation state
    int direction; // Player direction. Also used for animation

	// Initialize the player. This includes adding it into the world and giving it a starting position and velocity, usually for cutscene purposes.
    void Initialize(b2World* world, b2Vec2 position, b2Vec2 velocity);
	// Update the player. This includes applying velocity based on input, and detecting stuff like crouching.
	void Update();
    // Draw the player.
	void Draw();
    // Unload the player. Shouldn't be used often, only when switching levels!
    void Unload();
private:

    // If player is on a tile marked PROPERTY_NAMES.GROUND
    bool isGrounded;

    // Check if the player is on a tile marked PROPERTY_NAMES.GROUND. Loops through all bodies in a scene, so shouldn't be used often!
    bool IsGrounded(b2Body* playerBody);
};

#endif // !PLAYER_H

