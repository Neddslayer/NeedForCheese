#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL_FontCache.h"

/*
* A list of all SDL rendering function used in the code, because it'll just be easier to document them here
* SDL_RenderCopy(SDL_Renderer*, SDL_Texture*, SDL_Rect*, SDL_Rect*);
* SDL_RenderCopyEx(SDL_Renderer*, SDL_Texture*, SDL_Rect*, SDL_Rect*, double, const SDL_Point*, SDL_RendererFlip);
* SDL_RenderLine(SDL_Renderer*, int, int, int, int);
* SDL_RenderFillRect(SDL_Renderer*, SDL_Rect*);
* 
*/

#ifndef CAMERA_H
typedef struct Vector2
{
    float x;                // Vector x component
    float y;                // Vector y component
} Vector2;

typedef struct Camera2D
{
    Vector2 offset;         // Camera offset (displacement from target)
    Vector2 target;         // Camera target (rotation and zoom origin)
    float rotation;         // Camera rotation in degrees
    float zoom;             // Camera zoom (scaling), should be 1.0f by default
} Camera2D;

#endif // !CAMERA_H
