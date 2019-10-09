#pragma once

#define PAD_UP			KEYIN_UP
#define PAD_DOWN		KEYIN_DOWN
#define PAD_LEFT		KEYIN_LEFT
#define PAD_RIGHT		KEYIN_RIGHT
#define PAD_A			KEYIN_Z				/**/
#define PAD_B			KEYIN_X				/**/
#define PAD_X			KEYIN_C				/**/
#define PAD_Y			KEYIN_A				/**/
#define PAD_L			KEYIN_S				/**/
#define PAD_R			KEYIN_D				/**/
#define PAD_ZL			KEYIN_Q				/**/
#define PAD_ZR			KEYIN_W				/**/
#define PAD_LSTICKPUSH	KEYIN_ESC			/**/
#define PAD_RSTICKPUSH	KEYIN_SPC			/**/




/*
*1p�̃L�[���͕��@�����肷��
*AMINPUT_KEYBOARD	��	�L�[�{�[�h
*AMINPUT_PAD1		��	�p�b�h1
*AMINPUT_PAD2		��	�p�b�h2
*/
void keyInputModeDetermine1p(int method);
/*
*2p�̃L�[���͕��@�����肷��
*AMINPUT_KEYBOARD	��	�L�[�{�[�h
*AMINPUT_PAD1		��	�p�b�h1
*AMINPUT_PAD2		��	�p�b�h2
*/
void keyInputModeDetermine2p(int method);
/**
*  �L�[���͂��X�V����B
*/
void keyUpdate();
/**
*  �L�[��������Ă��邩���肷��B
*
*  @param key_name ���肷��L�[
*  @return	������Ă�ԁAtrue��Ԃ��B
*/
bool isFreshKeyBoard(int key_name);
bool isFreshPad1(int key_name);
bool isFreshPad2(int key_name);
/**
*  �L�[�������ꂽ�u�Ԃ𔻒肷��B
*  @param key_name �L�[�萔
*  @return	�����ꂽ�u�ԁAtrue��Ԃ��B
*/
bool isDownKeyBoard(int key_name);
bool isDownPad1(int key_name);
bool isDownPad2(int key_name);
/**
*  �L�[��������u�Ԃ𔻒肷��B
*  @param key_name �L�[�萔
*  @return	�������u�ԁAtrue��Ԃ��B
*/
bool isUpKeyBoard(int key_name);
bool isUpPad1(int key_name);
bool isUpPad2(int key_name);
