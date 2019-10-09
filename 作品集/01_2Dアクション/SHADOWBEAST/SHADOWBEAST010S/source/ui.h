#pragma once


#define _USE_MATH_DEFINES
#include<math.h>

//２次元整数型ベクトル
struct Vec{
	int x, y;
};

//ｈｐ
struct Hp{
	int current;			//現在のｈｐ
	int max;				//最大ｈｐ
	double drawCurrent;		//表示用の現在ｈｐ(currentの変動に合わせて動く)
};

extern int uiDrawPre();

//棒ゲージ-------------------------------------------------↓

//ｈｐゲージのメーター(棒)
struct HpBarGaugeMeter{
	Vec startOffset;		//オフセット開始位置
	Vec endOffset;			//オフセット終了位置
	int handle;				//画像ハンドル
};

//UIとして使われるhpゲージ(棒)
struct HpBarGaugeUi{
	Vec pos;
	Hp hp;
	HpBarGaugeMeter meter;
	int frameHandle;
};

extern int uiHpGaugeUpdate(HpBarGaugeUi *ui);

extern int uiHpGaugeDraw(HpBarGaugeUi *ui);

//棒ゲージ-------------------------------------------------↑


//円ゲージ-------------------------------------------------↓

//ｈｐゲージのメーター(円)
struct HpCircleGaugeMeter{
	Vec offset;				//オフセット位置
	double emptyRad;		//hpゼロの時の角度
	double fullRad;			//hp最大時の角度
	int radius;				//半径
	int handle;				//画像ハンドル
};


//UIとして使われるhpゲージ(円)
struct HpCircleGaugeUi{
	Vec pos;
	Hp hp;
	HpCircleGaugeMeter meter;
	int frameHandle;
};

extern int uiHpGaugeUpdate(HpCircleGaugeUi *ui);

extern int uiHpGaugeDraw(HpCircleGaugeUi *ui);

//円ゲージ-------------------------------------------------↑


//顔アイコン-------------------------------------------------↓


//顔アイコン
struct FaceIconUi{
	Vec pos;
	Vec size;
	int handle;
};





extern int uiFaceIconUpdate(FaceIconUi *ui);

extern int uiFaceIconDraw(FaceIconUi *ui);



//顔アイコン-------------------------------------------------↑





