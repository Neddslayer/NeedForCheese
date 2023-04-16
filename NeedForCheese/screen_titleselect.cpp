#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "screens.h"
#include "utilities.h"
#include <iostream>

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

// the positions of the button sprite
SDL_FRect playButton, equipButton, optionsButton, creditsButton;
// the actual hitbox of the buttons, essentially an approximation of the sprite to a rectangle
SDL_Rect playHitbox, equipHitbox, optionsHitbox, creditsHitbox;
bool playPressed, equipPressed, optionsPressed, creditsPressed;

Mix_Chunk* errorNotImplementedWAV;

bool transitioningToEquipScreen, transitioningScreen;

int uiOffsetX, increaseAmount;

void ResetButtonStates(void);

static int finishScreen;

void InitTitleSelectScreen(void)
{
	titleSelectBGSurfacea = IMG_Load("resources/img/title_select_bg.png");
	titleSelectButtonPlaySurface = IMG_Load("resources/img/button_title_play.png");
	titleSelectButtonEquipSurface = IMG_Load("resources/img/button_title_equip.png");
	titleSelectButtonOptionsSurface = IMG_Load("resources/img/button_title_options.png");
	titleSelectButtonCreditsSurface = IMG_Load("resources/img/button_title_credits.png");

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

	errorNotImplementedWAV = Mix_LoadWAV("resources/sfx/error.wav");
	playHitbox = { 0, 0, 0, 0 };
	equipHitbox = { 0, 0, 0, 0 };
	optionsHitbox = { 0, 0, 0, 0 };
	creditsHitbox = { 0, 0, 0, 0 };
}

void UpdateTitleSelectScreen(void)
{
	elapsed++;
	if (!mouseClicked)
	{
		ResetButtonStates();
	}
	if (ButtonClicked(playHitbox) && !playPressed)
	{
		playPressed = true;
		Mix_PlayChannel(-1, errorNotImplementedWAV, 0);
	}
	if (ButtonClicked(equipHitbox) && !equipPressed)
	{
		equipPressed = true;
		transitioningToEquipScreen = true;
		transitioningScreen = true;
		uiOffsetX = 0;
		increaseAmount = 100;

		playHitbox = { 0, 0, 0, 0 };
		equipHitbox = { 0, 0, 0, 0, };
		optionsHitbox = { 0, 0, 0, 0 };
		creditsHitbox = { 0, 0, 0, 0, };
	}
	if (!transitioningToEquipScreen)
	{
		if (elapsed > 40)
		{
			playHitbox = { 125, 50, 421, 200 };
		}
		if (elapsed > 55)
		{
			equipHitbox = { 755, 80, 421, 180 };
		}
		if (elapsed > 70)
		{
			optionsHitbox = { 140, 420, 410, 225 };
		}
		if (elapsed > 85)
		{ 
			creditsHitbox = { 750, 440, 420, 200 };
		}
	}
	else
	{
		uiOffsetX += increaseAmount;
		increaseAmount /= 1.05;
		if (uiOffsetX > 1000)
		{
			finishScreen = 2;
		}
	}
}

void DrawTitleSelectScreen(void)
{
	SDL_RenderCopy(renderer, titleSelectBGTexturea, NULL, NULL);
	
	playButton = { 125.0f + ((elapsed < 47) ? (rand() % 10 - 5) : 0) + ((MouseCollidingWithRect(playHitbox) && !transitioningScreen) ? (rand() % 2 - 1) : 0) - uiOffsetX, 10.0f + ((elapsed < 47) ? (rand() % 10 - 5) : 0) + ((MouseCollidingWithRect(playHitbox)) ? (rand() % 2 - 1) : 0), 421, 273 };

	if ((elapsed > 40) || transitioningToEquipScreen)
	{
		MouseCollidingWithRect(playHitbox) ? SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255) : SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderCopyF(renderer, titleSelectButtonPlayTexture, NULL, &playButton);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	}

	equipButton = { 750.0f + ((elapsed < 62) ? (rand() % 10 - 5) : 0) + ((MouseCollidingWithRect(equipHitbox) && !transitioningScreen) ? (rand() % 2 - 1) : 0) - uiOffsetX, 20.0f + ((elapsed < 62) ? (rand() % 10 - 5) : 0) + ((MouseCollidingWithRect(equipHitbox)) ? (rand() % 2 - 1) : 0), 427, 250 };

	if ((elapsed > 55) || transitioningToEquipScreen)
	{
		MouseCollidingWithRect(equipHitbox) ? SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255) : SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderCopyF(renderer, titleSelectButtonEquipTexture, NULL, &equipButton);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	}

	optionsButton = { 125.0f + ((elapsed < 77) ? (rand() % 10 - 5) : 0) + ((MouseCollidingWithRect(optionsHitbox) && !transitioningScreen) ? (rand() % 2 - 1) : 0) - uiOffsetX, 410.0f + ((elapsed < 77) ? (rand() % 10 - 5) : 0) + ((MouseCollidingWithRect(optionsHitbox)) ? (rand() % 2 - 1) : 0), 474, 276 };

	if ((elapsed > 70) || transitioningToEquipScreen)
	{
		MouseCollidingWithRect(optionsHitbox) ? SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255) : SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderCopyF(renderer, titleSelectButtonOptionsTexture, NULL, &optionsButton);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	}

	creditsButton = { 700.0f + ((elapsed < 92) ? (rand() % 10 - 5) : 0) + ((MouseCollidingWithRect(creditsHitbox) && !transitioningScreen) ? (rand() % 2 - 1) : 0) - uiOffsetX, 400.0f + ((elapsed < 92) ? (rand() % 10 - 5) : 0) + ((MouseCollidingWithRect(creditsHitbox)) ? (rand() % 2 - 1) : 0), 487, 283 };
	
	if ((elapsed > 85) || transitioningToEquipScreen)
	{
		MouseCollidingWithRect(creditsHitbox) ? SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255) : SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderCopyF(renderer, titleSelectButtonCreditsTexture, NULL, &creditsButton);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	}
}

void UnloadTitleSelectScreen(void)
{
	SDL_DestroyTexture(titleSelectBGTexturea);
	SDL_DestroyTexture(titleSelectButtonCreditsTexture);
	SDL_DestroyTexture(titleSelectButtonEquipTexture);
	SDL_DestroyTexture(titleSelectButtonOptionsTexture);
	SDL_DestroyTexture(titleSelectButtonPlayTexture);
	Mix_FreeChunk(errorNotImplementedWAV);
}

int FinishTitleSelectScreen(void)
{
	return finishScreen;
}

void ResetButtonStates(void)
{
	playPressed = false;
	equipPressed = false;
	optionsPressed = false;
	creditsPressed = false;
}