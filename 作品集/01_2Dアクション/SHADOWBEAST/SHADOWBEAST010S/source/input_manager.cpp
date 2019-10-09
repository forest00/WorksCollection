
#include "input_manager.h"
#include "amgame.h"

#include "forDebug.h"

// �L�[���͗p�̍\����
typedef struct {
	int inputMode;		///���͕��@
	int fresh;			///< ����̓���
	int last;			///< �O��̓���
	int down;			///< �������u�Ԃ̓���
	int up;				///< �������u�Ԃ̓���
}InputKey;



/**
*  �L�[����
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
*  �L�[���͂��X�V����B
*
*  ���t���[�����ƂɌĂяo�����ƂŃL�[���͂��X�V���܂��B
*/
void keyUpdate() {
	// 1p�L�[���̓f�[�^�̍X�V
	InputKey *key = nullptr;
	int keyUpOrDown = 0;
	for(unsigned int i = 0; i < DEVICE_NUM; i++){
		key = inputDevice[i];
		//1�t���[���O�̓���
		key->last = key->fresh;
		//���݉����Ă��邩
		key->fresh = CheckKey(key->inputMode);
		if(key->fresh == AMERR_NOHARDWARE){
			flashDebugInfo("�p�b�h���ڑ�:%i", i);
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
		// �������u��
		key->down = keyUpOrDown & key->fresh;
		// �������u��
		key->up = keyUpOrDown & key->last;
	}
	#ifdef _DBGOPT_DRAW_INPUT_
	flashDebugInfo("�L�[�{�[�h:%d", inputKeyBoard.fresh);
	flashDebugInfo("1P�p�b�h  :%d", inputPad1.fresh);
	flashDebugInfo("2P�p�b�h  :%d", inputPad2.fresh);
	flashDebugInfo();
	#endif // _DBGOPT_DRAW_INPUT_
}


/**
*  �L�[��������Ă��邩���肷��B
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
*  �L�[�������ꂽ�u�Ԃ𔻒肷��B
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
*  �L�[��������u�Ԃ𔻒肷��B
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