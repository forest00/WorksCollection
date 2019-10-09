
#include <Windows.h>
#include "amgame.h"

#include "image_manager.h"
#include "game_state.h"
#include "input_manager.h"
#include "sound_manager.h"
#include "title.h"

static int hBack = -1;

static int cGameOver = -1;

//�N�����Ɉ�񂾂����s�����
void initGameOver() {
    hBack = imageMgrGetHandle(IMAGE_TITLE_BG, 0);
	cGameOver = imageMgrGetHandle(IMAGE_GAME_OVER, 0);
}

//��Ԃ��ς�����^�C�~���O�ň�񂾂����s�����
void preGameOver() {
    
}

//������s�����
void processGameOver() {
	processRedEyes();
	
	if (isDownKeyBoard(KEYIN_RET)|| isDownPad1(PAD_B)) {
		changeGameState(MODESELECT);
		changeGameState(STAGESELECT);
	}
}

void drawGameOver() {
	SetDrawBright(1000, 1000, 1000);
	SetDrawMode(AMDRAW_NOBLEND, 0);
	DrawMem(0, 0, hBack);
	drawRedEyes();
	SetDrawMode(AMDRAW_NOBLEND, 0);
	SetDrawBright(1000, 1000, 1000);
	DrawMemTh(500, 336, cGameOver);
}

