#include "SDL2/SDL.h"

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

std::vector<std::string> split_string(const std::string& str, const char* delim)
{
    std::vector<std::string> tokens;
    std::string token;
    size_t start = 0, end = 0;
    while ((end = str.find(delim, start)) != std::string::npos)
    {
        token = str.substr(start, end - start);
        start = end + 1;
        tokens.push_back(token);
    }
    tokens.push_back(str.substr(start));
    return tokens;
}

#endif // !UTILITIES_H