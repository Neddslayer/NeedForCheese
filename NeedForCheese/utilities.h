#ifndef UTILITIES_H
#define UTILITIES_H

#include "SDL2/SDL.h"
#include "world.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

extern int WIDTH = 1920; // The width of the window, in pixels.
extern int HEIGHT = 1080; // The height of the window, in pixels.
extern int SCALE = WIDTH / 1280;

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
const int MET2PIX = 96 * SCALE;
const float PIX2MET = (3.0f * SCALE) / MET2PIX;

vector<string> split_string(const string& str, const char* delim);

#endif // !UTILITIES_H