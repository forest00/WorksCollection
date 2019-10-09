#pragma once
/*
*@file glitter_manager.h
*@brief きらきらエフェクトを管理する
*@auther 森　仁志
*/

//DirectXライブラリ
#include <d3d9.h>
#include <d3dx9.h>
//イージングタイプ
#include "..\..\..\..\easing\easing_type.h"

//きらきらエフェクトの種別
enum glitter_kind_t
{
	gk_none,
	gk_orange,
	gk_purple,
	gk_ring,
	gk_green,
	gk_blue,
	gk_white,
	gk_red,
	gk_max,
};

//きらきらエフェクト追加用構造体
struct glitter_add_t
{
	glitter_kind_t _kind;					//種類
	float _life_sec;						//寿命(秒)
	float _scale_base;						//拡大率
	easing_type_t _easing_type;				//イージングタイプ
	D3DXVECTOR3 _start_position;			//開始位置
	D3DXVECTOR3 _end_position;				//終了位置
	float _red;								//赤成分(0~1)
	float _green;							//緑成分(0~1)
	float _blue;							//青成分(0~1)
};

/*
*@fn glitter_manager_init
*@brief きらきらエフェクトを初期化する
*/
extern void glitter_manager_init();

/*
*@fn glitter_manager_end
*@brief　きらきらエフェクトを終了する
*/
extern void glitter_manager_end();

/*
*@fn
*@brief　きらきらエフェクトを更新する
*/
extern void glitter_manager_update();

/*
*@fn glitter_manager_draw
*@brief　きらきらエフェクトを描画する
*/
extern void glitter_manager_draw();

/*
*@fn glitter_manager_add
*@brief　きらきらエフェクトを追加する
*@detail きらきらエフェクトは時間経過で勝手に消えます
*/
extern void glitter_manager_add(const glitter_add_t *info);

/*
*@fn glitter_manager_random
*@brief ランダムな値を取得
*/
extern glitter_kind_t gk_random();
