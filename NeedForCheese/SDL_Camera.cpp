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
    // Save the current rendering target and viewport
    old_target = SDL_GetRenderTarget(renderer);
    SDL_RenderGetViewport(renderer, &old_viewport);

    // Set up the camera transformation matrix
    SDL_Rect viewport = { (int)camera.target.x + (int)camera.offset.x, (int)camera.target.y + (int)camera.offset.y, WIDTH, HEIGHT };
    SDL_RenderSetViewport(renderer, &viewport);
    SDL_RenderSetScale(renderer, camera.zoom, camera.zoom);
}
void EndMode2D()
{
    // Restore the old rendering target and viewport
    SDL_SetRenderTarget(renderer, old_target);
    SDL_RenderSetViewport(renderer, &old_viewport);

    // Reset the rendering transformations
    SDL_RenderSetScale(renderer, 1.0f, 1.0f);
}
