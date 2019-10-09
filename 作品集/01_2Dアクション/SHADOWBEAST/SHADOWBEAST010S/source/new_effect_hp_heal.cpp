

//include

#include <stddef.h>
#include "amgame.h"
#include "camera.h"

//define

#define EFFECT_NUM					16
#define EFFECT_ANIM_STEP			3
#define EFFECT_ANIM_FRAME_MAX		5
#define EFFECT_IMAGE_W				250
#define EFFECT_IMAGE_H				300

//struct

struct ImageData
{
	char *fileName;
	int handle;
};

struct HpHealEffect
{
	//for update
	bool isActive;		//アクティブか
	int activeFrame;	//時間
	int animStep;		//アニメーション一コマ分のフレーム数
	int animFrameMax;	//アニメーション最大駒数
	double worldX;		//位置x
	double worldY;		//位置y
	//for draw
	int drawW;			//描画用横幅
	int drawH;			//描画用縦幅
	int drawOffsetX;	//描画オフセット位置x
	int drawOffsetY;	//描画オフセット位置y
	int drawStartX;		//描画開始位置x
	int drawStartY;		//描画開始位置y
	int drawEndX;		//描画終了位置x
	int drawEndY;		//描画終了位置y
	int alpha;			//アルファ値
	int imageHandle;	//画像ハンドル
};

//objects

static HpHealEffect effects[EFFECT_NUM] = {};
static ImageData imageDataBase[EFFECT_ANIM_FRAME_MAX]{
	{"image/2018_10_04_新規エフェクト/回復エフェクト/asd1.png"			,},
	{"image/2018_10_04_新規エフェクト/回復エフェクト/asd2.png"			,},
	{"image/2018_10_04_新規エフェクト/回復エフェクト/asd3.png"			,},
	{"image/2018_10_04_新規エフェクト/回復エフェクト/asd4.png"			,},
	{"image/2018_10_04_新規エフェクト/回復エフェクト/asd5.png"			,},
};


//functions

static void readImage()
{
	static bool alreadyRead = false;
	if (alreadyRead == false)
	{
		for (int i = 0; i < EFFECT_ANIM_FRAME_MAX; i++)
		{
			imageDataBase[i].handle = LoadTexture(imageDataBase[i].fileName);
		}
		alreadyRead = true;
	}
}

static void init(HpHealEffect *obj)
{
	obj->isActive = false;
	obj->activeFrame = 0;
	obj->animStep = EFFECT_ANIM_STEP;
	obj->animFrameMax = EFFECT_ANIM_FRAME_MAX;
	obj->worldX = 0.0;
	obj->worldY = 0.0;
	obj->drawW = EFFECT_IMAGE_W;
	obj->drawH = EFFECT_IMAGE_H;
	obj->drawOffsetX = -(obj->drawW / 2);
	obj->drawOffsetY = -(obj->drawH);
	obj->drawStartX = 0;
	obj->drawStartY = 0;
	obj->drawEndX = 0;
	obj->drawEndY = 0;
	obj->alpha = 255;
	obj->imageHandle = -1;
}

static void update(HpHealEffect *obj)
{
	const int animFrame = obj->activeFrame / obj->animStep;
	obj->activeFrame++;
	if (animFrame < obj->animFrameMax)
	{
		obj->drawStartX = (int)(obj->worldX) + obj->drawOffsetX;
		obj->drawStartY = (int)(obj->worldY) + obj->drawOffsetY;
		obj->drawEndX = obj->drawStartX + obj->drawW;
		obj->drawEndY = obj->drawStartY + obj->drawH;
		correctByCamera(&obj->drawStartX, &obj->drawStartY);
		correctByCamera(&obj->drawEndX, &obj->drawEndY);
		obj->imageHandle = imageDataBase[animFrame].handle;
	}
	else
	{
		obj->isActive = false;
	}
}

static void draw(HpHealEffect *obj)
{
	if (!(obj->isActive))
	{
		return;
	}
	SetDrawMode(AMDRAW_ALPHABLEND, obj->alpha);
	DrawMemThExp(obj->drawStartX, obj->drawStartY, obj->drawEndX, obj->drawEndY, obj->imageHandle);
}

static void initAll(HpHealEffect *obj_array, size_t num_of_elements)
{
	for (size_t i = 0; i < num_of_elements; i++)
	{
		init(&obj_array[i]);
	}
}
static void updateAll(HpHealEffect *obj_array, size_t num_of_elements)
{
	for (size_t i = 0; i < num_of_elements; i++)
	{
		update(&obj_array[i]);
	}
}
static void drawAll(HpHealEffect *obj_array, size_t num_of_elements)
{
	for (size_t i = 0; i < num_of_elements; i++)
	{
		draw(&obj_array[i]);
	}
}

static HpHealEffect *findNonActive(HpHealEffect *obj_array, size_t num_of_elements)
{
	for (size_t i = 0; i < num_of_elements; i++)
	{
		if (!(obj_array[i].isActive))
		{
			return &obj_array[i];
		}
	}
	return nullptr;
}


extern void hpHealEffectInit()
{
	readImage();
	initAll(effects, EFFECT_NUM);
}

extern void hpHealEffectUpdate()
{
	updateAll(effects, EFFECT_NUM);
}

extern void hpHealEffectDraw()
{
	drawAll(effects, EFFECT_NUM);
}

extern void hpHealEffectEnd()
{
	initAll(effects, EFFECT_NUM);
}

extern void hpHealEffectSet(double worldX, double worldY)
{
	HpHealEffect *found = findNonActive(effects, EFFECT_NUM);
	if (found == nullptr)
	{
		return;
	}
	init(found);
	found->isActive = true;
	found->worldX = worldX;
	found->worldY = worldY;
}

