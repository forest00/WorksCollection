#pragma once
#include "common.h"

extern XYi initPosPlayer1;
extern XYi initPosPlayer2;
extern XYi initPosCastle;
extern int WORLD_WIDTH;
extern int WORLD_HEIGHT;
extern int WORLD_CENTER_X;
extern int WORLD_CENTER_Y;
extern int GROUND_Y;

//‰Šú‰»
int initStageData();
//ˆê‰ñŒÄ‚Ô
int loadFromFileStageData(int stage);
//–ˆ‰ñŒÄ‚Ô
int reflectStageData();
