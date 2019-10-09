
#include <Windows.h>
#include "amgame.h"

#include "game_state.h"
#include "input_manager.h"
#include "type_parameter.h"
#include "image_manager.h"
#include "sound_manager.h"
#include "forDebug.h"

static int hBack = -1;
static int hEnding = -1;

static int counter, start, now;
static int pushTime;
static int flag, i;
//起動時に一回だけ実行される
void initEnding() {
	hBack = imageMgrGetHandle(IMAGE_WHITE_BACK, 0);
	//hBack = imageMgrGetHandle(IMAGE_TITLE_BG, 0);
	hEnding = imageMgrGetHandle(IMAGE_ENDING_STAFFROLE, 0);
}

//状態が変わったタイミングで一回だけ実行される
void preEnding() {
	start = GetFrameCounter();
	pushTime = 0;
	flag = 0;
	soundMgrPlay(BGM_STAFF_ROLE);
}

//毎回実行される
void processEnding() {


	flashDebugInfo(i);


	flashDebugInfo(flag);

	now = getGameFrame();
	
	flag = 1;
	
	switch (flag) {
	case 0:
		break;
	case 1:
		i += 2;
		break;
	case 2:
		i += 0;
		i = 5040;
		break;

	}
	


	if (i > 5040) {
		flag = 2;
	}
	
	if (flag == 2) {
		if (isDownKeyBoard(KEYIN_RET) || isDownPad1(PAD_B)) {
			changeGameState(TITLE);
		}
	}
	

	if (isDownKeyBoard(KEYIN_RET)||isDownPad1(PAD_B)) {
		flag = 2;
	}
}

void drawEnding(){


	SetDrawMode(AMDRAW_NOBLEND, 0);
	DrawMem(0, 0, hBack);
	if (i < 5040) {
		SetDrawBright(1000, 1000, 1000);
		SetDrawMode(AMDRAW_NOBLEND, 0);
		DrawMemThExp(0, -i, 1280, 5760 - i, hEnding);
	}
	if (i >= 5040) {
		SetDrawBright(1000, 1000, 1000);
		SetDrawMode(AMDRAW_NOBLEND, 0);
		DrawMemThExp(0, - 5040, 1280, 5760 - 5040, hEnding);
	}
}
