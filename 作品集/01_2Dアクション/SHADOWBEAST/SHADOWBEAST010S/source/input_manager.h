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
*1pのキー入力方法を決定する
*AMINPUT_KEYBOARD	→	キーボード
*AMINPUT_PAD1		→	パッド1
*AMINPUT_PAD2		→	パッド2
*/
void keyInputModeDetermine1p(int method);
/*
*2pのキー入力方法を決定する
*AMINPUT_KEYBOARD	→	キーボード
*AMINPUT_PAD1		→	パッド1
*AMINPUT_PAD2		→	パッド2
*/
void keyInputModeDetermine2p(int method);
/**
*  キー入力を更新する。
*/
void keyUpdate();
/**
*  キーが押されているか判定する。
*
*  @param key_name 判定するキー
*  @return	押されてる間、trueを返す。
*/
bool isFreshKeyBoard(int key_name);
bool isFreshPad1(int key_name);
bool isFreshPad2(int key_name);
/**
*  キーが押された瞬間を判定する。
*  @param key_name キー定数
*  @return	押された瞬間、trueを返す。
*/
bool isDownKeyBoard(int key_name);
bool isDownPad1(int key_name);
bool isDownPad2(int key_name);
/**
*  キーを放した瞬間を判定する。
*  @param key_name キー定数
*  @return	放した瞬間、trueを返す。
*/
bool isUpKeyBoard(int key_name);
bool isUpPad1(int key_name);
bool isUpPad2(int key_name);
