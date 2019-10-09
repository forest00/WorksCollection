
#include <Windows.h>
#include "amgame.h"

#include "input_manager.h"
#include "image_manager.h"
#include "sound_manager.h"
#include "chara_player1.h"
#include "chara_player2.h"
#include "chara__manager.h"
#include "game_state.h"
#include "common.h"
#include "in_game_result.h"
#include "tutorial.h"
#include "stage_time.h"

#include "forDebug.h"

#define DISP_W_O (DISP_CENTER_X)
#define RESULT_DISPLAY_TIME 700

typedef struct
{
	int alpha;
	int red;
	int green;
	int blue;
}ColorElement;

typedef struct
{
	bool flag;
	int count;
	int delay;
	int countLimit;
	ColorElement fade;
}Control;

//背景の画像ハンドル
static int hBack = -1;
//現在のステージ
static int stage = -1;

//ゲームクリア時の文字画像のハンドル
static int cGameClear = -1;
//ゲームオーバー時の文字画像のハンドル
static int cGameOver = -1;

//ポーズ画面 追加実装
static int stageBack = 0;
static int modePause = 1;

static int operationGame = -1;
static int gameBack = -1;
static int stageSelectBack = -1;

static int hOperation = -1;

static int move = -1;
static int attack = -1;
static int subAttack = -1;
static int guard = -1;
static int avoid = -1;

static int aBack = -1;

static int gameOperationCharacter[5];

static int hTitle = -1;

static int stageSelect = 0;

static int choiseNum;

static int choiseStageSelectBack = -1;

static int choiseYes = -1;
static int choiseNo = -1;

static int choiseCharacter[2];

//リザルトを表示すべきか
static bool shouldDisplayResult = false;
//ゲームの暗転と明転の制御をする
Control gameStart = { 0 };	//開始時：明転
Control gameClear = { 0 };	//クリア時：暗転
Control gameOver = { 0 };		//ゲームオーバー時：暗転



//起動時に一回だけ実行される
void initGamePlay()
{
//文字素材
	cGameClear = imageMgrGetHandle(IMAGE_STAGE_CLEAR, 0);
	cGameOver = imageMgrGetHandle(IMAGE_GAME_OVER, 0);
	//操作確認
	move = imageMgrGetHandle(IMAGE_TUTORIAL1, 0);
	attack = imageMgrGetHandle(IMAGE_TUTORIAL2, 0);
	subAttack = imageMgrGetHandle(IMAGE_TUTORIAL3, 0);
	guard = imageMgrGetHandle(IMAGE_TUTORIAL4, 0);
	avoid = imageMgrGetHandle(IMAGE_TUTORIAL5, 0);

	//文字配列2
	gameOperationCharacter[0] = move;
	gameOperationCharacter[1] = attack;
	gameOperationCharacter[2] = subAttack;
	gameOperationCharacter[3] = guard;
	gameOperationCharacter[4] = avoid;

	aBack = imageMgrGetHandle(IMAGE_A_BACK, 0);
	hTitle = imageMgrGetHandle(IMAGE_TITLE_BG, 0);

	cGameClear = imageMgrGetHandle(IMAGE_STAGE_CLEAR, 0);
	cGameOver = imageMgrGetHandle(IMAGE_GAME_OVER, 0);
}

//状態が変わったタイミングで一回だけ実行される
void preGamePlay()
{
	intoGameStart();
	preCharacters();
	//フェード用情報操作部分↓
	stage = getSelectedStage();
	gameStart = gameOver = gameClear = { 0 };

	gameStart.flag = true;
	gameStart.delay = 16;
	gameStart.countLimit = 60;
	gameStart.fade = { 0, 0, 0, 0 };

	gameOver.delay = 64;
	gameOver.countLimit = 60;
	gameOver.fade = { 0, 255, 0, 0 };

	gameClear.delay = 64;
	gameClear.countLimit = 60;
	gameClear.fade = { 0, 0, 0, 0 };
	//フェード用情報操作部分↑

	stageSelect = 0;

	//ステージごとの分岐部分↓
	switch (stage)
	{
	case -1:
		//チュートリアル
		shouldDisplayResult = false;
		initTutorial();
		hBack = imageMgrGetHandle(IMAGE_GAME_PLAY_BG_STAGE1, 0);
		soundMgrPlay(BGM_STAGE1);
		stageSelect = 1;
		break;
	case 0:
	case 1:
		//ノーマルステージ1
		shouldDisplayResult = false;
		hBack = imageMgrGetHandle(IMAGE_GAME_PLAY_BG_STAGE1, 0);
		soundMgrPlay(BGM_STAGE1);
		break;
	case 2:
		//ボス1
		shouldDisplayResult = true;
		hBack = imageMgrGetHandle(IMAGE_GAME_PLAY_BG_STAGE1, 0);
		soundMgrPlay(BGM_BOSS);
		stageSelect = 1;
		break;
	case 3:
	case 4:
		//ノーマルステージ2
		shouldDisplayResult = false;
		hBack = imageMgrGetHandle(IMAGE_GAME_PLAY_BG_STAGE2, 0);
		soundMgrPlay(BGM_STAGE2);
		break;
	case 5:
		//ボス2　クリアするとエンディング
		shouldDisplayResult = true;
		hBack = imageMgrGetHandle(IMAGE_GAME_PLAY_BG_STAGE2, 0);
		soundMgrPlay(BGM_BOSS);
		intoEnding();
		break;
	default:
		break;
	}
	if (shouldDisplayResult == true)
	{
		gameClear.countLimit += RESULT_DISPLAY_TIME;
	}
	//ステージごとの分岐部分↑
	SetFontSize(16);
	//時間をリセット
	stageTimeStart();
}

//毎回実行される(処理部分)
void processGamePlay()
{
	CharactersUpdate();
	switch (stage)
	{
	case -1:
		if (updateTutorial())
		{
			gameClear.flag = true;
			gameClear.count += 8;
		}
	case 0:
		setSelectedStage(2);
		break;
	case 1:
		setSelectedStage(3);
		break;
	case 2:
		break;
	case 3:
		setSelectedStage(5);
		break;
	case 4:
		setSelectedStage(6);
		break;
	case 5:
	{
#ifdef _DBGOPT_KEYBOARD_
		if (ForDebugDownKey('N'))
		{
			intoGameClear();
		}
		if (ForDebugDownKey('B'))
		{
			intoGameOver();
		}
#endif
		bool dead1p = isPlayer1Dead();
		bool dead2p = isPlayer2Dead();
		if (dead1p && dead2p)
		{
			intoGameOver();
		}
	}
	break;
	default:
		break;
	}
	if (isDownKeyBoard(KEYIN_START) || isDownKeyBoard(KEYIN_W))
	{
		pause();
	}
	/**/
	/*ゲームステート管理部分*/
	if (isGameClear())
	{
		if (!gameOver.flag)
		{
			gameClear.flag = true;
		}
	}
	if (isGameOver())
	{
		if (!gameClear.flag)
		{
			gameOver.flag = true;
		}
	}
	//フラグによる制御↓
	//ゲーム開始直後か
	if (gameStart.flag)
	{
		int elapsed = 0;
		elapsed = gameStart.count - gameStart.delay;
		gameStart.count++;
		elapsed++;
		double t = (double)elapsed / (double)gameStart.countLimit;
		if (elapsed < 0)
		{
			t = 0;
		}
		double s = 1.0 - t;
		gameStart.fade.alpha = (int)(s * 255);
		if (elapsed >= gameStart.countLimit)
		{
			gameStart.flag = false;
		}
	}

	//クリアしているか
	if (gameClear.flag)
	{
		//クリアした瞬間か
		if (gameClear.count == 0)
		{
			stageTimeStageClear();
			//リザルトを出すべきか
			if (shouldDisplayResult)
			{
				//リザルトの準備
				resultInfoSetUp();
			}
		}
		resultInfoUpdate();
		int elapsed = 0;
		elapsed = gameClear.count - gameClear.delay;
		if (elapsed >= gameClear.countLimit)
		{
			resultInfoDestroy();
			if (isGameEnding())
			{
				if (isDownPad1(PAD_B))
				{
					changeGameState(STAFFROLL);
				}
			}
			if (stageSelect == 0 && !isGameEnding())
				if (isDownKeyBoard(KEYIN_RET) || isDownPad1(PAD_B))
				{
					changeGameState(GAMEPLAY);
				}

			if (stageSelect == 1)
			{
				if (isDownKeyBoard(KEYIN_RET) || isDownPad1(PAD_B))
				{
					soundMgrPlay(BGM_MODE_SELECT);
					changeGameState(STAGESELECT);
				}
			}
			//changeGameState(GAMECLEAR);
		}
		gameClear.count++;
		elapsed++;
		double t = (double)elapsed / (double)gameClear.countLimit;
		if (elapsed < 0)
		{
			t = 0;
		}
		gameClear.fade.alpha = (int)(t * 255);
	}
	//守りきれなかったか
	if (gameOver.flag)
	{
		int elapsed = 0;
		elapsed = gameOver.count - gameOver.delay;
		if (elapsed >= gameOver.countLimit)
		{
			if (isDownKeyBoard(KEYIN_RET) || isDownPad1(PAD_B))
			{
				soundMgrPlay(BGM_MODE_SELECT);
				changeGameState(STAGESELECT);
			}
			//changeGameState(GAMEOVER);
		}
		gameOver.count++;
		elapsed++;
		double t = (double)elapsed / (double)gameOver.countLimit;
		if (elapsed < 0)
		{
			t = 0;
		}
		gameOver.fade.alpha = (int)(t * 255);
	}
	//フラグによる制御↑
	//時間を更新
	stageTimeUpdate();
}
//毎回実行される(描画部分)
void drawGamePlay()
{
	SetDrawMode(AMDRAW_ADDBLEND, 256);
	DrawMemTh(0, 0, hBack);
	DrawTBox(0, 0, DISP_W, DISP_H, GetColor(255, 255, 255));
	drawCharacters();
	//フェード部分↓
	///開始時
	if (gameStart.flag)
	{
		const ColorElement *c = &gameStart.fade;
		int color = GetColor(c->red, c->green, c->blue);
		SetDrawMode(AMDRAW_ALPHABLEND, c->alpha);
		SetDrawBright(1000, 1000, 1000);
		DrawFBox(0, 0, DISP_W, DISP_H, color);
	}
	///クリア時
	if (gameClear.flag)
	{
		resultInfoDraw();
		const ColorElement *c = &gameClear.fade;
		//int color = GetColor(c->red, c->green, c->blue);
		SetDrawMode(AMDRAW_ALPHABLEND, c->alpha);
		//SetDrawBright(1000, 1000, 1000);
		DrawMemTh(460, 336, cGameClear);
		//DrawFBox(0, 0, DISP_W, DISP_H, color);
	}
	///ゲームオーバー時
	if (gameOver.flag)
	{
		const ColorElement *c = &gameOver.fade;
		//int color = GetColor(c->red, c->green, c->blue);
		SetDrawMode(AMDRAW_ALPHABLEND, c->alpha);
		//SetDrawBright(1000, 1000, 1000);
		DrawMemTh(500, 336, cGameOver);
		//DrawFBox(0, 0, DISP_W, DISP_H, color);
	}
	//フェード部分↑

}


//ポーズ
void pause()
{
	static int alphaPause = 0;
	static int alphaPauseMax = 200;
	static int select = 9;
	static const int deltaPause = 8;

	//ポーズ画面追加実装2
	static int flag, n, bright, brightFlag;

	static int hOprationChar = -1;
	static int hExit = -1;

	static int character[3];

	static int pauseWidth1[3] = {470, 470, 360};

	static int pauseWidth2[3] = {770, 760, 880};

	static int pauseHeight1[3] = { 250, 350, 450 };

	static int pauseHeight2[3] = { 310, 410, 510 };

	static int operationHeight[5] = { 143, 236, 329, 422, 515 };

	static int choiseWidth1[2] = { 500, 680 };

	static int choiseWidth2[2] = { 600, 780 };

	static int choiseHeight[2] = { 350, 450 };

	modePause = imageMgrGetHandle(IMAGE_TXSTBOX_STAGESELECT_TUTORIAL, 0);
	operationGame = imageMgrGetHandle(IMAGE_OPTION_OPRATION, 0);
	stageSelectBack = imageMgrGetHandle(IMAGE_STAGESELECT_MODEBACK, 0);

	gameBack = imageMgrGetHandle(IMAGE_BACK, 0);
	choiseCharacter[0] = choiseYes;
	choiseCharacter[1] = choiseNo;
	character[0] = operationGame;
	character[1] = gameBack;
	character[2] = stageSelectBack;

	int countPause = 0;

	choiseNum = 1;
	n = 0;
	flag = 0;
	brightFlag = 0;
	bright = 750;

	//ここまで
	//

	do
	{
	  //
	  //ポーズ画面追加実装3
		switch (brightFlag)
		{
		case 0:
			bright += 2;
			break;
		case 1:
			bright -= 2;
			break;
		case 2:
			bright = 1000;
			break;
		}

		if (bright > 255 && brightFlag != 2)
		{
			brightFlag = 1;
		}
		if (bright < 155 && brightFlag != 2)
		{
			brightFlag = 0;
		}

		if (flag == 0 && isDownKeyBoard(KEYIN_DOWN))
		{
			n++;
			soundMgrPlay(SE_CROSS_BUTTON);
		}
		if (n > 2)
		{
			n = 0;
		}


		if (flag == 0 && isDownKeyBoard(KEYIN_UP))
		{
			n--;
			soundMgrPlay(SE_CROSS_BUTTON);
		}
		if (n < 0)
		{
			n = 2;
		}

		//ステージセレクトに戻る処理

		if (flag == 2 && isDownKeyBoard(KEYIN_RIGHT))
		{
			choiseNum++;
			soundMgrPlay(SE_CROSS_BUTTON);
		}
		if (flag == 2 && choiseNum > 1)
		{
			choiseNum = 0;
		}

		if (flag == 2 && isDownKeyBoard(KEYIN_LEFT))
		{
			choiseNum--;
			soundMgrPlay(SE_CROSS_BUTTON);
		}

		if (flag == 2 && choiseNum < 0)
		{
			choiseNum = 1;
		}

		if (flag == 0 && (isDownKeyBoard(KEYIN_RET) || isDownPad1(PAD_B)))
		{
			soundMgrPlay(SE_DECISION_BUTTON);
			if (n == 0)
			{
				flag = 1;
			}
		}
		if (flag == 0 && (isDownKeyBoard(KEYIN_RET) || isDownPad1(PAD_B)))
		{
			soundMgrPlay(SE_DECISION_BUTTON);
			if (n == 2)
			{
				soundMgrPlay(BGM_MODE_SELECT);
				changeGameState(STAGESELECT);
			}
		}

		if (flag == 2 && choiseNum == 1)
		{
			if (isDownKeyBoard(KEYIN_RET) || isDownPad1(PAD_B))
			{
				flag = 0;
			}
		}

		if (isDownPad1(PAD_A))
		{
			soundMgrPlay(SE_DECISION_BUTTON);
			flag = 0;
		}

		HCls();
		drawGamePlay();
		countPause++;
		alphaPause = countPause * deltaPause;

		//SetDrawBright(1000, 1000, 1000);
		if (alphaPause >= alphaPauseMax)
		{
			alphaPause = alphaPauseMax;
		}
		if (flag == 0)
		{
			SetDrawMode(AMDRAW_ALPHABLEND, alphaPause);
			DrawMemThExp(280, 180, 960, 540, modePause);
			SetDrawMode(AMDRAW_NOBLEND, 0);
			SetFontSize(32);
			DrawMemThExp(320, 210, 476, 260, aBack);
			//DrawMemThExp(320, 210, 476, 260, aBack);
			for (int i = 0; i < 3; i++)
			{
//SetDrawBright(1000, 1000, 1000);
				if (n == i && flag == 0)
				{
					SetDrawMode(AMDRAW_ADDBLEND, bright);
				}
				if (n != i)
				{
					SetDrawMode(AMDRAW_ADDBLEND, 150);
				}
				DrawMemThExp(pauseWidth1[i], pauseHeight1[i], pauseWidth2[i], pauseHeight2[i], character[i]);
			}
		}
		if (flag == 1)
		{
			SetFontSize(32);
			//SetDrawBright(1000, 1000, 1000);
			SetDrawMode(AMDRAW_ALPHABLEND, alphaPause);
			DrawMemThExp(160, 80, 1120, 640, modePause);
			SetDrawMode(AMDRAW_NOBLEND, 0);
			for (int i = 0; i < 5; i++)
			{
//SetDrawBright(1000, 1000, 1000);
				SetDrawMode(AMDRAW_ADDBLEND, 255);
				DrawMemTh((DISP_W_O / 2) - 88,
					operationHeight[i],
					gameOperationCharacter[i]);
			}
			DrawMemThExp(10, 660, 166, 710,aBack);
			//DrawMemThExp(320, 210, 476, 260, aBack);
		}

		if (flag == 2)
		{

			SetDrawBright(1000, 1000, 1000);
			SetDrawMode(AMDRAW_ALPHABLEND, alphaPause);
			DrawMemThExp(320, 180, 920, 540, modePause);
			SetDrawMode(AMDRAW_NOBLEND, 0);
			for (int i = 0; i < 2; i++)
			{
//SetDrawBright(1000, 1000, 1000);
				if (choiseNum == i && flag == 2)
				{
					SetDrawMode(AMDRAW_ADDBLEND, bright);
				}
				if (choiseNum != i)
				{
					SetDrawMode(AMDRAW_ADDBLEND, 150);
				}
				DrawMemThExp(choiseWidth1[i],
					choiseHeight[0],
					choiseWidth2[i],
					choiseHeight[1],
					choiseCharacter[i]);
			}
		}

		//ここまで
		//

		keyUpdate();
		renderDebugInfo();

		//
		//操作確認画面時の画面遷移
		if (flag == 1 && (isDownKeyBoard(KEYIN_Q) || isDownPad1(PAD_B)))
		{
			flag = 0;
		}
		//ここまで
		//


		if (flag == 0 && (isDownKeyBoard(KEYIN_Q) || isDownKeyBoard(KEYIN_W) || isDownPad1(PAD_A)) || ((n == 1 || n == 2) && (isDownKeyBoard(KEYIN_RET) || isDownPad1(PAD_B))))
		{
			break;
		}
		Flip();
	} while (ProcessEvent());
	do
	{
		HCls();
		drawGamePlay();
		if (alphaPause >= alphaPauseMax)
		{
			alphaPause = alphaPauseMax;
		}
		SetDrawMode(AMDRAW_ALPHABLEND, alphaPause);
		DrawMemThExp(280, 180, 960, 540, modePause);
		SetDrawMode(AMDRAW_NOBLEND, 0);
		for (int i = 0; i < 3; i++) {
			//SetDrawBright(1000, 1000, 1000);
			if (n == i && flag == 0) {
				SetDrawMode(AMDRAW_ALPHABLEND, alphaPause);
			}
			if (n != i) {
				SetDrawMode(AMDRAW_ALPHABLEND, alphaPause);
			}
			DrawMemThExp(pauseWidth1[i], pauseHeight1[i], pauseWidth2[i], pauseHeight2[i], character[i]);
		}
		SetDrawBright(1000, 1000, 1000);
		SetDrawMode(AMDRAW_ALPHABLEND, alphaPause);
		DrawMemThExp(320, 210, 476, 260, aBack);
		renderDebugInfo();
		alphaPause -= deltaPause;
		if (alphaPause <= 0)
		{
			break;
		}
		Flip();
	} while (ProcessEvent());
	if (n == 2 && (isDownKeyBoard(KEYIN_RET) || isDownPad1(PAD_B)))
	{
		soundMgrPlay(BGM_MODE_SELECT);
		changeGameState(STAGESELECT);
	}
	SetDrawMode(AMDRAW_NOBLEND, 0);
	renderDebugInfo();
	return;
}
