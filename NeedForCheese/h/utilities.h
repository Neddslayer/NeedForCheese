#ifndef UTILITIES_H
#define UTILITIES_H

#include "SDL2/SDL.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

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

extern int WIDTH; // The width of the window, in pixels.
extern int HEIGHT; // The height of the window, in pixels.
extern float SCALE;
extern int MET2PIX;
extern float PIX2MET;
extern int SCALED_WIDTH; // The width of the window, in meters.
extern int SCALED_HEIGHT; // The height of the window, in meters.

vector<string> split_string(const string& str, const char* delim);

#endif // !UTILITIES_H