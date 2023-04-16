#include "SDL.h"
#include "SDL_image.h"
#include "screens.h"
#include "utilities.h"
#include <iostream>

SDL_Surface* titleEquipBGSurface = NULL;
SDL_Texture* titleEquipBGTexture = { 0 }; // for tghe love of god why can't i reuse variable names
SDL_Surface* titleEquipSectionSurface = NULL;
SDL_Texture* titleEquipSectionTexture = { 0 };
SDL_Surface* titleEquipSectionPreviewSurface = NULL;
SDL_Texture* titleEquipSectionPreviewTexture = { 0 };

static int finishScreen;

void InitEquipScreen(void)
{
	titleEquipBGSurface = IMG_Load("resources/img/title_select_bg.png");
	titleEquipSectionSurface = IMG_Load("resources/img/section_equip_costumeselect.png");
	titleEquipSectionPreviewSurface = IMG_Load("resources/img/section_equip_preview.png");

	titleEquipBGTexture = SDL_CreateTextureFromSurface(renderer, titleEquipBGSurface);
	titleEquipSectionTexture = SDL_CreateTextureFromSurface(renderer, titleEquipSectionSurface);
	titleEquipSectionPreviewTexture = SDL_CreateTextureFromSurface(renderer, titleEquipSectionPreviewSurface);

	SDL_FreeSurface(titleEquipBGSurface);
	SDL_FreeSurface(titleEquipSectionSurface);
	SDL_FreeSurface(titleEquipSectionPreviewSurface);
}
void UpdateEquipScreen(void)
{

}
void DrawEquipScreen(void)
{

}
void UnloadEquipScreen(void)
{
	SDL_DestroyTexture(titleEquipBGTexture);
	SDL_DestroyTexture(titleEquipSectionTexture);
	SDL_DestroyTexture(titleEquipSectionPreviewTexture);
}
int FinishEquipScreen(void)
{
	return finishScreen;
}