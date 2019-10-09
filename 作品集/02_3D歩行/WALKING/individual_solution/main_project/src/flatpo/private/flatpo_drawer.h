#pragma once

//板ポリゴン(以降、板ポリ)を扱う描画するファイルです
//板ポリ構造体一個に付き一個の板ポリが描画できます

#include <windows.h>
#include <d3dx9.h>
#include "..\public\flatpo_texture_loader.h"
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")


//命名規則
//
//関数名は頭に"flatpo_"を付け、スネークケースで記述
//関数の引数はスネークケースで記述
//グローバル変数は頭に"g_"を付け、スネークケースで記述
//構造体と列挙体の名前は末尾に"_t"を付け、スネークケースで記述
//構造体メンバーは頭に"_"を付け、スネークケースで記述
//列挙子は頭にタグ名と"_"を付け、スネークケースで記述
//その他のものは全て、スネークケースで記述
//


//仮想スクリーンの幅
enum flatpo_screen_t
{
	flatpo_screen_width = 1350,
	flatpo_screen_height = 730,
};


//板ポリ頂点(D3DFVFとは無関係です)
struct flatpo_vertex_t
{
	D3DXVECTOR3 _position;		//位置
	float _u;					//テクスチャ座標U
	float _v;					//テクスチャ座標V
	int _alpha;					//透明度 0~255(範囲外でもクランプされるから大丈夫)
	int _red;					//赤成分 0~255(範囲外でもクランプされるから大丈夫)
	int _green;					//緑成分 0~255(範囲外でもクランプされるから大丈夫)
	int _blue;					//青成分 0~255(範囲外でもクランプされるから大丈夫)
};


//板ポリ構造体
struct flatpo_t
{
	flatpo_texture_id_t _texture_id;			//貼り付けるテクスチャのID
	flatpo_vertex_t _vertices[4];			//頂点4つ(左回りになるようにしてください)
};


//板ポリシーン開始
extern void flatpo_begin_scene(IDirect3DDevice9 *device, float aspect);

//板ポリシーン終了
extern void flatpo_end_scene();

//板ポリの描画
extern void flatpo_draw(flatpo_t *flatpo);

