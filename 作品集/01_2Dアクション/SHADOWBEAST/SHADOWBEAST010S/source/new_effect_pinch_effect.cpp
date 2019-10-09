
#include "amgame.h"
#include "game_state.h"
#include "chara_castle.h"

static const int HP_MILD_PINCH_PERCENT = 40;
static const int HP_SEVERE_PINCH_PERCENT = 25;
static const double HP_MILD_PINCH_PERCENT_0_TO_1 = (double)(HP_MILD_PINCH_PERCENT) / 100.0;
static const double HP_SEVERE_PINCH_PERCENT_0_TO_1 = (double)(HP_SEVERE_PINCH_PERCENT) / 100.0;
static const int MILD_PINCH_ALPHA = 100;
static const int SEVERE_PINCH_ALPHA = 255;

static int pinchCountWork = 0;
static int pinchCount = 0;
static bool shouldDraw = false;
static int drawAlpha = 0;
static const int IMAGE_REPEAT_FRAME = 4;
static const int IMAGE_NUM = 6;
static int pinchEffectImageHandles[IMAGE_NUM] = {};
static char *fileNames[IMAGE_NUM] = {
"image/2018_10_04_新規エフェクト/ピンチエフェクト/f1.png",
"image/2018_10_04_新規エフェクト/ピンチエフェクト/f2.png",
"image/2018_10_04_新規エフェクト/ピンチエフェクト/f3.png",
"image/2018_10_04_新規エフェクト/ピンチエフェクト/f4.png",
"image/2018_10_04_新規エフェクト/ピンチエフェクト/f5.png",
"image/2018_10_04_新規エフェクト/ピンチエフェクト/f6.png",
};

static void readImage()
{
	for (int i = 0; i < IMAGE_NUM; i++)
	{
		pinchEffectImageHandles[i] = LoadTexture(fileNames[i]);
	}
}

static bool castleHpLessThan(double threshold)
{
	int hp = 0;
	int maxHp = 0;
	getCastleHp(&hp, &maxHp);
	double t = (double)(hp) / (double)(maxHp);
	return t <= threshold;
}

static bool mildPinch()
{
	return castleHpLessThan(HP_MILD_PINCH_PERCENT_0_TO_1);
}

static bool severePinch()
{
	return castleHpLessThan(HP_SEVERE_PINCH_PERCENT_0_TO_1);
}

extern void pinchEffectInit()
{
	static bool alreadyReadImage = false;
	if (alreadyReadImage == false)
	{
		alreadyReadImage = true;
		readImage();
	}
	pinchCountWork = 0;
	pinchCount = 0;
	drawAlpha = 0;
}
extern void pinchEffectUpdate()
{
	bool pinch = false;
	if (mildPinch())
	{
		pinch = true;
		drawAlpha = MILD_PINCH_ALPHA;
	}
	if (severePinch())
	{
		pinch = true;
		drawAlpha = SEVERE_PINCH_ALPHA;
	}
	if (pinch)
	{
		shouldDraw = true;
		pinchCountWork++;
		if (pinchCountWork == IMAGE_REPEAT_FRAME)
		{
			pinchCount++;
			pinchCount %= IMAGE_NUM;
		}
		pinchCountWork %= IMAGE_REPEAT_FRAME;
	}
	else
	{
		shouldDraw = false;
	}
}
extern void pinchEffectDraw()
{
	if (shouldDraw)
	{
		int bright = 1000;
		SetDrawBright(bright, bright, bright);
		SetDrawMode(AMDRAW_ALPHABLEND, drawAlpha);
		DrawMemThExp(0, 0, DISP_W, DISP_H, pinchEffectImageHandles[pinchCount]);
	}
}
extern void pinchEffectEnd()
{
	return;
}




