#pragma once

#include "..\..\x_file\public\x_file.h"
#include "..\..\common_def\public\common_def.h"

//位置
//回転
//拡大縮小
//メッシュ
//
//これらの情報を持っている構造体です
//描画のために使います
struct x_object_base_t
{
	vector3_t _position;				//位置
	vector3_t _angle;					//角度
	vector3_t _scale;					//拡大倍率
	x_file_t _x_file;					//描画用情報
};
