
#include <Windows.h>
#include "amgame.h"

#include "game_state.h"
#include "input_manager.h"
#include "type_parameter.h"
#include "image_manager.h"
#include "easing.h"
#include "forDebug.h"

// �w�i
static int hBack = -1;

// �`�[�����S
static int teamLogo = -1;

// �P�x
static int bright;

// �t���[���擾
static int nowFrame;

//�N�����Ɉ�񂾂����s�����
void initTeamLogo() {
	teamLogo = imageMgrGetHandle(IMAGE_TEAM_LOGO, 0);
	hBack = imageMgrGetHandle(IMAGE_WHITE_BACK, 0);
}

//��Ԃ��ς�����^�C�~���O�ň�񂾂����s�����
void preTeamLogo() {
	bright = 0;
}

//������s�����
void processTeamLogo() {

	flashDebugInfo(bright);

	nowFrame = getGameFrame();

	if (bright > 255) {
		bright = 255;
	}

	if(nowFrame < 60 && nowFrame > 0){
		bright += 6;
	}

	if (nowFrame > 105) {
		bright -= 5;
	}

	if (bright < 0) {
		changeGameState(TITLE);
	}
}

void drawTeamLogo() {
	SetDrawMode(AMDRAW_NOBLEND, 0);
	DrawMemTh(0, 0, hBack);

	SetDrawBright(1000, 1000, 1000);
	SetDrawMode(AMDRAW_ALPHABLEND, bright);
	DrawMemTh(440, 260, teamLogo);
}
