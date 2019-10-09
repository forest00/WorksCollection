#define STAGE_NUM 3
#define SELECTABLE_CMD_NUM 8

#define STAGESELECT_STR_Y 606 
#define FLAME_MOVE_X 320

#define FLAME_TOP 540

#define STAGE_SELECT_LEFT 0 
#define STAGE_SELECT_RIGHT 1280
#define STAGE_SELECT_TOP 0
#define STAGE_SELECT_BOTTOM 720

#include <Windows.h>
#include "amgame.h"

#include "input_manager.h"
#include "image_manager.h"
#include "game_state.h"
#include "sound_manager.h"
#include "forDebug.h"
#include "title.h"

//stage1
static int hBackGraundStage1 = -1;

static int castleMain = -1;
static int castleRed = -1;
static int castleGreen = -1;
static int castleBlue = -1;
static int stoneBlock = -1;


static int hBackGraundStage2 = -1;

static int ship = -1;

//フレーム・ステージ名
static int TextBox = -1;


static int stageTutorial = -1;

static int stage1 = -1;
static int stage2 = -1;

static int modeSelectBack = -1;

//はい・いいえ
static int selectflag;

//輝度まとめ
static int bright;
static int brightFlag;
static int defaultBright;

//縦横のなんか(後で名づけ直す)


///選ばれているステージ（この変数は、-1～6の値を取る　※-1がチュートリアル）
static int stageNumber;

static int enemyMotionNumber;
///横
static int selectNumber;

//フレーム移動
static int flameMoveX;

static int flameMoveX2;

static int flameFlag;

//フラグ管理
static int flag;

static unsigned int scriptCounter, start;

static int now;

static int nowStoper;

//ニュートラル
static int nTime;

//文字画像のｘ座標を格納する配列
static const int stageCoordx[4] = {
	10,
	330, 650,
	970,
};

//三角画像のｘ座標を格納する配列
static const int stageCoordx2[7] = {
	stageCoordx[0] + 140 + 8,//tutorial
	stageCoordx[1] + 140 + 8,//1-1
	stageCoordx[2] + 140 + 8,//1-2
	stageCoordx[3] + 140 + 8,//1-3
	stageCoordx[4] + 140 + 8,//2-1
	stageCoordx[5] + 140 + 8,//2-2
	stageCoordx[6] + 140 + 8,//2-3
};

//文字画像のｙ座標を格納する配列
static const int stageCoordy[8] = {
	630,
	24 + 48 * 2, 24 + 48 * 3, 24 + 48 * 4,
	24 + 48 * 5, 24 + 48 * 6, 24 + 48 * 7,
	24 + 48 * 8,};




//各文字画像のハンドルを格納する配列
//static int stageCoordNumber[8];

static int stageCoordNumber[4];

// ステージの初期化
//起動時に一回だけ実行される
void initStageSelect() {

	//背景



	//stage1
	hBackGraundStage1 = imageMgrGetHandle(IMAGE_GAME_PLAY_BG_STAGE1, 0);

	castleMain = imageMgrGetHandle(IMAGE_CASTLE_MAIN, 0);
	castleRed = imageMgrGetHandle(IMAGE_CASTLE_RED, 0);
	castleGreen = imageMgrGetHandle(IMAGE_CASTLE_GREEN, 0);
	castleBlue = imageMgrGetHandle(IMAGE_CASTLE_BLUE, 0);

	stoneBlock = imageMgrGetHandle(IMAGE_BLOCK_STAGE1, 0);
	//stage2
	hBackGraundStage2 = imageMgrGetHandle(IMAGE_GAME_PLAY_BG_STAGE2, 0);
	ship = imageMgrGetHandle(IMAGE_SHIP, 0);

	//テキストボックス
	TextBox = imageMgrGetHandle(IMAGE_TXSTBOX_STAGESELECT_TUTORIAL, 0);


	modeSelectBack = imageMgrGetHandle(IMAGE_BACK, 0);

	stageTutorial = imageMgrGetHandle(IMAGE_STAGESELECT_TUTORIAL_STR, 0);
	stage1 = imageMgrGetHandle(IMAGE_STAGESELECT_STAGE1_STR, 0);
	stage2 = imageMgrGetHandle(IMAGE_STAGESELECT_STAGE2_STR, 0);




	stageNumber = 1;

	flameMoveX = 320;

}

//状態が変わったタイミングで一回だけ実行される
void preStageSelect() {
	SetFontSize(16);
	bright = 750;
	brightFlag = 0;
	defaultBright = 650;


	flag = 0;

	start = getGameFrame();
	now = 0;

	nTime = 0;

	selectNumber = 0;

	enemyMotionNumber = 0;

	flameMoveX2 = flameMoveX + FLAME_MOVE_X;

	flameFlag = 0;

	//配列の準備
	stageCoordNumber[0] = stageTutorial;
	stageCoordNumber[1] = stage1;

	stageCoordNumber[2] = stage2;

	stageCoordNumber[3] = modeSelectBack;

}

//毎回実行される
void processStageSelect() {

	now = getGameFrame();
	scriptCounter = now - start;

	//輝度管理

	//赤い目の輝度変化
	processRedEyes();

	switch(brightFlag){
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

	if(bright > 850 && brightFlag != 2){
		brightFlag = 1;
	}
	if(bright < 750 && brightFlag != 2){
		brightFlag = 0;
	}

	if(isDownPad1(KEYIN_DOWN) || isDownPad1(KEYIN_UP) || isDownPad1(KEYIN_RIGHT) || isDownPad1(KEYIN_LEFT) || isDownPad1(PAD_B)){
		nTime = 0;
		nTime = now;
		soundMgrPlay(SE_CROSS_BUTTON);
	}

	if (flameFlag == 0) {
		flameMoveX += 0;
	}

	if (flameFlag == 1) {
		flameMoveX += 16;
		flameMoveX2 += 16;
	}

	if (flameFlag == 2) {
		flameMoveX -= 16;
		flameMoveX2 -= 16;
	}

	if (flameMoveX == 0 || flameMoveX == 320 || flameMoveX == 640 || flameMoveX == 960) {
		flameFlag = 0;
	}

	if (flameMoveX < -320) {
		flameMoveX = 1280;
	}

	if (flameMoveX > 1280) {
		flameMoveX = -320;
	}

	if (flameMoveX2 > 1600) {
		flameMoveX2 = 0;
	}

	if (flameMoveX2 < 0) {
		flameMoveX2 = 1600;
	}

	//横入力関連
	if (flameMoveX == 0 || flameMoveX == 320 || flameMoveX == 640 || flameMoveX == 960) {
		if (selectNumber == 0 && brightFlag != 2 && isDownPad1(KEYIN_RIGHT)) {
			flameFlag = 1;
			stageNumber++;
		}

		if (stageNumber > STAGE_NUM) {
			stageNumber = 0;
		}

		if (selectNumber == 0 && brightFlag != 2 && isDownPad1(KEYIN_LEFT)) {
			flameFlag = 2;
			stageNumber--;
		}

		if (stageNumber < 0) {
			stageNumber = STAGE_NUM;
		}
	}

	if((isDownPad1(PAD_B)&& (stageNumber != 2 && stageNumber < STAGE_NUM)) && selectNumber == 0&& flameFlag == 0){
		setSelectedStage(stageNumber);
			changeGameState(GAMEPLAY);
	}

	if(((isDownPad1(PAD_B)) && stageNumber == 2) && selectNumber == 0 && flameFlag == 0){
		changeGameState(OPENING2);
	}

	if((isDownPad1(PAD_B)) && selectNumber != 0 && flameFlag == 0){
		selectNumber = 0;
	}

	if(isDownPad1(PAD_A) || (isDownPad1(PAD_B) && stageNumber == STAGE_NUM ) && flameFlag == 0){
		changeGameState(MODESELECT);
	}
}

void drawStageSelect() {
	flashDebugInfo(defaultBright);

	//stage1-1~1-3
	if (stageNumber == 1 && flameFlag == 0) {
		SetDrawBright(1000, 1000, 1000);
	}
	else if (stageNumber != 1 && flameFlag == 0 || flameFlag != 0) {
		SetDrawBright(600, 600, 600);
	}
	SetDrawMode(AMDRAW_NOBLEND, 0);
	DrawMemThFlex(STAGE_SELECT_LEFT,STAGE_SELECT_TOP,
                  STAGE_SELECT_RIGHT, STAGE_SELECT_TOP,
                  STAGE_SELECT_LEFT, STAGE_SELECT_BOTTOM,
                  STAGE_SELECT_LEFT, STAGE_SELECT_BOTTOM,
                  hBackGraundStage1);


	//stage2-1~2-3
	if (stageNumber == 2 && flameFlag == 0) {
		SetDrawBright(1000, 1000, 1000);
	}
	else if (stageNumber != 2 || flameFlag == 0 || flameFlag !=0) {
		SetDrawBright(600, 600, 600);
	}
	SetDrawMode(AMDRAW_NOBLEND, 0);
	DrawMemThFlex(STAGE_SELECT_RIGHT,STAGE_SELECT_TOP,
                  STAGE_SELECT_RIGHT,STAGE_SELECT_TOP,
                  STAGE_SELECT_LEFT,STAGE_SELECT_BOTTOM,
                  STAGE_SELECT_RIGHT,STAGE_SELECT_BOTTOM,
                  hBackGraundStage2);

    if (flameFlag == 0) {
        SetDrawBright(1000, 1000, 1000);
    }
    else if (flameFlag != 0) {
        SetDrawBright(400, 400, 400);
    }
    SetDrawMode(AMDRAW_ADDBLEND, 150);
    DrawMemThExp(flameMoveX, FLAME_TOP, flameMoveX2, STAGE_SELECT_BOTTOM, TextBox);

	for (int i = 0; i < 4; i++) {
		if (stageNumber == i && flameFlag == 0) {
			SetDrawBright(bright, bright, bright);
		}

		else if (stageNumber != i && flameFlag == 0 || flameFlag != 0) {
			SetDrawBright(defaultBright, defaultBright, defaultBright);
		}

		SetDrawMode(AMDRAW_NOBLEND, 0);
		DrawMemTh(stageCoordx[i], STAGESELECT_STR_Y, stageCoordNumber[i]);
	}
}