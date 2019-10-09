#pragma once
#include "primitive/vector2d.h"

//敵の移動速度
#define EnemyMoveSpeed	0.3

//敵を出す
extern int SetEnemy(Vector2D pos);

//敵が消されたときに呼ばれる
extern void EnemyCleared(void *kind_info);

//敵更新
extern void UpdateEnemy(int object_index);

//敵の数を得る
extern size_t GetEnemyNum();
