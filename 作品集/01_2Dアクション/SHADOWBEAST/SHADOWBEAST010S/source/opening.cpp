
#include <Windows.h>
#include "amgame.h"

#include "game_state.h"
#include "input_manager.h"
#include "image_manager.h"
#include "sound_manager.h"

#define BLACK GetColor(0,0,0)

static int hBack = -1;
static int hOpText1 = -1;
static int hOpText2 = -1;
static int hOpText3 = -1;
static int hOpText4 = -1;
static int hOpText5 = -1;
static int hOpLogo = -1;
static int hOpening_bgm = -1;
static int scriptCounter, start, now;
static int pushTime;
static int flag;
static int bright1, bright2, bright3, bright4, bright5;

// オープニングの初期化
// 起動時に1回だけ実行される
void initOpening() {
	hOpText1 = imageMgrGetHandle(IMAGE_OPENING_TEXT1, 0);
	hOpText2 = imageMgrGetHandle(IMAGE_OPENING_TEXT2, 0);
	hOpText3 = imageMgrGetHandle(IMAGE_OPENING_TEXT3, 0);
	hOpText4 = imageMgrGetHandle(IMAGE_OPENING_TEXT4, 0);
	hOpText5 = imageMgrGetHandle(IMAGE_OPENING_TEXT5, 0);
	hOpLogo = imageMgrGetHandle(IMAGE_OPENING_PLOT_TITLELOGO, 0);
	//hOpening = imageMgrGetHandle(IMAGE_OPENING_TXST, 0);
}

//状態が変わったタイミングで一回だけ実行される
void preOpening() {
	start = getGameFrame();
	pushTime = 0;
	flag = 0;
	soundMgrPlay(BGM_OP);
	bright1 = 0;
	bright2 = 0;
	bright3 = 0;
	bright4 = 0;
	bright5 = 0;
}

//毎回実行される
void processOpening() {
	now = getGameFrame();
	scriptCounter = now - start;

	//暗転時間
	if (scriptCounter <= 60) {
		return;
	}

    switch (flag) {
    case 0:
        bright1 += 8;
        break;
    case 1:
        bright2 += 8;
        break;
    case 2:
        bright1 -= 8;
        bright2 -= 8;
        break;
    case 3:
        bright1 = 0;
        bright2 = 0;
        bright3 += 8;
        break;
    case 4:
        bright4 += 8;
        break;
    case 5:
        bright3 -= 8;
        bright4 -= 8;
        break;
    case 6:
        bright3 = 0;
        bright4 = 0;
        bright5 += 8;
        break;
    case 7:
        bright5 -= 8;
        break;
    case 8:
        bright5 = 0;
        break;
    }

	if (bright1 == 1000 && flag == 0) {
		flag = 1;
	}

	if (bright2 == 1000 && flag == 1) {
		flag = 2;
	}

	if ((bright1 == 0 && bright2 == 0) && flag == 2) {
		flag = 3;
	}

	if (bright3 == 1000 && flag == 3) {
		flag = 4;
	}

	if (bright4 == 1000 && flag == 4) {
		flag = 5;
	}

	if ((bright3 == 0 && bright4 == 0) && flag == 5) {
		flag = 6;
	}

	if (bright5 == 1000 && flag == 6) {
		flag = 7;
	}

	if (bright5 == 0 && flag == 7) {
		flag = 8;
	}

	if (bright5 == 0 && flag == 8) {
		flag = 9;
	}

	if (flag == 8) {
		if (isDownKeyBoard(KEYIN_RET) || isDownPad1(PAD_B)) {
			changeGameState(GAMEPLAY);
		}
	}

	if (flag == 9) {
		changeGameState(GAMEPLAY);
	}

	if (bright1 < 0) {
		bright1 = 0;
	}

	if (bright2 < 0) {
		bright2 = 0;
	}

	if (bright3 < 0) {
		bright3 = 0;
	}

	if (bright4 < 0) {
		bright4 = 0;
	}

	if (bright5 < 0) {
		bright5 = 0;
	}
	if (flag > 0 && flag < 9) {
		if (isDownKeyBoard(KEYIN_RET) || isDownPad1(PAD_B)) {
			setSelectedStage(0);
			changeGameState(GAMEPLAY);
		}
	}
}
void drawOpening() {

    DrawFBox(0, 0, 1280, 720, GetColor(0, 0, 0));
    if (flag >= 0 || flag <= 2) {
        SetDrawBright(bright1, bright1, bright1);
        SetDrawMode(AMDRAW_NOBLEND, 0);
        DrawMemTh(300, 200, hOpText1);
    }

    if (flag == 1 || flag == 2) {
        SetDrawBright(bright2, bright2, bright2);
        SetDrawMode(AMDRAW_NOBLEND, 0);
        DrawMemTh(740, 500, hOpText2);
    }

    if (flag >= 3 && flag <= 5) {
        SetDrawBright(bright3, bright3, bright3);
        SetDrawMode(AMDRAW_NOBLEND, 0);
        DrawMemTh(340, 200, hOpText3);
    }

    if (flag == 4 || flag == 5) {
        SetDrawBright(bright4, bright4, bright4);
        SetDrawMode(AMDRAW_NOBLEND, 0);
        DrawMemTh(260, 500, hOpText4);
    }

    if (flag == 6 || flag == 7) {
        SetDrawBright(bright5, bright5, bright5);
        SetDrawMode(AMDRAW_NOBLEND, 0);
        DrawMemTh(160, 360, hOpText5);
    }

    if (flag == 8) {
        SetDrawBright(1000, 1000, 1000);
        SetDrawMode(AMDRAW_NOBLEND, 0);
        DrawMemTh(120, 120, hOpLogo);
    }

}