#ifndef SCREENS_H
#define SCREENS_H

// The types of screens in the game. UNKNOWN should never be used!
enum GameScreen { UNKNOWN = -1, LOGO = 0, TITLE = 1, TITLESELECT, EQUIP, OPTIONS, CHARSEL, GAMEPLAY, ENDING};

extern GameScreen currentScreen; // The game's current screen.

void InitLogoScreen(void); // Initialize the logo screen.
void UpdateLogoScreen(void);
void DrawLogoScreen(void);
void UnloadLogoScreen(void);
int FinishLogoScreen(void);

void InitTitleScreen(void);// Initialize the title screen.
void UpdateTitleScreen(void);
void DrawTitleScreen(void);
void UnloadTitleScreen(void);
int FinishTitleScreen(void);

void InitTitleSelectScreen(void);// Initialize the title select screen.
void UpdateTitleSelectScreen(void);
void DrawTitleSelectScreen(void);
void UnloadTitleSelectScreen(void);
int FinishTitleSelectScreen(void);

void InitEquipScreen(void); // Initialize the equip screen.
void UpdateEquipScreen(void);
void DrawEquipScreen(void);
void UnloadEquipScreen(void);
int FinishEquipScreen(void);

void InitOptionsScreen(void); // Initialize the options screen.
void UpdateOptionsScreen(void);
void DrawOptionsScreen(void);
void UnloadOptionsScreen(void);
int FinishOptionsScreen(void);

void InitCharacterSelectScreen(void); // Initialize the options screen.
void UpdateCharacterSelectScreen(void);
void DrawCharacterSelectScreen(void);
void UnloadCharacterSelectScreen(void);
int FinishCharacterSelectScreen(void);

void InitGameplayScreen(void); // Initialize the gameplay screen.
void UpdateGameplayScreen(void);
void DrawGameplayScreen(void);
void UnloadGameplayScreen(void);
int FinishGameplayScreen(void);

#endif // !SCREENS_H