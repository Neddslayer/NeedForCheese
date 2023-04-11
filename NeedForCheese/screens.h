#include "SDL.h"
#include <iostream>

#ifndef SCREENS_H
#define SCREENS_H

typedef enum GameScreen { UNKNOWN = -1, LOGO = 0, TITLE = 1, OPTIONS, LEVSEL, GAMEPLAY, ENDING};

extern GameScreen currentScreen;
extern SDL_Renderer* renderer;
extern SDL_Window* window;
extern SDL_Surface* screenSurface;
extern const Uint8* keyboard;
extern bool low_res;
extern bool MouseCollidingWithRect(SDL_Rect rect);
extern bool MouseCollidingWithRectF(SDL_FRect rect);
extern bool mouseClicked;
extern int mouseX, mouseY;

void InitLogoScreen(void);
void UpdateLogoScreen(void);
void DrawLogoScreen(void);
void UnloadLogoScreen(void);
int FinishLogoScreen(void);

void InitTitleScreen(void);
void UpdateTitleScreen(void);
void DrawTitleScreen(void);
void UnloadTitleScreen(void);
int FinishTitleScreen(void);

void InitOptionsScreen(void);
void UpdateOptionsScreen(void);
void DrawOptionsScreen(void);
void UnloadOptionsScreen(void);
int FinishOptionsScreen(void);

void InitLevelSelectScreen(void);
void UpdateLevelSelectScreen(void);
void DrawLevelSelectScreen(void);
void UnloadLevelSelectScreen(void);
int FinishLevelSelectScreen(void);

#endif // !SCREENS_H