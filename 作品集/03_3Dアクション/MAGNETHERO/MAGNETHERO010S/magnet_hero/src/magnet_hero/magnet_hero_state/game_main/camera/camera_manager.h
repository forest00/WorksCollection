#pragma once
//	
//	冬期制作「Magnet Hero」
//	
//	カメラの管理を行います
//	
//	先にDirect3Dの初期化をしておく必要があります
//	
//	作成者	森	仁志
//	


#include "camera_definition.h"

#include "..\fbx_object\object_manager.h"

//初期化
extern void camera_manager_init();
//終了
extern void camera_manager_end();
//更新
extern void camera_manager_update(game_main_fbx_object_array_t *object_array);
//ビュー行列とプロジェクション行列を求めてライブラリのほうにセット
extern void camera_manager_set_view_and_projection(float near_clip, float far_clip);
//全情報取得
extern const game_main_camera_t *camera_manager_get();
//横方向の角度取得
extern float camera_manager_get_phi();
//縦方向の角度取得
extern float camera_manager_get_theta();
//横方向の角度指定
extern void camera_manager_set_phi(float phi);
//縦方向の角度指定
extern void camera_manager_set_theta(float theta);