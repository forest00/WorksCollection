
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
//c•
#define VERTICAL_WIDTH 120
//Å‚‹P“x
#define MAX_BRIGHT 1000
//”wŒi
static int hBack = -1;

//FX
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
//‹P“xÝ’èŠÖŒW
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
// ƒIƒvƒVƒ‡ƒ“‚Ì‰Šú‰»
// ‹N“®Žž‚Éˆê“x‚¾‚¯ŽÀs‚³‚ê‚é
void initOption() {
  //”wŒi
  hBack = imageMgrGetHandle(IMAGE_TITLE_BG, 0);
  
  //ƒtƒŒ[ƒ€
  hSelectBackGraund = imageMgrGetHandle(IMAGE_TXSTBOX_STAGESELECT_TUTORIAL, 0);

  //•¶Žš
  hBrightSelect = imageMgrGetHandle(IMAGE_OPTION_BRIGHTSELECT, 0);
  hOprationChar = imageMgrGetHandle(IMAGE_OPTION_OPRATION, 0);
  hExit = imageMgrGetHandle(IMAGE_BACK, 0);

  //‘€ìŠm”F
  move = imageMgrGetHandle(IMAGE_TUTORIAL1, 0);
  attack = imageMgrGetHandle(IMAGE_TUTORIAL2, 0);
  subAttack = imageMgrGetHandle(IMAGE_TUTORIAL3, 0);
  guard = imageMgrGetHandle(IMAGE_TUTORIAL4, 0);
  avoid = imageMgrGetHandle(IMAGE_TUTORIAL5, 0);

  aBack = imageMgrGetHandle(IMAGE_A_BACK, 0);

  //‹P“xŠÖ˜A
  hBrightSelect2 = imageMgrGetHandle(IMAGE_SELECTED_BRIGHT, 0);
  hBrightGauge = imageMgrGetHandle(IMAGE_BRIGHT_GAUGE, 0);

   
  optionCharacter[0] = hOprationChar;
  optionCharacter[1] = hBrightSelect;
  optionCharacter[2] = hExit;
  //•¶Žš”z—ñ
  optionCharacter[0] = hOprationChar;
  optionCharacter[1] = hBrightSelect;
  optionCharacter[2] = hExit;

  //•¶Žš”z—ñ2
  gameOperationCharacter[0] = move;
  gameOperationCharacter[1] = attack;
  gameOperationCharacter[2] = subAttack;
  gameOperationCharacter[3] = guard;
  gameOperationCharacter[4] = avoid;

  brightN = 4;
}

//ó‘Ô‚ª•Ï‚í‚Á‚½ƒ^ƒCƒ~ƒ“ƒO‚Åˆê‰ñ‚¾‚¯ŽÀs‚³‚ê‚é
void preOption() {
    SetFontSize(16);
    n = 0;
	flag = 0;
	brightFlag = 0;
	bright = 750;
}

// –ˆ‰ñŽÀs‚³‚ê‚é
void processOption() {

    //‹P“x•Ï‰»

    //–¶‚Ì•`‰æ
  processMist();

    //Ô‚¢–Ú‚Ì‹P“x•Ï‰» 
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

//@•`‰æˆ—
void drawOption() {

	//”wŒi•`‰æ
	DrawMem(0, 0, hBack);

	//–¶‚Ì•`‰æ
	drawMist();

	//Ô‚¢–Ú‚Ì•`‰æ
	drawRedEyes();

	//ƒtƒŒ[ƒ€•`‰æ
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
//–ß‚é“ñŒÂo‚é‚Ì‚Í”z—ñEif•¶Efor•¶‚ðŽg‚¢‚È‚ñ‚Æ‚©‚·‚é
