#include "SDL.h"
#include "screens.h"
#include "box2d/box2d.h"
#include "SDL_image.h"
#include "SDL_FontCache.h"
#include "SDL_mixer.h"

static int framesCounter = 0;
static int finishScreen = 0;

static float logoPositionX = 0;
static float logoPositionY = 0;

float logoScale = 0;

float alpha = 1.0f;

SDL_Surface* imageSurface = NULL;
SDL_Texture* logoTex = { 0 };

Mix_Chunk* logoBeep;
Mix_Chunk* textSound;

FC_Font* font;

SDL_FRect gdSprite;

bool didthe, beeped;
int didtheAmount;

void InitLogoScreen(void)
{
    finishScreen = 0;
    framesCounter = 0;

    logoPositionX = 640 - (128 * logoScale);
    logoPositionY = 360 - (128 * logoScale);

    alpha = 1.0f;
    imageSurface = IMG_Load("resources/logo.png");
    logoTex = SDL_CreateTextureFromSurface(renderer, imageSurface);
    logoBeep = Mix_LoadWAV("resources/logo.wav");
    textSound = Mix_LoadWAV("resources/text.wav");
    SDL_FreeSurface(imageSurface);
    font = FC_CreateFont();
    FC_LoadFont(font, renderer, "resources/RobotoSlab.ttf", 50, FC_MakeColor(0, 0, 0, 255), TTF_STYLE_NORMAL);
}

double clamp(double d, double min, double max)
{
    const double t = d < min ? min : d;
    return t > max ? max : t;
}

void UpdateLogoScreen(void)
{
    if (didthe)
    {
        framesCounter++;
        logoScale = clamp(framesCounter / 10.0f, 0.0f, 2.0f);
        logoPositionX = 640 - (128 * logoScale) / 2;
        logoPositionY = 360 - (128 * logoScale) / 2;
        gdSprite = { logoPositionX, logoPositionY, 128 * logoScale, 128 * logoScale };
        if (!beeped)
        {
            Mix_PlayChannel(-1, logoBeep, 0);
            beeped = true;
        }
        if (framesCounter > 100)
        {
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
        if (didtheAmount > 200)
        {
            didthe = true;
        }

    }
}

void DrawLogoScreen(void)
{
    if (didthe)
    {
        //SDL_RenderSetScale(renderer, logoScale, logoScale);
        SDL_RenderCopyF(renderer, logoTex, NULL, &gdSprite);
        //SDL_RenderSetScale(renderer, 1, 1);
        //DrawTextureEx(logoTex, { logoPositionX, logoPositionY }, 0, logoScale, Fade(WHITE, alpha));
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
            FC_Draw(font, renderer, logoPositionX - 100, logoPositionY + 256, logoText);//(logoText, logoPositionX - 112, logoPositionY + 256, 50, Fade(BLACK, alpha));
            std::cout << FC_GetWidth(font, logoText) << std::endl;
        }
        else
        {
            FC_Draw(font, renderer, logoPositionX - 100, logoPositionY + 256, "Made by NeddLabs");//DrawText("Made by NeddLabs", logoPositionX - 112, logoPositionY + 256, 50, Fade(BLACK, alpha));
        }
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        FC_Draw(font, renderer, 950, 600, "LOADING...");
        FC_DrawColor(font, renderer, 950, 600, FC_MakeColor(255, 255, 255, 255), "LOADING...");
    }
}

void UnloadLogoScreen(void)
{
    SDL_DestroyTexture(logoTex);
    Mix_FreeChunk(logoBeep);
    Mix_FreeChunk(textSound);
    FC_FreeFont(font);
}

int FinishLogoScreen(void)
{
    return finishScreen;
}