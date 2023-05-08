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

#ifndef SDL_CAMERA_H
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

int SDLC_RenderCopy(Camera2D camera, SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* srcrect, SDL_Rect* dstrect);
int SDLC_RenderCopyF(Camera2D camera, SDL_Renderer* renderer, SDL_Texture* texture, SDL_FRect* srcrect, SDL_FRect* dstrect);
int SDLC_RenderCopyEx(Camera2D camera, SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* srcrect, SDL_Rect* dstrect, double angle, const SDL_Point* center, SDL_RendererFlip flip);
int SDLC_RenderCopyExF(Camera2D camera, SDL_Renderer* renderer, SDL_Texture* texture, SDL_FRect* srcrect, SDL_FRect* dstrect, double angle, const SDL_Point* center, SDL_RendererFlip flip);
int SDLC_RenderLine(Camera2D camera, SDL_Renderer* renderer, int x1, int y1, int x2, int y2);
int SDLC_RenderFillRect(Camera2D camera, SDL_Renderer* renderer, SDL_Rect* rect);

#endif // !SDL_CAMERA_H
