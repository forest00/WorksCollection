#pragma once

/*
ゲーム開始時に一回だけ呼び出す
*/
void stageTimeStart();

/*
ゲームプレイ中、ポーズ画面以外で毎フレーム呼び出す
*/
void stageTimeUpdate();

/*
ゲームクリアの瞬間に呼び出す
*/
void stageTimeStageClear();

/*
ゲームの経過時間を得る
*/
int stageElapsedTime();

/*
ゲームのクリア時間を得る
*/
int stageClearTime();

