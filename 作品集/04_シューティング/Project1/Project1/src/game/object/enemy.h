#pragma once
#include "primitive/vector2d.h"

//“G‚ÌˆÚ“®‘¬“x
#define EnemyMoveSpeed	0.3

//“G‚ğo‚·
extern int SetEnemy(Vector2D pos);

//“G‚ªÁ‚³‚ê‚½‚Æ‚«‚ÉŒÄ‚Î‚ê‚é
extern void EnemyCleared(void *kind_info);

//“GXV
extern void UpdateEnemy(int object_index);

//“G‚Ì”‚ğ“¾‚é
extern size_t GetEnemyNum();
