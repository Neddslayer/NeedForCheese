#include "SDL.h"
#include "SDL_image.h"
#include "SDL_FontCache.h"
#include "SDL_mixer.h"
#include "screens.h"
#include <iostream>
#include <string.h>
#include <string>

using namespace std;

int framesCounter = 0;
int finishScreen = 0;

SDL_Surface* titleBGSurface = NULL;
SDL_Texture* titleBGTexture = { 0 };
SDL_Surface* titleGuySurface = NULL;
SDL_Texture* titleGuyTexture = { 0 };
SDL_Surface* titleTextSurface = NULL;
SDL_Texture* titleTextTexture = { 0 };

SDL_Point titleTextStart = { 600, 800 };
SDL_Point titleTextEnd = { 25, 10 };
SDL_Point titleTextExitStart = { 25, 10 };
SDL_Point titleTextExitEnd = { -500, 20 };
double startAngle = -380;
double endAngle = 0.0;
double exitAngle = 16.5;

SDL_Point titleGuyStart = { 600, 800 };
SDL_Point titleGuyEnd = { 600, 375 };
double startAngle2 = -80;
double endAngle2 = 0.0;

// Define duration of animation, in seconds
double duration = 150.0;
double duration2 = 100.0;
// Perform animation
double elapsed = 0.0;

bool incrementing = true, pulsing, secondPulse, renderText, subtractTime, secondScreen, finishedTransition;

double x, y, angle, overlayAlpha = -5;
double t2;
double x2, y2, angle2;
double t;

FC_Font* startFont;

bool transitioningToEquipScreen;

void InitTitleScreen(void)
{
	overlayAlpha = -5;
	titleBGSurface = IMG_Load("resources/title_bg.png");
	titleGuySurface = IMG_Load("resources/title_theman.png");
	titleTextSurface = IMG_Load("resources/title_text.png");
	titleBGTexture = SDL_CreateTextureFromSurface(renderer, titleBGSurface);
	titleGuyTexture = SDL_CreateTextureFromSurface(renderer, titleGuySurface);
	titleTextTexture = SDL_CreateTextureFromSurface(renderer, titleTextSurface);
	SDL_FreeSurface(titleBGSurface);
	SDL_FreeSurface(titleGuySurface);
	SDL_FreeSurface(titleTextSurface);
	
	startFont = FC_CreateFont();
	FC_LoadFont(startFont, renderer, "resources/RobotoSlab.ttf", 60, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);
	
}

void UpdateTitleScreen(void)
{
	if (!subtractTime) elapsed += 2;
	else if (!finishedTransition) elapsed -= 2;
	else elapsed++;
	if (!finishedTransition)
	{
		t = elapsed / duration;
		t = fmax(0.0, fmin(1.0, t)); // Cap the time so the animation doesn't start looping
		if (t > 0.95 && !pulsing && overlayAlpha == -5)
		{
			overlayAlpha = 255;
			pulsing = true; // this makes the overlay alpha subtract by two (or three if it's subtracting the animation)
			renderText = true; // Make the "press any button to start" text visible
		}
		x = !subtractTime ? (titleTextStart.x + (titleTextEnd.x - titleTextStart.x) * t * (2 - t)) : ((titleTextExitEnd.x + (titleTextExitStart.x - titleTextExitEnd.x) * t * (2 - t)));
		y = !subtractTime ? (titleTextStart.y + (titleTextEnd.y - titleTextStart.y) * t * (2 - t)) : ((titleTextExitEnd.y + (titleTextExitStart.y - titleTextExitEnd.y) * t * (2 - t)));
		angle = !subtractTime ? (startAngle + (endAngle - startAngle) * t * (2 - t)) : (exitAngle + (endAngle - exitAngle) * t * (2 - t));
		x2 = titleGuyStart.x + (titleGuyEnd.x - titleGuyStart.x) * t * (2 - t);
		y2 = titleGuyStart.y + (titleGuyEnd.y - titleGuyStart.y) * t * (2 - t);
		angle2 = startAngle2 + (endAngle2 - startAngle2) * t * (2 - t);
	}
	if (pulsing)
	{
		overlayAlpha -= subtractTime ? 3 : 2;
	}
	if (overlayAlpha <= 0 && pulsing)
	{
		overlayAlpha = -6;
		pulsing = false;
	}
	if (!subtractTime && t >= 1)
	{
		for (int scancode = 0; scancode < SDL_NUM_SCANCODES; ++scancode)
		{
			if (keyboard[scancode])
			{
				// Handle the pressed key
				subtractTime = true; // make the animation subtract
				secondScreen = true; // make the second screen render
				renderText = false; // remove the text
				overlayAlpha = 255; // make the screen white
				elapsed = 50;
				duration = 50;
				finishScreen = 1;
			}
		}
	}
	
}

void DrawTitleScreen(void)
{
	if (!secondScreen)
	{
		SDL_RenderCopy(renderer, titleBGTexture, NULL, NULL);
		SDL_FRect rect = { (float)x, (float)y, 493, 306.5 };
		SDL_FRect rect2 = { (float)x2, (float)y2, 508, 348 };
		SDL_RenderCopyExF(renderer, titleTextTexture, NULL, &rect, angle, NULL, SDL_FLIP_NONE);
		SDL_RenderCopyExF(renderer, titleGuyTexture, NULL, &rect2, angle2, NULL, SDL_FLIP_NONE);
		if (renderText) FC_Draw(startFont, renderer, 175, 600, "PRESS ANY BUTTON TO BEGIN");
	}
	else
	{
		SDL_RenderCopy(renderer, titleSelectBGTexture, NULL, NULL);
		// Only render the characters while they're on screen
		if (t >= 0.01 && !transitioningToEquipScreen)
		{
			SDL_FRect rect = { (float)x, (float)y, 493, 306.5 };
			SDL_RenderCopyExF(renderer, titleTextTexture, NULL, &rect, angle, NULL, SDL_FLIP_NONE);
			SDL_FRect rect2 = { (float)x2, (float)y2, 508, 348 };
			SDL_RenderCopyExF(renderer, titleGuyTexture, NULL, &rect2, angle2, NULL, SDL_FLIP_NONE);
		}
		else
		{
			if (!finishedTransition)
				elapsed = 0;
			finishedTransition = true;
			finishScreen = 1;
		}
		playButton = { 125.0f + ((elapsed < 47) ? (rand() % 10 - 5) : 0) + ((playPressable && MouseCollidingWithRectF(playHitbox)) ? (rand() % 2 - 1) : 0), 10.0f + ((elapsed < 47) ? (rand() % 10 - 5) : 0) + ((playPressable && MouseCollidingWithRectF(playHitbox)) ? (rand() % 2 - 1) : 0), 421, 273 };
		if ((elapsed > 40 && finishedTransition) || transitioningToEquipScreen)
		{
			playPressable = true;
			SDL_RenderCopyF(renderer, titleSelectButtonPlayTexture, NULL, &playButton);
			playHitbox = { 125, 50, 421, 200 };
		}
		equipButton = { 750.0f + ((elapsed < 62) ? (rand() % 10 - 5) : 0) + ((equipPressable && MouseCollidingWithRectF(equipHitbox)) ? (rand() % 2 - 1) : 0), 20.0f + ((elapsed < 62) ? (rand() % 10 - 5) : 0) + ((equipPressable && MouseCollidingWithRectF(equipHitbox)) ? (rand() % 2 - 1) : 0), 427, 250};
		if ((elapsed > 55 && finishedTransition) || transitioningToEquipScreen)
		{
			equipPressable = true;
			SDL_RenderCopyF(renderer, titleSelectButtonEquipTexture, NULL, &equipButton);
			equipHitbox = { 755, 80, 421, 180 };
		}
		optionsButton = { 125.0f + ((elapsed < 77) ? (rand() % 10 - 5) : 0) + ((optionsPressable && MouseCollidingWithRectF(optionsHitbox)) ? (rand() % 2 - 1) : 0), 410.0f + ((elapsed < 77) ? (rand() % 10 - 5) : 0) + ((optionsPressable && MouseCollidingWithRectF(optionsHitbox)) ? (rand() % 2 - 1) : 0), 474, 276 };
		if ((elapsed > 70 && finishedTransition) || transitioningToEquipScreen)
		{
			optionsPressable = true;
			SDL_RenderCopyF(renderer, titleSelectButtonOptionsTexture, NULL, &optionsButton);
			optionsHitbox = { 140, 420, 410, 225 };
		}
		creditsButton = { 700.0f + ((elapsed < 92) ? (rand() % 10 - 5) : 0) + ((creditsPressable && MouseCollidingWithRectF(creditsHitbox)) ? (rand() % 2 - 1) : 0), 400.0f + ((elapsed < 92) ? (rand() % 10 - 5) : 0) + ((creditsPressable && MouseCollidingWithRectF(creditsHitbox)) ? (rand() % 2 - 1) : 0), 487, 283 };
		if ((elapsed > 85 && finishedTransition) || transitioningToEquipScreen)
		{
			creditsPressable = true;
			SDL_RenderCopyF(renderer, titleSelectButtonCreditsTexture, NULL, &creditsButton);
			creditsHitbox = { 750, 440, 420, 200 };
		}
	}
	
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, pulsing ? overlayAlpha : 0);
	SDL_RenderFillRect(renderer, NULL);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

void UnloadTitleScreen(void)
{
	SDL_DestroyTexture(titleBGTexture);
	SDL_DestroyTexture(titleGuyTexture);
	SDL_DestroyTexture(titleTextTexture);
	FC_FreeFont(startFont);
}

int FinishTitleScreen(void)
{
	return finishScreen;
}