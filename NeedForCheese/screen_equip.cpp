#include "SDL.h"
#include "SDL_image.h"
#include "screens.h"
#include "utilities.h"
#include <iostream>

SDL_Surface* titleEquipBGSurface = NULL;
SDL_Texture* titleEquipBGTexture = { 0 }; // for tghe love of god why can't i reuse variable names
SDL_Surface* titleEquipSectionSurface = NULL;
SDL_Texture* titleEquipSectionTexture = { 0 };

void InitEquipScreen(void)
{
	titleEquipBGSurface = IMG_Load("resources/title_select_bg.png");
	titleEquipSectionSurface = IMG_Load("resources/title_equip_section.png");

	titleEquipBGTexture = SDL_CreateTextureFromSurface(renderer, titleEquipBGSurface);
	titleEquipSectionTexture = SDL_CreateTextureFromSurface(renderer, titleEquipSectionSurface);


	SDL_FreeSurface(titleEquipBGSurface);
	SDL_FreeSurface(titleEquipSectionSurface);
}
void UpdateEquipScreen(void)
{

}
void DrawEquipScreen(void)
{

}
void UnloadEquipScreen(void)
{

}
int FinishEquipScreen(void)
{
	return 0;
}