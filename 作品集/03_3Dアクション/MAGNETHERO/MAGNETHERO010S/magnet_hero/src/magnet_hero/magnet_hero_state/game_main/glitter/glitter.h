#pragma once
/*
*@file glitter.h
*@brief きらきらエフェクト単体を操作する
*@auther 森　仁志
*/

//DirectXライブラリ
#include <d3d9.h>
#include <d3dx9.h>
//イージングタイプ
#include "..\..\..\..\easing\easing_type.h"
//テクスチャID
#include "..\..\..\..\texture_manager\texuture_id.h"

//きらきら構造体
struct glitter_t
{
	float _spawn_sec;						//生まれた瞬間(秒)
	float _life_sec;						//寿命(秒)
	float _scale_base;						//拡大率
	easing_type_t _easing_type;				//イージングタイプ
	texture_id_t _texture_id;				//テクスチャID
	D3DXVECTOR3 _start_position;			//開始位置
	D3DXVECTOR3 _end_position;				//終了位置
	float _red;								//赤成分(0~1)
	float _green;							//緑成分(0~1)
	float _blue;							//青成分(0~1)
};

//きらきら構造体初期化用構造体
struct glitter_initializer_t
{
	float _life_sec;						//寿命(秒)
	float _scale_base;						//拡大率
	easing_type_t _easing_type;				//イージングタイプ
	texture_id_t _texture_id;				//テクスチャID
	D3DXVECTOR3 _start_position;			//開始位置
	D3DXVECTOR3 _end_position;				//終了位置
	float _red;								//赤成分(0~1)
	float _green;							//緑成分(0~1)
	float _blue;							//青成分(0~1)
};

/*
*@fn glitter_t_init_with_alloc
*@brief きらきらエフェクトをメモリ確保した上で初期化する初期化する
*@detail 内部でメモリ確保しているので外部でglitter_t_end_with_free()を使って終了処理をしなければならない
*/
extern void glitter_t_init_with_alloc(glitter_t **obj, const glitter_initializer_t *initializer);

/*
*@fn glitter_t_init
*@brief きらきらエフェクトを初期化する
*/
extern void glitter_t_init(glitter_t *obj, const glitter_initializer_t *initializer);

/*
*@fn glitter_t_end_with_free
*@brief　きらきらエフェクト終了処理をした上でメモリ解放する
*@detail この関数で終了処理をしたら、それ以降はどの操作もしてはいけない
*/
extern void glitter_t_end_with_free(glitter_t **obj);

/*
*@fn glitter_t_end
*@brief　きらきらエフェクト終了処理をする
*/
extern void glitter_t_end(glitter_t *obj);

/*
*@fn glitter_t_update
*@brief　きらきらエフェクトを更新する
*/
extern void glitter_t_update(glitter_t *obj, bool *should_end);

/*
*@fn glitter_t_draw
*@brief　きらきらエフェクトを描画する
*/
extern void glitter_t_draw(const glitter_t *obj);

/*
*@fn glitter_t_elapsed_sec
*@brief　きらきらエフェクトの経過時間を取得する
*/
extern void glitter_t_elapsed_sec(const glitter_t *obj, float *elapsed_sec);

/*
*@fn glitter_t_now_position
*@brief　きらきらエフェクトの現在位置を取得する
*/
extern void glitter_t_now_position(const glitter_t *obj, D3DXVECTOR3 *now_position);

/*
*@fn glitter_t_alpha
*@brief きらきらエフェクトの透明度を得る(0~255)
*/
extern void glitter_t_alpha(const glitter_t *obj, int *alpha);
