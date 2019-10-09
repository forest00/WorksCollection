
#include <Windows.h>
#include "amgame.h"

#include "game_state.h"
#include "input_manager.h"

static int hBack = -1;

//起動時に一回だけ実行される
void initGameComplete() {
	//hBack = LoadTexture("res/name");
}

//状態が変わったタイミングで一回だけ実行される
void preGameComplete() {

}

//毎回実行される
void processGameComplete() {

	DrawMem(0 , 0 , hBack);


	if (isDownKeyBoard(KEYIN_RET)) {
		changeGameState(TITLE);
	}
}

void drawGameComplete() {

}

