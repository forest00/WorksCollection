#pragma once

#include "x_object_base_structure.h"


//モデルの使用開始
extern void x_object_base_init(IDirect3DDevice9 *device);

//モデルの使用終了
extern void x_object_base_end();


//初期化
extern void x_object_base_t_init(x_object_base_t **object,
	x_file_type_t type,
	const vector3_t *position,
	const vector3_t *angle,
	const vector3_t *scale
);

//終了
extern void x_object_base_t_end(x_object_base_t **object);

//描画(その時持っていた情報がそのまま描画される)
extern void x_object_base_t_draw(const x_object_base_t *object, IDirect3DDevice9 *device);
