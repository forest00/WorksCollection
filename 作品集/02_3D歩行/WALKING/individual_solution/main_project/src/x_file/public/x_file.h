#pragma once

//xファイルを読み込んで描画します
//
//	使い方
//	
//	init()	全読み込み	これを最初に一回
//	
//	load_from_database()	データベースにあるものを読み込んできます
//	
//	draw()	描画します
//	
//	end()	全解放	これを最後に一回
//	
//	データベースから取ってきて描画のサイクルを作ればOK
//
//
//作成者		森	仁志
//						最終更新		2018/11/27 (12:11)


#include "x_file_structure.h"
#include "x_file_type.h"



//xファイル使用開始
extern void x_file_init(IDirect3DDevice9 *device);

//xファイル使用終了
extern void x_file_end();

//モデル情報をデータベースから取ってくる
extern void x_file_t_load_from_database(x_file_t *strage, x_file_type_t type);
extern void x_file_t_load_from_database(x_file_t *strage, size_t type);

//モデルを描画
extern void x_file_t_draw(IDirect3DDevice9 *device, const x_file_t *x_file);

