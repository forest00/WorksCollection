#pragma once
#include "FbxMeshAmg.h"
enum ex_fbx_mesh_amg_blend_mode_t
{
	efmabm_no_blend = 0,
	efmabm_alpha_blend = 1,
	efmabm_add_blend = 2,
};
class ExFbxMeshAmg : private FbxMeshAmg
{
protected:
	//頂点構造体
	using exVertex = PolygonVertex;
	//ポリゴン構造体
	struct exPolygon
	{
		int ParrentMeshID;			//親メッシュID
		int VertexIDs[3];			//頂点ID
	};
	//メッシュ構造体
	struct exMesh
	{
		bool DoIgnore;				//このメッシュを無視するかどうか
		int NumPolygon;				//何個のポリゴンによって構成されているか
		int *PolygonIDs;			//ポリゴンID
		exMesh() :
			PolygonIDs(nullptr)
		{
		}
		~exMesh()
		{
			delete[] PolygonIDs;
		}
	};
	int &exNumVertices;				//頂点数
	int exNumPolygons;				//ポリゴン数
	int &exNumMeshes;				//メッシュ数
	exVertex* &exVertices;			//頂点データ(注意:参照変数)
	exPolygon *exPolygons;			//ポリゴンデータ
	exMesh *exMeshes;				//メッシュデータ
	//例:i番目のメッシュが参照するべきポリゴンは(MeshStartPolygon[i])から(MeshStartPolygon[i]+MeshNumPolygon[i]-1)まで
	void Load(IDirect3DDevice9* d3d_device, const char* filename);
public:
	ExFbxMeshAmg();
	~ExFbxMeshAmg();
	void Create(IDirect3DDevice9* d3d_device, const char* filename);
	void Render(IDirect3DDevice9* d3d_device);
	void RenderWithSimpleShader(IDirect3DDevice9* d3d_device, ex_fbx_mesh_amg_blend_mode_t blend_mode);
	void RenderWithSimpleShaderMaxAlpha(IDirect3DDevice9* d3d_device);
	void AddMotion(std::string name, const char* filename)
	{
		FbxMeshAmg::AddMotion(name, filename);
	}
	void Skinning()
	{
		//	モーションが存在する場合はSkinning
		if (motion[MotionName].NumFrame > 0)
		{
			FbxMeshAmg::Skinning();
		}
	}
	void SetTransform(const D3DXMATRIX *world)
	{
		this->transform = *world;
	}
	void SetFrame(float frame)
	{
		float now_motion_frame = (float)(motion[MotionName].NumFrame);
		if (now_motion_frame)
		{
			frame = fmodf(frame, now_motion_frame - 1.0f);
		}
		Frame = frame;
	}
	float GetMotionFrame(std::string motion_name)
	{
		return (float)(motion[motion_name].NumFrame);
	}
	void Play(std::string name)
	{
		FbxMeshAmg::Play(name);
	}
	void Raycast(const D3DXVECTOR3& pos, const D3DXVECTOR3& vec, float dist, D3DXVECTOR3& out, D3DXVECTOR3& normal, int *polygon_id, int *mesh_id);
	void SphereCast(const D3DXVECTOR3& pos, const D3DXVECTOR3& vec, float radius, float dist, D3DXVECTOR3& out, D3DXVECTOR3& normal, int *polygon_id, int *mesh_id);
	void IgnoreMesh(int mesh_id, bool value)
	{
		exMeshes[mesh_id].DoIgnore = value;
	}
	void IgnoreMeshAll(bool value)
	{
		for (int i = 0; i < NumMesh;i++)
		{
			exMeshes[i].DoIgnore = value;
		}
	}
	void GetLocalAABB(D3DXVECTOR3 *minimum, D3DXVECTOR3 *maximum)
	{
		float min_x = INFINITY;
		float min_y = INFINITY;
		float min_z = INFINITY;
		float max_x = -INFINITY;
		float max_y = -INFINITY;
		float max_z = -INFINITY;
		for (int i = 0; i < NumVertices;i++)
		{
			min_x = min(min_x, Vertices[i].x);
			min_y = min(min_y, Vertices[i].y);
			min_z = min(min_z, Vertices[i].z);
			max_x = max(max_x, Vertices[i].x);
			max_y = max(max_y, Vertices[i].y);
			max_z = max(max_z, Vertices[i].z);
		}
		*minimum = D3DXVECTOR3(min_x, min_y, min_z);
		*maximum = D3DXVECTOR3(max_x, max_y, max_z);
	}
	//現在の姿勢で、指定のポリゴンの重心を得る
	void GetPolygonPos(int polygon_id, D3DXVECTOR3 *pos);
	//現在の姿勢で、指定の範囲のポリゴンの中心位置を得る
	void GetPolygonPosRange(int min_polygon_id, int max_polygon_id, D3DXVECTOR3 *pos);
	//現在の姿勢で、指定のメッシュの中心位置を得る
	void GetMeshPos(int mesh_id, D3DXVECTOR3 *pos);
};
