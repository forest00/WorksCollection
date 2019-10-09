#pragma once
#include "mode.h"

//ƒ^ƒCƒgƒ‹
extern int InitTitle();
extern void EndTitle();
extern GameMode UpdateTitle(GameMode mode, const Keyboard *key);
extern void DrawTitle();
