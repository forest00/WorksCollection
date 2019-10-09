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

//初期化
int initStageData();
//一回呼ぶ
int loadFromFileStageData(int stage);
//毎回呼ぶ
int reflectStageData();
