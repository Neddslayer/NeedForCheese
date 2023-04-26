#include "SDL2/SDL.h"
#include <iostream>
#include <string>
#include <vector>

#ifndef UTILITIES_H
#define UTILITIES_H

extern SDL_Renderer* renderer;
extern SDL_Window* window;
extern SDL_Surface* screenSurface;
extern const Uint8* keyboard;
extern bool low_res;
extern bool MouseCollidingWithRect(SDL_Rect rect);
extern bool MouseCollidingWithRectF(SDL_FRect rect);
extern bool ButtonClicked(SDL_Rect button);
extern bool ButtonClickedF(SDL_FRect button);
extern bool mouseClicked;
extern int mouseX, mouseY;
extern Uint32 frameTime;
extern Uint32 updateTime;
extern bool development_mode;
const int MET2PIX = 96;

std::vector<std::string> split_string(const std::string& str, const char* delim);

#endif // !UTILITIES_H