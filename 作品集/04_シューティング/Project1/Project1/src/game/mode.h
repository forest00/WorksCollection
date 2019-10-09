#pragma once
#include "object/primitive/keyboard.h" 

//モード定義
enum GameMode
{
	GameMode_None,
	GameMode_Title,
	GameMode_MainPlay,
	GameMode_Ended,
	GameMode_Max,
};

//モードを初期化
extern int InitGameMode(GameMode mode);

//モードを終了
extern void EndGameMode(GameMode mode);

//モードを更新
extern GameMode UpdateGameMode(GameMode mode, const Keyboard *key);

//モードを描画
extern void DrawGameMode(GameMode mode);
