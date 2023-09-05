#include "SDL_Camera.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "../util/SDL_FontCache.h"
#include "../h/utilities.h"
#include "../h/world.h"

// Render a texture with a camera offset.
int SDL_RenderCopy_Camera(Camera2D camera, SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* srcrect, SDL_Rect* dstrect)
{
    if (dstrect == NULL) return SDL_RenderCopy(renderer, texture, srcrect, dstrect);
    SDL_Rect moddstrect =
    {
        dstrect->x - camera.target.x - camera.offset.x,
        dstrect->y + camera.target.y + camera.offset.y,
        dstrect->w * camera.zoom.x, 
        dstrect->h * camera.zoom.y 
    };
    return SDL_RenderCopy(renderer, texture, srcrect, &moddstrect);
}

// Render a texture with more customizability and a camera offset.
int SDL_RenderCopyEx_Camera(Camera2D camera, SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* srcrect, SDL_Rect* dstrect, double angle, const SDL_Point* center, SDL_RendererFlip flip)
{
    if (dstrect == NULL) return SDL_RenderCopyEx(renderer, texture, srcrect, dstrect, angle, center, flip);
    SDL_Rect moddstrect = 
    {
        dstrect->x - camera.target.x - camera.offset.x,
        dstrect->y + camera.target.y + camera.offset.y,
        dstrect->w * camera.zoom.x, 
        dstrect->h * camera.zoom.y 
    };
    return SDL_RenderCopyEx(renderer, texture, srcrect, &moddstrect, angle, center, flip);
}

// Draw a line with a camera offset.
int SDL_RenderDrawLine_Camera(Camera2D camera, SDL_Renderer* renderer, int x1, int y1, int x2, int y2)
{
    
    return SDL_RenderDrawLine(renderer, 
        x1 - camera.target.x - camera.offset.x,
        y1 + camera.target.y + camera.offset.y,
        x2 - camera.target.x - camera.offset.x,
        y2 + camera.target.y + camera.offset.y);
}

// Draw a rect with a camera offset.
int SDL_RenderFillRect_Camera(Camera2D camera, SDL_Renderer* renderer, SDL_Rect* rect)
{
    SDL_Rect modrect = 
    { 
        rect->x - camera.target.x - camera.offset.x,
        rect->y + camera.target.y + camera.offset.y,
        rect->w * camera.zoom.x,
        rect->h * camera.zoom.y
    };
    return SDL_RenderFillRect(renderer, &modrect);
}

void Camera_UpdatePositionFromBounds(Camera2D camera)
{
    cout << camera.target.x << endl;
    camera.target = Vector2d(SDL_clamp(camera.target.x, camera.bounds.x, camera.bounds.w), SDL_clamp(camera.target.y, camera.bounds.y, camera.bounds.h));
}

Vector2f Vector2f::operator+(const Vector2f& other) const
{
    return Vector2f(x + other.x, y + other.y);
}
Vector2f Vector2f::operator-(const Vector2f& other) const
{
    return Vector2f(x - other.x, y - other.y);
}
Vector2f& Vector2f::operator+=(const Vector2f& other)
{
    x += other.x;
    y += other.y;
    return *this;
}
Vector2f& Vector2f::operator-=(const Vector2f& other)
{
    x -= other.x;
    y -= other.y;
    return *this;
}
Vector2d Vector2d::operator+(const Vector2d& other) const
{
    return Vector2d(x + other.x, y + other.y);
}
Vector2d Vector2d::operator-(const Vector2d& other) const
{
    return Vector2d(x - other.x, y - other.y);
}
Vector2d& Vector2d::operator+=(const Vector2d& other)
{
    x += other.x;
    y += other.y;
    return *this;
}
Vector2d& Vector2d::operator-=(const Vector2d& other)
{
    x -= other.x;
    y -= other.y;
    return *this;
}
