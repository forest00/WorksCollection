#pragma once

#include <time.h>
#include <Windows.h>
#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
#include "input_mouse_code.h"
#include "input_controller_code.h"

/*------------------------------------------------------------------------------*/
//入力の初期化
//これを呼び出すと入力周りの関数が有効な値を返すようになります
//プログラム内で一回だけ呼び出すようにしてください
extern int input_init(HWND window_handle);

//入力の更新
//枚フレーム呼び出す必要があります
//初期化されていることを前提に動くので注意
extern void input_update();

//入力の終了
//呼び出すと入力系の関数が正常に動作しなくなります
extern void input_end();
/*------------------------------------------------------------------------------*/






/*------------------------------------------------------------------------------*/
//キーが押されている時間を返す(押されていなければ0)
//"DIK_"から始まるマクロを使う
extern int input_keyboard_count(int code);
//キーが押された瞬間か判断する
//"DIK_"から始まるマクロを使う
extern bool input_keyboard_trigger(int code);
//キーが押されているか判断する
//"DIK_"から始まるマクロを使う
extern bool input_keyboard_repeat(int code);
//キーが離された瞬間か判断する
//"DIK_"から始まるマクロを使う
extern bool input_keyboard_release(int code);
/*------------------------------------------------------------------------------*/









/*------------------------------------------------------------------------------*/
//マウスが横に動いた量を得ます
//大体-5~5
//素早く動かすと10を超すことも
extern int input_mouse_move_x();
//マウスが縦に動いた量を得ます
//大体-5~5
//素早く動かすと10を超すことも
extern int input_mouse_move_y();
//マウスのホイール量を得ます
// 戻り値は以下の3つ
//(+120, ±0, -120)
extern int input_mouse_wheel();
//キーが押されている時間を返す(押されていなければ0)
extern int input_mouse_count(input_mouse_code_t code);
//キーが押された瞬間か判断する
extern bool input_mouse_trigger(input_mouse_code_t code);
//キーが押されているか判断する
extern bool input_mouse_repeat(input_mouse_code_t code);
//キーが離された瞬間か判断する
extern bool input_mouse_release(input_mouse_code_t code);
/*------------------------------------------------------------------------------*/










/*------------------------------------------------------------------------------*/
//左スティックの横の傾き
//大体-1.0~1.0
extern double input_controller_left_x();
//左スティックの縦の傾き
//大体-1.0~1.0
extern double input_controller_left_y();
//右スティックの横の傾き
//大体-1.0~1.0
extern double input_controller_right_x();
//右スティックの縦の傾き
//大体-1.0~1.0
extern double input_controller_right_y();
//ボタンが押されている時間を返す(押されていなければ0)
extern int input_controller_count(input_controller_code_t code);
//ボタンが押された瞬間か判断する
extern bool input_controller_trigger(input_controller_code_t code);
//ボタンが押されているか判断する
extern bool input_controller_repeat(input_controller_code_t code);
//ボタンが離された瞬間か判断する
extern bool input_controller_release(input_controller_code_t code);
//十字ボタンが押されている時間を返す(押されていなければ0)
extern int input_controller_cross_count(input_controller_cross_code_t code);
//十字ボタンが押された瞬間か判断する
extern bool input_controller_cross_trigger(input_controller_cross_code_t code);
//十字ボタンが押されているか判断する
extern bool input_controller_cross_repeat(input_controller_cross_code_t code);
//十字ボタンが離された瞬間か判断する
extern bool input_controller_cross_release(input_controller_cross_code_t code);
//ZLとZRの入力値を足した値を返す
//(ZLを押すと正の方向、ZRで負の方向、どっちも押していると0、範囲は99~-100)
extern int input_controller_ZL_ZR();
/*------------------------------------------------------------------------------*/


