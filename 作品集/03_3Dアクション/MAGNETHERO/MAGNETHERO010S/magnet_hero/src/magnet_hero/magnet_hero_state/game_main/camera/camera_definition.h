#pragma once
//	
//	冬期制作「Magnet Hero」
//	
//	カメラの型定義です
//	
//	先にDirect3Dの初期化をしておく必要があります
//	
//	作成者	森	仁志
//	

//DirectX
#include <d3dx9.h>

//カメラ
struct game_main_camera_t
{
	D3DXVECTOR3 _eye;							//視点の位置
	D3DXVECTOR3 _at;							//注視点の位置
	D3DXVECTOR3 _up;							//上方向のベクトル(0,1,0)
	float _distance;							//注視点からの距離
	float _theta;								//上下角度
	float _phi;									//左右角度
	float _user_move_force;						//
	float _user_close_force;					//
	float _user_turning_force;					//
	float _user_move_weight;					//
	float _user_close_weight;					//
	float _user_turning_weight;					//
	D3DXVECTOR3 _user_move_result;				//
	float _user_close_result;					//
	float _user_theta_result;					//
	float _user_phi_result;						//
	D3DXVECTOR3 _user_move_accumulate;			//
	float _distance_upper;						//注視点から離れられる上限
	float _distance_lower;						//注視点から離れられる下限
	float _theta_upper;							//上下角度の上限
	float _theta_lower;							//上下角度の下限
	float _shift_up;							//上方向に修正する量
	float _move_time;							//
	float _move_power;							//
	D3DXVECTOR3 _true_eye;						//本当の視点位置
	float _jump_point;							//プレーヤーがジャンプ中なら増えていき、そうじゃなければ減っていく値
};

//カメラのデータベース
struct camera_database_t
{
	float _stage1_default_distance;
	float _stage1_default_phi;
	float _stage1_default_theta;
	float _user_move_force;
	float _user_close_force;
	float _user_turning_force;
	float _user_move_weight;
	float _user_close_weight;
	float _user_turning_weight;
	float _distance_upper;
	float _distance_lower;
	float _theta_upper;
	float _theta_lower;
	float _shift_up;
	float _move_time;
	float _move_power;
};