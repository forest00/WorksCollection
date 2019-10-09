#pragma once
//照準合わせをする
//作成者　森　仁志

//既定の位置
extern float default_aiming_cursor_pos();

//無効な位置
extern float invalid_aiming_cursor_pos();

//照準カーソルの位置初期化(プレーヤーが呼ぶ)
extern void aiming_cursor_pos_init();

//更新(プレーヤーが呼ぶ)
extern void aiming_cursor_pos_update(float target_x, float target_y, float target_z);

//照準カーソルのスクリーン上の位置を得る
extern void aiming_cursor_get_pos(float *x, float *y, float *z);

//照準カーソルのターゲットのスクリーン上の位置を得る
extern void aiming_cursor_get_target_pos(float *x, float *y, float *z);
