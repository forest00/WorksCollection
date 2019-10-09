//親ヘッダー
#include "fbx_table.h"
//その他のヘッダー
#include "..\mylib\mylib.h"
#include "..\amlib\ExFbxMeshAmg.h"
#include "..\file_reader\file_reader.h"
#include "..\file_data\file_data.h"

#define _motion_name_buffer_size()		(8)

//全てのFBXの内、ステージ123のFBXだけを読み込むデバッグ用のマクロオプション
#define _stage123_only					0

#if _stage123_only
#define _read_player					0
#define _read_stage1					1
#define _read_stage2					1
#define _read_stage3					1
//#define _read_stage4					0
#define _read_sky_sphere				0
#define _read_wrench_and_bibibi			0
#else
#define _read_player					1
#define _read_stage1					1
#define _read_stage2					1
#define _read_stage3					1
//#define _read_stage4					1
#define _read_sky_sphere				1
#define _read_wrench_and_bibibi			1
#endif

//グローバル変数
struct global_t
{
	ExFbxMeshAmg *_ex_fbx_mesh_amg[(size_t)(fti_max)];
	char _motion_name[(size_t)(ftmi_max)][_motion_name_buffer_size()];
	fbx_table_motion_id_t _mesh_to_motion_id_start[(size_t)(fti_max)];
	fbx_table_id_t _motion_id_to_mesh[(size_t)(ftmi_max)];
	bool _already_read[(size_t)(fri_max)];
};
static global_t g;

//FBXのテーブルを初期化
//初期化された段階ではまだ描画できないので、先に読み込み処理をしよう
extern void fbx_table_init()
{
	//読み込み情報をセット
	for (size_t i = 0; i < (size_t)(fti_max); i++)
	{
		g._ex_fbx_mesh_amg[i] = nullptr;
	}
	//モーション名セット
	for (size_t i = 0; i < (size_t)(ftmi_max); i++)
	{
		g._motion_name[i];
		sprintf_s(g._motion_name[i], _motion_name_buffer_size(), "m%d", i);
	}
	//メッシュIDからモーションIDへの変換準備
	for (size_t i = 0; i < (size_t)(fti_max); i++)
	{
		g._mesh_to_motion_id_start[i] = ftmi_none;
	}
	g._mesh_to_motion_id_start[(size_t)(fti_player)] = ftmi_player_data_start;
	//モーションIDからメッシュIDへの変換準備
	for (size_t i = 0; i < (size_t)(ftmi_max); i++)
	{
		g._motion_id_to_mesh[i] = fti_none;
	}
	//読み込み完了フラグを初期化
	for (size_t i = 0; i < (size_t)(fri_max);i++)
	{
		g._already_read[i] = false;
	}
}

//FBXのテーブルを終了
//この関数を呼んだあとはFBXは使えなくなるよ
extern void fbx_table_end()
{
	fbx_table_destroy();
}

//FBXを全部読み込む
void fbx_table_load(fbx_read_index_t index)
{
	IDirect3DDevice9 *d3d_device = mylib_get_d3d_device();
	switch (index)
	{
	case fri_player:
	{
#if _read_player
		//プレーヤーを読み込んで良いかチェック
		if (g._ex_fbx_mesh_amg[(size_t)(fti_player)] == nullptr)
		{
			//プレーヤー読み込み
			size_t data_count = 0;
			const datum_t *data = file_data_get(fdi_player_fbx_file_database, nullptr);
			g._ex_fbx_mesh_amg[(size_t)(fti_player)] = new ExFbxMeshAmg;
			g._ex_fbx_mesh_amg[(size_t)(fti_player)]->Create(d3d_device, data[data_count++]._String);
			for (size_t i = (size_t)(ftmi_player_data_start)+1; i < (size_t)(ftmi_player_data_end);i++)
			{
				if (strlen(data[data_count]._String) == 0)
				{
					continue;
				}
				g._motion_id_to_mesh[i] = fti_player;
				g._ex_fbx_mesh_amg[(size_t)(g._motion_id_to_mesh[i])]->AddMotion(g._motion_name[i], data[data_count++]._String);
			}
		}
#endif
	}
	break;
	case fri_stage1:
	{
#if _read_stage1
		//stage1を読み込んで良いかチェック
		if (g._ex_fbx_mesh_amg[(size_t)(fti_stage1_base)] == nullptr)
		{
			//stage1読み込み
			const datum_t *data = file_data_get(fdi_stage1_fbx_file_database, nullptr);
			g._ex_fbx_mesh_amg[(size_t)(fti_stage1_base)] = new ExFbxMeshAmg;
			g._ex_fbx_mesh_amg[(size_t)(fti_stage1_magobj)] = new ExFbxMeshAmg;
			g._ex_fbx_mesh_amg[(size_t)(fti_stage1_nomagobj)] = new ExFbxMeshAmg;
			g._ex_fbx_mesh_amg[(size_t)(fti_stage1_base)]->Create(d3d_device, (data++)->_String);
			g._ex_fbx_mesh_amg[(size_t)(fti_stage1_magobj)]->Create(d3d_device, (data++)->_String);
			g._ex_fbx_mesh_amg[(size_t)(fti_stage1_nomagobj)]->Create(d3d_device, (data++)->_String);
		}
#endif
	}
	break;
	case fri_stage2:
	{
#if _read_stage2
		//stage2を読み込んで良いかチェック
		if (g._ex_fbx_mesh_amg[(size_t)(fti_stage2_base)] == nullptr)
		{
			//stage2読み込み
			const datum_t *data = file_data_get(fdi_stage2_fbx_file_database, nullptr);
			g._ex_fbx_mesh_amg[(size_t)(fti_stage2_base)] = new ExFbxMeshAmg;
			g._ex_fbx_mesh_amg[(size_t)(fti_stage2_magobj)] = new ExFbxMeshAmg;
			g._ex_fbx_mesh_amg[(size_t)(fti_stage2_nomagobj)] = new ExFbxMeshAmg;
			g._ex_fbx_mesh_amg[(size_t)(fti_stage2_base)]->Create(d3d_device, (data++)->_String);
			g._ex_fbx_mesh_amg[(size_t)(fti_stage2_magobj)]->Create(d3d_device, (data++)->_String);
			g._ex_fbx_mesh_amg[(size_t)(fti_stage2_nomagobj)]->Create(d3d_device, (data++)->_String);
		}
#endif
	}
	break;
	case fri_stage3:
	{
#if _read_stage3
		//stage3を読み込んで良いかチェック
		if (g._ex_fbx_mesh_amg[(size_t)(fti_stage3_base)] == nullptr)
		{
			//stage3読み込み
			const datum_t *data = file_data_get(fdi_stage3_fbx_file_database, nullptr);
			g._ex_fbx_mesh_amg[(size_t)(fti_stage3_base)] = new ExFbxMeshAmg;
			g._ex_fbx_mesh_amg[(size_t)(fti_stage3_magobj)] = new ExFbxMeshAmg;
			g._ex_fbx_mesh_amg[(size_t)(fti_stage3_nomagobj)] = new ExFbxMeshAmg;
			g._ex_fbx_mesh_amg[(size_t)(fti_stage3_base)]->Create(d3d_device, (data++)->_String);
			g._ex_fbx_mesh_amg[(size_t)(fti_stage3_magobj)]->Create(d3d_device, (data++)->_String);
			g._ex_fbx_mesh_amg[(size_t)(fti_stage3_nomagobj)]->Create(d3d_device, (data++)->_String);
		}
#endif
	}
	break;
#if 0
	case fri_stage4:
	{
#if _read_stage4
		//stage4を読み込んで良いかチェック
		if (g._ex_fbx_mesh_amg[(size_t)(fti_stage4_base)] == nullptr)
		{
			//stage4読み込み
			const datum_t *data = file_data_get(fdi_stage4_fbx_file_database, nullptr);
			g._ex_fbx_mesh_amg[(size_t)(fti_stage4_base)] = new ExFbxMeshAmg;
			g._ex_fbx_mesh_amg[(size_t)(fti_stage4_magobj)] = new ExFbxMeshAmg;
			g._ex_fbx_mesh_amg[(size_t)(fti_stage4_nomagobj)] = new ExFbxMeshAmg;
			g._ex_fbx_mesh_amg[(size_t)(fti_stage4_base)]->Create(d3d_device, (data++)->_String);
			g._ex_fbx_mesh_amg[(size_t)(fti_stage4_magobj)]->Create(d3d_device, (data++)->_String);
			g._ex_fbx_mesh_amg[(size_t)(fti_stage4_nomagobj)]->Create(d3d_device, (data++)->_String);
		}
#endif
	}
#endif
	break;
	case fri_sky_sphere:
	{
#if _read_sky_sphere
		//スカイスフィアを読み込んで良いかチェック
		if (g._ex_fbx_mesh_amg[(size_t)(fti_sky)] == nullptr)
		{
			//スカイスフィア読み込み
			const datum_t *data = file_data_get(fbi_sky_sphere_fbx_file_database, nullptr);
			g._ex_fbx_mesh_amg[(size_t)(fti_sky)] = new ExFbxMeshAmg;
			g._ex_fbx_mesh_amg[(size_t)(fti_sky_sphere)] = new ExFbxMeshAmg;
			g._ex_fbx_mesh_amg[(size_t)(fti_sky)]->Create(d3d_device, (data++)->_String);
			g._ex_fbx_mesh_amg[(size_t)(fti_sky_sphere)]->Create(d3d_device, (data++)->_String);
		}
#endif
	}
	break;
	case fri_wrench_and_bibibi:
	{
#if _read_wrench_and_bibibi
		//レンチ&ビリビリオブジェクトを読み込んで良いかチェック
		if (g._ex_fbx_mesh_amg[(size_t)(fti_wrench)] == nullptr)
		{
			//レンチ&ビリビリオブジェクト読み込み
			const datum_t *data = file_data_get(fdi_magnetic_fbx_file, nullptr);
			g._ex_fbx_mesh_amg[(size_t)(fti_wrench)] = new ExFbxMeshAmg;
			g._ex_fbx_mesh_amg[(size_t)(fti_bibibi_red_3d)] = new ExFbxMeshAmg;
			g._ex_fbx_mesh_amg[(size_t)(fti_wrench)]->Create(d3d_device, (data++)->_String);
			g._ex_fbx_mesh_amg[(size_t)(fti_bibibi_red_3d)]->Create(d3d_device, (data++)->_String);
		}
#endif
	}
	break;
	case fri_max:
		break;
	default:
		break;
	}
}

//読み込んだFBXを破棄する
void fbx_table_destroy()
{
	for (size_t i = 0; i < (size_t)(fti_max); i++)
	{
		if (g._ex_fbx_mesh_amg[i])
		{
			delete g._ex_fbx_mesh_amg[i];
			g._ex_fbx_mesh_amg[i] = nullptr;
		}
	}
}

//読み込んだFBXを描画する
void fbx_table_draw(fbx_table_id_t looks, fbx_table_motion_id_t motion, const D3DXMATRIX *world, float motion_frame)
{
	size_t index = (size_t)(looks);
	if (g._ex_fbx_mesh_amg[index] == nullptr)
	{
		return;
	}
	g._ex_fbx_mesh_amg[index]->Play(g._motion_name[(size_t)(motion)]);
	g._ex_fbx_mesh_amg[index]->SetFrame(motion_frame);
	g._ex_fbx_mesh_amg[index]->SetTransform(world);
	g._ex_fbx_mesh_amg[index]->Render(mylib_get_d3d_device());
}

//読み込んだFBXをシェーダーで描画する
void fbx_table_draw_with_simple_shader(
	fbx_table_id_t looks,
	fbx_table_motion_id_t motion,
	const D3DXMATRIX *world,
	float motion_frame,
	ex_fbx_mesh_amg_blend_mode_t blend_mode
)
{
	size_t index = (size_t)(looks);
	if (g._ex_fbx_mesh_amg[index] == nullptr)
	{
		return;
	}
	g._ex_fbx_mesh_amg[index]->Play(g._motion_name[(size_t)(motion)]);
	g._ex_fbx_mesh_amg[index]->SetFrame(motion_frame);
	g._ex_fbx_mesh_amg[index]->SetTransform(world);
	g._ex_fbx_mesh_amg[index]->RenderWithSimpleShader(mylib_get_d3d_device(), blend_mode);
}

//読み込んだFBXをシェーダーでアルファ値最大にしたうえで描画する
extern void fbx_table_draw_with_simple_shader_max_alpha(
	fbx_table_id_t looks,
	fbx_table_motion_id_t motion,
	const D3DXMATRIX *world,
	float motion_frame
)
{
	size_t index = (size_t)(looks);
	if (g._ex_fbx_mesh_amg[index] == nullptr)
	{
		return;
	}
	g._ex_fbx_mesh_amg[index]->Play(g._motion_name[(size_t)(motion)]);
	g._ex_fbx_mesh_amg[index]->SetFrame(motion_frame);
	g._ex_fbx_mesh_amg[index]->SetTransform(world);
	g._ex_fbx_mesh_amg[index]->RenderWithSimpleShaderMaxAlpha(mylib_get_d3d_device());
}

//特定のFBXに向かってレイを飛ばす
void fbx_table_raycast(
	fbx_table_id_t looks,
	fbx_table_motion_id_t motion,
	const D3DXMATRIX *world,
	float motion_frame,
	const D3DXVECTOR3 *ray_position,
	const D3DXVECTOR3 *ray_vector,
	D3DXVECTOR3 *intersection,
	D3DXVECTOR3 *normal,
	int *face,
	bool *hitting,
	int *mesh_id
)
{
	if (ray_position == nullptr || ray_vector == nullptr || intersection == nullptr || normal == nullptr)
	{
		return;
	}
	size_t index = (size_t)(looks);
	if (g._ex_fbx_mesh_amg[index] == nullptr)
	{
		return;
	}
	g._ex_fbx_mesh_amg[index]->Play(g._motion_name[(size_t)(motion)]);
	g._ex_fbx_mesh_amg[index]->SetFrame(motion_frame);
	g._ex_fbx_mesh_amg[index]->SetTransform(world);
	g._ex_fbx_mesh_amg[index]->Skinning();
	D3DXMATRIX inverse_world = {};
	D3DXVECTOR3 work_ray_position = *ray_position;
	D3DXVECTOR3 work_ray_vector = *ray_vector;
	D3DXVECTOR4 trans_ray_position = {};
	D3DXVECTOR4 trans_ray_vector = {};
	D3DXVECTOR4 trans_intersect = {};
	D3DXMatrixInverse(&inverse_world, nullptr, world);
	D3DXVec3Transform(&trans_ray_position, &work_ray_position, &inverse_world);
	D3DXVec3Transform(&trans_ray_vector, &work_ray_vector, &inverse_world);
	work_ray_position = D3DXVECTOR3(trans_ray_position.x, trans_ray_position.y, trans_ray_position.z);
	work_ray_vector = D3DXVECTOR3(trans_ray_vector.x, trans_ray_vector.y, trans_ray_vector.z);
	int _face = -1;
	g._ex_fbx_mesh_amg[index]->Raycast(work_ray_position, work_ray_vector, 1.0f, *intersection, *normal, &_face, mesh_id);
	D3DXVec3Transform(&trans_ray_position, &work_ray_position, world);
	D3DXVec3Transform(&trans_ray_vector, &work_ray_vector, world);
	D3DXVec3Transform(&trans_intersect, intersection, world);
	work_ray_position = D3DXVECTOR3(trans_ray_position.x, trans_ray_position.y, trans_ray_position.z);
	work_ray_vector = D3DXVECTOR3(trans_ray_vector.x, trans_ray_vector.y, trans_ray_vector.z);
	*intersection = D3DXVECTOR3(trans_intersect.x, trans_intersect.y, trans_intersect.z);
	if (face)
	{
		*face = _face;
	}
	if (hitting)
	{
		*hitting = _face >= 0;
	}
}

//特定のFBXに向かって太いレイを飛ばす
extern void fbx_table_sphere_cast(
	fbx_table_id_t looks,
	fbx_table_motion_id_t motion,
	const D3DXMATRIX *world,
	float motion_frame,
	const D3DXVECTOR3 *ray_position,
	const D3DXVECTOR3 *ray_vector,
	float radius,
	D3DXVECTOR3 *intersection,
	D3DXVECTOR3 *normal,
	int *face,
	bool *hitting,
	int *mesh_id
)
{
	if (ray_position == nullptr || ray_vector == nullptr || intersection == nullptr || normal == nullptr)
	{
		return;
	}
	size_t index = (size_t)(looks);
	if (g._ex_fbx_mesh_amg[index] == nullptr)
	{
		return;
	}
	g._ex_fbx_mesh_amg[index]->Play(g._motion_name[(size_t)(motion)]);
	g._ex_fbx_mesh_amg[index]->SetFrame(motion_frame);
	g._ex_fbx_mesh_amg[index]->SetTransform(world);
	g._ex_fbx_mesh_amg[index]->Skinning();
	D3DXMATRIX inverse_world = {};
	D3DXVECTOR3 work_ray_position = *ray_position;
	D3DXVECTOR3 work_ray_vector = *ray_vector;
	D3DXVECTOR4 trans_ray_position = {};
	D3DXVECTOR4 trans_ray_vector = {};
	D3DXVECTOR4 trans_intersect = {};
	D3DXMatrixInverse(&inverse_world, nullptr, world);
	D3DXVec3Transform(&trans_ray_position, &work_ray_position, &inverse_world);
	D3DXVec3Transform(&trans_ray_vector, &work_ray_vector, &inverse_world);
	work_ray_position = D3DXVECTOR3(trans_ray_position.x, trans_ray_position.y, trans_ray_position.z);
	work_ray_vector = D3DXVECTOR3(trans_ray_vector.x, trans_ray_vector.y, trans_ray_vector.z);
	int _face = -1;
	g._ex_fbx_mesh_amg[index]->SphereCast(work_ray_position, work_ray_vector, radius, 1.0f, *intersection, *normal, &_face, mesh_id);
	D3DXVec3Transform(&trans_ray_position, &work_ray_position, world);
	D3DXVec3Transform(&trans_ray_vector, &work_ray_vector, world);
	D3DXVec3Transform(&trans_intersect, intersection, world);
	work_ray_position = D3DXVECTOR3(trans_ray_position.x, trans_ray_position.y, trans_ray_position.z);
	work_ray_vector = D3DXVECTOR3(trans_ray_vector.x, trans_ray_vector.y, trans_ray_vector.z);
	*intersection = D3DXVECTOR3(trans_intersect.x, trans_intersect.y, trans_intersect.z);
	if (face)
	{
		*face = _face;
	}
	if (hitting)
	{
		*hitting = _face >= 0;
	}
}

//指定のFBXの指定の姿勢で、指定のポリゴンの重心を得る
extern void fbx_table_get_polygon_position(
	fbx_table_id_t looks,
	fbx_table_motion_id_t motion,
	const D3DXMATRIX *world,
	float motion_frame,
	int polygon_id,
	D3DXVECTOR3 *pos
)
{
	size_t index = (size_t)(looks);
	if (g._ex_fbx_mesh_amg[index] == nullptr)
	{
		return;
	}
	g._ex_fbx_mesh_amg[index]->Play(g._motion_name[(size_t)(motion)]);
	g._ex_fbx_mesh_amg[index]->SetFrame(motion_frame);
	g._ex_fbx_mesh_amg[index]->SetTransform(world);
	g._ex_fbx_mesh_amg[index]->GetPolygonPos(polygon_id, pos);
}

//指定のFBXの指定の姿勢で、指定の範囲のポリゴンの中心位置を得る
extern void fbx_table_get_polygon_position_range(
	fbx_table_id_t looks,
	fbx_table_motion_id_t motion,
	const D3DXMATRIX *world,
	float motion_frame,
	int min_polygon_id,
	int max_polygon_id,
	D3DXVECTOR3 *pos
)
{
	size_t index = (size_t)(looks);
	if (g._ex_fbx_mesh_amg[index] == nullptr)
	{
		return;
	}
	g._ex_fbx_mesh_amg[index]->Play(g._motion_name[(size_t)(motion)]);
	g._ex_fbx_mesh_amg[index]->SetFrame(motion_frame);
	g._ex_fbx_mesh_amg[index]->SetTransform(world);
	g._ex_fbx_mesh_amg[index]->GetPolygonPosRange(min_polygon_id, max_polygon_id, pos);
}

//指定のFBXの指定の姿勢で、指定のメッシュの中心位置を得る
extern void fbx_table_get_mesh_position(
	fbx_table_id_t looks,
	fbx_table_motion_id_t motion,
	const D3DXMATRIX *world,
	float motion_frame,
	int mesh_id,
	D3DXVECTOR3 *pos
)
{
	size_t index = (size_t)(looks);
	if (g._ex_fbx_mesh_amg[index] == nullptr)
	{
		return;
	}
	g._ex_fbx_mesh_amg[index]->Play(g._motion_name[(size_t)(motion)]);
	g._ex_fbx_mesh_amg[index]->SetFrame(motion_frame);
	g._ex_fbx_mesh_amg[index]->SetTransform(world);
	g._ex_fbx_mesh_amg[index]->GetMeshPos(mesh_id, pos);
}

//メッシュを無視するフラグを立てる
void fbx_table_ignore(fbx_table_id_t looks, int mesh_id, bool do_flag)
{
	size_t index = (size_t)(looks);
	if (g._ex_fbx_mesh_amg[index] == nullptr)
	{
		return;
	}
	g._ex_fbx_mesh_amg[index]->IgnoreMesh(mesh_id, do_flag);
}

//メッシュを無視するフラグをリセットする
extern void fbx_table_ignore_reset(fbx_table_id_t looks)
{
	size_t index = (size_t)(looks);
	if (g._ex_fbx_mesh_amg[index] == nullptr)
	{
		return;
	}
	g._ex_fbx_mesh_amg[index]->IgnoreMeshAll(false);
}

//モーション1再生にかかる時間(フレーム)を取得
float fbx_table_motion_frame(fbx_table_motion_id_t motion)
{
	return g._ex_fbx_mesh_amg[g._motion_id_to_mesh[(size_t)(motion)]]->GetMotionFrame(g._motion_name[(size_t)(motion)]);
}
