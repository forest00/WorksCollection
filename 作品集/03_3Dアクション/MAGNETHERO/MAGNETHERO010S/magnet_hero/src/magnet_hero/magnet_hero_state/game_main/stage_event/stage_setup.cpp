//親ヘッダー
#include "stage_setup.h"
//その他のヘッダー
#include "stage_data.h"
#include "..\camera\camera_manager.h"
#include "..\connecter\info_holder.h"
#include "..\stage_event\stage_event.h"



//stage1
#if 1
//ステージ1のセットアップをする
static void stage1_setup(game_main_fbx_object_array_t *obj_array)
{
	//FBXを読み込み
	fbx_table_load(fri_stage1);
	//一時的にクリエイトするキャラたちの受け取り皿を用意
	game_main_fbx_object_t player;
	game_main_fbx_object_t stage_base;
	game_main_fbx_object_t stage_magobj;
	game_main_fbx_object_t stage_nomagobj;
	game_main_fbx_object_t sky_sphere;
	//プレーヤーをクリエイト
	gmfo_t_create(&player,
		gmfok_player, fti_player, ftmi_player_stand,
		0.0f,
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.1f, 0.1f, 0.1f)
	);
	//見た目用オブジェクトをクリエイト
	gmfo_t_create(&stage_base,
		gmfok_bg, fti_stage1_base, ftmi_none,
		0.0f,
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.1f, 0.1f, 0.1f)
	);
	//磁石のあたり判定用オブジェクトをクリエイト
	gmfo_t_create(&stage_magobj,
		gmfok_stage_magnet, fti_stage1_magobj, ftmi_none,
		0.0f,
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.1f, 0.1f, 0.1f)
	);
	//地面のあたり判定用オブジェクトをクリエイト
	gmfo_t_create(&stage_nomagobj,
		gmfok_stage_ground, fti_stage1_nomagobj, ftmi_none,
		0.0f,
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.1f, 0.1f, 0.1f)
	);
	//スカイスフィアをクリエイト
	gmfo_t_create(&sky_sphere,
		gmfok_sky_sphere, fti_sky_sphere, ftmi_none,
		0.0f,
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.05f, 0.05f, 0.05f)
	);
	//クリエイトしたキャラを使う
	gmfoa_t_add(obj_array, &player, nullptr);
	gmfoa_t_add(obj_array, &stage_base, nullptr);
	gmfoa_t_add(obj_array, &stage_magobj, nullptr);
	gmfoa_t_add(obj_array, &stage_nomagobj, nullptr);
	gmfoa_t_add(obj_array, &sky_sphere, nullptr);
}

//ステージ1のリセットをする(2019/02/05現在は何もしない)
extern void stage1_reset()
{
	//fbx_table_ignore_reset(fti_stage1_base);
}

//ステージ1に使われているオブジェクトをすべて破棄する
static void stage1_destroy(game_main_fbx_object_array_t *obj_array)
{
	gmfoa_t_clear(obj_array);
}
#endif

//stage2
#if 1
//ステージ2のセットアップをする
static void stage2_setup(game_main_fbx_object_array_t *obj_array)
{
	//FBXを読み込み
	fbx_table_load(fri_stage2);
	//一時的にクリエイトするキャラたちの受け取り皿を用意
	game_main_fbx_object_t player;
	game_main_fbx_object_t stage_base;
	game_main_fbx_object_t stage_magobj;
	game_main_fbx_object_t stage_nomagobj;
	game_main_fbx_object_t sky_sphere;
	//プレーヤーをクリエイト
	gmfo_t_create(&player,
		gmfok_player, fti_player, ftmi_player_stand,
		0.0f,
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.1f, 0.1f, 0.1f)
	);
	//見た目用オブジェクトをクリエイト
	gmfo_t_create(&stage_base,
		gmfok_bg, fti_stage2_base, ftmi_none,
		0.0f,
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.1f, 0.1f, 0.1f)
	);
	//磁石のあたり判定用オブジェクトをクリエイト
	gmfo_t_create(&stage_magobj,
		gmfok_stage_magnet, fti_stage2_magobj, ftmi_none,
		0.0f,
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.1f, 0.1f, 0.1f)
	);
	//地面のあたり判定用オブジェクトをクリエイト
	gmfo_t_create(&stage_nomagobj,
		gmfok_stage_ground, fti_stage2_nomagobj, ftmi_none,
		0.0f,
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.1f, 0.1f, 0.1f)
	);
	//スカイスフィアをクリエイト
	gmfo_t_create(&sky_sphere,
		gmfok_sky_sphere, fti_sky_sphere, ftmi_none,
		0.0f,
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.05f, 0.05f, 0.05f)
	);
	//クリエイトしたキャラを使う
	gmfoa_t_add(obj_array, &player, nullptr);
	gmfoa_t_add(obj_array, &stage_base, nullptr);
	gmfoa_t_add(obj_array, &stage_magobj, nullptr);
	gmfoa_t_add(obj_array, &stage_nomagobj, nullptr);
	gmfoa_t_add(obj_array, &sky_sphere, nullptr);
}

//ステージ2のリセットをする(2019/02/05現在は何もしない)
extern void stage2_reset()
{
	//fbx_table_ignore_reset(fti_stage2_base);
}

//ステージ2に使われているオブジェクトをすべて破棄する
static void stage2_destroy(game_main_fbx_object_array_t *obj_array)
{
	gmfoa_t_clear(obj_array);
}
#endif


//stage3
#if 1
//ステージ3のセットアップをする
static void stage3_setup(game_main_fbx_object_array_t *obj_array)
{
	//FBXを読み込み
	fbx_table_load(fri_stage3);
	//一時的にクリエイトするキャラたちの受け取り皿を用意
	game_main_fbx_object_t player;
	game_main_fbx_object_t stage_base;
	game_main_fbx_object_t stage_magobj;
	game_main_fbx_object_t stage_nomagobj;
	game_main_fbx_object_t sky_sphere;
	//プレーヤーをクリエイト
	gmfo_t_create(&player,
		gmfok_player, fti_player, ftmi_player_stand,
		0.0f,
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.1f, 0.1f, 0.1f)
	);
	//見た目用オブジェクトをクリエイト
	gmfo_t_create(&stage_base,
		gmfok_bg, fti_stage3_base, ftmi_none,
		0.0f,
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.1f, 0.1f, 0.1f)
	);
	//磁石のあたり判定用オブジェクトをクリエイト
	gmfo_t_create(&stage_magobj,
		gmfok_stage_magnet, fti_stage3_magobj, ftmi_none,
		0.0f,
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.1f, 0.1f, 0.1f)
	);
	//地面のあたり判定用オブジェクトをクリエイト
	gmfo_t_create(&stage_nomagobj,
		gmfok_stage_ground, fti_stage3_nomagobj, ftmi_none,
		0.0f,
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.1f, 0.1f, 0.1f)
	);
	//スカイスフィアをクリエイト
	gmfo_t_create(&sky_sphere,
		gmfok_sky_sphere, fti_sky_sphere, ftmi_none,
		0.0f,
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.06f, 0.06f, 0.06f)
	);
	//クリエイトしたキャラを使う
	gmfoa_t_add(obj_array, &player, nullptr);
	gmfoa_t_add(obj_array, &stage_base, nullptr);
	gmfoa_t_add(obj_array, &stage_magobj, nullptr);
	gmfoa_t_add(obj_array, &stage_nomagobj, nullptr);
	gmfoa_t_add(obj_array, &sky_sphere, nullptr);
}

//ステージ3のリセットをする(2019/02/05現在は何もしない)
extern void stage3_reset()
{
	//fbx_table_ignore_reset(fti_stage3_base);
}

//ステージ3に使われているオブジェクトをすべて破棄する
static void stage3_destroy(game_main_fbx_object_array_t *obj_array)
{
	gmfoa_t_clear(obj_array);
}
#endif


//stage4
#if 0
//ステージ4のセットアップをする
static void stage4_setup(game_main_fbx_object_array_t *obj_array)
{
	//FBXを読み込み
	fbx_table_load(fri_stage4);
	//一時的にクリエイトするキャラたちの受け取り皿を用意
	game_main_fbx_object_t player;
	game_main_fbx_object_t stage_base;
	game_main_fbx_object_t stage_magobj;
	game_main_fbx_object_t stage_nomagobj;
	game_main_fbx_object_t sky_sphere;
	//プレーヤーをクリエイト
	gmfo_t_create(&player,
		gmfok_player, fti_player, ftmi_player_stand,
		0.0f,
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.1f, 0.1f, 0.1f)
	);
	//見た目用オブジェクトをクリエイト
	gmfo_t_create(&stage_base,
		gmfok_bg, fti_stage4_base, ftmi_none,
		0.0f,
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.1f, 0.1f, 0.1f)
	);
	//磁石のあたり判定用オブジェクトをクリエイト
	gmfo_t_create(&stage_magobj,
		gmfok_stage_magnet, fti_stage4_magobj, ftmi_none,
		0.0f,
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.1f, 0.1f, 0.1f)
	);
	//地面のあたり判定用オブジェクトをクリエイト
	gmfo_t_create(&stage_nomagobj,
		gmfok_stage_ground, fti_stage4_nomagobj, ftmi_none,
		0.0f,
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.1f, 0.1f, 0.1f)
	);
	//スカイスフィアをクリエイト
	gmfo_t_create(&sky_sphere,
		gmfok_sky_sphere, fti_sky_sphere, ftmi_none,
		0.0f,
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.05f, 0.05f, 0.05f)
	);
	//クリエイトしたキャラを使う
	gmfoa_t_add(obj_array, &player, nullptr);
	gmfoa_t_add(obj_array, &stage_base, nullptr);
	gmfoa_t_add(obj_array, &stage_magobj, nullptr);
	gmfoa_t_add(obj_array, &stage_nomagobj, nullptr);
	gmfoa_t_add(obj_array, &sky_sphere, nullptr);
}

//ステージ4のリセットをする(2019/02/05現在は何もしない)
extern void stage4_reset()
{
	//fbx_table_ignore_reset(fti_stage_base);
}

//ステージ4に使われているオブジェクトをすべて破棄する
static void stage4_destroy(game_main_fbx_object_array_t *obj_array)
{
	gmfoa_t_clear(obj_array);
}
#endif



//N番目のステージのセットアップを行う関数です
//初期化済みのカメラと、初期化済みのFBXオブジェクト配列管理用構造体が必要です
extern void stage_setup(stage_index_t n, game_main_fbx_object_array_t *obj_array)
{
	switch (n)
	{
	case stage_none:
		break;
	case stage_1:
		stage1_setup(obj_array);
		break;
	case stage_2:
		stage2_setup(obj_array);
		break;
	case stage_3:
		stage3_setup(obj_array);
		break;
	//case stage_4:
	//	stage4_setup(obj_array);
		break;
	case stage_max:
		break;
	default:
		break;
	}
	stage_event_stage_start();
	float start_camera_phi;
	float start_camera_theta;
	stage_data_get_start_camera_angle(n, &start_camera_phi, &start_camera_theta);
	camera_manager_set_phi(start_camera_phi);
	camera_manager_set_theta(start_camera_theta);
}

//N番目のステージをリセットする関数です
extern void stage_reset(stage_index_t n)
{
	switch (n)
	{
	case stage_none:
		break;
	case stage_1:
		stage1_reset();
		break;
	case stage_2:
		stage2_reset();
		break;
	case stage_3:
		stage3_reset();
		break;
	//case stage_4:
	//	stage4_reset();
		break;
	case stage_max:
		break;
	default:
		break;
	}
	float start_camera_phi;
	float start_camera_theta;
	stage_data_get_start_camera_angle(n, &start_camera_phi, &start_camera_theta);
	camera_manager_set_phi(start_camera_phi);
	camera_manager_set_theta(start_camera_theta);
}

//N番目のステージを破棄する関数です
extern void stage_destroy(stage_index_t n, game_main_fbx_object_array_t *obj_array)
{
	switch (n)
	{
	case stage_none:
		break;
	case stage_1:
		stage1_destroy(obj_array);
		break;
	case stage_2:
		stage2_destroy(obj_array);
		break;
	case stage_3:
		stage3_destroy(obj_array);
		break;
	//case stage_4:
	//	stage4_destroy(obj_array);
		break;
	case stage_max:
		break;
	default:
		break;
	}
}
