
#include <Windows.h>
#include "amgame.h"

#include "out_game_effect.h"
#include "game_state.h"
#include "input_manager.h"
#include "type_parameter.h"
#include "image_manager.h"
#include "sound_manager.h"
#include "easing.h"
#include "forDebug.h"
#include <string.h>
#include "title.h" 

#define DISP_W_OP (DISP_CENTER_X)
#define DISP_H_OP (DISP_CENTER_Y)
#define M_PI 3.141592653

// �w�i
static int hBack = -1;
static int titleLogo = -1;

// �G�t�F�N�g
static int claw = -1;
static int hDark = -1;
static int RedEyes = -1;
static int RedEyes2 = -1;

static int mist = -1;
static int mist2 = -1;


/*�P�x�֌W*/
// �^�C�g�����S�P�x
static int titleLogoBright;

// �Ԃ��ڂ̋P�x
static int RedEyesBright, RedEyesBright2;

// ���̋P�x
static int mistBright, mistBright2;

// �Ԃ��ڂ̋P�x�̕ω���
static int rEyesBright1Num, rEyesBright2Num;

// �Ԃ��ڂ̋P�x�̕ω���
static int mistBright1Num, mistBright2Num;

// �܂̋P�x
static int clawBright;

// ����
static int Amazons;

// �t���O�Ǘ�
static int flag;

static unsigned int scriptCounter, start, now;
// ���͎擾
static int pushTime;
// ����
int nowLoading = -1;


// �^�C�g���̏�����
// �N������1�񂾂����s�����
void initTitle() {
  hBack = imageMgrGetHandle(IMAGE_TITLE_BG, 0);
  titleLogo = imageMgrGetHandle(IMAGE_TITLE_LOGO, 0);
	RedEyes = imageMgrGetHandle(IMAGE_TITLE_REDEYES, 0);
	RedEyes2 = imageMgrGetHandle(IMAGE_TITLE_REDEYES2, 0);

	mist = imageMgrGetHandle(IMAGE_TITLE_MIST, 0);
	mist2 = imageMgrGetHandle(IMAGE_TITLE_MIST2, 0);

	claw = imageMgrGetHandle(IMAGE_CLAW, 0);
  hDark = imageMgrGetHandle(IMAGE_DARK, 0);

  nowLoading = imageMgrGetHandle(IMAGE_TITLE_PRESS_ANY_BUTTON, 0);
}

//��Ԃ��ς�����^�C�~���O�ň�񂾂����s�����
void preTitle() {
    //�����̑傫��
	SetFontSize(32);

  // �^�C�g�����S�̋P�x
  titleLogoBright = 0;

  // �܂̋P�x
  clawBright = 0;

	Amazons = 0;

    // �t���O�Ǘ�
	flag = 0;

  // �Ԃ��ڂ̋P�x
  RedEyesBright = 80;
  RedEyesBright2 = 60;

	// ���̋P�x
	mistBright = 60;
	mistBright2 = 240;

  // �Ԃ��ڂ̋P�x�ω���
  rEyesBright1Num = 3;
  rEyesBright2Num = 2;

	// ���̋P�x�ω���
	mistBright1Num = 1;
	mistBright2Num = 3;

  // �t���[������
	start = getGameFrame();
	pushTime = 0;
	soundMgrPlay(BGM_TITLE);
}

//������s�����
void processTitle() {	

  flashDebugInfo(mistBright);

	now = getGameFrame();
	scriptCounter = now - start;
	
    flashDebugInfo(titleLogoBright);

    flashDebugInfo(clawBright);

	//�Ó]����
	if (scriptCounter <= 15) {
		return ;
	}

	//�^�C�g���̏�ԃt���O
    switch (flag) {
	case 0:
        titleLogoBright = 0;
        break;
    case 1:
        titleLogoBright += 4;
		break;
    case 2:
        clawBright = 255;
        titleLogoBright = 150;
        break;
    case 3:
        titleLogoBright -= 2;
        clawBright -= 3;
        break;
    case 4:
        break;
}

    //�P�x�ω�

    //�Ԃ��ڂ̋P�x�ω�
    processRedEyes();

		processMist();

	if (now - (now - 75) == 75 && flag == 0) {
		flag = 1;
	}

	if (titleLogoBright < 150 && flag == 0) {
		if (isDownPad1(PAD_B) ) {
			flag = 1;
		}
	}
  
    if (titleLogoBright > 150) {
        titleLogoBright = 150;
    }

	//flag1�ŉ����ꂽ�u�Ԃ��擾
	if (flag == 1) {
		if (isDownPad1(PAD_B) || isDownPad1(PAD_A) || isDownPad1(PAD_X) || isDownPad1(PAD_Y) ||  isDownPad1(PAD_ZR) || isDownPad1(PAD_ZL) || isDownPad1(PAD_Y) ||  isDownPad1(PAD_L) || isDownPad1(PAD_R) || isDownPad1(PAD_UP) || isDownPad1(PAD_DOWN) || isDownPad1(PAD_LEFT) || isDownPad1(PAD_RIGHT)) {
			flag = 2;
		  //pushTime = now;
		}
	}

  if (flag == 2) {
      changeGameState(MODESELECT);
  }

	//�ܘA��
    if (now - pushTime < 1 && pushTime > 0) {
        soundMgrPlay(SE_TITLE_DECISION_BUTTON);
    }
	if (now - pushTime < 18 && pushTime > 0) {
		claw = imageMgrGetHandle(IMAGE_CLAW, (now - pushTime )/ 2);
	}

	//���[�h�Z���N�g��ʂɈڍs
	if (((now - pushTime )/2) == 48 && pushTime > 0) {
        soundMgrPlay(SE_TITLE_ROAR);
		flag = 3;
	}

    if (titleLogoBright < 0&& flag != 0) {
        flag = 4;
        soundMgrPlay(BGM_MODE_SELECT);
        changeGameState(MODESELECT);
    }

	//�uPress any button�v�̓_�ŏ���
	if (getGameFrame() % 45 == 0) {
		Amazons =! Amazons;
	}
}

//�`��̏���
void drawTitle() {

	//�w�i�̕`��
	SetDrawBright(1000, 1000, 1000);
	SetDrawMode(AMDRAW_NOBLEND, 0);
	DrawMem(0, 0, hBack);

	//���̕`��
	drawMist();

    //�Ԃ��ڂ̕`��
    drawRedEyes();
	
    //�^�C�g�����S
    SetDrawBright(1000, 1000, 1000);
    SetDrawMode(AMDRAW_ALPHABLEND, titleLogoBright);
    DrawMemThExp(256, 0, 1024, 450, titleLogo);

	//�uPress any button�v�̕`��
	if (flag == 1) {
		if (Amazons == 0 && pushTime == 0) {
			SetDrawBright(1000, 1000, 1000);
			SetDrawMode(AMDRAW_ALPHABLEND, 255);
			DrawMemThExp(DISP_W_OP - 150, DISP_H_OP +150, DISP_W_OP + 150, DISP_H_OP +200,nowLoading);
		}
	}

	//�܂̕`�揈��
	if (pushTime > 0) {
        SetDrawBright(1000, 1000, 1000);
		SetDrawMode(AMDRAW_ALPHABLEND, clawBright);
		DrawMemTh(440, 15, claw);
	}
}

// �Ԃ��ڂ̏���
void processRedEyes(){
    //�Ԃ���1
    RedEyesBright += rEyesBright1Num;

    if (RedEyesBright > 255) {
        RedEyesBright = 255;
        rEyesBright1Num *= -1;
    }

    if (RedEyesBright < 20)
    {
        RedEyesBright = 20;
        rEyesBright1Num *= -1;
    }

    //�Ԃ���2
    RedEyesBright2 += rEyesBright2Num;

    if (RedEyesBright2 > 255) {
        RedEyesBright2 = 255;
        rEyesBright2Num *= -1;
    }

    if (RedEyesBright2 < 225)
    {
        RedEyesBright2 = 225;
        rEyesBright2Num *= -1;
    }
}

// �Ԃ��ڂ̕`�揈��
void drawRedEyes() {
    //�Ԃ���1�̕`�揈��
    SetDrawBright(1000, 1000, 1000);
    SetDrawMode(AMDRAW_ADDBLEND, RedEyesBright);
    DrawMemTh(0, 0, RedEyes);
    //�Ԃ���2�̕`�揈��
    SetDrawBright(1000, 1000, 1000);
    SetDrawMode(AMDRAW_ADDBLEND, RedEyesBright2);
    DrawMemTh(0, 0, RedEyes2);
}

//���̏���
void processMist() {
	//�Ԃ���1
	mistBright += mistBright1Num;

	if (mistBright > 200) {
		mistBright = 200;
		mistBright1Num *= -1;
	}

	if (mistBright < 40)
	{
		mistBright = 40;
		mistBright1Num *= -1;
	}

	//�Ԃ���2
	mistBright2 += mistBright2Num;

	if (mistBright2 > 255) {
		mistBright2Num *= -1;
	}

	if (mistBright2 > 0) {
		mistBright2Num *= -1;
	}

}

//���̏���
void drawMist() {
	//�Ԃ���1�̕`�揈��
	SetDrawBright(1000, 1000, 1000);
	SetDrawMode(AMDRAW_ADDBLEND, mistBright);
	DrawMemTh(0, 0, mist);
	//�Ԃ���2�̕`�揈��
	SetDrawBright(1000, 1000, 1000);
	SetDrawMode(AMDRAW_ADDBLEND, mistBright2);
	DrawMemTh(0, 0, mist2);
}