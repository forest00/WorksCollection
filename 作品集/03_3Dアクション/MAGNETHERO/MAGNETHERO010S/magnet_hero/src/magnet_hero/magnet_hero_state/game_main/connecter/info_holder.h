#pragma once
//プレーヤーの情報を一度とっておいて別の場所に渡すために存在するヘッダーです
//必要であればプレーヤー以外の情報もここでやり取りします
//ここで提供される機能はすべてゲームのメインシーンのみで使います
//作成者　森仁志

//DirectXのヘッダー
#include <d3dx9.h>


//プレーヤーの位置を保存
extern void holder_player_position_set(const D3DXVECTOR3 *position);
//プレーヤーの位置を取得
extern void holder_player_position_get(D3DXVECTOR3 *position);


//プレーヤーの飛ばす手の位置・角度を保存
extern void holder_player_magnet_hand_position_set(bool do_draw, time_t frame, const D3DXVECTOR3 *position);
//プレーヤーの飛ばす手の位置・角度を取得
extern void holder_player_magnet_hand_position_get(bool *do_draw, time_t *frame, D3DXVECTOR3 *position);


//プレーヤーの影の位置を保存
extern void holder_player_shadow_position_set(const D3DXVECTOR3 *position);
//プレーヤーの影の位置を取得
extern void holder_player_shadow_position_get(D3DXVECTOR3 *position);


//プレーヤーの影の法線を保存
extern void holder_player_shadow_normal_set(const D3DXVECTOR3 *normal);
//プレーヤーの影の法線を取得
extern void holder_player_shadow_normal_get(D3DXVECTOR3 *normal);


//プレーヤーの伸ばすビームとオブジェクトが当たった時間を保存(カメラ用)
extern void player_beam_hit_record();
//プレーヤーの伸ばすビームとオブジェクトが当たった時間を取得(カメラ用)
extern DWORD player_beam_hit_get();


//プレーヤーが落下した時間を保存
extern void holder_falled_player_record();
//プレーヤーが落下した時間を取得
extern DWORD holder_falled_player_get();


//プレーヤーがビームを打ったら当たるかどうか判定済みの値を渡す
extern void player_beam_hit_object_set(bool flag);
//プレーヤーがビームを打ったら当たるかどうか判定済みの値を返す
extern bool player_beam_hit_object_get();


//この関数は毎フレーム呼び出し、プレーヤーが磁力移行状態ならtrueを渡し、そうでなければfalseを渡さなければいけない
extern void ask_player_state_is_sticking_migration(bool is);
//プレーヤーが磁力移行状態ならtrueを返す
extern bool player_state_is_sticking_migration();


//プレーヤーがジャンプ中ならtrueで呼ぶ、そうでなければfalseで呼ぶ
extern void player_is_jumping_set(bool is);
//プレーヤーがジャンプ中ならtrueが返る
extern bool player_is_jumping_get();


//プレーヤーY座標移動量を毎フレーム渡す
extern void player_jump_move_y_set(float y);
//プレーヤーY座標移動量
extern float player_jump_move_y_get();


//プレーヤーがジャンプした瞬間に呼ぶ(時間を秒単位で渡す)
extern void player_do_jump_moment_set(float moment);
//プレーヤーがジャンプした瞬間の時間を秒単位で返す
extern float player_do_jump_moment_get();
