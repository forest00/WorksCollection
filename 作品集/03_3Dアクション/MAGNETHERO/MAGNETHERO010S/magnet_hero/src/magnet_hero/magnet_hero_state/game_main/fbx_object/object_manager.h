#pragma once
//	
//	冬期制作「Magnet Hero」
//	
//	FBXオブジェクト
//	
//	先にDirect3Dの初期化をしておく必要があります
//	
//	作成者	森	仁志
//	
//	最終更新		2019/01/16
//	


#include <d3d9.h>
#include <d3dx9.h>
#include <time.h>
#include "..\..\..\..\amlib\FbxMeshAmg.h"
#include "..\..\..\..\fbx_table\fbx_table.h"

//特定のオブジェクトを指し示すためのハンドル
struct game_main_fbx_object_handle_t;

//オブジェクトの配列
struct game_main_fbx_object_array_t;

//オブジェクトの種類
enum game_main_fbx_object_kind_t
{
	gmfok_none,						//無し
	gmfok_player,					//プレーヤー
	gmfok_stage_ground,				//地面
	gmfok_stage_magnet,				//磁力でくっつく物体
	gmfok_bg,						//あたり判定のない物体
	gmfok_bg_rotate,				//あたり判定のない物体(回転する)
	gmfok_sky_sphere,				//スカイスフィア
	gmfok_max,						//最大
};

//オブジェクト
struct game_main_fbx_object_t
{
	/*配列の要素として必要な情報*/
	game_main_fbx_object_array_t *_parrent_array;											//自分が属している配列
	game_main_fbx_object_handle_t *_handle;													//自分を指し示すハンドル
	size_t _index;																			//自分の要素番号
	/*オブジェクトとしての情報*/
	game_main_fbx_object_kind_t _kind;														//自分の種類
	fbx_table_id_t _looks;																	//自分の見た目(どのFBXを使うか)
	fbx_table_motion_id_t _now_motion;														//現在のモーション
	float _motion_frame;																	//現在のモーション経過時間(フレーム)
	D3DXVECTOR3 _position;																	//位置
	D3DXVECTOR3 _rotation;																	//角度
	D3DXVECTOR3 _scale;																		//拡大倍率
	void *_kind_parameter;																	//種類ごとのパラメーター
	/*各操作に使う関数*/
	void(*_init)(game_main_fbx_object_t *object);											//初期化する関数
	void(*_end)(game_main_fbx_object_t *object);											//終了処理をする関数
	void(*_update)(game_main_fbx_object_t *object);											//更新する関数
	void(*_draw)(const game_main_fbx_object_t *object);										//描画する関数
};

//オブジェクトの配列
struct game_main_fbx_object_array_t
{
	size_t _capacity;								//限界数
	game_main_fbx_object_t *_elements;				//要素
};


//オブジェクトからハンドルにする
extern game_main_fbx_object_handle_t *to_handle(game_main_fbx_object_t *object);

//ハンドルからオブジェクトに戻す
extern game_main_fbx_object_t *to_object(game_main_fbx_object_handle_t *handle);

//単体を初期化
extern void gmfo_t_init(game_main_fbx_object_t *object, game_main_fbx_object_array_t *parrent_array, size_t index);

//単体を生成(生成といってもメモリ確保を行うものではなく単純に代入のみをする)
extern void gmfo_t_create(
	game_main_fbx_object_t *object,
	game_main_fbx_object_kind_t kind,
	fbx_table_id_t looks,
	fbx_table_motion_id_t motion_id,
	float motion_frame,
	const D3DXVECTOR3 *position,
	const D3DXVECTOR3 *rotation,
	const D3DXVECTOR3 *scale
);

//単体を終了
extern void gmfo_t_end(game_main_fbx_object_t *object);

//単体を更新
extern void gmfo_t_update(game_main_fbx_object_t *object);

//単体を描画
extern void gmfo_t_draw(game_main_fbx_object_t *object);

//ハンドルを取得
extern void gmfo_t_get_handle(const game_main_fbx_object_t *object, game_main_fbx_object_handle_t **getter);




//配列の初期化
extern void gmfoa_t_init(game_main_fbx_object_array_t *object_array, size_t capacity);

//配列の解放
extern void gmfoa_t_end(game_main_fbx_object_array_t *object_array);

//配列の中身を全部更新
extern void gmfoa_t_update(game_main_fbx_object_array_t *object_array);

//配列の中身を全部描画
extern void gmfoa_t_draw(game_main_fbx_object_array_t *object_array);

//配列の要素追加
extern void gmfoa_t_add(game_main_fbx_object_array_t *object_array, const game_main_fbx_object_t *obj, game_main_fbx_object_handle_t **added);

//配列の要素削除
extern void gmfoa_t_delete(game_main_fbx_object_array_t *object_array, game_main_fbx_object_handle_t **handle);

//配列の要素全削除
extern void gmfoa_t_clear(game_main_fbx_object_array_t *object_array);

/*配列の中から指定された種類のオブジェクトを探してハンドルを取得
**@param object_array		対象の配列
**@param kind				対象の種類
**@param offset				検索を開始する位置までのオフセット、nullを指定するとすべての要素の中から検索する
**@param target				見つけたハンドルが返される、見つからなければnullがセットされる
**@return 見つかった場合、true
*/
extern bool gmfoa_t_search_kind(game_main_fbx_object_array_t *object_array, game_main_fbx_object_kind_t kind, game_main_fbx_object_t *offset, game_main_fbx_object_handle_t **target);

//オブジェクトの位置を取得
extern void gmfoh_t_get_position(game_main_fbx_object_handle_t *handle, D3DXVECTOR3 *position);

//オブジェクトの角度を取得
extern void gmfoh_t_get_rotation(game_main_fbx_object_handle_t *handle, D3DXVECTOR3 *rotation);

//オブジェクトの拡大倍率を取得
extern void gmfoh_t_get_scale(game_main_fbx_object_handle_t *handle, D3DXVECTOR3 *scale);

//オブジェクトの位置、角度、拡大倍率によりワールド行列を生成
//inverse_worldがnullじゃない場合、生成したワールド行列の逆行列が入る
//transpose_inverse_worldがnullじゃない場合、生成したワールド行列の逆行列の転置行列が入る
extern void gmfoh_t_get_world(game_main_fbx_object_handle_t *handle, D3DXMATRIX *world, D3DXMATRIX *inverse_world);


//オブジェクトにレイを当てる
extern void gmfoh_t_raycast(
	game_main_fbx_object_handle_t *handle,
	const D3DXVECTOR3 *ray_position,
	const D3DXVECTOR3 *ray_vector,
	D3DXVECTOR3 *intersection,
	D3DXVECTOR3 *normal,
	int *face,
	bool *hitting,
	int *mesh_id
);

//オブジェクトに太いレイを当てる
extern void gmfoh_t_sphere_cast(
	game_main_fbx_object_handle_t *handle,
	const D3DXVECTOR3 *ray_position,
	const D3DXVECTOR3 *ray_vector,
	float radius,
	D3DXVECTOR3 *intersection,
	D3DXVECTOR3 *normal,
	int *face,
	bool *hitting,
	int *mesh_id
);

//オブジェクトにメッシュを無視するフラグをセット
extern void gmfoh_t_set_ignore_flag(game_main_fbx_object_handle_t *handle, int mesh_id, bool value);
