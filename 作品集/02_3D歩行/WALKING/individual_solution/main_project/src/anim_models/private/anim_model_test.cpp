//親ヘッダー
#include "..\public\anim_model_test.h"
//その他のヘッダー
#include "..\public\anim_data_loader.h"
#include "things\robot.h"
#include "things\ken.h"
#include "things\golem.h"
#include "things\josh.h"
#include "things\usagi.h"
#include "things\block_man.h"
#include "..\..\input\public\input.h"
#include "..\..\sound\public\sound.h"



//全てのテスト用のモデルを初期化し、Xファイルを読み込み、出現させる
extern void anim_model_tester_all_init(IDirect3DDevice9 *device)
{
	//ファイルを読み込む
	anim_data_load_all(device);
	//アニメーションするやつらを全部出す
	robot_init();
	ken_init();
	golem_init();
	josh_init();
	usagi_init();
	block_man_init();
}

//全てのモデルを破棄する
extern void anim_model_tester_all_end()
{
	//アニメーションするやつらを全部破棄
	robot_end();
	ken_end();
	golem_end();
	josh_end();
	usagi_end();
	block_man_end();
	//読み込んだファイルを破棄
	anim_data_destroy_all();
}

//全てのモデルを更新する
extern void anim_model_tester_all_update()
{
	//アニメーションするやつらを全部更新
	robot_update();
	ken_update();
	golem_update();
	josh_update();
	usagi_update();
	block_man_update();
	//音を出してみる
	if (input_keyboard_trigger(DIK_F9))
	{
		sound_play(sound_index_se_1);
	}
}

//全てのモデルを描画する
extern void anim_model_tester_all_draw(IDirect3DDevice9 *device)
{
	//アニメーションするやつらを全部描画
	robot_draw(device);
	ken_draw(device);
	golem_draw(device);
	josh_draw(device);
	usagi_draw(device);
	block_man_draw(device);
}




//プレイヤーとして使うモデルの位置を得る
void anim_model_get_the_position_of_the_model_to_use_as_a_player(D3DXVECTOR3 *out)
{
	golem_get_position_special(out);
}

