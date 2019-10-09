#pragma once
//FBXファイルを読み込んで描画し、破棄するところです
//作成者　森仁志


#include <d3dx9.h>
#include "..\amlib\ExFbxMeshAmg.h"

//実際に読み込むときだけ使うインデックス
enum fbx_read_index_t
{
	fri_player,
	fri_stage1,
	fri_stage2,
	fri_stage3,
	//fri_stage4,
	fri_sky_sphere,
	fri_wrench_and_bibibi,
	fri_max,
};

//テーブル上の特定のFBXを指定するID
enum fbx_table_id_t
{
	fti_none,							//無し
	fti_player,							//プレーヤー
	fti_stage1_base,					//ステージ1の見た目用オブジェクト
	fti_stage1_magobj,					//ステージ1の磁石あたり判定用オブジェクト
	fti_stage1_nomagobj,				//ステージ1の地面あたり判定用オブジェクト
	fti_stage2_base,					//ステージ2の見た目用オブジェクト
	fti_stage2_magobj,					//ステージ2の磁石あたり判定用オブジェクト
	fti_stage2_nomagobj,				//ステージ2の地面あたり判定用オブジェクト
	fti_stage3_base,					//ステージ3の見た目用オブジェクト
	fti_stage3_magobj,					//ステージ3の磁石あたり判定用オブジェクト
	fti_stage3_nomagobj,				//ステージ3の地面あたり判定用オブジェクト
	fti_stage4_base,					//ステージ4の見た目用オブジェクト
	fti_stage4_magobj,					//ステージ4の磁石あたり判定用オブジェクト
	fti_stage4_nomagobj,				//ステージ4の地面あたり判定用オブジェクト
	fti_sky,							//スカイスフィア1
	fti_sky_sphere,						//スカイスフィア2
	fti_wrench,							//レンチ
	fti_bibibi_red_3d,					//ビリビリオブジェクト
	fti_max,							//最大
};

//モーション付きFBXのモーションを指定するID
enum fbx_table_motion_id_t
{
	ftmi_none,							//無し
	ftmi_player_data_start,				//プレーヤー:モーションデータ開始
	ftmi_player_stand,					//プレーヤー:立ったまま
	ftmi_player_front_walk,				//プレーヤー:前歩き
	ftmi_player_back_walk,				//プレーヤー:後ろ歩き
	ftmi_player_side_walk1,				//プレーヤー:右歩き
	ftmi_player_side_walk2,				//プレーヤー:左歩き
	ftmi_player_run,					//プレーヤー:走り
	ftmi_player_jump,					//プレーヤー:ジャンプ
	ftmi_player_damage,					//プレーヤー:ダメージ
	ftmi_player_fall,					//プレーヤー:落ちる
	ftmi_player_round,					//プレーヤー:ぐるぐる
	ftmi_player_data_end,				//プレーヤー:モーションデータ終わり
	ftmi_max,							//最大
};

//FBXのテーブルを初期化
//初期化された段階ではまだ描画できないので、先に読み込み処理をしよう
extern void fbx_table_init();

//FBXのテーブルを終了
//この関数を呼んだあとはFBXは使えなくなるよ
extern void fbx_table_end();

//FBXを読み込む
extern void fbx_table_load(fbx_read_index_t index);

//読み込んだFBXを破棄する
extern void fbx_table_destroy();

//読み込んだFBXを描画する
extern void fbx_table_draw(fbx_table_id_t looks, fbx_table_motion_id_t motion, const D3DXMATRIX *world, float motion_frame);

//読み込んだFBXをシェーダーで描画する
extern void fbx_table_draw_with_simple_shader(
	fbx_table_id_t looks,
	fbx_table_motion_id_t motion,
	const D3DXMATRIX *world,
	float motion_frame,
	ex_fbx_mesh_amg_blend_mode_t blend_mode
);

//読み込んだFBXをシェーダーでアルファ値最大にしたうえで描画する
extern void fbx_table_draw_with_simple_shader_max_alpha(
	fbx_table_id_t looks,
	fbx_table_motion_id_t motion,
	const D3DXMATRIX *world,
	float motion_frame
);

//特定のFBXに向かってレイを飛ばす
extern void fbx_table_raycast(
	fbx_table_id_t looks,
	fbx_table_motion_id_t motion,
	const D3DXMATRIX *world,
	float motion_frame,
	const D3DXVECTOR3 *ray_position,
	const D3DXVECTOR3 *ray_vector,
	D3DXVECTOR3 *intersection,
	D3DXVECTOR3 *normal,
	int *face,
	bool *hitting,
	int *mesh_id
);

//特定のFBXに向かって太いレイを飛ばす
extern void fbx_table_sphere_cast(
	fbx_table_id_t looks,
	fbx_table_motion_id_t motion,
	const D3DXMATRIX *world,
	float motion_frame,
	const D3DXVECTOR3 *ray_position,
	const D3DXVECTOR3 *ray_vector,
	float radius,
	D3DXVECTOR3 *intersection,
	D3DXVECTOR3 *normal,
	int *face,
	bool *hitting,
	int *mesh_id
);

//指定のFBXの指定の姿勢で、指定のポリゴンの重心を得る
extern void fbx_table_get_polygon_position(
	fbx_table_id_t looks,
	fbx_table_motion_id_t motion,
	const D3DXMATRIX *world,
	float motion_frame,
	int polygon_id,
	D3DXVECTOR3 *pos
);

//指定のFBXの指定の姿勢で、指定の範囲のポリゴンの中心位置を得る
extern void fbx_table_get_polygon_position_range(
	fbx_table_id_t looks,
	fbx_table_motion_id_t motion,
	const D3DXMATRIX *world,
	float motion_frame,
	int min_polygon_id,
	int max_polygon_id,
	D3DXVECTOR3 *pos
);

//指定のFBXの指定の姿勢で、指定のメッシュの中心位置を得る
extern void fbx_table_get_mesh_position(
	fbx_table_id_t looks,
	fbx_table_motion_id_t motion,
	const D3DXMATRIX *world,
	float motion_frame,
	int mesh_id,
	D3DXVECTOR3 *pos
);

//メッシュを無視するフラグをセットする
extern void fbx_table_ignore(fbx_table_id_t looks, int mesh_id, bool do_flag);

//メッシュを無視するフラグをリセットする
extern void fbx_table_ignore_reset(fbx_table_id_t looks);

//モーション1再生にかかる時間(フレーム)を取得
extern float fbx_table_motion_frame(fbx_table_motion_id_t motion);
