#pragma once

#include <d3dx9.h>


//アニメーションモデルの種類
enum anim_model_kind_t
{
	anim_model_kind_none,			//無
	anim_model_kind_robot,			//ロボット
	anim_model_kind_ken,			//剣だすやつ
	anim_model_kind_golem,			//ゴーレム
	anim_model_kind_josh,			//joshさん
	anim_model_kind_usagi,			//ウサギさん
	anim_model_kind_block_man,		//黒いブロック男
	anim_model_kind_max,			//最大
};

//Xファイルに入っているアニメーションの情報を読み込む構造体
//実際に読み込むにはヒエラルキーも必要です
struct anim_data_t
{
	D3DXFRAME *pFrameRoot;								//?
	ID3DXAnimationSet **pAnimSet;						//?
	ID3DXAnimationController *pAnimController;			//?
};




//アニメデータを全部読み込む
extern void anim_data_load_all(IDirect3DDevice9 *device);

//読み込んだアニメデータを全部破棄
extern void anim_data_destroy_all();

//読み込んだアニメデータを取得
extern const anim_data_t *anim_data_get(anim_model_kind_t anim_kind);

//アニメーションコントローラーのクローンを生成
extern void anim_controller_clone(anim_model_kind_t anim_kind, ID3DXAnimationController **destination);

