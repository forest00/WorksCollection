#pragma once

#include "amgame.h"
#include "common.h"

typedef struct {
	XYi pos;						//現在座標
	XYi relative;					//相対座標
	XYi size;						//横幅、縦幅
	XYi home;						//回転、拡大の原点
	XYd scale;						//拡大倍率
	XYi quake;						//揺れ
	double angle;					//回転角度
	int blend;						//ブレンド方法
	int alpha;						//不透明度
	int red, green, blue;			//RGB
	int handle;						//画像ハンドル
	bool flipFlagX, flipFlagY;		//横縦反転フラグ
	bool doHide;					//隠す(非表示にする)
}DrawInfo, *pDrawInfo;

const DrawInfo dispInfoDefault = {
	{0,0},					//左上
	{0,0},					//相対座標ゼロ
	{0,0},					//幅設定なし
	{0,0},					//拡大、回転の原点は左上
	{1,1},					//等倍で表示
	{0,0},					//揺れ無し
	0,						//回転なし
	AMDRAW_ALPHABLEND,		//アルファブレンド
	255,					//不透明
	255,255,255,			//RGBすべて255
	-1,						//ハンドルなし
	false,false,			//反転なし
	false,					//隠さない
};

/*画像の初期化*/
int charaDispInfoInit(pDrawInfo target);

/*画面上の位置の情報格納*/
int charaDispInfoOffSet(pDrawInfo target, int dispX, int dispY);

/*回転の拡大・回転の中心位置の情報格納*/
int charaDispInfoHome(pDrawInfo target, int cenX, int cenY);

/*画像の縦・横の拡大倍率の変更*/
int charaDispInfoScale(pDrawInfo target, double scaleX, double scaleY);

/*画像の回転*/
int charaDispInfoRot(pDrawInfo target, double angle);

/**
画像の
ブレンド方法(-:ブレンドなし　0:加算　+:乗算)
透明度(0~255)
各色の明度(0~255)
を設定
*/
int charaDispInfoColor(pDrawInfo target, int blend, int alpha,
					   int red, int green, int blue);

/*画像のハンドルの情報格納*/
int charaDispInfoHandle(pDrawInfo target, int handle);

/*画像の左右反転*/
int charaDispInfoFlip(pDrawInfo target, bool flagX, bool flagY);

/*画像の描画*/
int charaDispInfoRender(pDrawInfo target);



