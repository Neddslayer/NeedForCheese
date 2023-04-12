#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "screens.h"

SDL_Surface* titleSelectBGSurfacea = NULL;
SDL_Texture* titleSelectBGTexturea = { 0 };
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
SDL_FRect playButton, equipButton, optionsButton, creditsButton;
// the actual hitbox of the buttons, essentially an approximation of the sprite to a rectangle
SDL_Rect playHitbox, equipHitbox, optionsHitbox, creditsHitbox;
bool playPressed, equipPressed, optionsPressed, creditsPressed;

Mix_Chunk* errorNotImplementedWAV;

bool transitioningToEquipScreen;

void InitTitleSelectScreen(void)
{
	titleSelectBGSurfacea = IMG_Load("resources/title_select_bg.png");
	titleSelectButtonPlaySurface = IMG_Load("resources/button_title_play.png");
	titleSelectButtonEquipSurface = IMG_Load("resources/button_title_equip.png");
	titleSelectButtonOptionsSurface = IMG_Load("resources/button_title_options.png");
	titleSelectButtonCreditsSurface = IMG_Load("resources/button_title_credits.png");
	titleSelectBGTexturea = SDL_CreateTextureFromSurface(renderer, titleSelectBGSurfacea);
	titleSelectButtonPlayTexture = SDL_CreateTextureFromSurface(renderer, titleSelectButtonPlaySurface);
	titleSelectButtonEquipTexture = SDL_CreateTextureFromSurface(renderer, titleSelectButtonEquipSurface);
	titleSelectButtonOptionsTexture = SDL_CreateTextureFromSurface(renderer, titleSelectButtonOptionsSurface);
	titleSelectButtonCreditsTexture = SDL_CreateTextureFromSurface(renderer, titleSelectButtonCreditsSurface);
	SDL_FreeSurface(titleSelectBGSurfacea);
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
	elapsed++;
}
void DrawTitleSelectScreen(void)
{
	SDL_RenderCopy(renderer, titleSelectBGTexturea, NULL, NULL);
	playButton = { 125.0f + ((elapsed < 47) ? (rand() % 10 - 5) : 0) + ((playPressable && MouseCollidingWithRect(playHitbox)) ? (rand() % 2 - 1) : 0), 10.0f + ((elapsed < 47) ? (rand() % 10 - 5) : 0) + ((playPressable && MouseCollidingWithRect(playHitbox)) ? (rand() % 2 - 1) : 0), 421, 273 };
	if ((elapsed > 40) || transitioningToEquipScreen)
	{
		playPressable = true;
		SDL_RenderCopyF(renderer, titleSelectButtonPlayTexture, NULL, &playButton);
		playHitbox = { 125, 50, 421, 200 };
	}
	equipButton = { 750.0f + ((elapsed < 62) ? (rand() % 10 - 5) : 0) + ((equipPressable && MouseCollidingWithRect(equipHitbox)) ? (rand() % 2 - 1) : 0), 20.0f + ((elapsed < 62) ? (rand() % 10 - 5) : 0) + ((equipPressable && MouseCollidingWithRect(equipHitbox)) ? (rand() % 2 - 1) : 0), 427, 250 };
	if ((elapsed > 55) || transitioningToEquipScreen)
	{
		equipPressable = true;
		SDL_RenderCopyF(renderer, titleSelectButtonEquipTexture, NULL, &equipButton);
		equipHitbox = { 755, 80, 421, 180 };
	}
	optionsButton = { 125.0f + ((elapsed < 77) ? (rand() % 10 - 5) : 0) + ((optionsPressable && MouseCollidingWithRect(optionsHitbox)) ? (rand() % 2 - 1) : 0), 410.0f + ((elapsed < 77) ? (rand() % 10 - 5) : 0) + ((optionsPressable && MouseCollidingWithRect(optionsHitbox)) ? (rand() % 2 - 1) : 0), 474, 276 };
	if ((elapsed > 70) || transitioningToEquipScreen)
	{
		optionsPressable = true;
		SDL_RenderCopyF(renderer, titleSelectButtonOptionsTexture, NULL, &optionsButton);
		optionsHitbox = { 140, 420, 410, 225 };
	}
	creditsButton = { 700.0f + ((elapsed < 92) ? (rand() % 10 - 5) : 0) + ((creditsPressable && MouseCollidingWithRect(creditsHitbox)) ? (rand() % 2 - 1) : 0), 400.0f + ((elapsed < 92) ? (rand() % 10 - 5) : 0) + ((creditsPressable && MouseCollidingWithRect(creditsHitbox)) ? (rand() % 2 - 1) : 0), 487, 283 };
	if ((elapsed > 85) || transitioningToEquipScreen)
	{
		creditsPressable = true;
		SDL_RenderCopyF(renderer, titleSelectButtonCreditsTexture, NULL, &creditsButton);
		creditsHitbox = { 750, 440, 420, 200 };
	}
}
void UnloadTitleSelectScreen(void)
{
	SDL_DestroyTexture(titleSelectBGTexturea);
	SDL_DestroyTexture(titleSelectButtonCreditsTexture);
	SDL_DestroyTexture(titleSelectButtonEquipTexture);
	SDL_DestroyTexture(titleSelectButtonOptionsTexture);
	SDL_DestroyTexture(titleSelectButtonPlayTexture);
}
int FinishTitleSelectScreen(void)
{
	return 0;
}