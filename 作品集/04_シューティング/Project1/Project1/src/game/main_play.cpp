#include "main_play.h"
#include "object/object.h"
#include "object/player.h"
#include "object/spawn_enemy_random.h"
#include "object/primitive/screen.h"
//for debug
#include "object/primitive/debug.h"

extern int InitMainPlay()
{
	if (InitObject() != 0)
	{
		return -1;
	}
	if (SetPlayer(Vector2D_Literal(ScreenWidth() / 5, ScreenHeight() / 2)) != 0)
	{
		return -1;
	}
	return 0;
}
extern void EndMainPlay()
{
	EndObject();
}
extern GameMode UpdateMainPlay(GameMode mode, const Keyboard *key)
{
	SpawnEnemyRandom();
	UpdateObject(key);
	return mode;
}
extern void DrawMainPlay()
{
	DrawObject();
	DrawDebug();
}
