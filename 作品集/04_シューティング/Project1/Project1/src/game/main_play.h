#pragma once
#include "mode.h"

//ゲームプレイ
extern int InitMainPlay();
extern void EndMainPlay();
extern GameMode UpdateMainPlay(GameMode mode, const Keyboard *key);
extern void DrawMainPlay();
