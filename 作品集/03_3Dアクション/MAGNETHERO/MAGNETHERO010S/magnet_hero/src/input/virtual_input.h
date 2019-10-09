#pragma once
//仮想の入力を受け付ける場所
//キーボードやマウスからの入力はあらかじめ取得できていることが条件になります
//作成者　森　仁志

//仮想入力
struct virtual_input_t
{
	float _lstick_x;	//左スティックの横の傾き
	float _lstick_y;	//左スティックの縦の傾き
	float _rstick_x;	//右スティックの横の傾き
	float _rstick_y;	//右スティックの縦の傾き
	bool _lkey_left;	//WASD左
	bool _lkey_up;		//WASD上
	bool _lkey_right;	//WASD右
	bool _lkey_down;	//WASD下
	bool _rkey_left;	//矢印キー左
	bool _rkey_up;		//矢印キー上
	bool _rkey_right;	//矢印キー右
	bool _rkey_down;	//矢印キー下
	bool _a;			//A(決定キー)
	bool _b;			//B(戻るキー)
	bool _pause;		//ポーズキー
	char _padding;		//パディング
};

//仮想の入力を受け付けます
//repeatには今フレームの入力が入ります
//triggerには押した瞬間の入力が入ります
//releaseには離した瞬間の入力が入ります
extern void virtual_input_update(virtual_input_t *repeat, virtual_input_t *trigger, virtual_input_t *release);


