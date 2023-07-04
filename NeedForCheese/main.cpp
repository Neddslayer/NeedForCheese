#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL_FontCache.h"
#include "SDL2/SDL_mixer.h"
#include <iostream>
#include "screens.h"
#include "utilities.h"
#include "world.h"
#include "debug.h"
#include <string>
#undef main
#define fr ;
#define nah false
#define ong true
#define bro int
#define dude float
#define should =
#define respectfully ==
#define yeet return
#define leave break
#define say <<

bool development_mode, consoleShowing, noclip, showHitboxes, playerInfo, old_collision_generation;

SDL_Renderer* renderer; // The main SDL renderer.
SDL_Window* window; // The game window.
SDL_Surface* screenSurface; // The game window as a surface.
const Uint8* keyboard; // All keyboard input.
bool isRunning;
bool fullscreen;

static void ChangeToScreen(GameScreen);
static void TransitionToScreen(GameScreen);
static void UpdateTransition(void);
static void DrawTransition(void);
void HandleEvents();
void UpdateDrawFrame();

GameScreen currentScreen;

static float transAlpha = 0.0f;
static bool transFadeOut = false;
static int transFromScreen = -1;
static bool transitioning = false;
static GameScreen transToScreen = UNKNOWN;

// Set the target frame rate
const int targetFPS = 60;
const int targetFrameTime = 1000 / targetFPS;

// Get the initial tick count
Uint32 prevTime = SDL_GetTicks();

int mouseX, mouseY;
bool mouseClicked;

int WIDTH, HEIGHT, MET2PIX, SCALED_WIDTH, SCALED_HEIGHT; // The width of the window, in pixels.
float SCALE, PIX2MET;

bro main(int argc, char* argv[])
{
	old_collision_generation = false;

#ifdef _DEBUG
	development_mode = true;
#endif // _DEBUG

	for (int i = 0; i < argc; i++)
	{
		if (argv != nullptr)
		{
			if (strcmp(argv[i], "--debug") == 0 || strcmp(argv[i], "-d") == 0)
			{
				development_mode = true;
			}
		}
	}

	if (development_mode) cout << "Debug mode enabled!" << endl;

	bro flags should 0 fr fr
	
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		SDL_DisplayMode displayMode;
		SDL_GetCurrentDisplayMode(0, &displayMode);
		WIDTH = 1920;
		HEIGHT = 1080;
		SCALE = WIDTH / 1280.0;
		MET2PIX = 96 * SCALE;
		PIX2MET = (3.0f * SCALE) / MET2PIX;
		SCALED_WIDTH = WIDTH / MET2PIX;
		SCALED_HEIGHT = HEIGHT / MET2PIX;

		cout << "DEBUG INFORMATION - SCREENSHOT THIS!\n";
		cout << "------------------------------------\n";
		cout << WIDTH << endl;
		cout << HEIGHT << endl;
		cout << SCALE << endl;
		cout << displayMode.driverdata << endl;
		cout << displayMode.format << endl;
		cout << displayMode.refresh_rate << endl;
		cout << MET2PIX << endl;
		cout << "------------------------------------\n";

		cout say "Subsystems Initialized!\n";

		window should SDL_CreateWindow("Need for Cheese", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, flags) fr
		if (window)
		{
			cout say "Window Created!\n";
			SDL_SetWindowMinimumSize(window, 100, 100) fr
		}
		screenSurface = SDL_GetWindowSurface(window);
		if (!screenSurface)
		{
			fprintf(stderr, "Screen surface could not be created: %s\n", SDL_GetError());
			SDL_Quit();
			return 1;
		}
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED) fr
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 121, 121, 121, 255) fr
			cout say "Renderer created!\n" fr
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD) fr
			isRunning should ong;
		}
		// init audio
		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
		{
			// handle error
			throw runtime_error("Opening audio failed!");
			return 1;
			
		}
		cout say "Audio initialized!\n" fr
	}
	TTF_Init();
	currentScreen = LOGO;
	InitLogoScreen();

	while (isRunning respectfully ong)
	{
		Uint32 currentTime = SDL_GetTicks();
		Uint32 deltaTime = currentTime - prevTime;

		HandleEvents();
		UpdateDrawFrame();

		// Delay to meet the target frame rate
		Uint32 frameTime = SDL_GetTicks() - currentTime;
		if (frameTime < targetFrameTime)
		{
			SDL_Delay(targetFrameTime - frameTime);
		}

		// Update prevTime for the next frame
		prevTime = currentTime;
	}
	switch (currentScreen)
	{
	case UNKNOWN:
		break;
	case LOGO:
		UnloadLogoScreen();
		break;
	case TITLE:
		UnloadTitleScreen();
		break;
	case TITLESELECT:
		UnloadTitleSelectScreen();
		break;
	case EQUIP:
		UnloadEquipScreen();
		break;
	case OPTIONS:
		//UnloadOptionsScreen();
		break;
	case CHARSEL:
		//UnloadLevelSelectScreen();
		break;
	case GAMEPLAY:
		//UnloadGameplayScreen();
		break;
	case ENDING:
		//UnloadEndingScreen();
		break;
	default:
		break;
	}
	TTF_Quit();

	Mix_CloseAudio();

	//frees memory associated with renderer and window
	SDL_DestroyRenderer(renderer) fr;
	SDL_DestroyWindow(window) fr
	SDL_Quit() fr

	yeet 0 fr fr
}

// I have no clue why it does this but i had to go through so much hell to get the release build to acutally compile so if it works, it works.
int WinMain(int argc, char* argv[])
{
	return main(argc, argv);
}

void HandleEvents()
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:
		isRunning should nah fr
		leave fr
	case SDL_MOUSEMOTION:
		mouseX = event.motion.x;
		mouseY = event.motion.y;
		leave fr
	case SDL_MOUSEBUTTONDOWN:
		mouseClicked = true;
		leave fr;
	case SDL_MOUSEBUTTONUP:
		mouseClicked = false;
		leave;
	default:
		leave fr
	}
}

bool MouseCollidingWithRect(SDL_Rect rect)
{
	return (mouseX >= rect.x && mouseX < rect.x + rect.w && mouseY >= rect.y && mouseY < rect.y + rect.h);
}

bool MouseCollidingWithRectF(SDL_FRect rect)
{
	return (mouseX >= rect.x && mouseX < rect.x + rect.w && mouseY >= rect.y && mouseY < rect.y + rect.h);
}

// Change to screen immediately, usually for a transition within the screen itself rather than a fade
static void ChangeToScreen(GameScreen screen)
{
	// Unload current screen
	switch (currentScreen)
	{
	case LOGO: UnloadLogoScreen(); break;
	case TITLE: UnloadTitleScreen(); break;
	case TITLESELECT: UnloadTitleSelectScreen(); break;
	case EQUIP: UnloadEquipScreen(); break;
	case OPTIONS: /*UnloadOptionsScreen();*/ break;
	case CHARSEL: /*UnloadLevelSelectScreen();*/ break;
	case GAMEPLAY: /*UnloadGameplayScreen();*/ break;
	case ENDING: /*UnloadEndingScreen();*/ break;
	default: break;
	}

	// Init next screen
	switch (screen)
	{
	case LOGO: InitLogoScreen(); break;
	case TITLE: InitTitleScreen(); break;
	case TITLESELECT: InitTitleSelectScreen(); break;
	case EQUIP: InitEquipScreen(); break;
	case OPTIONS: /*InitOptionsScreen();*/ break;
	case CHARSEL: /*InitLevelSelectScreen();*/ break;
	case GAMEPLAY: InitGameplayScreen(); break;
	case ENDING: /*InitEndingScreen();*/ break;
	default: break;
	}

	currentScreen = screen;
}

// Request transition to next screen
static void TransitionToScreen(GameScreen screen)
{
	transitioning = true;
	transFadeOut = false;
	transFromScreen = currentScreen;
	transToScreen = screen;
	transAlpha = 0.0f;
}

// Update transition effect (fade-in, fade-out)
static void UpdateTransition(void)
{
	if (!transFadeOut)
	{
		transAlpha += 0.05f;
		// NOTE: Due to float internal representation, condition jumps on 1.0f instead of 1.05f
		// For that reason we compare against 1.01f, to avoid last frame loading stop
		if (transAlpha > 1.01f)
		{
			transAlpha = 1.0f;

			// Unload current screen
			switch (transFromScreen)
			{
			case LOGO: UnloadLogoScreen(); break;
			case TITLE: UnloadTitleScreen(); break;
			case OPTIONS: /*UnloadOptionsScreen();*/ break;
			case CHARSEL: /*UnloadGameplayScreen();*/ break;
			case GAMEPLAY: UnloadGameplayScreen(); break;
			case ENDING: /*UnloadEndingScreen();*/ break;
			default: break;
			}

			// Load next screen
			switch (transToScreen)
			{
			case LOGO: InitLogoScreen(); break;
			case TITLE: InitTitleScreen(); break;
			case OPTIONS: /*InitOptionsScreen();*/ break;
			case CHARSEL: /*InitLevelSelectScreen();*/ break;
			case GAMEPLAY: InitGameplayScreen(); break;
			case ENDING: /*InitEndingScreen();*/ break;
			default: break;
			}

			currentScreen = transToScreen;

			// Activate fade out effect to next loaded screen
			transFadeOut = true;
		}
	}
	else  // Transition fade out logic
	{
		transAlpha -= 0.05f;
		switch (currentScreen)
		{
		case LOGO:
		{
			UpdateLogoScreen();

			if (FinishLogoScreen()) TransitionToScreen(TITLE);
		} break;
		case TITLE:
		{
			UpdateTitleScreen();

			if (FinishTitleScreen() == 1) TransitionToScreen(OPTIONS);
			else if (FinishTitleScreen() == 2) TransitionToScreen(CHARSEL);
		} break;
		case OPTIONS:
		{/*
			UpdateOptionsScreen();

			if (FinishOptionsScreen()) TransitionToScreen(TITLE);
			*/
		} break;
		case CHARSEL:
		{
			/*
			UpdateLevelSelectScreen();

			if (FinishLevelSelectScreen()) TransitionToScreen(GAMEPLAY);
			*/
		} break;
		case GAMEPLAY:
		{
			UpdateGameplayScreen();

			if (FinishGameplayScreen() == 1) TransitionToScreen(ENDING);
			else if (FinishGameplayScreen() == 2) TransitionToScreen(TITLE);
			
		} break;
		case ENDING:
		{
			/*
			UpdateEndingScreen();

			if (FinishEndingScreen() == 1) TransitionToScreen(TITLE);
			*/
		} break;
		default: break;
		}
		if (transAlpha < -0.01f)
		{
			transAlpha = 0.0f;
			transFadeOut = false;
			transitioning = false;
			transFromScreen = -1;
			transToScreen = UNKNOWN;
		}
	}
}

static void DrawTransition(void)
{
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, (transAlpha * 255));
	SDL_Rect fadeRect = { 0, 0, 1280, 720 };
	SDL_RenderFillRect(renderer, &fadeRect);
}

// Update and draw the frame
void UpdateDrawFrame()
{
	SDL_PumpEvents();
	keyboard = SDL_GetKeyboardState(NULL);
	if (!transitioning)
	{
		switch (currentScreen)
		{
		case LOGO:
		{
			UpdateLogoScreen();

			if (FinishLogoScreen()) TransitionToScreen(TITLE);
		} break;
		case TITLE:
		{
			UpdateTitleScreen();

			if (FinishTitleScreen()) ChangeToScreen(TITLESELECT);
		} break;
		case TITLESELECT:
		{
			UpdateTitleSelectScreen();

			if (FinishTitleSelectScreen() == 1) ChangeToScreen(CHARSEL);
			if (FinishTitleSelectScreen() == 2) ChangeToScreen(EQUIP);
		} break;
		case EQUIP:
		{
			UpdateEquipScreen();

			//if (FinishEquipScreen()) ChangeToScreen(TITLESELECT);
		} break;
		case OPTIONS:
		{/*
			UpdateOptionsScreen();

			if (FinishOptionsScreen()) TransitionToScreen(TITLE);
			*/
		} break;
		case CHARSEL:
		{
			/*
			UpdateGameplayScreen();

			if (FinishGameplayScreen()) TransitionToScreen(GAMEPLAY);
			*/
		} break;
		case GAMEPLAY:
		{
			UpdateGameplayScreen();

			if (FinishGameplayScreen() == 1) TransitionToScreen(ENDING);
			else if (FinishGameplayScreen() == 2) TransitionToScreen(TITLE);
		} break;
		case ENDING:
		{
			/*
			UpdateEndingScreen();

			if (FinishEndingScreen() == 1) TransitionToScreen(TITLE);
			*/
		} break;
		default: break;
		}
	}
	else
	{
		UpdateTransition();
	}

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer); // Clear the renderer
	switch (currentScreen)
	{
	case LOGO: DrawLogoScreen(); break;
	case TITLE: DrawTitleScreen(); break;
	case TITLESELECT: DrawTitleSelectScreen(); break;
	case EQUIP: DrawEquipScreen(); break;
	case OPTIONS: /*DrawOptionsScreen();*/ break;
	case CHARSEL: /*DrawLevelSelectScreen();*/ break;
	case GAMEPLAY: DrawGameplayScreen(); break;
	case ENDING: /*DrawEndingScreen();*/ break;
	default: cout << "???"; break;
	}

	if (transitioning) DrawTransition();
	SDL_RenderPresent(renderer);
}

bool ButtonClicked(SDL_Rect button)
{
	return MouseCollidingWithRect(button) && mouseClicked;
}
bool ButtonClickedF(SDL_FRect button)
{
	return MouseCollidingWithRectF(button) && mouseClicked;
}