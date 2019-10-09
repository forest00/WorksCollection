
//include

#include <stddef.h>
#include <stdio.h>
#include "amgame.h"
#include "camera.h"

//define



//struct

struct Shield
{
	//for update
	int activeFrameCounter;		//時間
	int animStep;				//アニメーション一コマ分のフレーム数
	int animFrameMax;			//アニメーション最大駒数
	double worldX;				//位置x
	double worldY;				//位置y
	//for draw
	int drawW;					//描画用横幅
	int drawH;					//描画用縦幅
	int drawOffsetX;			//描画オフセット位置x
	int drawOffsetY;			//描画オフセット位置y
	int drawStartX;				//描画開始位置x
	int drawStartY;				//描画開始位置y
	int drawEndX;				//描画終了位置x
	int drawEndY;				//描画終了位置y
	int alpha;					//アルファ値
	int imageHandle;			//画像ハンドル
};

struct ShieldArrayNode
{
	bool isActive;
	Shield obj;
};


//constant
static const size_t EFFECT_NUM = 8;
static const int EFFECT_ANIM_STEP = 2;
static const char FILE_NAME[FILENAME_MAX] = "image/effect/1p盾/1p_shield.png";		//ファイル名
static const size_t FRAME_TRUE_SIZE_X = 512;										//本当の画像サイズ
static const size_t FRAME_TRUE_SIZE_Y = 512;										//本当の画像サイズ
static const size_t FRAME_NUM_X = 7;
static const size_t FRAME_NUM_Y = 1;
static const size_t FRAME_NUM_ALL = 7;
static const size_t FRAME_GAME_SIZE_X = 200;										//ゲーム内のサイズ
static const size_t FRAME_GAME_SIZE_Y = 200;										//ゲーム内のサイズ


//objects

static ShieldArrayNode effects[EFFECT_NUM];
static int imageHandles[FRAME_NUM_ALL];

//functions

static void readImage()
{
	static bool alreadyRead = false;
	if (alreadyRead == false)
	{
		LoadBlkTexture((char*)FILE_NAME, FRAME_TRUE_SIZE_X, FRAME_TRUE_SIZE_Y, FRAME_NUM_X, FRAME_NUM_Y, FRAME_NUM_ALL, imageHandles);
		alreadyRead = true;
	}
}

static void init(Shield *obj)
{
	obj->activeFrameCounter = 0;
	obj->animStep = EFFECT_ANIM_STEP;
	obj->animFrameMax = FRAME_NUM_ALL;
	obj->worldX = 0.0;
	obj->worldY = 0.0;
	obj->drawW = FRAME_GAME_SIZE_X;
	obj->drawH = FRAME_GAME_SIZE_Y;
	obj->drawOffsetX = -obj->drawW / 2;
	obj->drawOffsetY = -obj->drawH / 2;
	obj->drawStartX = 0;
	obj->drawStartY = 0;
	obj->drawEndX = 0;
	obj->drawEndY = 0;
	obj->alpha = 255;
	obj->imageHandle = -1;
}

static int update(Shield *obj)
{
	int animFrame = obj->activeFrameCounter / obj->animStep;
	if (animFrame >= obj->animFrameMax)
	{
		animFrame = obj->animFrameMax - 1;
		//return -1;
	}
	obj->drawStartX = (int)(obj->worldX) + obj->drawOffsetX;
	obj->drawStartY = (int)(obj->worldY) + obj->drawOffsetY;
	obj->drawEndX = obj->drawStartX + obj->drawW;
	obj->drawEndY = obj->drawStartY + obj->drawH;
	correctByCamera(&obj->drawStartX, &obj->drawStartY);
	correctByCamera(&obj->drawEndX, &obj->drawEndY);
	obj->imageHandle = imageHandles[animFrame];
	obj->activeFrameCounter++;
	return 0;
}
static void draw(Shield *obj)
{
	SetDrawMode(AMDRAW_ALPHABLEND, obj->alpha);
	DrawMemThExp(obj->drawStartX, obj->drawStartY, obj->drawEndX, obj->drawEndY, obj->imageHandle);
}

static void initAll(ShieldArrayNode *obj_array, size_t num_of_elements)
{
	ShieldArrayNode init_node = {};
	init_node.isActive = false;
	init(&init_node.obj);
	for (size_t i = 0; i < num_of_elements; i++)
	{
		obj_array[i] = init_node;
	}
}
static void updateAll(ShieldArrayNode *obj_array, size_t num_of_elements)
{
	for (size_t i = 0; i < num_of_elements; i++)
	{
		if (obj_array[i].isActive)
		{
			if (update(&obj_array[i].obj) != 0)
			{
				obj_array[i].isActive = false;
			}
		}
	}
}
static void drawAll(ShieldArrayNode *obj_array, size_t num_of_elements)
{
	for (size_t i = 0; i < num_of_elements; i++)
	{
		if (obj_array[i].isActive)
		{
			draw(&obj_array[i].obj);
		}
	}
}
static int findNonActive(ShieldArrayNode *obj_array, size_t num_of_elements)
{
	for (size_t i = 0; i < num_of_elements; i++)
	{
		if (!(obj_array[i].isActive))
		{
			return (int)(i);
		}
	}
	return -1;
}





//盾
extern void newEffectShieldInit()
{
	readImage();
	initAll(effects, EFFECT_NUM);
}

//盾
extern void newEffectShieldUpdate()
{
	updateAll(effects, EFFECT_NUM);
}

//盾
extern void newEffectShieldDraw()
{
	drawAll(effects, EFFECT_NUM);
}

//盾
extern void newEffectShieldEnd()
{
	initAll(effects, EFFECT_NUM);
}


//一個出す
extern int newEffectShieldSet(double worldX, double worldY)
{
	int found = findNonActive(effects, EFFECT_NUM);
	if (found == -1)
	{
		return -1;
	}
	effects[found].isActive = true;
	Shield *obj = &effects[found].obj;
	init(obj);
	obj->worldX = worldX;
	obj->worldY = worldY;
	return found;
}

//一個消す
extern void newEffectShieldDelete(int handle)
{
	if (handle < 0 || handle >= EFFECT_NUM)
	{
		return;
	}
	effects[handle].isActive = false;
}
