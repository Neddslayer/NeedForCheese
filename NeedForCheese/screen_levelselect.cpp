#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "screens.h"
#include "utilities.h"
#include "box2d/box2d.h"


static int finishScreen;

b2Vec2 gravity(0.0f, -19.84f);
b2World world(gravity);
b2BodyDef playerDef;
b2Body* playerBody;
b2PolygonShape playerShape;
b2FixtureDef playerFixtureDef;

void InitLevelSelectScreen(void)
{

}
void UpdateLevelSelectScreen(void)
{

}
void DrawLevelSelectScreen(void)
{

}
void UnloadLevelSelectScreen(void)
{

}
int FinishLevelSelectScreen(void)
{
	return finishScreen;
}