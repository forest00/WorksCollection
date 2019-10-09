
#include "input_manager.h"
#include "amgame.h"

#include "forDebug.h"

// キー入力用の構造体
typedef struct {
	int inputMode;		///入力方法
	int fresh;			///< 今回の入力
	int last;			///< 前回の入力
	int down;			///< 押した瞬間の入力
	int up;				///< 放した瞬間の入力
}InputKey;



/**
*  キー入力
*/
static const unsigned int DEVICE_NUM = 3;

static InputKey inputKeyBoard = {AMINPUT_KEYBOARD,0};
static InputKey inputPad1 = {AMINPUT_PAD1,0};
static InputKey inputPad2 = {AMINPUT_PAD2,0};

static InputKey *inputDevice[DEVICE_NUM] = {
	&inputKeyBoard,&inputPad1,&inputPad2
};



void keyInputModeDetermine1p(int method){
	switch(method){
		case AMINPUT_KEYBOARD:
			inputPad1.inputMode = AMINPUT_KEYBOARD;
			break;
		case AMINPUT_PAD1:
			inputPad1.inputMode = AMINPUT_PAD1;
			break;
		case AMINPUT_PAD2:
			inputPad1.inputMode = AMINPUT_PAD2;
			break;
		default:
			break;
	}
}
void keyInputModeDetermine2p(int method){
	switch(method){
		case AMINPUT_KEYBOARD:
			inputPad2.inputMode = AMINPUT_KEYBOARD;
			break;
		case AMINPUT_PAD1:
			inputPad2.inputMode = AMINPUT_PAD1;
			break;
		case AMINPUT_PAD2:
			inputPad2.inputMode = AMINPUT_PAD2;
			break;
		default:
			break;
	}
}

/**
*  キー入力を更新する。
*
*  毎フレームごとに呼び出すことでキー入力を更新します。
*/
void keyUpdate() {
	// 1pキー入力データの更新
	InputKey *key = nullptr;
	int keyUpOrDown = 0;
	for(unsigned int i = 0; i < DEVICE_NUM; i++){
		key = inputDevice[i];
		//1フレーム前の入力
		key->last = key->fresh;
		//現在押しているか
		key->fresh = CheckKey(key->inputMode);
		if(key->fresh == AMERR_NOHARDWARE){
			flashDebugInfo("パッド未接続:%i", i);
			key->fresh = 0;
		}
		if(i == 0){
			int pad1 = CheckKey(AMINPUT_PAD1);
			int pad2 = CheckKey(AMINPUT_PAD2);
			if(pad1!= AMERR_NOHARDWARE){
				key->fresh |= pad1;
			}
			if(pad2!= AMERR_NOHARDWARE){
				key->fresh |= pad2;
			}
		}
		keyUpOrDown = key->fresh ^ key->last;
		// 押した瞬間
		key->down = keyUpOrDown & key->fresh;
		// 放した瞬間
		key->up = keyUpOrDown & key->last;
	}
	#ifdef _DBGOPT_DRAW_INPUT_
	flashDebugInfo("キーボード:%d", inputKeyBoard.fresh);
	flashDebugInfo("1Pパッド  :%d", inputPad1.fresh);
	flashDebugInfo("2Pパッド  :%d", inputPad2.fresh);
	flashDebugInfo();
	#endif // _DBGOPT_DRAW_INPUT_
}


/**
*  キーが押されているか判定する。
*/
bool isFreshKeyBoard(int key_name) {
	return !!(inputKeyBoard.fresh & key_name);
}
bool isFreshPad1(int key_name) {
	return !!(inputPad1.fresh & key_name);
}
bool isFreshPad2(int key_name) {
	return !!(inputPad2.fresh & key_name);
}
/**
*  キーが押された瞬間を判定する。
*/
bool isDownKeyBoard(int key_name) {
	return !!(inputKeyBoard.down & key_name);
}
bool isDownPad1(int key_name) {
	return !!(inputPad1.down & key_name);
}
bool isDownPad2(int key_name) {
	return !!(inputPad2.down & key_name);
}
/**
*  キーを放した瞬間を判定する。
*/
bool isUpKeyBoard(int key_name) {
	return !!(inputKeyBoard.up & key_name);
}
bool isUpPad1(int key_name) {
	return !!(inputPad1.up & key_name);
}
bool isUpPad2(int key_name) {
	return !!(inputPad2.up & key_name);
}