#include <Windows.h>
#include "amgame.h"

#include "input_manager.h"
#include "image_manager.h"
#include "game_state.h"
#include "sound_manager.h"
#include "title.h"

#define DISP_W_MS (DISP_CENTER_X)
#define DISP_H_MS (DISP_CENTER_Y)

#define CHARACTER_SIZE 56

#define CHARACTER_HEIGHT 80

// 背景
static int hBack = -1;
// 
static int n = 0;

// タイトルロゴ輝度
static int titleLogoBright;


//爪
static int claw;

// 爪の輝度
static int clawBright;


////////
static int hStart = -1;

static int titleLogo = -1;

static int hContinue = -1;
static int hOption = -1;
static int hExit = -1;
static int hSelectBack = -1;

////////

static unsigned int  start, now;
// 入力取得
static int pushTime;

////////
static int bright = 0;
static int flag, brightFlag, flameFlag, charaFlag;

static int i1,i2;

static int flameX1, flameY1, flameX2, flameY2;

static int Height[4] = {CHARACTER_HEIGHT,
					    CHARACTER_HEIGHT+(CHARACTER_HEIGHT/2)+(CHARACTER_HEIGHT/4),
						CHARACTER_HEIGHT*2 +(CHARACTER_HEIGHT/2),
						CHARACTER_HEIGHT*3+(CHARACTER_HEIGHT/4)
					    };

static int width[4] = {127, 189, 147, 106};


// モードセレクトの初期化
// 起動時に一回だけ実行される
void initModeSelect() {
  hBack = imageMgrGetHandle(IMAGE_TITLE_BG, 0);
  titleLogo = imageMgrGetHandle(IMAGE_TITLE_LOGO, 0);
	hStart = imageMgrGetHandle(IMAGE_MODESELECT_START, 0);
	hContinue = imageMgrGetHandle(IMAGE_MODESELECT_CONTINUE, 0);
	hOption = imageMgrGetHandle(IMAGE_MODESELECT_OPTION, 0);
	hExit = imageMgrGetHandle(IMAGE_MODESELECT_EXIT, 0);
	
  claw = imageMgrGetHandle(IMAGE_CLAW, 0);

}

// 状態が変わったタイミングで一回だけ実行される
void preModeSelect() {
  SetFontSize(16);
	pushTime = 0;
  
  bright = 750;
  brightFlag = 0;
	
  // タイトルの輝度
  titleLogoBright = 150;

  // 爪の輝度
  clawBright = 0;

  flameFlag = 0;
	charaFlag = 0;
	
  n = 0;
  i1 = 0;
	i2 = 0;
	flag = 0;

  // フレーム処理
  start = getGameFrame();
  pushTime = 0;

}

// 毎回実行される
void processModeSelect() {
	now = getGameFrame();

  //輝度変化

  //霧の描画
  processMist();

  //赤い目の輝度変化
  processRedEyes();

  if (bright > 850 && brightFlag != 2) {
      brightFlag = 1;
  }
  if (bright < 750 && brightFlag != 2) {
      brightFlag = 0;
  }

	switch (flag) {
	case 0:
		i1 += 3;
		i2 += 3;
		break;
	case 1:
		i1 = 255;
		i2 = 255;
		break;
  case 2:
    i2 -= 17;
    clawBright = 255;
      break;
  case 3:
    titleLogoBright -= 2;
    clawBright -= 3;
    break;
  case 4:
    break;
	}

    switch (brightFlag) {
    case 0:
        bright += 10;
        break;
    case 1:
        bright -= 10;
        break;
    case 2:
        bright = 1000;
        break;
    }
	
	if (i1 > 255) {
		i1 = 255;
	}

	if (i2 > 255) {
		flag = 1;
	}

	if (isDownPad1(PAD_DOWN) && flag == 1) {
		n++;
		soundMgrPlay(SE_CROSS_BUTTON);
	}

	if (n > 3) {
		n = 0;
	}

	if (isDownPad1(PAD_UP) && flag == 1) {
		n--;
		soundMgrPlay(SE_CROSS_BUTTON);
	}

	if (n < 0) {
		n = 3;
	}


	if (isDownPad1(PAD_B) && flag == 1) {
		pushTime = now;
    flag = 2;

	}

  if (flag == 2)
  {
    if (n == 0 || n == 1)
    {
      //爪連番
      if (now - pushTime < 1 && pushTime > 0)
      {
        soundMgrPlay(SE_TITLE_DECISION_BUTTON);
      }
      if (now - pushTime < 18 && pushTime > 0)
      {
        claw = imageMgrGetHandle(IMAGE_CLAW, (now - pushTime) / 2);
      }
    }

    if (now - pushTime == 120 && pushTime > 0 && n == 0)
    {
      changeGameState(OPENING);
    }

	if (now - pushTime == 120 && pushTime > 0 && n == 1)
    {
      changeGameState(STAGESELECT);
    }

    //モードセレクト画面に移行
    if (((now - pushTime) / 2) == 48 && pushTime > 0)
    {
      soundMgrPlay(SE_TITLE_ROAR);
      flag = 3;
    }
    
    //else if (n == 1)
    //{
    //  soundMgrPlay(SE_DECISION_BUTTON);
    //  changeGameState(STAGESELECT);
    //}

    else if (n == 2)
    {
      soundMgrPlay(SE_DECISION_BUTTON);
      changeGameState(OPTION);
    }
    else if (n == 3)
    {
      exit(0);
    }
  }


  if (titleLogoBright < 0 && flag != 0 && n == 0)
  {
    changeGameState(OPENING);
  }

  if (titleLogoBright < 0 && flag != 0 && n == 1)
  {
	  i1 = 0;
    changeGameState(STAGESELECT);
  }

}

// 描画処理
void drawModeSelect() {
	//背景描画
	DrawMem(0, 0, hBack);

  //霧の描画
  drawMist();

  //赤い目の描画
  drawRedEyes();

  //タイトルロゴ
  SetDrawBright(1000, 1000, 1000);
  SetDrawMode(AMDRAW_ALPHABLEND, titleLogoBright);
  DrawMemThExp(256, 0, 1024, 450, titleLogo);

  //爪の描画処理
  if (pushTime > 0)
  {
    SetDrawBright(1000, 1000, 1000);
    SetDrawMode(AMDRAW_ALPHABLEND, clawBright);
    DrawMemTh(440, -15, claw);
  }

 // //フレーム描画
 // SetDrawBright(1000, 1000, 1000);
 // SetDrawMode(AMDRAW_ADDBLEND, i);
 // DrawMemThExp(-100, -100, 1400, 800, hSelectBack);

    /*文字画像描画*/
	int character[4] = {hStart, hContinue, hOption, hExit};

	int y = 150;
    //文字輝度設定
	//for (int i = 0;i < 4; i++) {
	//	if(n == i) {
	//		SetDrawBright(bright, bright, bright);  
	//	}else if(n != i) { 
	//		SetDrawBright(700, 700, 700);
	//	}
	//	SetDrawMode(AMDRAW_ADDBLEND, i2);
	//	DrawMemTh(DISP_W_MS - width[i], y + Height[i], character[i]);
	//}

  //文字輝度設定
  for (int i = 0; i < 4; i++)
  {
    if (n == i)
    {
      SetDrawBright(bright, bright, bright);
      SetDrawMode(AMDRAW_ADDBLEND, i1);
    }
    else if (n != i)
    {
      SetDrawBright(700, 700, 700);
      SetDrawMode(AMDRAW_ADDBLEND, i2);
    }
    DrawMemThExp(DISP_W_MS - width[i], DISP_H_MS + Height[i],
				 DISP_W_MS + width[i], DISP_H_MS + Height[i]+CHARACTER_SIZE,
				 character[i]
	);
	  }

}


//フレームを広げながら出す
//
