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
#define SDL_CAMERA_H
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "../util/SDL_FontCache.h"

struct Vector2f
{
    float x;                // Vector x component
    float y;                // Vector y component

    Vector2f(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

    Vector2f operator+(const Vector2f& other) const;
    Vector2f operator-(const Vector2f& other) const;
    Vector2f& operator+=(const Vector2f& other);
    Vector2f& operator-=(const Vector2f& other);
};
struct Vector2d
{
    double x;                // Vector x component
    double y;                // Vector y component

    Vector2d(double x = 0.0f, double y = 0.0f) : x(x), y(y) {}

    Vector2d operator+(const Vector2d& other) const;
    Vector2d operator-(const Vector2d& other) const;
    Vector2d& operator+=(const Vector2d& other);
    Vector2d& operator-=(const Vector2d& other);
};

struct Camera2D
{
    Vector2d offset;            // Camera offset (displacement from target)
    Vector2d target;            // Camera target (rotation and zoom origin)
    float rotation;             // Camera rotation in degrees.
    Vector2d zoom;              // Camera zoom (scaling), should be 1.0f by default.
    SDL_FRect bounds;           // Camera bounds.
    Camera2D(Vector2d offset = Vector2d(), Vector2d target = Vector2d(), float rotation = 0.0f, Vector2d zoom = Vector2d(1.0f,1.0f), SDL_FRect bounds = SDL_FRect()) : offset(offset), target(target), rotation(rotation), zoom(zoom), bounds(bounds) {};

};

int SDL_RenderCopy_Camera(Camera2D camera, SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* srcrect, SDL_Rect* dstrect);
int SDL_RenderCopyEx_Camera(Camera2D camera, SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* srcrect, SDL_Rect* dstrect, double angle, const SDL_Point* center, SDL_RendererFlip flip);
int SDL_RenderDrawLine_Camera(Camera2D camera, SDL_Renderer* renderer, int x1, int x2, int y1, int y2);
int SDL_RenderFillRect_Camera(Camera2D camera, SDL_Renderer* renderer, SDL_Rect* rect);
void Camera_UpdatePositionFromBounds(Camera2D camera);

#endif // !SDL_CAMERA_H
