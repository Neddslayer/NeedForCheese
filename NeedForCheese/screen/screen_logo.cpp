#include "SDL2/SDL.h"
#include "screens.h"
#include "../h/utilities.h"
#include "SDL2/SDL_image.h"
#include "../util/SDL_FontCache.h"
#include "SDL2/SDL_mixer.h"
#include <chrono>
#include <iostream>

static int framesCounter = 0;
static int finishScreen = 0;

static float logoPositionX = 0;
static float logoPositionY = 0;

float logoScale = 0;

float alpha = 1.0f;

SDL_Surface* imageSurface = NULL;
SDL_Texture* logoTex = { 0 };

Mix_Chunk* textSound;

FC_Font* font;

SDL_FRect gdSprite;

bool didthe, beeped;
int didtheAmount;

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::duration<float> fsec;

auto startTime = Clock::now();
auto endTime = Clock::now();

void InitLogoScreen(void)
{
    finishScreen = 0;
    framesCounter = 0;

    logoPositionX = WIDTH / 2.0 - (128 * logoScale);
    logoPositionY = HEIGHT / 2.0 - (128 * logoScale);

    alpha = 1.0f;
    imageSurface = IMG_Load("resources/img/logo.png");
    logoTex = SDL_CreateTextureFromSurface(renderer, imageSurface);
    textSound = Mix_LoadWAV("resources/sfx/text.wav");
    SDL_FreeSurface(imageSurface);
    font = FC_CreateFont();
    FC_LoadFont(font, renderer, "resources/font/RobotoSlab.ttf", 50.0f * SCALE, FC_MakeColor(0, 0, 0, 255), TTF_STYLE_NORMAL);
    startTime = Clock::now();
}

void UpdateLogoScreen(void)
{
    if (didthe)
    {
        framesCounter++;
        logoScale = SDL_clamp(framesCounter / 10.0f, 0.0f, 2.0f);
        logoPositionX = WIDTH / 2.0 - (128 * logoScale) / 2;
        logoPositionY = HEIGHT / 2.0 - (128 * logoScale) / 2;
        gdSprite = { logoPositionX, logoPositionY, 128 * logoScale, 128 * logoScale };
        if (framesCounter > 4000)
        {
            cout << "Time took " << startTime.time_since_epoch().count() << " for 4000" << endl;
            alpha -= 0.05f;
            if (alpha <= 0.0f)
            {
                alpha = 0.0f;
                finishScreen = 1;
            }
        }
    }
    else
    {
        didtheAmount++;
        if (didtheAmount > 100)
        {
            didthe = true;
        }

    }
}

void DrawLogoScreen(void)
{
    if (didthe)
    {
        SDL_RenderCopyF(renderer, logoTex, NULL, &gdSprite);

        const char* logoText = "";
        switch (framesCounter)
        {
        case 70:
            logoText = "";break;
        case 71:
            logoText = "M";break;
        case 72:
            logoText = "Ma";break;
        case 73:
            logoText = "Mad";break;
        case 74:
            logoText = "Made";break;
        case 75:
            logoText = "Made b";break;
        case 76:
            logoText = "Made by";break;
        case 77:
            logoText = "Made by N";break;
        case 78:
            logoText = "Made by Ne";break;
        case 79:
            logoText = "Made by Ned";break;
        case 80:
            logoText = "Made by Nedd";break;
        case 81:
            logoText = "Made by NeddL";break;
        case 82:
            logoText = "Made by NeddLa";break;
        case 83:
            logoText = "Made by NeddLab";break;
        case 84:
            logoText = "Made by NeddLabs";break;
        default:
            logoText = "";break;
        }
        if (framesCounter < 84)
        {
            if (framesCounter >= 70 && framesCounter % 3 == 0) Mix_PlayChannel(-1, textSound, 0);
            FC_Draw(font, renderer, logoPositionX - FC_GetWidth(font, logoText) / (2 * SCALE), logoPositionY + 256, logoText);
        }
        else
        {
            FC_Draw(font, renderer, logoPositionX - FC_GetWidth(font, "Made by NeddLabs") / (2 * SCALE), logoPositionY + 256, "Made by NeddLabs");
        }
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        FC_DrawColor(font, renderer, WIDTH - FC_GetWidth(font, "LOADING...") - 50 * SCALE, HEIGHT - FC_GetHeight(font, "LOADING...") - 5 * SCALE, FC_MakeColor(255, 255, 255, 255), "LOADING...");
    }
}

void UnloadLogoScreen(void)
{
    SDL_DestroyTexture(logoTex);
    Mix_FreeChunk(textSound);
    FC_FreeFont(font);
}

int FinishLogoScreen(void)
{
    return finishScreen;
}