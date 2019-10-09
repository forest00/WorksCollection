
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

// 背景
static int hBack = -1;
static int titleLogo = -1;

// エフェクト
static int claw = -1;
static int hDark = -1;
static int RedEyes = -1;
static int RedEyes2 = -1;

static int mist = -1;
static int mist2 = -1;


/*輝度関係*/
// タイトルロゴ輝度
static int titleLogoBright;

// 赤い目の輝度
static int RedEyesBright, RedEyesBright2;

// 霧の輝度
static int mistBright, mistBright2;

// 赤い目の輝度の変化量
static int rEyesBright1Num, rEyesBright2Num;

// 赤い目の輝度の変化量
static int mistBright1Num, mistBright2Num;

// 爪の輝度
static int clawBright;

// 文字
static int Amazons;

// フラグ管理
static int flag;

static unsigned int scriptCounter, start, now;
// 入力取得
static int pushTime;
// 文字
int nowLoading = -1;


// タイトルの初期化
// 起動時に1回だけ実行される
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

//状態が変わったタイミングで一回だけ実行される
void preTitle() {
    //文字の大きさ
	SetFontSize(32);

  // タイトルロゴの輝度
  titleLogoBright = 0;

  // 爪の輝度
  clawBright = 0;

	Amazons = 0;

    // フラグ管理
	flag = 0;

  // 赤い目の輝度
  RedEyesBright = 80;
  RedEyesBright2 = 60;

	// 霧の輝度
	mistBright = 60;
	mistBright2 = 240;

  // 赤い目の輝度変化量
  rEyesBright1Num = 3;
  rEyesBright2Num = 2;

	// 霧の輝度変化量
	mistBright1Num = 1;
	mistBright2Num = 3;

  // フレーム処理
	start = getGameFrame();
	pushTime = 0;
	soundMgrPlay(BGM_TITLE);
}

//毎回実行される
void processTitle() {	

  flashDebugInfo(mistBright);

	now = getGameFrame();
	scriptCounter = now - start;
	
    flashDebugInfo(titleLogoBright);

    flashDebugInfo(clawBright);

	//暗転時間
	if (scriptCounter <= 15) {
		return ;
	}

	//タイトルの状態フラグ
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

    //輝度変化

    //赤い目の輝度変化
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

	//flag1で押された瞬間を取得
	if (flag == 1) {
		if (isDownPad1(PAD_B) || isDownPad1(PAD_A) || isDownPad1(PAD_X) || isDownPad1(PAD_Y) ||  isDownPad1(PAD_ZR) || isDownPad1(PAD_ZL) || isDownPad1(PAD_Y) ||  isDownPad1(PAD_L) || isDownPad1(PAD_R) || isDownPad1(PAD_UP) || isDownPad1(PAD_DOWN) || isDownPad1(PAD_LEFT) || isDownPad1(PAD_RIGHT)) {
			flag = 2;
		  //pushTime = now;
		}
	}

  if (flag == 2) {
      changeGameState(MODESELECT);
  }

	//爪連番
    if (now - pushTime < 1 && pushTime > 0) {
        soundMgrPlay(SE_TITLE_DECISION_BUTTON);
    }
	if (now - pushTime < 18 && pushTime > 0) {
		claw = imageMgrGetHandle(IMAGE_CLAW, (now - pushTime )/ 2);
	}

	//モードセレクト画面に移行
	if (((now - pushTime )/2) == 48 && pushTime > 0) {
        soundMgrPlay(SE_TITLE_ROAR);
		flag = 3;
	}

    if (titleLogoBright < 0&& flag != 0) {
        flag = 4;
        soundMgrPlay(BGM_MODE_SELECT);
        changeGameState(MODESELECT);
    }

	//「Press any button」の点滅処理
	if (getGameFrame() % 45 == 0) {
		Amazons =! Amazons;
	}
}

//描画の処理
void drawTitle() {

	//背景の描画
	SetDrawBright(1000, 1000, 1000);
	SetDrawMode(AMDRAW_NOBLEND, 0);
	DrawMem(0, 0, hBack);

	//霧の描画
	drawMist();

    //赤い目の描画
    drawRedEyes();
	
    //タイトルロゴ
    SetDrawBright(1000, 1000, 1000);
    SetDrawMode(AMDRAW_ALPHABLEND, titleLogoBright);
    DrawMemThExp(256, 0, 1024, 450, titleLogo);

	//「Press any button」の描画
	if (flag == 1) {
		if (Amazons == 0 && pushTime == 0) {
			SetDrawBright(1000, 1000, 1000);
			SetDrawMode(AMDRAW_ALPHABLEND, 255);
			DrawMemThExp(DISP_W_OP - 150, DISP_H_OP +150, DISP_W_OP + 150, DISP_H_OP +200,nowLoading);
		}
	}

	//爪の描画処理
	if (pushTime > 0) {
        SetDrawBright(1000, 1000, 1000);
		SetDrawMode(AMDRAW_ALPHABLEND, clawBright);
		DrawMemTh(440, 15, claw);
	}
}

// 赤い目の処理
void processRedEyes(){
    //赤い目1
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

    //赤い目2
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

// 赤い目の描画処理
void drawRedEyes() {
    //赤い目1の描画処理
    SetDrawBright(1000, 1000, 1000);
    SetDrawMode(AMDRAW_ADDBLEND, RedEyesBright);
    DrawMemTh(0, 0, RedEyes);
    //赤い目2の描画処理
    SetDrawBright(1000, 1000, 1000);
    SetDrawMode(AMDRAW_ADDBLEND, RedEyesBright2);
    DrawMemTh(0, 0, RedEyes2);
}

//霧の処理
void processMist() {
	//赤い目1
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

	//赤い目2
	mistBright2 += mistBright2Num;

	if (mistBright2 > 255) {
		mistBright2Num *= -1;
	}

	if (mistBright2 > 0) {
		mistBright2Num *= -1;
	}

}

//霧の処理
void drawMist() {
	//赤い目1の描画処理
	SetDrawBright(1000, 1000, 1000);
	SetDrawMode(AMDRAW_ADDBLEND, mistBright);
	DrawMemTh(0, 0, mist);
	//赤い目2の描画処理
	SetDrawBright(1000, 1000, 1000);
	SetDrawMode(AMDRAW_ADDBLEND, mistBright2);
	DrawMemTh(0, 0, mist2);
}