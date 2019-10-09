
#include <stdio.h>
#include <stdlib.h>
#include "amgame.h"
#include "chara_enemy_death.h"
#include "chara_point.h"

static PointList *head = nullptr;

static int enemyDeadEffectSetPoint(Point *point){
	PointList *creature = pointListInsert(head);
	creature->data = *point;
	if(head == nullptr){
		head = creature;
	}
	return 0;
}

struct Img{
	char fName[FILENAME_MAX];
	int handle;
	int width;
	int height;
	int elem_num;
	pointRGB *rgbArrayPtr;
};

static int initImg(Img *img){
	img->handle = LoadTexture(img->fName);
	GetPictureSize(img->handle, &(img->width), &(img->height));
	img->elem_num = img->width * img->height;
	img->rgbArrayPtr = (pointRGB*)malloc(sizeof(pointRGB) * (img->elem_num));
	if(img->rgbArrayPtr == nullptr){
		exit(0);
	}
	HCls();
	DrawMem(0, 0, img->handle);
	for(int yi = 0; yi < img->height; yi++){
		for(int xi = 0; xi < img->width; xi++){
			int index = yi * img->width + xi;
			int pixelColor = GetPixel(xi, yi);
			toRGB(pixelColor, &(img->rgbArrayPtr[index]));
		}
	}
	return 0;
}

static int setImgPoint(Img *img, double x, double y, Direction dir, int scale){
	double xp = 1.5;
	double yp = 1.0;
	for(int yi = 0; yi < img->height; yi++){
		for(int xi = 0; xi < img->width; xi++){
			int index = yi * img->width + xi;
			pointRGB *rgb = &(img->rgbArrayPtr[index]);
			if(rgb->red == 255 && rgb->green == 255 && rgb->blue == 255){
				continue;
			}
			if(rand() % 8){
				continue;
			}
			Point point;
			point.isDead = false;
			point.elapsed = 0;
			if(dir == LEFT){
				point.pos.x = x + xi * scale;
			} else{
				point.pos.x = x + img->width - xi * scale;
			}
			point.pos.y = y + yi * scale;
			point.velocity.x = xp * (rand0to1() * 2 - 1);
			point.velocity.y = yp * (rand0to1() * 2 - 1);
			point.alpha = rand() % (256);
			point.color = *rgb;
			enemyDeadEffectSetPoint(&point);
		}
	}
	return 0;
}

static int endImg(Img *img){
	free(img->rgbArrayPtr);
	*img = {};
	return 0;
}

static Img goblin = {"image/�G���S���G�t�F�N�g�p/�S�u����.png"};
static Img wyvern = {"image/�G���S���G�t�F�N�g�p/���C�o�[��.png"};
static Img snake = {"image/�G���S���G�t�F�N�g�p/�w�r.png"};
static Img titan = {"image/�G���S���G�t�F�N�g�p/���l.png"};
static Img seaSnake = {"image/�G���S���G�t�F�N�g�p/�E�~�w�r.png"};

/*
�G�����񂾂Ƃ��̃G�t�F�N�g���o���֐�
@param	type	�G�̎���ID
@param	x		�G��x���W
@param	y		�G��y���W
@return			0...���� -1...���s
*/
int enemyDeadEffectSet(EnemyType type, double x, double y, Direction dir){
	switch(type){
	case ENEMY_GOBLIN:
		setImgPoint(&goblin, x, y, dir, 1);
		break;
	case ENEMY_WYVERN:
		setImgPoint(&wyvern, x, y, dir, 1);
		break;
	case ENEMY_SNAKE:
		setImgPoint(&snake, x, y, dir, 1);
		break;
	case BOSS_TITAN:
		setImgPoint(&titan, x, y, dir, 1);
		break;
	case BOSS_SEASNAKE:
		setImgPoint(&seaSnake, x, y, dir, 2);
		break;
	default:
		break;
	}
	return 0;
}

/*
�G���S���̃G�t�F�N�g���o�����߂̏���������֐�
���̊֐����Ăяo���Ȃ��Ƒ��̊֐��͖��Ӗ��ɂȂ�܂�
@return			0...���� -1...���s
*/
int enemyDeadEffectInit(){
	initImg(&goblin);
	initImg(&wyvern);
	initImg(&snake);
	initImg(&titan);
	initImg(&seaSnake);
	return 0;
}

/*
�G���S���̃G�t�F�N�g���X�V����֐�
�Ăяo�����тɃG�t�F�N�g��1�t���[�������������܂�
@return			0...���� -1...���s
*/
int enemyDeadEffectUpdate(){
	pointUpdateList(&head);
	return 0;
}

/*
�G���S���̃G�t�F�N�g��`�悷��֐�
@return			0...���� -1...���s
*/
int enemyDeadEffectDraw(){
	pointDrawList(head);
	return 0;
}

int enemyDeadEffectEnd(){
	endImg(&goblin);
	endImg(&wyvern);
	endImg(&snake);
	endImg(&titan);
	endImg(&seaSnake);
	return 0;
}

