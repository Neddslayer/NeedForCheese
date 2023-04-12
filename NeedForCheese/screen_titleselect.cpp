#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "screens.h"

SDL_Surface* titleSelectBGSurface = NULL;
SDL_Texture* titleSelectBGTexture = { 0 };
SDL_Surface* titleSelectButtonPlaySurface = NULL;
SDL_Texture* titleSelectButtonPlayTexture = { 0 };
SDL_Surface* titleSelectButtonEquipSurface = NULL;
SDL_Texture* titleSelectButtonEquipTexture = { 0 };
SDL_Surface* titleSelectButtonOptionsSurface = NULL;
SDL_Texture* titleSelectButtonOptionsTexture = { 0 };
SDL_Surface* titleSelectButtonCreditsSurface = NULL;
SDL_Texture* titleSelectButtonCreditsTexture = { 0 };

int elapsed;

bool playPressable, equipPressable, optionsPressable, creditsPressable;

// the positions of the button sprite
SDL_Rect playButton, equipButton, optionsButton, creditsButton;
// the actual hitbox of the buttons, essentially an approximation of the sprite to a rectangle
SDL_Rect playHitbox, equipHitbox, optionsHitbox, creditsHitbox;
bool playPressed, equipPressed, optionsPressed, creditsPressed;

Mix_Chunk* errorNotImplementedWAV;

void InitTitleSelectScreen(void)
{
	titleSelectBGSurface = IMG_Load("resources/title_select_bg.png");
	titleSelectButtonPlaySurface = IMG_Load("resources/button_title_play.png");
	titleSelectButtonEquipSurface = IMG_Load("resources/button_title_equip.png");
	titleSelectButtonOptionsSurface = IMG_Load("resources/button_title_options.png");
	titleSelectButtonCreditsSurface = IMG_Load("resources/button_title_credits.png");
	titleSelectBGTexture = SDL_CreateTextureFromSurface(renderer, titleSelectBGSurface);
	titleSelectButtonPlayTexture = SDL_CreateTextureFromSurface(renderer, titleSelectButtonPlaySurface);
	titleSelectButtonEquipTexture = SDL_CreateTextureFromSurface(renderer, titleSelectButtonEquipSurface);
	titleSelectButtonOptionsTexture = SDL_CreateTextureFromSurface(renderer, titleSelectButtonOptionsSurface);
	titleSelectButtonCreditsTexture = SDL_CreateTextureFromSurface(renderer, titleSelectButtonCreditsSurface);
	SDL_FreeSurface(titleSelectBGSurface);
	SDL_FreeSurface(titleSelectButtonPlaySurface);
	SDL_FreeSurface(titleSelectButtonEquipSurface);
	SDL_FreeSurface(titleSelectButtonOptionsSurface);
	SDL_FreeSurface(titleSelectButtonCreditsSurface);

	errorNotImplementedWAV = Mix_LoadWAV("resources/error.wav");
	playHitbox = { 0, 0, 0, 0 };
	equipHitbox = { 0, 0, 0, 0 };
	optionsHitbox = { 0, 0, 0, 0 };
	creditsHitbox = { 0, 0, 0, 0 };
}
void UpdateTitleSelectScreen(void)
{

}
void DrawTitleSelectScreen(void)
{

}
void UnloadTitleSelectScreen(void)
{

}
int FinishTitleSelectScreen(void)
{

}