
#include <Windows.h>
#include "amgame.h"

#include "game_state.h"
#include "input_manager.h"
#include "image_manager.h"
#include "sound_manager.h"
#include "stage_select.h"
#define BLACK GetColor(0,0,0)

static int hBack = -1;
static int hOpText1 = -1;
static int hOpLogo = -1;
//static int hOpening = -1;
static int hOpening_bgm = -1;
static int scriptCounter, start, now;
static int pushTime;
static int flag;
static int bright1, bright2, bright3, bright4, bright5;

// オープニングの初期化
// 起動時に1回だけ実行される
void initOpening2() {
    hOpText1 = imageMgrGetHandle(IMAGE_STAGE2_OP, 0);
    hOpLogo = imageMgrGetHandle(IMAGE_OPENING_PLOT_TITLELOGO, 0);
    //hOpening = imageMgrGetHandle(IMAGE_OPENING_TXST, 0);
}

//状態が変わったタイミングで一回だけ実行される
void preOpening2() {
    start = getGameFrame();
    pushTime = 0;
    flag = 0;
    soundMgrPlay(BGM_OP);
}

//毎回実行される
void processOpening2() {
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
        bright1 -= 8;
        break;
    case 2:
        bright1 = 0;
        break;

    }

    if (bright1 == 1000 && flag == 0) {
        flag = 1;
    }

    if (bright1 == 0 && flag == 1) {
        flag = 2;
    }


    if (flag == 2) {
        if (isDownKeyBoard(KEYIN_RET) || isDownPad1(PAD_B)) {
            setSelectedStage(3);
            if (1) {
                changeGameState(GAMEPLAY);
            }
        }
    }

    if (flag == 3) {
        setSelectedStage(4);
        if (1) {
            changeGameState(GAMEPLAY);
        }
    }

    if (bright1 < 0) {
        bright1 = 0;
    }

    if (flag < 2) {
        if (isDownKeyBoard(KEYIN_RET) || isDownPad1(PAD_B)) {
            setSelectedStage(4);
            if (1) {
                changeGameState(GAMEPLAY);
            }
        }
    }
}
void drawOpening2() {
    DrawFBox(0, 0, 1280, 720, GetColor(0, 0, 0));
    if (flag >= 0 || flag <= 2) {
        SetDrawBright(bright1, bright1, bright1);
        SetDrawMode(AMDRAW_NOBLEND, 0);
        DrawMemTh(300, 200, hOpText1);
    }

    if (flag == 2) {
        SetDrawBright(1000, 1000, 1000);
        SetDrawMode(AMDRAW_NOBLEND, 0);
        DrawMemTh(120, 120, hOpLogo);
    }
}