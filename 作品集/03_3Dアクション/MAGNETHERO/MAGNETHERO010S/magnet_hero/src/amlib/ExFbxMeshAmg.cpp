//SDK���V�F�A����(?)
#define FBXSDK_SHARED
//�e�w�b�_�[
#include "ExFbxMeshAmg.h"
//���̑��̃w�b�_�[
#include "..\mylib\mylib.h"
#include "..\shader_use\simple_shader.h"
#include "..\calc\calc.h"

//#pragma comment( lib, "release/libfbxsdk.lib")

ExFbxMeshAmg::ExFbxMeshAmg() :
	FbxMeshAmg(),
	exNumVertices(NumVertices),
	exNumMeshes(NumMesh),
	exVertices(Vertices),			//���_�f�[�^(����:�Q�ƕϐ�)
	exPolygons(nullptr),			//�|���S���f�[�^
	exMeshes(nullptr)				//���b�V���f�[�^
{
}

ExFbxMeshAmg::~ExFbxMeshAmg()
{
	delete[] exPolygons;
	delete[] exMeshes;
}
void ExFbxMeshAmg::Load(IDirect3DDevice9* d3d_device, const char* filename)
{
	FbxManager* manager = FbxManager::Create();
	FbxScene* scene = FbxScene::Create(manager, "");
	//	�t�@�C������V�[���ɓǂݍ���
	FbxImporter* importer = FbxImporter::Create(manager, "");
	importer->Initialize(filename);
	importer->Import(scene);
	importer->Destroy();

	//	���[�V�������擾
	FbxArray<FbxString*> names;
	scene->FillAnimStackNameArray(names);

	if (names != nullptr)
	{
		//	���[�V���������݂���Ƃ�
		FbxTakeInfo* take = scene->GetTakeInfo(names[0]->Buffer());
		FbxLongLong start = take->mLocalTimeSpan.GetStart().Get();
		FbxLongLong stop = take->mLocalTimeSpan.GetStop().Get();
		FbxLongLong fps60 = FbxTime::GetOneFrameValue(FbxTime::eFrames60);
		StartFrame = (int)(start / fps60);
		motion["default"].NumFrame = (int)((stop - start) / fps60);
	}
	else
	{
		StartFrame = 0;
		motion["default"].NumFrame = 0;
	}

	//	���f�����ގ����Ƃɕ���
	FbxGeometryConverter fgc(manager);
	fgc.SplitMeshesPerMaterial(scene, true);
	fgc.Triangulate(scene, true);

	NumBone = 0;

	//���b�V����
	NumMesh = scene->GetSrcObjectCount<FbxMesh>();
	//���b�V���������p��
	exMeshes = new exMesh[exNumMeshes];
	//�|���S�����v�Z��Ɨp�̕ϐ�
	int p_work = 0;
	//	���_���v�Z��Ɨp�̕ϐ�
	int work = 0;
	//���b�V���̐��������[�v
	for (int m = 0; m < NumMesh; m++)
	{
		//m�Ԗڂ̃��b�V���擾
		FbxMesh* mesh = scene->GetSrcObject<FbxMesh>(m);
		//���b�V���Ɋ܂܂�钸�_���擾
		int num = mesh->GetPolygonVertexCount();
		// ���v���_��
		work += num;
		//���b�V���Ɋ܂܂��|���S�����擾
		int NumPolygon = mesh->GetPolygonCount();
		//m�Ԗڂ̃��b�V���𖳎����Ȃ�
		exMeshes[m].DoIgnore = false;
		//���b�V���Ɋ܂܂��|���S�����ۑ�
		exMeshes[m].NumPolygon = NumPolygon;
		//�S�Ẵ|���S�������J�E���g�A�b�v
		p_work += NumPolygon;
	}
	//�|���S�����ۑ�
	exNumPolygons = p_work;
	//�|���S���������p��
	exPolygons = new exPolygon[exNumPolygons];
	{
		//���[�v��Ɨp�ϐ�
		int work = 0;
		//���[�v��Ɨp�ϐ�
		int NumPolygon = 0;
		//���b�V���̕��������[�v
		for (int i = 0; i < exNumMeshes; i++)
		{
			//���b�V���̃C���f�b�N�X
			int mesh_index = i;
			//���b�V��
			exMesh &mesh = exMeshes[i];
			//��Ɨp�ϐ��J�E���g�A�b�v
			work += NumPolygon;
			//���b�V���̃|���S����
			NumPolygon = mesh.NumPolygon;
			//�|���S��ID�������p��
			mesh.PolygonIDs = new int[NumPolygon];
			//�|���S���̐��������[�v
			for (int k = 0; k < NumPolygon; k++)
			{
				//�|���S���̃C���f�b�N�X
				int polygon_index = work + k;
				//�|���S��
				exPolygon &polygon = exPolygons[polygon_index];
				//�|���S���̃C���f�b�N�X�����b�V���ɋ�����
				mesh.PolygonIDs[k] = polygon_index;
				//�e���b�V���̌���
				polygon.ParrentMeshID = mesh_index;
				//���_�C���f�b�N�X3���̌���
				for (int m = 0; m < 3;m++)
				{
					polygon.VertexIDs[m] = polygon_index * 3 + m;
				}
			}
		}
	}
	//	���_�m��
	Vertices = new PolygonVertex[work];
	Indices = new DWORD[work];
	Weights = new WEIGHT[work];

	NumVertices = 0;
	//	������
	for (int v = 0; v < work; v++)
	{
		Weights[v].count = 0;
	}

	//�ގ����Ƃ̃|���S�����_��
	MaterialFaces = new int[NumMesh];
	MeshMaterial = new int[NumMesh];
	Textures = new IDirect3DTexture9*[NumMesh];
	NTextures = new IDirect3DTexture9*[NumMesh];
	for (int m = 0; m < NumMesh; m++)
	{
		Textures[m] = nullptr;
		NTextures[m] = nullptr;
	}

	//	���_�ǂݍ���
	for (int m = 0; m < NumMesh; m++)
	{
		FbxMesh* mesh = scene->GetSrcObject<FbxMesh>(m);
		int num = mesh->GetPolygonVertexCount();
		MaterialFaces[m] = num;

		//	���b�V���̎g�p�ގ��擾
		FbxLayerElementMaterial* LEM = mesh->GetElementMaterial();
		if (LEM != nullptr)
		{
			//	�|���S���ɓ\���Ă���ގ��ԍ�
			int material_index = LEM->GetIndexArray().GetAt(0);
			//	���b�V���ގ���material_index�Ԗڂ��擾
			FbxSurfaceMaterial* _material = mesh->GetNode()->GetSrcObject<FbxSurfaceMaterial>(material_index);
			LoadMaterial(d3d_device, m, _material);
		}
		//	�g�p�ގ��ݒ�
		MeshMaterial[m] = m;

		//	���_���ǂݍ���
		LoadPosition(mesh);		//	���W�ǂݍ���

								//	�C���f�b�N�X�ݒ�(�O�p�`����)
		int i = 0;
		for (/*int */i = 0; i < num; i += 3)
		{
			Indices[i + 0 + NumVertices] = (DWORD)(i + 2 + NumVertices);
			Indices[i + 1 + NumVertices] = (DWORD)(i + 1 + NumVertices);
			Indices[i + 2 + NumVertices] = (DWORD)(i + 0 + NumVertices);
		}

		LoadNormal(mesh);		//	�@���ǂݍ���
		LoadUV(mesh);			//	�e�N�X�`��UV
		LoadVertexColor(mesh);	//	���_�J���[�ǂݍ���

								//	�{�[���ǂݍ���
		LoadBone(mesh);
		NumVertices += num;
	}

	//NumFaces = NumVertices / 3;
	//OptimizeVertices();

	//	���_���f�[�^�ۑ�
	VerticesSrc = new PolygonVertex[NumVertices];
	memcpy(VerticesSrc, Vertices, sizeof(PolygonVertex)*NumVertices);

	//	�E�F�C�g���K��
	// �T�{�ȏ�ɂ܂������Ă�ꍇ�̂���
	for (int v = 0; v < NumVertices; v++)
	{
		float n = 0;
		//	���_�̃E�F�C�g�̍��v�l
		for (int w = 0; w < Weights[v].count; w++)
		{
			n += Weights[v].weight[w];
		}
		//	���K��
		for (int w = 0; w < Weights[v].count; w++)
		{
			Weights[v].weight[w] /= n;
		}
	}

	//	���
	scene->Destroy();
	manager->Destroy();

	Play("default");
}
void ExFbxMeshAmg::Create(IDirect3DDevice9* d3d_device, const char* filename)
{
	//�t�@�C��������菜��
	strcpy_s(FBXDir, sizeof(char) * 128, filename);
	for (int n = strlen(FBXDir) - 1; n >= 0; n--)
	{
		if (FBXDir[n] == '/' || FBXDir[n] == '\\')
		{
			FBXDir[n + 1] = '\0';
			break;
		}
	}
	//�ǂݍ���
	Load(d3d_device, filename);
	//	��񏉊���
	Frame = 0;
	position = D3DXVECTOR3(0, 0, 0);
	rotation = D3DXVECTOR3(0, 0, 0);
	scale = D3DXVECTOR3(1, 1, 1);
	Update();
}

void ExFbxMeshAmg::Render(IDirect3DDevice9 *d3d_device)
{
	d3d_device->SetTransform(D3DTS_WORLD, &transform);

	//	���[�V���������݂���ꍇ��Skinning
	if (motion[MotionName].NumFrame > 0)
	{
		Skinning();
	}

	//	d3d_device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE );
	//mylib_set_fvf(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_DIFFUSE);
	mylib_set_fvf(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1);

	int start = 0;
	for (int m = 0; m < NumMesh; m++)
	{
		//�������郁�b�V���Ȃ疳��
		if (exMeshes[m].DoIgnore)
		{
			start += MaterialFaces[m];
			continue;
		}
		//�}�e���A���ԍ����擾
		int material_no = MeshMaterial[m];
		d3d_device->SetMaterial(&material);
		//�e�N�X�`��������ꍇ�̓e�N�X�`����\��
		if (Textures[material_no] != nullptr)
		{
			d3d_device->SetTexture(0, Textures[material_no]);
		}
		//	�`��
		d3d_device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, NumVertices,
			MaterialFaces[m] / 3, &Indices[start], D3DFMT_INDEX32, Vertices, sizeof(PolygonVertex));
		start += MaterialFaces[m];
	}
	d3d_device->SetTexture(0, 0);
}

void ExFbxMeshAmg::RenderWithSimpleShader(IDirect3DDevice9* d3d_device, ex_fbx_mesh_amg_blend_mode_t blend_mode)
{
	//�s����擾
	D3DXMATRIX view;
	D3DXMATRIX projection;
	D3DXMATRIX inv_world;
	D3DXMATRIX trans_inv_world;
	D3DXMatrixTranspose(&trans_inv_world, D3DXMatrixInverse(&inv_world, nullptr, &transform));
	d3d_device->GetTransform(D3DTS_VIEW, &view);
	d3d_device->GetTransform(D3DTS_PROJECTION, &projection);
	//�V�F�[�_�[�ɒl��n��
	D3DXMATRIX wvp = transform * view * projection;
	float now_sec = mylib_now_sec();
	D3DXVECTOR4 light_dir = D3DXVECTOR4(0.5f, sinf(now_sec) * 0.1f + 0.9f, 0.0f, 1.0f);
	D3DXVECTOR4 light_intensity = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	D3DXVECTOR4 ambient = D3DXVECTOR4(0.9f, 0.9f, 0.9f, 0.9f);
	simple_shader_set_light_dir(&light_dir);
	simple_shader_set_light_intensity(&light_intensity);
	simple_shader_set_ambient(&ambient);
	simple_shader_set_wvp(&wvp);
	simple_shader_set_wit(&trans_inv_world);
	simple_shader_begin_scene((simple_shader_pass_t)(int)(blend_mode));
	//	���[�V���������݂���ꍇ��Skinning
	if (motion[MotionName].NumFrame > 0)
	{
		Skinning();
	}
	mylib_set_fvf(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1);
	int start = 0;
	for (int m = 0; m < NumMesh; m++)
	{
		//�������郁�b�V���Ȃ疳��
		if (exMeshes[m].DoIgnore)
		{
			start += MaterialFaces[m];
			continue;
		}
		//�}�e���A���ԍ����擾
		int material_no = MeshMaterial[m];
		D3DXVECTOR4 diffuse = D3DXVECTOR4(material.Diffuse.r, material.Diffuse.g, material.Diffuse.b, material.Diffuse.a);
		simple_shader_set_diffuse(&diffuse);
		simple_shader_commit();
		//�e�N�X�`��������ꍇ�̓e�N�X�`����\��
		if (Textures[material_no] != nullptr)
		{
			simple_shader_set_texture(Textures[material_no]);
			simple_shader_commit();
		}
		//	�`��
		d3d_device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, NumVertices,
			MaterialFaces[m] / 3, &Indices[start], D3DFMT_INDEX32, Vertices, sizeof(PolygonVertex));
		start += MaterialFaces[m];
	}
	simple_shader_set_texture(nullptr);
	simple_shader_end_scene();
}

void ExFbxMeshAmg::RenderWithSimpleShaderMaxAlpha(IDirect3DDevice9* d3d_device)
{
	//�s����擾
	D3DXMATRIX view;
	D3DXMATRIX projection;
	D3DXMATRIX inv_world;
	D3DXMATRIX trans_inv_world;
	D3DXMatrixTranspose(&trans_inv_world, D3DXMatrixInverse(&inv_world, nullptr, &transform));
	d3d_device->GetTransform(D3DTS_VIEW, &view);
	d3d_device->GetTransform(D3DTS_PROJECTION, &projection);
	//�V�F�[�_�[�ɒl��n��
	D3DXMATRIX wvp = transform * view * projection;
	float now_sec = mylib_now_sec();
	D3DXVECTOR4 light_dir = D3DXVECTOR4(0.5f, sinf(now_sec) * 0.1f + 0.9f, 0.0f, 1.0f);
	D3DXVECTOR4 light_intensity = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	D3DXVECTOR4 ambient = D3DXVECTOR4(0.9f, 0.9f, 0.9f, 0.9f);
	simple_shader_max_alpha_set_light_dir(&light_dir);
	simple_shader_max_alpha_set_light_intensity(&light_intensity);
	simple_shader_max_alpha_set_ambient(&ambient);
	simple_shader_max_alpha_set_wvp(&wvp);
	simple_shader_max_alpha_set_wit(&trans_inv_world);
	simple_shader_max_alpha_begin_scene();
	//	���[�V���������݂���ꍇ��Skinning
	if (motion[MotionName].NumFrame > 0)
	{
		Skinning();
	}
	mylib_set_fvf(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1);
	int start = 0;
	for (int m = 0; m < NumMesh; m++)
	{
		//�������郁�b�V���Ȃ疳��
		if (exMeshes[m].DoIgnore)
		{
			start += MaterialFaces[m];
			continue;
		}
		//�}�e���A���ԍ����擾
		int material_no = MeshMaterial[m];
		D3DXVECTOR4 diffuse = D3DXVECTOR4(material.Diffuse.r, material.Diffuse.g, material.Diffuse.b, material.Diffuse.a);
		simple_shader_max_alpha_set_diffuse(&diffuse);
		simple_shader_max_alpha_commit();
		//�e�N�X�`��������ꍇ�̓e�N�X�`����\��
		if (Textures[material_no] != nullptr)
		{
			simple_shader_max_alpha_set_texture(Textures[material_no]);
			simple_shader_max_alpha_commit();
		}
		//	�`��
		d3d_device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, NumVertices,
			MaterialFaces[m] / 3, &Indices[start], D3DFMT_INDEX32, Vertices, sizeof(PolygonVertex));
		start += MaterialFaces[m];
	}
	simple_shader_max_alpha_set_texture(nullptr);
	simple_shader_max_alpha_end_scene();
}

void  ExFbxMeshAmg::Raycast(const D3DXVECTOR3& pos, const D3DXVECTOR3& vec, float dist, D3DXVECTOR3& out, D3DXVECTOR3& normal, int *polygon_id, int *mesh_id)
{
	int		ret = -1;
	int NumFace = NumVertices / 3;

	D3DXVECTOR3 v1, v2, v3;
	D3DXVECTOR3	N;
	D3DXVECTOR3	line1, line2, line3;
	D3DXVECTOR3	temp;
	D3DXVECTOR3	cp;

	D3DXVECTOR3 p;
	D3DXVECTOR3 p1, p2, p3;

	float neart = dist;

	//	�S�|���S�����[�v
	for (int face = 0; face < NumFace; face++)
	{
		if (exMeshes[exPolygons[face].ParrentMeshID].DoIgnore)
		{
			continue;
		}
		//	�ʒ��_�擾
		int a = Indices[face * 3 + 0];
		v1.x = Vertices[a].x;	v1.y = Vertices[a].y;	v1.z = Vertices[a].z;

		int b = Indices[face * 3 + 1];
		v2.x = Vertices[b].x;	v2.y = Vertices[b].y;	v2.z = Vertices[b].z;

		int c = Indices[face * 3 + 2];
		v3.x = Vertices[c].x;	v3.y = Vertices[c].y;	v3.z = Vertices[c].z;

		//	3��
		line1 = v2 - v1; // v1 -> v2
		line2 = v3 - v2; // v2 -> v3
		line3 = v1 - v3; // v3 -> v1

						 //	�ʖ@��(�Q�ӂ̊O��)
		D3DXVec3Cross(&N, &line1, &line2);

		//	(pos->1�_)��@���Ɏˉe(������)
		p = v1 - pos;
		float tp = D3DXVec3Dot(&p, &N);
		//	���C��@���Ɏˉe
		float tv = D3DXVec3Dot(&vec, &N);
		//	�\������
		if (tv >= 0) continue;

		//	��_�Z�o
		float t = tp / tv;
		cp = vec * t + pos;

		//	���΂�NG
		if (t < 0) continue;
		//	��ԋ߂��_�̂�
		if (t > neart) continue;

		//	���_����
		p1 = v1 - cp; //��_->1�_
		D3DXVec3Cross(&temp, &p1, &line1);
		//�@���Ό���(�}90���ȏ�)�Ȃ�ΊO
		if (D3DXVec3Dot(&temp, &N) < 0)
			continue;

		p2 = v2 - cp; //��_->1�_
		D3DXVec3Cross(&temp, &p2, &line2);
		//�@���Ό���(�}90���ȏ�)�Ȃ�ΊO
		if (D3DXVec3Dot(&temp, &N) < 0)
			continue;

		p3 = v3 - cp; //��_->1�_
		D3DXVec3Cross(&temp, &p3, &line3);
		//�@���Ό���(�}90���ȏ�)�Ȃ�ΊO
		if (D3DXVec3Dot(&temp, &N) < 0)
			continue;

		//	���ݒ�
		out = cp;
		normal = N;
		ret = face;
		neart = t;
	}
	//	�@�����K��
	D3DXVec3Normalize(&normal, &normal);

	if (polygon_id)
	{
		*polygon_id = ret;
	}

	if (mesh_id)
	{
		if (ret >= 0)
		{
			*mesh_id = exPolygons[ret].ParrentMeshID;
		}
		else
		{
			*mesh_id = -1;
		}
	}
}

void  ExFbxMeshAmg::SphereCast(const D3DXVECTOR3& pos, const D3DXVECTOR3& vec, float radius, float dist, D3DXVECTOR3& out, D3DXVECTOR3& normal, int *polygon_id, int *mesh_id)
{
	int		ret = -1;
	int NumFace = NumVertices / 3;

	D3DXVECTOR3 v1, v2, v3;
	D3DXVECTOR3	N;
	D3DXVECTOR3	line1, line2, line3;
	D3DXVECTOR3	temp;
	D3DXVECTOR3	cp;

	D3DXVECTOR3 p;
	D3DXVECTOR3 p1, p2, p3;

	D3DXVECTOR3 g;
	D3DXVECTOR3 g_p;
	D3DXVECTOR3 g_to;
	D3DXVECTOR3 g_sub;
	D3DXVECTOR3 g_correct;

	float neart = dist;

	//	�S�|���S�����[�v
	for (int face = 0; face < NumFace; face++)
	{
		if (exMeshes[exPolygons[face].ParrentMeshID].DoIgnore)
		{
			continue;
		}
		//	�ʒ��_�擾
		int a = Indices[face * 3 + 0];
		v1.x = Vertices[a].x;	v1.y = Vertices[a].y;	v1.z = Vertices[a].z;

		int b = Indices[face * 3 + 1];
		v2.x = Vertices[b].x;	v2.y = Vertices[b].y;	v2.z = Vertices[b].z;

		int c = Indices[face * 3 + 2];
		v3.x = Vertices[c].x;	v3.y = Vertices[c].y;	v3.z = Vertices[c].z;

		//�d�S���擾
		g = (v1 + v2 + v3) / 3.0f;

		//�d�S������܂ł̕������擾
		ClosestPointPointSegment(pos, pos + vec, g, &g_p);
		g_sub = g_p - g;
		D3DXVec3Normalize(&g_to, &g_sub);

		//�ړ��ʂ��擾
		float len = D3DXVec3Length(&g_sub);
		float r = min(radius, len);
		g_correct = g_to * r;

		//3�̒��_�𓮂���
		v1 += g_correct;
		v2 += g_correct;
		v3 += g_correct;

		//	3��
		line1 = v2 - v1; // v1 -> v2
		line2 = v3 - v2; // v2 -> v3
		line3 = v1 - v3; // v3 -> v1

						 //	�ʖ@��(�Q�ӂ̊O��)
		D3DXVec3Cross(&N, &line1, &line2);

		//	(pos->1�_)��@���Ɏˉe(������)
		p = v1 - pos;
		float tp = D3DXVec3Dot(&p, &N);
		//	���C��@���Ɏˉe
		float tv = D3DXVec3Dot(&vec, &N);
		//	�\������
		if (tv >= 0.0f) continue;

		//	��_�Z�o
		float t = tp / tv;
		cp = vec * t + pos;

		//	���΂�NG
		if (t < 0.0f) continue;
		//	��ԋ߂��_�̂�
		if (t > neart) continue;

		//	���_����
		p1 = v1 - cp; //��_->1�_
		D3DXVec3Cross(&temp, &p1, &line1);
		//�@���Ό���(�}90���ȏ�)�Ȃ�ΊO
		if (D3DXVec3Dot(&temp, &N) < 0)
			continue;

		p2 = v2 - cp; //��_->1�_
		D3DXVec3Cross(&temp, &p2, &line2);
		//�@���Ό���(�}90���ȏ�)�Ȃ�ΊO
		if (D3DXVec3Dot(&temp, &N) < 0)
			continue;

		p3 = v3 - cp; //��_->1�_
		D3DXVec3Cross(&temp, &p3, &line3);
		//�@���Ό���(�}90���ȏ�)�Ȃ�ΊO
		if (D3DXVec3Dot(&temp, &N) < 0)
			continue;

		//	���ݒ�
		out = cp;
		normal = N;
		ret = face;
		neart = t;
	}
	//	�@�����K��
	D3DXVec3Normalize(&normal, &normal);

	if (polygon_id)
	{
		*polygon_id = ret;
	}

	if (mesh_id)
	{
		if (ret >= 0)
		{
			*mesh_id = exPolygons[ret].ParrentMeshID;
		}
		else
		{
			*mesh_id = -1;
		}
	}
}

void ExFbxMeshAmg::GetPolygonPos(int polygon_id, D3DXVECTOR3 *pos)
{
	float divide = 3.0f;
	D3DXVECTOR3 p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	exPolygon &polygon = exPolygons[polygon_id];
	for (int i = 0; i < 3; i++)
	{
		const int index = polygon.VertexIDs[i];
		p.x += Vertices[index].x;
		p.y += Vertices[index].y;
		p.z += Vertices[index].z;
	}
	p /= divide;
	D3DXVec3TransformCoord(pos, &p, &transform);
}

void ExFbxMeshAmg::GetPolygonPosRange(int min_polygon_id, int max_polygon_id, D3DXVECTOR3 *pos)
{
	int num = max_polygon_id - min_polygon_id;
	float divide = num * 3.0f;
	D3DXVECTOR3 p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < num; i++)
	{
		exPolygon &polygon = exPolygons[min_polygon_id + i];
		for (int k = 0; k < 3; k++)
		{
			const int index = polygon.VertexIDs[k];
			p.x += Vertices[index].x;
			p.y += Vertices[index].y;
			p.z += Vertices[index].z;
		}
	}
	p /= divide;
	D3DXVec3TransformCoord(pos, &p, &transform);
}

void ExFbxMeshAmg::GetMeshPos(int mesh_id, D3DXVECTOR3 *pos)
{
	exMesh &mesh = exMeshes[mesh_id];
	int num = mesh.NumPolygon;
	float divide = num * 3.0f;
	D3DXVECTOR3 p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < num; i++)
	{
		exPolygon &polygon = exPolygons[mesh.PolygonIDs[i]];
		for (int k = 0; k < 3; k++)
		{
			const int index = polygon.VertexIDs[k];
			p.x += Vertices[index].x;
			p.y += Vertices[index].y;
			p.z += Vertices[index].z;
		}
	}
	p /= divide;
	D3DXVec3TransformCoord(pos, &p, &transform);
}