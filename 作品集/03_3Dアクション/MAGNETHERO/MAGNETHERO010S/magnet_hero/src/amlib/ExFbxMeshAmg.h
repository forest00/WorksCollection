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
	//���_�\����
	using exVertex = PolygonVertex;
	//�|���S���\����
	struct exPolygon
	{
		int ParrentMeshID;			//�e���b�V��ID
		int VertexIDs[3];			//���_ID
	};
	//���b�V���\����
	struct exMesh
	{
		bool DoIgnore;				//���̃��b�V���𖳎����邩�ǂ���
		int NumPolygon;				//���̃|���S���ɂ���č\������Ă��邩
		int *PolygonIDs;			//�|���S��ID
		exMesh() :
			PolygonIDs(nullptr)
		{
		}
		~exMesh()
		{
			delete[] PolygonIDs;
		}
	};
	int &exNumVertices;				//���_��
	int exNumPolygons;				//�|���S����
	int &exNumMeshes;				//���b�V����
	exVertex* &exVertices;			//���_�f�[�^(����:�Q�ƕϐ�)
	exPolygon *exPolygons;			//�|���S���f�[�^
	exMesh *exMeshes;				//���b�V���f�[�^
	//��:i�Ԗڂ̃��b�V�����Q�Ƃ���ׂ��|���S����(MeshStartPolygon[i])����(MeshStartPolygon[i]+MeshNumPolygon[i]-1)�܂�
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
		//	���[�V���������݂���ꍇ��Skinning
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
	//���݂̎p���ŁA�w��̃|���S���̏d�S�𓾂�
	void GetPolygonPos(int polygon_id, D3DXVECTOR3 *pos);
	//���݂̎p���ŁA�w��͈̔͂̃|���S���̒��S�ʒu�𓾂�
	void GetPolygonPosRange(int min_polygon_id, int max_polygon_id, D3DXVECTOR3 *pos);
	//���݂̎p���ŁA�w��̃��b�V���̒��S�ʒu�𓾂�
	void GetMeshPos(int mesh_id, D3DXVECTOR3 *pos);
};
