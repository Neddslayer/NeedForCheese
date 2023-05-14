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
        (int)camera.target.x + (int)camera.offset.x,
        (int)camera.target.y + (int)camera.offset.y,
        (int)(WIDTH),
        (int)(HEIGHT)
    };
    int thing = SDL_RenderSetViewport(renderer, &viewport);
    std::cout << thing << endl;
}
void EndMode2D()
{
    SDL_RenderSetViewport(renderer, &old_viewport);

    // Reset the rendering transformations
    SDL_RenderSetScale(renderer, 1.0f, 1.0f);
}

int SDL_RenderCopy_Camera(Camera2D camera, SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* srcrect, SDL_Rect* dstrect)
{
    if (dstrect == NULL) return SDL_RenderCopy(renderer, texture, srcrect, dstrect);
    SDL_Rect moddstrect = { dstrect->x - camera.target.x - camera.offset.x, dstrect->y + camera.target.y + camera.offset.y, dstrect->w * camera.zoom.x, dstrect->h * camera.zoom.y };
    return SDL_RenderCopy(renderer, texture, srcrect, &moddstrect);
}
int SDL_RenderCopyEx_Camera(Camera2D camera, SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* srcrect, SDL_Rect* dstrect, double angle, const SDL_Point* center, SDL_RendererFlip flip)
{
    if (dstrect == NULL) return SDL_RenderCopyEx(renderer, texture, srcrect, dstrect, angle, center, flip);
    SDL_Rect moddstrect = { dstrect->x - camera.target.x - camera.offset.x, dstrect->y + camera.target.y + camera.offset.y, dstrect->w * camera.zoom.x, dstrect->h * camera.zoom.y };
    return SDL_RenderCopyEx(renderer, texture, srcrect, &moddstrect, angle, center, flip);
}
int SDL_RenderDrawLine_Camera(Camera2D camera, SDL_Renderer* renderer, int x1, int x2, int y1, int y2)
{
    return SDL_RenderDrawLine(renderer, x1 - camera.target.x - camera.offset.x, x1 - camera.target.x - camera.offset.x, y1 - camera.target.y - camera.offset.y, y2 - camera.target.y - camera.offset.y);
}
int SDL_RenderFillRect_Camera(Camera2D camera, SDL_Renderer* renderer, SDL_Rect* rect)
{
    SDL_Rect modrect = { rect->x - camera.target.x - camera.offset.x, rect->y + camera.target.y + camera.offset.y, rect->w * camera.zoom.x, rect->h * camera.zoom.y };
    return SDL_RenderFillRect(renderer, &modrect);
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
