#pragma once
//コンマ区切りの文書ファイルを読み込む関数が書かれたヘッダ^ファイルです
//作成者　森　仁志

#include "..\file_reader\file_reader.h"

//ファイルのIDです
enum file_data_id_t
{
	fdi_none,
	fdi_camera_database,
	fdi_icon_file_name,
	fdi_init_state,
	fdi_player_database,
	fdi_player_fbx_file_database,
	fdi_sound_file_database,
	fbi_sky_sphere_fbx_file_database,
	fdi_texture_file_databse,
	fdi_window_startup_option,
	fdi_magnetic_fbx_file,
	fdi_stage1_fbx_file_database,
	fdi_stage2_fbx_file_database,
	fdi_stage3_fbx_file_database,
	fdi_stage4_fbx_file_database,
	fdi_stage1_event,
	fdi_stage2_event,
	fdi_stage3_event,
	fdi_qte_config,
	fdi_max,
};

//ファイル名を取得する関数です
extern const char *file_data_name(file_data_id_t id);

//読み込む関数です
extern void file_data_load_all();

//破棄する関数です
extern void file_data_destroy_all();

//読み込んだデータを取得する関数です
extern const datum_t *file_data_get(file_data_id_t id, size_t *data_num);
