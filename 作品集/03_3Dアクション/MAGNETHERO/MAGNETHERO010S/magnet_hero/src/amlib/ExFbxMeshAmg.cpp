//SDKをシェアする(?)
#define FBXSDK_SHARED
//親ヘッダー
#include "ExFbxMeshAmg.h"
//その他のヘッダー
#include "..\mylib\mylib.h"
#include "..\shader_use\simple_shader.h"
#include "..\calc\calc.h"

//#pragma comment( lib, "release/libfbxsdk.lib")

ExFbxMeshAmg::ExFbxMeshAmg() :
	FbxMeshAmg(),
	exNumVertices(NumVertices),
	exNumMeshes(NumMesh),
	exVertices(Vertices),			//頂点データ(注意:参照変数)
	exPolygons(nullptr),			//ポリゴンデータ
	exMeshes(nullptr)				//メッシュデータ
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
	//	ファイルからシーンに読み込み
	FbxImporter* importer = FbxImporter::Create(manager, "");
	importer->Initialize(filename);
	importer->Import(scene);
	importer->Destroy();

	//	モーション情報取得
	FbxArray<FbxString*> names;
	scene->FillAnimStackNameArray(names);

	if (names != nullptr)
	{
		//	モーションが存在するとき
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

	//	モデルを材質ごとに分割
	FbxGeometryConverter fgc(manager);
	fgc.SplitMeshesPerMaterial(scene, true);
	fgc.Triangulate(scene, true);

	NumBone = 0;

	//メッシュ数
	NumMesh = scene->GetSrcObjectCount<FbxMesh>();
	//メッシュメモリ用意
	exMeshes = new exMesh[exNumMeshes];
	//ポリゴン数計算作業用の変数
	int p_work = 0;
	//	頂点数計算作業用の変数
	int work = 0;
	//メッシュの数だけループ
	for (int m = 0; m < NumMesh; m++)
	{
		//m番目のメッシュ取得
		FbxMesh* mesh = scene->GetSrcObject<FbxMesh>(m);
		//メッシュに含まれる頂点数取得
		int num = mesh->GetPolygonVertexCount();
		// 合計頂点数
		work += num;
		//メッシュに含まれるポリゴン数取得
		int NumPolygon = mesh->GetPolygonCount();
		//m番目のメッシュを無視しない
		exMeshes[m].DoIgnore = false;
		//メッシュに含まれるポリゴン数保存
		exMeshes[m].NumPolygon = NumPolygon;
		//全てのポリゴン数をカウントアップ
		p_work += NumPolygon;
	}
	//ポリゴン数保存
	exNumPolygons = p_work;
	//ポリゴンメモリ用意
	exPolygons = new exPolygon[exNumPolygons];
	{
		//ループ作業用変数
		int work = 0;
		//ループ作業用変数
		int NumPolygon = 0;
		//メッシュの分だけループ
		for (int i = 0; i < exNumMeshes; i++)
		{
			//メッシュのインデックス
			int mesh_index = i;
			//メッシュ
			exMesh &mesh = exMeshes[i];
			//作業用変数カウントアップ
			work += NumPolygon;
			//メッシュのポリゴン数
			NumPolygon = mesh.NumPolygon;
			//ポリゴンIDメモリ用意
			mesh.PolygonIDs = new int[NumPolygon];
			//ポリゴンの数だけループ
			for (int k = 0; k < NumPolygon; k++)
			{
				//ポリゴンのインデックス
				int polygon_index = work + k;
				//ポリゴン
				exPolygon &polygon = exPolygons[polygon_index];
				//ポリゴンのインデックスをメッシュに教える
				mesh.PolygonIDs[k] = polygon_index;
				//親メッシュの決定
				polygon.ParrentMeshID = mesh_index;
				//頂点インデックス3つ分の決定
				for (int m = 0; m < 3;m++)
				{
					polygon.VertexIDs[m] = polygon_index * 3 + m;
				}
			}
		}
	}
	//	頂点確保
	Vertices = new PolygonVertex[work];
	Indices = new DWORD[work];
	Weights = new WEIGHT[work];

	NumVertices = 0;
	//	初期化
	for (int v = 0; v < work; v++)
	{
		Weights[v].count = 0;
	}

	//材質ごとのポリゴン頂点数
	MaterialFaces = new int[NumMesh];
	MeshMaterial = new int[NumMesh];
	Textures = new IDirect3DTexture9*[NumMesh];
	NTextures = new IDirect3DTexture9*[NumMesh];
	for (int m = 0; m < NumMesh; m++)
	{
		Textures[m] = nullptr;
		NTextures[m] = nullptr;
	}

	//	頂点読み込み
	for (int m = 0; m < NumMesh; m++)
	{
		FbxMesh* mesh = scene->GetSrcObject<FbxMesh>(m);
		int num = mesh->GetPolygonVertexCount();
		MaterialFaces[m] = num;

		//	メッシュの使用材質取得
		FbxLayerElementMaterial* LEM = mesh->GetElementMaterial();
		if (LEM != nullptr)
		{
			//	ポリゴンに貼られている材質番号
			int material_index = LEM->GetIndexArray().GetAt(0);
			//	メッシュ材質のmaterial_index番目を取得
			FbxSurfaceMaterial* _material = mesh->GetNode()->GetSrcObject<FbxSurfaceMaterial>(material_index);
			LoadMaterial(d3d_device, m, _material);
		}
		//	使用材質設定
		MeshMaterial[m] = m;

		//	頂点情報読み込み
		LoadPosition(mesh);		//	座標読み込み

								//	インデックス設定(三角形ごと)
		int i = 0;
		for (/*int */i = 0; i < num; i += 3)
		{
			Indices[i + 0 + NumVertices] = (DWORD)(i + 2 + NumVertices);
			Indices[i + 1 + NumVertices] = (DWORD)(i + 1 + NumVertices);
			Indices[i + 2 + NumVertices] = (DWORD)(i + 0 + NumVertices);
		}

		LoadNormal(mesh);		//	法線読み込み
		LoadUV(mesh);			//	テクスチャUV
		LoadVertexColor(mesh);	//	頂点カラー読み込み

								//	ボーン読み込み
		LoadBone(mesh);
		NumVertices += num;
	}

	//NumFaces = NumVertices / 3;
	//OptimizeVertices();

	//	頂点元データ保存
	VerticesSrc = new PolygonVertex[NumVertices];
	memcpy(VerticesSrc, Vertices, sizeof(PolygonVertex)*NumVertices);

	//	ウェイト正規化
	// ５本以上にまたっがてる場合のため
	for (int v = 0; v < NumVertices; v++)
	{
		float n = 0;
		//	頂点のウェイトの合計値
		for (int w = 0; w < Weights[v].count; w++)
		{
			n += Weights[v].weight[w];
		}
		//	正規化
		for (int w = 0; w < Weights[v].count; w++)
		{
			Weights[v].weight[w] /= n;
		}
	}

	//	解放
	scene->Destroy();
	manager->Destroy();

	Play("default");
}
void ExFbxMeshAmg::Create(IDirect3DDevice9* d3d_device, const char* filename)
{
	//ファイル名を取り除く
	strcpy_s(FBXDir, sizeof(char) * 128, filename);
	for (int n = strlen(FBXDir) - 1; n >= 0; n--)
	{
		if (FBXDir[n] == '/' || FBXDir[n] == '\\')
		{
			FBXDir[n + 1] = '\0';
			break;
		}
	}
	//読み込み
	Load(d3d_device, filename);
	//	情報初期化
	Frame = 0;
	position = D3DXVECTOR3(0, 0, 0);
	rotation = D3DXVECTOR3(0, 0, 0);
	scale = D3DXVECTOR3(1, 1, 1);
	Update();
}

void ExFbxMeshAmg::Render(IDirect3DDevice9 *d3d_device)
{
	d3d_device->SetTransform(D3DTS_WORLD, &transform);

	//	モーションが存在する場合はSkinning
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
		//無視するメッシュなら無視
		if (exMeshes[m].DoIgnore)
		{
			start += MaterialFaces[m];
			continue;
		}
		//マテリアル番号を取得
		int material_no = MeshMaterial[m];
		d3d_device->SetMaterial(&material);
		//テクスチャがある場合はテクスチャを貼る
		if (Textures[material_no] != nullptr)
		{
			d3d_device->SetTexture(0, Textures[material_no]);
		}
		//	描画
		d3d_device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, NumVertices,
			MaterialFaces[m] / 3, &Indices[start], D3DFMT_INDEX32, Vertices, sizeof(PolygonVertex));
		start += MaterialFaces[m];
	}
	d3d_device->SetTexture(0, 0);
}

void ExFbxMeshAmg::RenderWithSimpleShader(IDirect3DDevice9* d3d_device, ex_fbx_mesh_amg_blend_mode_t blend_mode)
{
	//行列を取得
	D3DXMATRIX view;
	D3DXMATRIX projection;
	D3DXMATRIX inv_world;
	D3DXMATRIX trans_inv_world;
	D3DXMatrixTranspose(&trans_inv_world, D3DXMatrixInverse(&inv_world, nullptr, &transform));
	d3d_device->GetTransform(D3DTS_VIEW, &view);
	d3d_device->GetTransform(D3DTS_PROJECTION, &projection);
	//シェーダーに値を渡す
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
	//	モーションが存在する場合はSkinning
	if (motion[MotionName].NumFrame > 0)
	{
		Skinning();
	}
	mylib_set_fvf(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1);
	int start = 0;
	for (int m = 0; m < NumMesh; m++)
	{
		//無視するメッシュなら無視
		if (exMeshes[m].DoIgnore)
		{
			start += MaterialFaces[m];
			continue;
		}
		//マテリアル番号を取得
		int material_no = MeshMaterial[m];
		D3DXVECTOR4 diffuse = D3DXVECTOR4(material.Diffuse.r, material.Diffuse.g, material.Diffuse.b, material.Diffuse.a);
		simple_shader_set_diffuse(&diffuse);
		simple_shader_commit();
		//テクスチャがある場合はテクスチャを貼る
		if (Textures[material_no] != nullptr)
		{
			simple_shader_set_texture(Textures[material_no]);
			simple_shader_commit();
		}
		//	描画
		d3d_device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, NumVertices,
			MaterialFaces[m] / 3, &Indices[start], D3DFMT_INDEX32, Vertices, sizeof(PolygonVertex));
		start += MaterialFaces[m];
	}
	simple_shader_set_texture(nullptr);
	simple_shader_end_scene();
}

void ExFbxMeshAmg::RenderWithSimpleShaderMaxAlpha(IDirect3DDevice9* d3d_device)
{
	//行列を取得
	D3DXMATRIX view;
	D3DXMATRIX projection;
	D3DXMATRIX inv_world;
	D3DXMATRIX trans_inv_world;
	D3DXMatrixTranspose(&trans_inv_world, D3DXMatrixInverse(&inv_world, nullptr, &transform));
	d3d_device->GetTransform(D3DTS_VIEW, &view);
	d3d_device->GetTransform(D3DTS_PROJECTION, &projection);
	//シェーダーに値を渡す
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
	//	モーションが存在する場合はSkinning
	if (motion[MotionName].NumFrame > 0)
	{
		Skinning();
	}
	mylib_set_fvf(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1);
	int start = 0;
	for (int m = 0; m < NumMesh; m++)
	{
		//無視するメッシュなら無視
		if (exMeshes[m].DoIgnore)
		{
			start += MaterialFaces[m];
			continue;
		}
		//マテリアル番号を取得
		int material_no = MeshMaterial[m];
		D3DXVECTOR4 diffuse = D3DXVECTOR4(material.Diffuse.r, material.Diffuse.g, material.Diffuse.b, material.Diffuse.a);
		simple_shader_max_alpha_set_diffuse(&diffuse);
		simple_shader_max_alpha_commit();
		//テクスチャがある場合はテクスチャを貼る
		if (Textures[material_no] != nullptr)
		{
			simple_shader_max_alpha_set_texture(Textures[material_no]);
			simple_shader_max_alpha_commit();
		}
		//	描画
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

	//	全ポリゴンループ
	for (int face = 0; face < NumFace; face++)
	{
		if (exMeshes[exPolygons[face].ParrentMeshID].DoIgnore)
		{
			continue;
		}
		//	面頂点取得
		int a = Indices[face * 3 + 0];
		v1.x = Vertices[a].x;	v1.y = Vertices[a].y;	v1.z = Vertices[a].z;

		int b = Indices[face * 3 + 1];
		v2.x = Vertices[b].x;	v2.y = Vertices[b].y;	v2.z = Vertices[b].z;

		int c = Indices[face * 3 + 2];
		v3.x = Vertices[c].x;	v3.y = Vertices[c].y;	v3.z = Vertices[c].z;

		//	3辺
		line1 = v2 - v1; // v1 -> v2
		line2 = v3 - v2; // v2 -> v3
		line3 = v1 - v3; // v3 -> v1

						 //	面法線(２辺の外積)
		D3DXVec3Cross(&N, &line1, &line2);

		//	(pos->1点)を法線に射影(≒高さ)
		p = v1 - pos;
		float tp = D3DXVec3Dot(&p, &N);
		//	レイを法線に射影
		float tv = D3DXVec3Dot(&vec, &N);
		//	表裏判定
		if (tv >= 0) continue;

		//	交点算出
		float t = tp / tv;
		cp = vec * t + pos;

		//	反対はNG
		if (t < 0) continue;
		//	一番近い点のみ
		if (t > neart) continue;

		//	内点判定
		p1 = v1 - cp; //交点->1点
		D3DXVec3Cross(&temp, &p1, &line1);
		//　反対向き(±90°以上)ならば外
		if (D3DXVec3Dot(&temp, &N) < 0)
			continue;

		p2 = v2 - cp; //交点->1点
		D3DXVec3Cross(&temp, &p2, &line2);
		//　反対向き(±90°以上)ならば外
		if (D3DXVec3Dot(&temp, &N) < 0)
			continue;

		p3 = v3 - cp; //交点->1点
		D3DXVec3Cross(&temp, &p3, &line3);
		//　反対向き(±90°以上)ならば外
		if (D3DXVec3Dot(&temp, &N) < 0)
			continue;

		//	情報設定
		out = cp;
		normal = N;
		ret = face;
		neart = t;
	}
	//	法線正規化
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

	//	全ポリゴンループ
	for (int face = 0; face < NumFace; face++)
	{
		if (exMeshes[exPolygons[face].ParrentMeshID].DoIgnore)
		{
			continue;
		}
		//	面頂点取得
		int a = Indices[face * 3 + 0];
		v1.x = Vertices[a].x;	v1.y = Vertices[a].y;	v1.z = Vertices[a].z;

		int b = Indices[face * 3 + 1];
		v2.x = Vertices[b].x;	v2.y = Vertices[b].y;	v2.z = Vertices[b].z;

		int c = Indices[face * 3 + 2];
		v3.x = Vertices[c].x;	v3.y = Vertices[c].y;	v3.z = Vertices[c].z;

		//重心を取得
		g = (v1 + v2 + v3) / 3.0f;

		//重心から線までの方向を取得
		ClosestPointPointSegment(pos, pos + vec, g, &g_p);
		g_sub = g_p - g;
		D3DXVec3Normalize(&g_to, &g_sub);

		//移動量を取得
		float len = D3DXVec3Length(&g_sub);
		float r = min(radius, len);
		g_correct = g_to * r;

		//3つの頂点を動かす
		v1 += g_correct;
		v2 += g_correct;
		v3 += g_correct;

		//	3辺
		line1 = v2 - v1; // v1 -> v2
		line2 = v3 - v2; // v2 -> v3
		line3 = v1 - v3; // v3 -> v1

						 //	面法線(２辺の外積)
		D3DXVec3Cross(&N, &line1, &line2);

		//	(pos->1点)を法線に射影(≒高さ)
		p = v1 - pos;
		float tp = D3DXVec3Dot(&p, &N);
		//	レイを法線に射影
		float tv = D3DXVec3Dot(&vec, &N);
		//	表裏判定
		if (tv >= 0.0f) continue;

		//	交点算出
		float t = tp / tv;
		cp = vec * t + pos;

		//	反対はNG
		if (t < 0.0f) continue;
		//	一番近い点のみ
		if (t > neart) continue;

		//	内点判定
		p1 = v1 - cp; //交点->1点
		D3DXVec3Cross(&temp, &p1, &line1);
		//　反対向き(±90°以上)ならば外
		if (D3DXVec3Dot(&temp, &N) < 0)
			continue;

		p2 = v2 - cp; //交点->1点
		D3DXVec3Cross(&temp, &p2, &line2);
		//　反対向き(±90°以上)ならば外
		if (D3DXVec3Dot(&temp, &N) < 0)
			continue;

		p3 = v3 - cp; //交点->1点
		D3DXVec3Cross(&temp, &p3, &line3);
		//　反対向き(±90°以上)ならば外
		if (D3DXVec3Dot(&temp, &N) < 0)
			continue;

		//	情報設定
		out = cp;
		normal = N;
		ret = face;
		neart = t;
	}
	//	法線正規化
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