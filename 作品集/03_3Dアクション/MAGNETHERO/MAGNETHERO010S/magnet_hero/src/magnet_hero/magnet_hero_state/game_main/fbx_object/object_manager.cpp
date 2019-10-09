//	
//	冬期制作「Magnet Hero」
//	
//	作成者	森	仁志
//	
//	最終更新		2019/01/16
//	

//親ヘッダー
#include "object_manager.h"
//その他のヘッダー
#include "..\..\..\..\mylib\mylib.h"
#include "player.h"
#include "ground.h"
#include "magnet.h"
#include "bg.h"
#include "bg_rotate.h"
#include "sky_sphare.h"
#include "..\..\..\..\shader_use\simple_shader.h"

//オブジェクトからハンドルにする
extern game_main_fbx_object_handle_t *to_handle(game_main_fbx_object_t *object)
{
	//void*型にする
	void *void_ptr = (void *)(object);
	//変換して返す
	return (game_main_fbx_object_handle_t*)(void_ptr);
}

//ハンドルからオブジェクトに戻す
extern game_main_fbx_object_t *to_object(game_main_fbx_object_handle_t *handle)
{
	//void*型にする
	void *void_ptr = (void *)(handle);
	//変換して返す
	return (game_main_fbx_object_t*)(void_ptr);
}


//単体を初期化
extern void gmfo_t_init(game_main_fbx_object_t *object, game_main_fbx_object_array_t *parrent_array, size_t index)
{
	object->_parrent_array = parrent_array;
	object->_handle = to_handle(object);
	object->_index = index;
	object->_kind = gmfok_none;
	object->_looks = fti_none;
	object->_now_motion = ftmi_none;
	object->_motion_frame = 0.0f;
	object->_kind_parameter = nullptr;
	object->_init = nullptr;
	object->_end = nullptr;
	object->_update = nullptr;
	object->_draw = nullptr;
}

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
)
{
	if (object == nullptr)
	{
		return;
	}
	object->_kind = kind;
	object->_looks = looks;
	object->_now_motion = motion_id;
	object->_motion_frame = motion_frame;
	object->_position = *position;
	object->_rotation = *rotation;
	object->_scale = *scale;
	switch (kind)
	{
	case gmfok_none:
		object->_init = nullptr;
		object->_end = nullptr;
		object->_update = nullptr;
		object->_draw = nullptr;
		break;
	case gmfok_player:
		object->_init = gmfo_t_player_init;
		object->_end = gmfo_t_player_end;
		object->_update = gmfo_t_player_update;
		object->_draw = gmfo_t_player_draw;
		break;
	case gmfok_stage_ground:
		object->_init = gmfo_t_ground_init;
		object->_end = gmfo_t_ground_end;
		object->_update = gmfo_t_ground_update;
		object->_draw = gmfo_t_ground_draw;
		break;
	case gmfok_stage_magnet:
		object->_init = gmfo_t_magnet_init;
		object->_end = gmfo_t_magnet_end;
		object->_update = gmfo_t_magnet_update;
		object->_draw = gmfo_t_magnet_draw;
		break;
	case gmfok_bg:
		object->_init = gmfo_t_stage_bg_init;
		object->_end = gmfo_t_stage_bg_end;
		object->_update = gmfo_t_stage_bg_update;
		object->_draw = gmfo_t_stage_bg_draw;
		break;
	case gmfok_bg_rotate:
		object->_init = gmfo_t_stage_bg_rotate_init;
		object->_end = gmfo_t_stage_bg_rotate_end;
		object->_update = gmfo_t_stage_bg_rotate_update;
		object->_draw = gmfo_t_stage_bg_rotate_draw;
		break;
	case gmfok_sky_sphere:
		object->_init = gmfo_t_sky_sphere_init;
		object->_end = gmfo_t_sky_sphere_end;
		object->_update = gmfo_t_sky_sphere_update;
		object->_draw = gmfo_t_sky_sphere_draw;
		break;
	case gmfok_max:
		object->_init = nullptr;
		object->_end = nullptr;
		object->_update = nullptr;
		object->_draw = nullptr;
		break;
	default:
		object->_init = nullptr;
		object->_end = nullptr;
		object->_update = nullptr;
		object->_draw = nullptr;
		break;
	}
	if (object->_init)
	{
		object->_init(object);
	}
}

//単体を終了
extern void gmfo_t_end(game_main_fbx_object_t *object)
{
	if (object->_end)
	{
		object->_end(object);
	}
}

//単体を更新
extern void gmfo_t_update(game_main_fbx_object_t *object)
{
	if (object->_update)
	{
		object->_update(object);
	}
}

//単体を描画
extern void gmfo_t_draw(game_main_fbx_object_t *object)
{
	if (object->_draw)
	{
		object->_draw(object);
	}
}

//ハンドルを取得
extern void gmfo_t_get_handle(const game_main_fbx_object_t *object, game_main_fbx_object_handle_t **getter)
{
	if (getter)
	{
		*getter = object->_handle;
	}
}





//配列の初期化
extern void gmfoa_t_init(game_main_fbx_object_array_t *object_array, size_t capacity)
{
	object_array->_capacity = capacity;
	object_array->_elements = (game_main_fbx_object_t*)malloc(sizeof(game_main_fbx_object_t) * capacity);
	for (size_t i = 0; i < object_array->_capacity; i++)
	{
		gmfo_t_init(&object_array->_elements[i], object_array, i);
	}
}

//配列の解放
extern void gmfoa_t_end(game_main_fbx_object_array_t *object_array)
{
	for (size_t i = 0; i < object_array->_capacity; i++)
	{
		gmfo_t_end(&object_array->_elements[i]);
	}
	_mylib_safe_free(object_array->_elements);
}

//配列の中身を全部更新
extern void gmfoa_t_update(game_main_fbx_object_array_t *object_array)
{
	for (size_t i = 0; i < object_array->_capacity; i++)
	{
		gmfo_t_update(&object_array->_elements[i]);
	}
}

//配列の中身を全部描画
extern void gmfoa_t_draw(game_main_fbx_object_array_t *object_array)
{
	mylib_set_lighting(false);
	mylib_set_blend_mode(mbm_factor);
	for (size_t i = 0; i < object_array->_capacity; i++)
	{
		gmfo_t_draw(&object_array->_elements[i]);
	}
}

//配列の要素追加
extern void gmfoa_t_add(game_main_fbx_object_array_t *object_array, const game_main_fbx_object_t *obj, game_main_fbx_object_handle_t **added)
{
	game_main_fbx_object_handle_t *space;
	gmfoa_t_search_kind(object_array, gmfok_none, nullptr, &space);
	game_main_fbx_object_t *object = to_object(space);
	game_main_fbx_object_array_t *parrent_array = object->_parrent_array;
	game_main_fbx_object_handle_t *handle = object->_handle;
	size_t index = object->_index;
	*object = *obj;
	object->_parrent_array = parrent_array;
	object->_handle = handle;
	object->_index = index;
	if (added)
	{
		*added = space;
	}
}

//配列の要素削除
extern void gmfoa_t_delete(game_main_fbx_object_array_t *object_array, game_main_fbx_object_handle_t **handle)
{
	if (handle == nullptr)
	{
		return;
	}
	game_main_fbx_object_t *object = to_object(*handle);
	if (object == nullptr)
	{
		return;
	}
	if (object->_parrent_array != object_array)
	{
		return;
	}
	gmfo_t_end(object);
	gmfo_t_init(object, object_array, object->_index);
	*handle = nullptr;
}

//配列の要素全削除
extern void gmfoa_t_clear(game_main_fbx_object_array_t *object_array)
{
	for (size_t i = 0; i < object_array->_capacity; i++)
	{
		gmfo_t_end(&object_array->_elements[i]);
		gmfo_t_init(&object_array->_elements[i], object_array, i);
	}
}



/*配列の中から指定された種類のオブジェクトを探してハンドルを取得
**@param object_array		対象の配列
**@param kind				対象の種類
**@param offset				検索を開始する位置までのオフセット、nullを指定するとすべての要素の中から検索する
**@param target				見つけたハンドルが返される、見つからなければnullがセットされる
**@return 見つかった場合、true
*/
extern bool gmfoa_t_search_kind(game_main_fbx_object_array_t *object_array, game_main_fbx_object_kind_t kind, game_main_fbx_object_t *offset, game_main_fbx_object_handle_t **target)
{
	if (target == nullptr)
	{
		return false;
	}
	{
		size_t i_start = 0;
		if (offset)
		{
			i_start = offset->_index;
		}
		for (size_t i = i_start; i < object_array->_capacity; i++)
		{
			if (object_array->_elements[i]._kind == kind)
			{
				*target = object_array->_elements[i]._handle;
				return true;
			}
		}
	}
	*target = nullptr;
	return false;
}


//オブジェクトの位置を取得
extern void gmfoh_t_get_position(game_main_fbx_object_handle_t *handle, D3DXVECTOR3 *position)
{
	if (position == nullptr)
	{
		return;
	}
	game_main_fbx_object_t *object = to_object(handle);
	if (object == nullptr)
	{
		return;
	}
	*position = object->_position;
}

//オブジェクトの角度を取得
extern void gmfoh_t_get_rotation(game_main_fbx_object_handle_t *handle, D3DXVECTOR3 *rotation)
{
	if (rotation == nullptr)
	{
		return;
	}
	game_main_fbx_object_t *object = to_object(handle);
	if (object == nullptr)
	{
		return;
	}
	*rotation = object->_rotation;
}

//オブジェクトの拡大倍率を取得
extern void gmfoh_t_get_scale(game_main_fbx_object_handle_t *handle, D3DXVECTOR3 *scale)
{
	if (scale == nullptr)
	{
		return;
	}
	game_main_fbx_object_t *object = to_object(handle);
	if (object == nullptr)
	{
		return;
	}
	*scale = object->_scale;
}

//オブジェクトの位置、角度、拡大倍率によりワールド行列を生成
extern void gmfoh_t_get_world(game_main_fbx_object_handle_t *handle, D3DXMATRIX *world, D3DXMATRIX *inverse_world)
{
	const game_main_fbx_object_t *object = to_object(handle);
	if (object == nullptr)
	{
		return;
	}
	if (world)
	{
		mylib_world_transform(world, &object->_position, &object->_rotation, &object->_scale);
		if (inverse_world)
		{
			D3DXMatrixInverse(inverse_world, nullptr, world);
		}
	}
}


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
)
{
	if (ray_position == nullptr || ray_vector == nullptr || intersection == nullptr || normal == nullptr)
	{
		return;
	}
	game_main_fbx_object_t *object = to_object(handle);
	if (object == nullptr)
	{
		return;
	}
	D3DXMATRIX world;
	gmfoh_t_get_world(handle, &world, nullptr);
	fbx_table_raycast(object->_looks, object->_now_motion, &world, object->_motion_frame, ray_position, ray_vector, intersection, normal, face, hitting, mesh_id);
}

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
)
{
	if (ray_position == nullptr || ray_vector == nullptr || intersection == nullptr || normal == nullptr)
	{
		return;
	}
	game_main_fbx_object_t *object = to_object(handle);
	if (object == nullptr)
	{
		return;
	}
	D3DXMATRIX world;
	gmfoh_t_get_world(handle, &world, nullptr);
	fbx_table_sphere_cast(object->_looks, object->_now_motion, &world, object->_motion_frame, ray_position, ray_vector, radius, intersection, normal, face, hitting, mesh_id);
}

//オブジェクトにメッシュを無視するフラグをセット
extern void gmfoh_t_set_ignore_flag(game_main_fbx_object_handle_t *handle, int mesh_id, bool value)
{
	game_main_fbx_object_t *object = to_object(handle);
	fbx_table_ignore(object->_looks, mesh_id, value);
}