#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL_FontCache.h"

/*
* A list of all SDL rendering function used in the code, because it'll just be easier to document them here
* int SDL_RenderCopy(SDL_Renderer*, SDL_Texture*, SDL_Rect*, SDL_Rect*);
* int SDL_RenderCopyEx(SDL_Renderer*, SDL_Texture*, SDL_Rect*, SDL_Rect*, double, const SDL_Point*, SDL_RendererFlip);
* int SDL_RenderLine(SDL_Renderer*, int, int, int, int);
* int SDL_RenderFillRect(SDL_Renderer*, SDL_Rect*);
* 
*/

// Thanks to raylib for some of this

#ifndef SDL_CAMERA_H
struct Vector2
{
    float x;                // Vector x component
    float y;                // Vector y component

    Vector2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

    Vector2 operator+(const Vector2& other) const
    {
        return Vector2(x + other.x, y + other.y);
    }
};

struct Camera2D
{
    Vector2 offset;         // Camera offset (displacement from target)
    Vector2 target;         // Camera target (rotation and zoom origin)
    float rotation;         // Camera rotation in degrees
    float zoom;             // Camera zoom (scaling), should be 1.0f by default
    Camera2D(Vector2 offset = Vector2(), Vector2 target = Vector2(), float rotation = 0.0f, float zoom = 1.0f) : offset(offset), target(target), rotation(rotation), zoom(zoom) {};

};

void BeginMode2D(Camera2D camera);
void EndMode2D(void);

#endif // !SDL_CAMERA_H
