#include "SDL_Camera.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL_FontCache.h"

int SDLC_RenderCopy(Camera2D camera, SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* srcrect, SDL_Rect* dstrect)
{
	if (dstrect == NULL) return SDL_RenderCopy(renderer, texture, srcrect, dstrect);
	SDL_Rect srcrect_copy = { srcrect->x, srcrect->y, srcrect->w, srcrect->h };
	SDL_Rect dstrect_copy = { dstrect->x, dstrect->y, dstrect->w, dstrect->h };
	dstrect_copy.x += camera.target.x;
	dstrect_copy.y += camera.target.y;
}