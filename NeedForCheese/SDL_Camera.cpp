#include "SDL_Camera.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL_FontCache.h"
#include "utilities.h"
#include "world.h"

SDL_Texture* old_target;
SDL_Rect old_viewport;

void BeginMode2D(Camera2D camera)
{
    SDL_RenderGetViewport(renderer, &old_viewport);

    // Set up the camera transformation matrix
    SDL_Rect viewport = {
        SDL_max((int)camera.target.x + (int)camera.offset.x, 0),
        SDL_max((int)camera.target.y + (int)camera.offset.y, 0),
        (int)(WIDTH),
        (int)(HEIGHT)
    };
    SDL_RenderSetViewport(renderer, &viewport);
}

void EndMode2D()
{
    SDL_RenderSetViewport(renderer, &old_viewport);

    // Reset the rendering transformations
    SDL_RenderSetScale(renderer, 1.0f, 1.0f);
}
Vector2 Vector2::operator+(const Vector2& other) const
{
    return Vector2(x + other.x, y + other.y);
}
Vector2 Vector2::operator-(const Vector2& other) const
{
    return Vector2(x - other.x, y - other.y);
}
Vector2& Vector2::operator+=(const Vector2& other)
{
    x += other.x;
    y += other.y;
    return *this;
}
Vector2& Vector2::operator-=(const Vector2& other)
{
    x -= other.x;
    y -= other.y;
    return *this;
}
