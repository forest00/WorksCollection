#pragma once
#include "rect.h"

//スクリーン初期化
//成功...0
//失敗...-1
//メモリが不足しているとき、または、連続で呼び出したとき、失敗する
extern int InitScreen();

//スクリーン終了
//成功...0
//失敗...-1
//初期化されていないとき、失敗する
extern int EndScreen();

//スクリーン全消し
extern void ClearScreen();

//スクリーン幅取得
extern int ScreenWidth();

//スクリーン高さ取得
extern int ScreenHeight();

//スクリーン書き換え(一文字)
//成功...0
//失敗...-1
//範囲外に書き込もうとしたとき、または、ヌル文字を書き込もうとしたとき、失敗する
extern int WriteScreen(int x, int y, char c);

//スクリーン書き換え(文字列)
//成功...0
//失敗...-1
//文字列が長すぎてスクリーンに収まらないとき、失敗する
extern int WriteScreen(int x, int y, const char *s);

//スクリーン描画
extern void DrawScreen();

//座標からインデックスに変換
//インデックスを返す
extern int ToScreenIndex(int x, int y);

//インデックスから座標に変換
//座標のX値を返し、Y値をポインタ経由で返す
extern int ToScreenCoord(int index, int *y);

//スクリーン範囲外チェック
//範囲外のとき真
extern bool OutOfScreen(int x, int y);

//スクリーン範囲外チェック
//範囲外のとき真
extern bool OutOfScreen(Rect rect);

//時間待ち
//ミリ秒で指定する
extern void Wait(int millisec);