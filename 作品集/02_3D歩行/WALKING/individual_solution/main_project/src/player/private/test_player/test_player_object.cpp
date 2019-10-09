
//親ヘッダー
#include "test_player_object.h"
//その他のヘッダー
#include <assert.h>
#include "..\..\..\math_with_defines\math_with_defines.h"
#include "test_player_brain.h"
#include "..\..\..\x_object\public\x_object.h"
#include "..\..\..\sound\public\sound.h"
#include "..\..\..\input\public\input.h"
#include "..\..\..\camera\public\camera.h"



struct test_player_jump_t
{
	bool _doing;
	double _move_y;
};

struct test_player_object_t
{
	x_object_t *_object;								//基本情報
	test_player_brain_t _brain;							//自分の脳
	const test_player_database_t *_database;			//データベース
	test_player_jump_t _jump;							//ジャンプ情報
};

extern test_player_object_t *test_player_object_t_init(const test_player_initializer_t *initializer, const test_player_database_t *database)
{
	assert(initializer);
	assert(database);
	test_player_object_t *object = (test_player_object_t*)malloc(sizeof(test_player_object_t));
	assert(object);


	x_object_t_init(&object->_object,
		initializer->_type,
		&initializer->_position,
		&vector3_t(0.0f, 0.0f, 0.0f),
		&vector3_t(0.05f, 0.05f, 0.05f)
	);

	object->_database = database;

	return object;
}

extern void test_player_object_t_update(test_player_object_t *object)
{
	//脳を更新
	test_player_brain_t_judge_action(&object->_brain, object);
	//基本情報を取得
	vector3_t position = object->_object->_base->_position;
	vector3_t angle = object->_object->_base->_angle;
	//入力方向に進む
	{
		//カメラがある方向
		angle_t camera_theta = 0.0;
		camera_get_theta(&camera_theta);
		//カメラが向いている方向
		const angle_t camera_dir = camera_theta + M_PI;
		if (object->_brain._move_force >= 0.5)
		{
			//目標の方向
			angle_t target_dir = -camera_dir - M_PI - object->_brain._move_direction;
			//目標の方向に旋回
			angle.y = angle_follow(angle.y, target_dir, object->_database->_turning_force);
			//進行方向に対して前向きのベクトル			
			vector3_t face_forward_vec = {
				(float)(-cos((-angle.y) + M_PI_2)),
				(float)(0.0f),
				(float)(-sin((-angle.y) + M_PI_2))
			};
			//前進する
			position +=
				face_forward_vec *
				(float)(object->_brain._move_force) *
				(float)(object->_database->_move_speed)
				;
		}
	}

	//ジャンプ関係
	{
		if (object->_brain._should_jump)
		{
			if (object->_jump._doing == false)
			{
				object->_jump._doing = true;
				object->_jump._move_y = object->_database->_jump_pow;
			}
		}

		object->_jump._move_y += object->_database->_gravity;
		position.y += (float)(object->_jump._move_y);
		if (position.y < 0.0f)
		{
			position.y = 0.0f;
			object->_jump._doing = false;
			object->_jump._move_y = 0.0f;
		}
	}

	//描画情報をセット
	object->_object->_base->_position = position;
	object->_object->_base->_angle = angle;

}

extern void test_player_object_t_draw(const test_player_object_t *object, IDirect3DDevice9 *device)
{
	///本体を描画
	x_object_t_draw(object->_object, device);

	///影を描画(不可能)
#if 0
	{
		//ワールド変換
		vector3_t position_copy = object->_draw._position;
		vector3_t scale_copy = object->_draw._scale;
		position_copy.y = 1.0f;
		scale_copy.y = 0.0f;
		x_model_world_transformation(&matrix_world, &position_copy, &object->_draw._angle, &scale_copy);
		d3d_device()->SetTransform(D3DTS_WORLD, &matrix_world);
		//描画
		DWORD ambient = 0;
		d3d_device()->GetRenderState(D3DRS_AMBIENT, &ambient);
		d3d_device()->SetRenderState(D3DRS_AMBIENT, 0x00000000);
		d3d_device()->LightEnable(0, false);
		x_model_draw(object->_draw._index);
		d3d_device()->SetRenderState(D3DRS_AMBIENT, ambient);
		d3d_device()->LightEnable(0, true);
}
#endif
}

extern void test_player_object_t_end(test_player_object_t **object)
{
	safe_free(*object);
}

extern const vector3_t *test_player_object_t_get_position(const test_player_object_t *object)
{
	return &object->_object->_base->_position;
}
