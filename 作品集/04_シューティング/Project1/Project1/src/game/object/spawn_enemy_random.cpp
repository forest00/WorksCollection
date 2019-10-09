#include "spawn_enemy_random.h"
#include "enemy.h"
#include "primitive/screen.h"
#include "primitive/random.h"

static double lerp(double x, double y, double t)
{
	return x + (y - x) * t;
}

extern void SpawnEnemyRandom()
{
	if (UniformRandom() < 0.1)
	{
		Vector2D pos;
		pos.x = lerp(1.0, 1.5, UniformRandom()) * ScreenWidth();
		pos.y = lerp(0.25, 0.75, UniformRandom()) * ScreenHeight();
		SetEnemy(pos);
	}
}
