#pragma once
#include "mode.h"

//�Q�[���v���C
extern int InitMainPlay();
extern void EndMainPlay();
extern GameMode UpdateMainPlay(GameMode mode, const Keyboard *key);
extern void DrawMainPlay();
