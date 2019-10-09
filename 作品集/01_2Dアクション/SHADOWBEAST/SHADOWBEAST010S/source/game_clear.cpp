
#include <Windows.h>
#include "amgame.h"

#include "image_manager.h"
#include "sound_manager.h"
#include "game_state.h"
#include "input_manager.h"
#include "title.h"

static int hBack = -1;

static int cGameClear = -1;

#define DISP_W_MS (DISP_CENTER_X)

// �N�����Ɉ�񂾂����s�����
// ��Ԃ��ς�����^�C�~���O�ň�񂾂����s�����
void initGameClear() {
    hBack = imageMgrGetHandle(IMAGE_TITLE_BG, 0);
	cGameClear = imageMgrGetHandle(IMAGE_STAGE_CLEAR, 0);
}

// ������s�����
void preGameClear() {

}

void processGameClear() {
	processRedEyes();

	if (isDownKeyBoard(KEYIN_RET) || isDownPad1(PAD_B)) {
		changeGameState(STAGESELECT);
	}
}

void drawGameClear() {
	SetDrawMode(AMDRAW_NOBLEND, 0);
	SetDrawBright(1000, 1000, 1000);
	DrawMem(0, 0, hBack);
	drawRedEyes();
	SetDrawMode(AMDRAW_NOBLEND, 0);
	SetDrawBright(1000, 1000, 1000);
	DrawMemTh(460, 336, cGameClear);
}
