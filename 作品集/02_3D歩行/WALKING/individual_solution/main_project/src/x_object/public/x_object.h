#pragma once

#include <time.h>
#include "..\..\x_file\public\x_file_type.h"
#include "..\..\common_def\public\common_def.h"
#include "..\..\x_object_base\public\x_object_base_structure.h"


//ゲーム内の物体などを司る型です
//これを汎用ゲームオブジェクト型と呼ぶことにします
struct x_object_t
{
	//type var;		//付随する情報
	x_object_base_t *_base;					//位置+回転+拡大縮小+メッシュ
};


//汎用ゲームオブジェクト型の使用開始
extern void x_object_init(IDirect3DDevice9 *device);

//汎用ゲームオブジェクト型の使用終了
extern void x_object_end();

//初期化する
void x_object_t_init(x_object_t **object,
	x_file_type_t type,
	const vector3_t *position,
	const vector3_t *angle,
	const vector3_t *scale
);

//解放する
void x_object_t_end(x_object_t **object);

//描画する
void x_object_t_draw(const x_object_t *object, IDirect3DDevice9 *device);




