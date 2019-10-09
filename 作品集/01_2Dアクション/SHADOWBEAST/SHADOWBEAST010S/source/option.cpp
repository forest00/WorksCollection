
#include <Windows.h>
#include "amgame.h"

#include "out_game_effect.h"
#include "input_manager.h"
#include "image_manager.h"
#include "sound_manager.h"
#include "game_state.h"
#include "easing.h"
#include "forDebug.h"
#include "title.h"

#define DISP_W_O (DISP_CENTER_X)
#define DISP_H_O (DISP_CENTER_Y)
//縦幅
#define VERTICAL_WIDTH 120
//最高輝度
#define MAX_BRIGHT 1000
//背景
static int hBack = -1;

//色々
static int hOperation = -1;
static int hSelectBackGraund = -1;


static int hBrightSelect = -1;
static int hOprationChar = -1;
static int hExit = -1;

static int move = -1;
static int attack = -1;
static int subAttack = -1;
static int guard = -1;
static int avoid = -1;

static int aBack = -1;
//輝度設定関係
static int hBrightSelect2 = -1;
static int hBrightGauge = -1;

static int n = 0;
static int bright = 0;
static int flag, brightFlag;
static int brightN = 0;

static int optionCharacter[3];
static int gameOperationCharacter[5];
static int flag1Height[3] = {0, VERTICAL_WIDTH * 1, VERTICAL_WIDTH * 2,};

static int flag2Height[3] = {0, VERTICAL_WIDTH * 1, VERTICAL_WIDTH * 3,};

static int brightGauge[5] = {40, 40 + 140 * 1, 40 + 140 * 2, 40 + 140 * 3,40 + 140 * 4};

static int backGraundBright[5] = {MAX_BRIGHT - (120 * 4), MAX_BRIGHT - (120 * 3),MAX_BRIGHT - (120 * 2), MAX_BRIGHT - (120 * 1), MAX_BRIGHT};

static int operationHeight[5] = {143,236,329,422,515};
// オプションの初期化
// 起動時に一度だけ実行される
void initOption() {
  //背景
  hBack = imageMgrGetHandle(IMAGE_TITLE_BG, 0);
  
  //フレーム
  hSelectBackGraund = imageMgrGetHandle(IMAGE_TXSTBOX_STAGESELECT_TUTORIAL, 0);

  //文字
  hBrightSelect = imageMgrGetHandle(IMAGE_OPTION_BRIGHTSELECT, 0);
  hOprationChar = imageMgrGetHandle(IMAGE_OPTION_OPRATION, 0);
  hExit = imageMgrGetHandle(IMAGE_BACK, 0);

  //操作確認
  move = imageMgrGetHandle(IMAGE_TUTORIAL1, 0);
  attack = imageMgrGetHandle(IMAGE_TUTORIAL2, 0);
  subAttack = imageMgrGetHandle(IMAGE_TUTORIAL3, 0);
  guard = imageMgrGetHandle(IMAGE_TUTORIAL4, 0);
  avoid = imageMgrGetHandle(IMAGE_TUTORIAL5, 0);

  aBack = imageMgrGetHandle(IMAGE_A_BACK, 0);

  //輝度関連
  hBrightSelect2 = imageMgrGetHandle(IMAGE_SELECTED_BRIGHT, 0);
  hBrightGauge = imageMgrGetHandle(IMAGE_BRIGHT_GAUGE, 0);

   
  optionCharacter[0] = hOprationChar;
  optionCharacter[1] = hBrightSelect;
  optionCharacter[2] = hExit;
  //文字配列
  optionCharacter[0] = hOprationChar;
  optionCharacter[1] = hBrightSelect;
  optionCharacter[2] = hExit;

  //文字配列2
  gameOperationCharacter[0] = move;
  gameOperationCharacter[1] = attack;
  gameOperationCharacter[2] = subAttack;
  gameOperationCharacter[3] = guard;
  gameOperationCharacter[4] = avoid;

  brightN = 4;
}

//状態が変わったタイミングで一回だけ実行される
void preOption() {
    SetFontSize(16);
    n = 0;
	flag = 0;
	brightFlag = 0;
	bright = 750;
}

// 毎回実行される
void processOption() {

    //輝度変化

    //霧の描画
  processMist();

    //赤い目の輝度変化 
    processRedEyes();

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
	
	if (bright > 950 && brightFlag != 2) {
		brightFlag = 1;
	}
	if (bright < 750 && brightFlag != 2) {
		brightFlag = 0;
	}

	if (brightFlag !=2 && flag != 1 && flag != 2 && isDownPad1(PAD_DOWN)) {
		n++;
		soundMgrPlay(SE_CROSS_BUTTON);
	}
	if (n > 2) {
		n = 0;
	}


	if (brightFlag != 2 && flag != 1 && flag != 2 && isDownPad1(PAD_UP)) {
		n--;
		soundMgrPlay(SE_CROSS_BUTTON);
	}
	if (n < 0) {
		n = 2;
	}

	if (flag == 2 &&isDownPad1(PAD_LEFT)) {
		brightN--;
		soundMgrPlay(SE_CROSS_BUTTON);
	}
	if (brightN > 4) {
		brightN = 0;
	}
	if (flag == 2 && isDownPad1(PAD_RIGHT)) {
		brightN++;
		soundMgrPlay(SE_CROSS_BUTTON);
	}
	if (brightN < 0) {
		brightN = 4;
	}

	if (isDownKeyBoard(KEYIN_RET) || isDownPad1(PAD_B)) {
		soundMgrPlay(SE_DECISION_BUTTON);
		if (n == 2) {
		GameState next = (GameState)0;
		next = MODESELECT;
		changeGameState(next);
        }	
	}

	if (isDownKeyBoard(KEYIN_RET) || isDownPad1(PAD_B)) {
		soundMgrPlay(SE_DECISION_BUTTON);
		if (n == 0) {
			flag = 1;
		}
		if (n == 1) {
			flag = 2;
		}
	}

	if (flag == 2) {
		
	}
	if (flag == 1 || flag == 2) {
		if (isDownPad1(PAD_A)) {
			soundMgrPlay(SE_DECISION_BUTTON);
			flag = 0;
		}
	}
}

void brightOption(){
	SetDrawBright(backGraundBright[brightN], backGraundBright[brightN], backGraundBright[brightN]);

	//flashDebugInfo(backGraundBright[brightN]);

}

//　描画処理
void drawOption() {

	//背景描画
	DrawMem(0, 0, hBack);

	//霧の描画
	drawMist();

	//赤い目の描画
	drawRedEyes();

	//フレーム描画
	SetDrawBright(1000, 1000, 1000);
	SetDrawMode(AMDRAW_ALPHABLEND, 150);
	DrawMemThExp(160, 80, 1120, 640, hSelectBackGraund);

	int y = 150;

	if (flag != 2 && flag != 1) {
		for (int i = 0; i < 3; i++) {
			if (n == i && flag != 2) {
				SetDrawBright(bright, bright, bright);
			} else if (n != i) {
				SetDrawBright(550, 550, 550);
			} else if (flag == 2) {
				SetDrawBright(950, 950, 950);
			}
			SetDrawMode(AMDRAW_ADDBLEND, 255);
			DrawMemThExp(DISP_W_O - 150, y + flag1Height[i] - 25, DISP_W_O + 150, y + flag1Height[i] + 25, optionCharacter[i]);
		}
	}
	if (flag == 2 && flag != 1) {

		SetDrawBright(MAX_BRIGHT, MAX_BRIGHT, MAX_BRIGHT);
		SetDrawMode(AMDRAW_ALPHABLEND, 100);
		DrawMem(160 + 120, 345, hBrightSelect2);

		for (int i = 0; i < 3; i++) {
			if (i != 1) {
				SetDrawBright(550, 550, 550);
			}
			if (i == 1) {
				SetDrawBright(850, 850, 850);
			}
			SetDrawMode(AMDRAW_ADDBLEND, 255);
			DrawMemThExp(DISP_W_O - 150, y + flag1Height[i] - 25, DISP_W_O + 150, y + flag1Height[i] + 25, optionCharacter[i]);
		}

		SetDrawBright(backGraundBright[brightN], backGraundBright[brightN], backGraundBright[brightN]);
		SetDrawMode(AMDRAW_NOBLEND, 0);
		DrawMem(160 + 120 + brightGauge[brightN], 345 + 20, hBrightGauge);


		if (brightN - 1 < brightN && brightN > 0) {
			int brightNB = brightN - 1;
			for (brightNB = 0; brightNB < brightN; brightNB++) {
				SetDrawBright(backGraundBright[brightN], backGraundBright[brightN], backGraundBright[brightN]);
				SetDrawMode(AMDRAW_NOBLEND, 0);
				DrawMem(160 + 120 + brightGauge[brightNB], 345 + 20, hBrightGauge);
			}
		}
	}

	if (flag != 0 && flag == 1) {
		SetFontSize(32);
		for (int i = 0; i < 5; i++) {
			SetDrawBright(1000, 1000, 1000);
			SetDrawMode(AMDRAW_ADDBLEND, 255);
			DrawMemTh((DISP_W_O / 2) - 88, operationHeight[i], gameOperationCharacter[i]);
		}
		if (flag != 0) {
			SetDrawBright(1000, 1000, 1000);
			SetDrawMode(AMDRAW_ADDBLEND, 255);
			DrawMemThExp(15, 650, 171, 700, aBack);
		}
	}

}
//戻る二個出るのは配列・if文・for文を使いなんとかする
