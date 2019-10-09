//親ヘッダー
#include "info_holder.h"
//その他のヘッダー
//#include "info_holder.h"

struct global_t
{
	D3DXVECTOR3 _player_position;
	bool _player_magnet_hand_do_draw;
	time_t _player_magnet_hand_frame;
	D3DXVECTOR3 _player_magnet_hand_position;
	D3DXVECTOR3 _player_shadow_position;
	D3DXVECTOR3 _player_shadow_normal;
	DWORD _player_falled_time;
	DWORD _player_goal_time;
	float _distance_from_player_to_goal;
	DWORD _stage_start_moment;
	DWORD _player_beam_hit_moment;
	bool _player_beam_hit_object;
	bool _player_state_is_sticking_migration;
	bool _player_is_jumping;
	float _player_jump_move_y;
	float _player_do_jump_moment;
};

static global_t g = {};

//プレーヤーの位置を保存
extern void holder_player_position_set(const D3DXVECTOR3 *position)
{
	g._player_position = *position;
}
//プレーヤーの位置を取得
extern void holder_player_position_get(D3DXVECTOR3 *position)
{
	*position = g._player_position;
}

//プレーヤーの飛ばす手の位置を保存
extern void holder_player_magnet_hand_position_set(bool do_draw, time_t frame, const D3DXVECTOR3 *position)
{
	g._player_magnet_hand_do_draw = do_draw;
	g._player_magnet_hand_frame = frame;
	g._player_magnet_hand_position = *position;
}
//プレーヤーの飛ばす手の位置を取得
extern void holder_player_magnet_hand_position_get(bool *do_draw, time_t *frame, D3DXVECTOR3 *position)
{
	*do_draw = g._player_magnet_hand_do_draw;
	*frame = g._player_magnet_hand_frame;
	*position = g._player_magnet_hand_position;
}

//プレーヤーの影の位置を保存
extern void holder_player_shadow_position_set(const D3DXVECTOR3 *position)
{
	g._player_shadow_position = *position;
}
//プレーヤーの影の位置を取得
extern void holder_player_shadow_position_get(D3DXVECTOR3 *position)
{
	*position = g._player_shadow_position;
}

//プレーヤーの影の法線を保存
extern void holder_player_shadow_normal_set(const D3DXVECTOR3 *normal)
{
	g._player_shadow_normal = *normal;
}
//プレーヤーの影の法線を取得
extern void holder_player_shadow_normal_get(D3DXVECTOR3 *normal)
{
	*normal = g._player_shadow_normal;
}

//プレーヤーの伸ばすビームとオブジェクトが当たった時間を保存
extern void player_beam_hit_record()
{
	g._player_beam_hit_moment = timeGetTime();
}
//プレーヤーの伸ばすビームとオブジェクトが当たった時間を取得
extern DWORD player_beam_hit_get()
{
	return g._player_beam_hit_moment;
}

//プレーヤーが落下した時間を保存
extern void holder_falled_player_record()
{
	g._player_falled_time = timeGetTime();
}
//プレーヤーが落下した時間を取得
extern DWORD holder_falled_player_get()
{
	return g._player_falled_time;
}


//プレーヤーがビームを打ったら当たるかどうか判定済みの値を渡す
extern void player_beam_hit_object_set(bool flag)
{
	g._player_beam_hit_object = flag;
}
//プレーヤーがビームを打ったら当たるかどうか判定済みの値を返す
extern bool player_beam_hit_object_get()
{
	return g._player_beam_hit_object;
}

//この関数は毎フレーム呼び出し、プレーヤーが磁力移行状態ならtrueを渡し、そうでなければfalseを渡さなければいけない
extern void ask_player_state_is_sticking_migration(bool is)
{
	g._player_state_is_sticking_migration = is;
}
//プレーヤーが磁力移行状態ならtrueを返す
extern bool player_state_is_sticking_migration()
{
	return g._player_state_is_sticking_migration;
}


//プレーヤーがジャンプ中ならtrueで呼ぶ、そうでなければfalseで呼ぶ
extern void player_is_jumping_set(bool is)
{
	g._player_is_jumping = is;
}
//プレーヤーがジャンプ中ならtrueが返る
extern bool player_is_jumping_get()
{
	return g._player_is_jumping;
}


//プレーヤーY座標移動量を毎フレーム渡す
extern void player_jump_move_y_set(float y)
{
	g._player_jump_move_y = y;
}
//プレーヤーY座標移動量
extern float player_jump_move_y_get()
{
	return g._player_jump_move_y;
}


//プレーヤーがジャンプした瞬間に呼ぶ(時間を秒単位で渡す)
extern void player_do_jump_moment_set(float moment)
{
	g._player_do_jump_moment = moment;
}
//プレーヤーがジャンプした瞬間の時間を秒単位で返す
extern float player_do_jump_moment_get()
{
	return g._player_do_jump_moment;
}
