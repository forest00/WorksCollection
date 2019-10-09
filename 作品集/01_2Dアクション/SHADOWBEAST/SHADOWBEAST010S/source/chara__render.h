#pragma once

#include "amgame.h"
#include "common.h"

typedef struct {
	XYi pos;						//���ݍ��W
	XYi relative;					//���΍��W
	XYi size;						//�����A�c��
	XYi home;						//��]�A�g��̌��_
	XYd scale;						//�g��{��
	XYi quake;						//�h��
	double angle;					//��]�p�x
	int blend;						//�u�����h���@
	int alpha;						//�s�����x
	int red, green, blue;			//RGB
	int handle;						//�摜�n���h��
	bool flipFlagX, flipFlagY;		//���c���]�t���O
	bool doHide;					//�B��(��\���ɂ���)
}DrawInfo, *pDrawInfo;

const DrawInfo dispInfoDefault = {
	{0,0},					//����
	{0,0},					//���΍��W�[��
	{0,0},					//���ݒ�Ȃ�
	{0,0},					//�g��A��]�̌��_�͍���
	{1,1},					//���{�ŕ\��
	{0,0},					//�h�ꖳ��
	0,						//��]�Ȃ�
	AMDRAW_ALPHABLEND,		//�A���t�@�u�����h
	255,					//�s����
	255,255,255,			//RGB���ׂ�255
	-1,						//�n���h���Ȃ�
	false,false,			//���]�Ȃ�
	false,					//�B���Ȃ�
};

/*�摜�̏�����*/
int charaDispInfoInit(pDrawInfo target);

/*��ʏ�̈ʒu�̏��i�[*/
int charaDispInfoOffSet(pDrawInfo target, int dispX, int dispY);

/*��]�̊g��E��]�̒��S�ʒu�̏��i�[*/
int charaDispInfoHome(pDrawInfo target, int cenX, int cenY);

/*�摜�̏c�E���̊g��{���̕ύX*/
int charaDispInfoScale(pDrawInfo target, double scaleX, double scaleY);

/*�摜�̉�]*/
int charaDispInfoRot(pDrawInfo target, double angle);

/**
�摜��
�u�����h���@(-:�u�����h�Ȃ��@0:���Z�@+:��Z)
�����x(0~255)
�e�F�̖��x(0~255)
��ݒ�
*/
int charaDispInfoColor(pDrawInfo target, int blend, int alpha,
					   int red, int green, int blue);

/*�摜�̃n���h���̏��i�[*/
int charaDispInfoHandle(pDrawInfo target, int handle);

/*�摜�̍��E���]*/
int charaDispInfoFlip(pDrawInfo target, bool flagX, bool flagY);

/*�摜�̕`��*/
int charaDispInfoRender(pDrawInfo target);



