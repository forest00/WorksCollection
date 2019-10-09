#include "game.h"
#include "mode.h"
#include "object/primitive/screen.h"
#include "object/primitive/keyboard.h"
#include "object/looks.h"
#include <stdlib.h>

static Keyboard *key = nullptr;
static GameMode now_mode = GameMode_None;
static GameMode init_mode = GameMode_Title;

static int ChengeMode(GameMode next_mode)
{
	//変更の必要がない場合
	if (next_mode == now_mode)
	{
		return 0;
	}
	//モード終了
	EndGameMode(now_mode);
	//次のモードへ
	now_mode = next_mode;
	//モード初期化
	return InitGameMode(now_mode);
}

static GameMode GetNowMode()
{
	return now_mode;
}

extern int InitGame()
{
	//スクリーン初期化
	InitScreen();
	//キャラの見た目読み込み
	InitLooks();
	//キー入力初期化
	key = (Keyboard *)(malloc(sizeof(Keyboard)));
	if (key == nullptr)
	{
		return -1;
	}
	CheckKey(key);
	//ゲームモード初期化
	if (ChengeMode(init_mode) != 0)
	{
		return -1;
	}
	return 0;
}

extern void EndGame()
{
	EndGameMode(GetNowMode());
	EndLooks();
	EndScreen();
}

extern bool ContinueGame()
{
	//時間待ち
	Wait(64);
	//すでに終了しているかチェック
	if (GetNowMode() == GameMode_Ended)
	{
		return false;
	}
	//エスケープキー入力チェック
	return key->push[EscapeKey()] == false;
}

extern void UpdateGame()
{
	//キーボードチェック
	CheckKey(key);
	//現在のモードを更新
	GameMode next_mode = UpdateGameMode(GetNowMode(), key);
	//次のモードへ
	if (ChengeMode(next_mode) != 0)
	{
		ChengeMode(GameMode_Ended);
	}
}

extern void DrawGame()
{
	//スクリーンクリア
	ClearScreen();
	//現在のモードを描画
	DrawGameMode(GetNowMode());
	//スクリーン描画
	DrawScreen();
}
