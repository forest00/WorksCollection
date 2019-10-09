#pragma once

//キーボードの入力を取得するための構造体
struct Keyboard
{
	bool push[256];
};

//キーボードの入力を取得する
extern void CheckKey(Keyboard *key);

//矢印キーコード↓
extern int ArrowUp();
extern int ArrowDown();
extern int ArrowRight();
extern int ArrowLeft();
//矢印キーコード↑

//エスケープキーコード
extern int EscapeKey();
