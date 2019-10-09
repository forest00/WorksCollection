//親ヘッダー
#include "camera_manager.h"
//その他のヘッダー
#include "camera_controll.h"
#include "..\..\..\..\file_data\file_data.h"
#include "..\..\..\..\shader_use\simple_shader.h"

//グローバル変数
struct global_t
{
	game_main_camera_t _camera;
	camera_database_t _database;
};

static global_t g;

static void set_database()
{
	static bool should_read = true;
	if (should_read)
	{
		should_read = false;
		const datum_t *data = file_data_get(fdi_camera_database, nullptr);
		g._database._stage1_default_distance = (data++)->_Float;
		g._database._stage1_default_phi = D3DXToRadian((data++)->_Float);
		g._database._stage1_default_theta = D3DXToRadian((data++)->_Float);
		g._database._user_move_force = (data++)->_Float;
		g._database._user_close_force = (data++)->_Float;
		g._database._user_turning_force = (data++)->_Float;
		g._database._user_move_weight = (data++)->_Float;
		g._database._user_close_weight = (data++)->_Float;
		g._database._user_turning_weight = (data++)->_Float;
		g._database._distance_upper = (data++)->_Float;
		g._database._distance_lower = (data++)->_Float;
		g._database._theta_upper = (data++)->_Float;
		g._database._theta_lower = (data++)->_Float;
		g._database._shift_up = (data++)->_Float;
		g._database._move_time = (data++)->_Float;
		g._database._move_power = (data++)->_Float;
	}
}


//初期化
extern void camera_manager_init()
{
	set_database();
	camera_init(&g._camera, &g._database);
}
//終了
extern void camera_manager_end()
{
	camera_end(&g._camera);
}
//更新
extern void camera_manager_update(game_main_fbx_object_array_t *object_array)
{
	camera_update(object_array, &g._camera);
}
//ビュー行列とプロジェクション行列を求めてライブラリのほうにセット
extern void camera_manager_set_view_and_projection(float near_clip, float far_clip)
{
	camera_set_view_and_projection(&g._camera, near_clip, far_clip);
	simple_shader_set_near_far_clip(near_clip, far_clip);
}
//全情報取得
extern const game_main_camera_t *camera_manager_get()
{
	return &g._camera;
}
//横方向の角度取得
extern float camera_manager_get_phi()
{
	return g._camera._phi;
}
//縦方向の角度取得
extern float camera_manager_get_theta()
{
	return g._camera._theta;
}
//横方向の角度指定
extern void camera_manager_set_phi(float phi)
{
	g._camera._phi = phi;
}
//縦方向の角度指定
extern void camera_manager_set_theta(float theta)
{
	g._camera._theta = theta;
}