

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
	bool isActive;		//�A�N�e�B�u��
	int activeFrame;	//����
	int animStep;		//�A�j���[�V������R�}���̃t���[����
	int animFrameMax;	//�A�j���[�V�����ő�
	double worldX;		//�ʒux
	double worldY;		//�ʒuy
	//for draw
	int drawW;			//�`��p����
	int drawH;			//�`��p�c��
	int drawOffsetX;	//�`��I�t�Z�b�g�ʒux
	int drawOffsetY;	//�`��I�t�Z�b�g�ʒuy
	int drawStartX;		//�`��J�n�ʒux
	int drawStartY;		//�`��J�n�ʒuy
	int drawEndX;		//�`��I���ʒux
	int drawEndY;		//�`��I���ʒuy
	int alpha;			//�A���t�@�l
	int imageHandle;	//�摜�n���h��
};

//objects

static HpHealEffect effects[EFFECT_NUM] = {};
static ImageData imageDataBase[EFFECT_ANIM_FRAME_MAX]{
	{"image/2018_10_04_�V�K�G�t�F�N�g/�񕜃G�t�F�N�g/asd1.png"			,},
	{"image/2018_10_04_�V�K�G�t�F�N�g/�񕜃G�t�F�N�g/asd2.png"			,},
	{"image/2018_10_04_�V�K�G�t�F�N�g/�񕜃G�t�F�N�g/asd3.png"			,},
	{"image/2018_10_04_�V�K�G�t�F�N�g/�񕜃G�t�F�N�g/asd4.png"			,},
	{"image/2018_10_04_�V�K�G�t�F�N�g/�񕜃G�t�F�N�g/asd5.png"			,},
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

