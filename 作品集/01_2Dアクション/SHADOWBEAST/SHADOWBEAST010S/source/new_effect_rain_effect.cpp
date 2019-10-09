
#include "amgame.h"
#include "game_state.h"

static const int IMAGE_REPEAT_FRAME = 4;
static const int IMAGE_NUM = 5;
static int rainImageHandles[IMAGE_NUM] = {};
static char *fileNames[IMAGE_NUM] = {
	"image/2018_10_04_新規エフェクト/雨エフェクト/1.png",
	"image/2018_10_04_新規エフェクト/雨エフェクト/2.png",
	"image/2018_10_04_新規エフェクト/雨エフェクト/3.png",
	"image/2018_10_04_新規エフェクト/雨エフェクト/4.png",
	"image/2018_10_04_新規エフェクト/雨エフェクト/5.png",
};

static int rainCountWork = 0;
static int rainCount = 0;
static bool shouldDraw = false;

static void readImage()
{
	for (int i = 0; i < IMAGE_NUM; i++)
	{
		rainImageHandles[i] = LoadTexture(fileNames[i]);
	}
}

extern void rainEffectInit()
{
	static bool alreadyReadImage = false;
	if (alreadyReadImage == false)
	{
		alreadyReadImage = true;
		readImage();
	}
	rainCountWork = 0;
	rainCount = 0;
}

extern void rainEffectUpdate()
{
	int stage = getSelectedStage();
	if (stage >= 3 && stage < 6)
	{
		shouldDraw = true;
		rainCountWork++;
		if (rainCountWork == IMAGE_REPEAT_FRAME)
		{
			rainCount++;
			rainCount %= IMAGE_NUM;
		}
		rainCountWork %= IMAGE_REPEAT_FRAME;
	}
	else
	{
		shouldDraw = false;
	}
}


extern void rainEffectDraw()
{
	if (shouldDraw)
	{
		int bright = 1000;
		SetDrawBright(bright, bright, bright);
		SetDrawMode(AMDRAW_ALPHABLEND, 255);
		DrawMemThExp(0, 0, DISP_W, DISP_H, rainImageHandles[rainCount]);
	}
}


extern void rainEffectEnd()
{
	return;
}





