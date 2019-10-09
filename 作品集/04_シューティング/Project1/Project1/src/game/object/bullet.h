#pragma once
#include "primitive/vector2d.h"

//弾を出す
extern int SetBullet(Vector2D pos, Vector2D velocity);

//弾が消されたときに呼ばれる
extern void BulletCleared(void *kind_info);

//弾更新
extern void UpdateBullet(int object_index);

//弾の数を得る
extern size_t GetBulletNum();
