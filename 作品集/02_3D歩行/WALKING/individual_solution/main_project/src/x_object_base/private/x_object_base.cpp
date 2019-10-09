
//親ヘッダー
#include "..\public\x_object_base.h"
//その他のヘッダー
#include <assert.h>
#include "..\..\world_transformation\public\world_transformation.h"





//モデルの使用開始
extern void x_object_base_init(IDirect3DDevice9 *device)
{
	x_file_init(device);
}

//モデルの使用終了
extern void x_object_base_end()
{
	x_file_end();
}


//初期化
//初期化
extern void x_object_base_t_init(
	x_object_base_t **object,
	x_file_type_t type,
	const vector3_t *position,
	const vector3_t *angle,
	const vector3_t *scale
)
{
	assert(object);
	assert(position);
	assert(angle);
	assert(scale);
	*object = (x_object_base_t*)malloc(sizeof(x_object_base_t));
	assert(*object);
	(*object)->_position = *position;
	(*object)->_angle = *angle;
	(*object)->_scale = *scale;
	x_file_t_load_from_database(&(*object)->_x_file, type);
}

//終了
extern void x_object_base_t_end(x_object_base_t **object)
{
	safe_free(*object);
}

//描画(その時持っていた情報がそのまま描画される)
extern void x_object_base_t_draw(const x_object_base_t *object, IDirect3DDevice9 *device)
{
	matrix4x4 world;
	world_transformation(
		&world,
		&object->_position,
		&object->_angle,
		&object->_scale
	);
	device->SetTransform(D3DTS_WORLD, &world);
	x_file_t_draw(device, &object->_x_file);
}

