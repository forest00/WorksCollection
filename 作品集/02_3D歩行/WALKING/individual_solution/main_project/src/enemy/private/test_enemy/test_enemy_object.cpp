
//親ヘッダー
#include "test_enemy_object.h"
//その他のヘッダー
#include <assert.h>
#include "..\..\..\math_with_defines\math_with_defines.h"
#include "test_enemy_brain.h"
#include "..\..\..\x_object\public\x_object.h"
#include "..\..\..\camera\public\camera.h"



//ジャンプ用の構造体
struct test_enemy_jump_t
{
	bool _doing;			//ジャンプしているか
	double _move_y;			//ジャンプの移動量
};

//テスト用敵キャラオブジェクト
struct test_enemy_object_t
{
	x_object_t *_object;								//基本情報
	test_enemy_brain_t _brain;							//自分の脳
	const test_enemy_database_t *_database;				//自分を更新するときに使うデータ(どっかで確保しといてね☆)
	test_enemy_jump_t _jump;							//ジャンプ用情報
};

extern test_enemy_object_t *test_enemy_object_t_init(const test_enemy_database_t *database, const test_enemy_initializer_t *initializer)
{
	assert(initializer);
	assert(database);
	void *creature = malloc(sizeof(test_enemy_object_t));
	assert(creature);
	test_enemy_object_t *object = (test_enemy_object_t*)(creature);
	assert(object);
	vector3_t position = initializer->_position;
	vector3_t angle = vector3_t(0.0f, (float)(initializer->_direction), 0.0f);
	vector3_t scale = vector3_t(1.0f, 1.0f, 1.0f);
	x_object_t_init(&object->_object, initializer->_type, &position, &angle, &scale);
	object->_jump._doing = false;
	object->_jump._move_y = 0.0f;
	object->_database = database;

	return object;
}

extern void test_enemy_object_t_update(test_enemy_object_t *object)
{
	//脳を更新
	test_enemy_brain_t_judge_action(&object->_brain, object);
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

extern void test_enemy_object_t_draw(test_enemy_object_t *object, IDirect3DDevice9 *device)
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

extern void test_enemy_object_t_end(test_enemy_object_t **object)
{
	safe_free(*object);
}

//テスト用敵キャラの位置を得る
extern const vector3_t *test_enemy_object_t_get_position(const test_enemy_object_t *object)
{
	return &object->_object->_base->_position;
}

//テスト用敵キャラの角度を得る
extern const angle_t *test_enemy_object_t_get_direction(const test_enemy_object_t *object)
{
	return (angle_t*)(&object->_object->_base->_angle.y);
}
