/*
* A list of all SDL rendering function used in the code, because it'll just be easier to document them here
* int SDL_RenderCopy(SDL_Renderer*, SDL_Texture*, SDL_Rect*, SDL_Rect*);
* int SDL_RenderCopyEx(SDL_Renderer*, SDL_Texture*, SDL_Rect*, SDL_Rect*, double, const SDL_Point*, SDL_RendererFlip);
* int SDL_RenderLine(SDL_Renderer*, int, int, int, int);
* int SDL_RenderFillRect(SDL_Renderer*, SDL_Rect*);
* 
*/

/*
* A camera system for using with SDL.
* I would have used SDL_SetRenderViewpoint, but it was just pure hell, so I'm doing this approach.
* raylib's code helped this, check them out.
*/

#ifndef SDL_CAMERA_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL_FontCache.h"

struct Vector2
{
    float x;                // Vector x component
    float y;                // Vector y component

    Vector2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

    Vector2 operator+(const Vector2& other) const;
    Vector2 operator-(const Vector2& other) const;
    Vector2& operator+=(const Vector2& other);
    Vector2& operator-=(const Vector2& other);
};

struct Camera2D
{
    Vector2 offset;         // Camera offset (displacement from target)
    Vector2 target;         // Camera target (rotation and zoom origin)
    float rotation;         // Camera rotation in degrees
    Vector2 zoom;             // Camera zoom (scaling), should be 1.0f by default
    Camera2D(Vector2 offset = Vector2(), Vector2 target = Vector2(), float rotation = 0.0f, Vector2 zoom = Vector2(1.0f,1.0f)) : offset(offset), target(target), rotation(rotation), zoom(zoom) {};

};

void BeginMode2D(Camera2D camera);
void EndMode2D(void);

int SDL_RenderCopy_Camera(Camera2D camera, SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* srcrect, SDL_Rect* dstrect);
int SDL_RenderCopyEx_Camera(Camera2D camera, SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* srcrect, SDL_Rect* dstrect, double angle, const SDL_Point* center, SDL_RendererFlip flip);
int SDL_RenderLine_Camera(Camera2D camera, SDL_Renderer* renderer, int x1, int x2, int y1, int y2);
int SDL_RenderFillRect_Camera(Camera2D camera, SDL_Renderer* renderer, SDL_Rect* rect);

#endif // !SDL_CAMERA_H
