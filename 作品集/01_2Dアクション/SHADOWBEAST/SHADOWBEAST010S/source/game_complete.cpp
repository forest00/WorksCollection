
#include <Windows.h>
#include "amgame.h"

#include "game_state.h"
#include "input_manager.h"

static int hBack = -1;

//�N�����Ɉ�񂾂����s�����
void initGameComplete() {
	//hBack = LoadTexture("res/name");
}

//��Ԃ��ς�����^�C�~���O�ň�񂾂����s�����
void preGameComplete() {

}

//������s�����
void processGameComplete() {

	DrawMem(0 , 0 , hBack);


	if (isDownKeyBoard(KEYIN_RET)) {
		changeGameState(TITLE);
	}
}

void drawGameComplete() {

}

