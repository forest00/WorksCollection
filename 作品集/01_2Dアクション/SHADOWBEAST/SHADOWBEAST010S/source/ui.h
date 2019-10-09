#pragma once


#define _USE_MATH_DEFINES
#include<math.h>

//�Q���������^�x�N�g��
struct Vec{
	int x, y;
};

//����
struct Hp{
	int current;			//���݂̂���
	int max;				//�ő傈��
	double drawCurrent;		//�\���p�̌��݂���(current�̕ϓ��ɍ��킹�ē���)
};

extern int uiDrawPre();

//�_�Q�[�W-------------------------------------------------��

//�����Q�[�W�̃��[�^�[(�_)
struct HpBarGaugeMeter{
	Vec startOffset;		//�I�t�Z�b�g�J�n�ʒu
	Vec endOffset;			//�I�t�Z�b�g�I���ʒu
	int handle;				//�摜�n���h��
};

//UI�Ƃ��Ďg����hp�Q�[�W(�_)
struct HpBarGaugeUi{
	Vec pos;
	Hp hp;
	HpBarGaugeMeter meter;
	int frameHandle;
};

extern int uiHpGaugeUpdate(HpBarGaugeUi *ui);

extern int uiHpGaugeDraw(HpBarGaugeUi *ui);

//�_�Q�[�W-------------------------------------------------��


//�~�Q�[�W-------------------------------------------------��

//�����Q�[�W�̃��[�^�[(�~)
struct HpCircleGaugeMeter{
	Vec offset;				//�I�t�Z�b�g�ʒu
	double emptyRad;		//hp�[���̎��̊p�x
	double fullRad;			//hp�ő厞�̊p�x
	int radius;				//���a
	int handle;				//�摜�n���h��
};


//UI�Ƃ��Ďg����hp�Q�[�W(�~)
struct HpCircleGaugeUi{
	Vec pos;
	Hp hp;
	HpCircleGaugeMeter meter;
	int frameHandle;
};

extern int uiHpGaugeUpdate(HpCircleGaugeUi *ui);

extern int uiHpGaugeDraw(HpCircleGaugeUi *ui);

//�~�Q�[�W-------------------------------------------------��


//��A�C�R��-------------------------------------------------��


//��A�C�R��
struct FaceIconUi{
	Vec pos;
	Vec size;
	int handle;
};





extern int uiFaceIconUpdate(FaceIconUi *ui);

extern int uiFaceIconDraw(FaceIconUi *ui);



//��A�C�R��-------------------------------------------------��





