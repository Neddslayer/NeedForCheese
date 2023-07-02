#ifndef PLAYER_H
#define PLAYER_H
#include "SDL2/SDL.h"
#include "box2d/box2d.h"
#include "utilities.h"
#include "world.h"
#include "SDL_Camera.h"
#include <iostream>
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

class Player
{
public:

    enum PlayerType
    {
        PLACEMAN,
        POGST,
        RUDYKIDS,
        NEDDSLAYER,
        KAUTION
    };

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

    int state, prevState; // Player animation state, and previous state for animation purposes
    int direction; // Player direction. Also used for animation

    bool sprinting; // If the player is sprinting.

    PlayerType playerType = PLACEMAN;

	// Initialize the player. This includes adding it into the world and giving it a starting position and velocity, usually for cutscene purposes.
    void Initialize(b2World* world, b2Vec2 position, b2Vec2 velocity);

	// Update the player. This includes applying velocity based on input, and detecting stuff like crouching.
	void Update();

    // Draw the player.
	void Draw(Camera2D camera);

    // Update the player's animation state.
    void UpdateState();

    // Unload the player. Shouldn't be used often, only when switching levels!
    void Unload();
private:

    // If player is on a tile marked PROPERTY_NAMES.GROUND
    bool isGrounded;

    // Check if the player is on a tile marked PROPERTY_NAMES.GROUND.
    bool IsGrounded(b2Body* playerBody);

    // The player's JSON animation file.
    json animationData;
    
    // The player's animation time before ticking to a new frame.
    int animationTimer;

    // The player's current animation indices index.
    int animationIndex;

    // The player's animation speed; stored here so the JSON isn't being called every frame.
    int animationSpeed;

    // Gets the current animation indices from the current state.
    json getCurrentAnimationIndices();

    // Sets and ticks the current animation.
    void setAnimationIndex(const char* animation);
};

#endif // !PLAYER_H

