#pragma once
#include "primitive/vector2d.h"
#include "primitive/keyboard.h"

#define PlayerMoveSpeed	1.0

//プレーヤーを出す
extern int SetPlayer(Vector2D pos);

//プレーヤーが消されたときに呼ばれる
extern void PlayerCleared(void *kind_info);

//プレーヤー更新
extern void UpdatePlayer(int object_index, const Keyboard *key);

//プレーヤーの数を得る
extern size_t GetPlayerNum();
