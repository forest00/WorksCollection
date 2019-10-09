#define FBXSDK_SHARED
//#include "DX9System.h"
#define _CRT_SECURE_NO_WARNINGS
#include "FbxMeshAmg.h"


#pragma comment( lib, "release/libfbxsdk.lib")
//****************************************************************
//
//	更新
//
//****************************************************************
//------------------------------------------------
//	更新
//------------------------------------------------
void FbxMeshAmg::Update()
{
	D3DXMATRIX mat;
	//D3DXMatrixIdentity(&mat);
	//	ワールド変換行列(=姿勢)
	D3DXMatrixRotationYawPitchRoll(
		&mat,
		rotation.y, rotation.x, rotation.z
	);

	mat._11 *= scale.x;
	mat._12 *= scale.x;
	mat._13 *= scale.x;
	mat._21 *= scale.y;
	mat._22 *= scale.y;
	mat._23 *= scale.y;
	mat._31 *= scale.z;
	mat._32 *= scale.z;
	mat._33 *= scale.z;

	mat._41 = position.x;	// X座標
	mat._42 = position.y;	// Y座標
	mat._43 = position.z;	// Z座標

							//	行列保存
	transform = mat;
}

//------------------------------------------------
//	アニメーション
//------------------------------------------------
void FbxMeshAmg::Animate(float sec)
{
	float DeltaTime = sec;
	//	モーション時間の更新
	Frame += DeltaTime * 60;
	//	ループチェック
	if (Frame >= motion[MotionName].NumFrame - 1)
	{
		// ループ
		Frame = 0;		// 全体をループ
	}
}

//****************************************************************
//
//	描画
//
//****************************************************************
//------------------------------------------------
//	描画
//------------------------------------------------
void FbxMeshAmg::Render(IDirect3DDevice9* device)
{
	device->SetTransform(D3DTS_WORLD, &transform);

	//	モーションが存在する場合はSkinning
	if (motion[MotionName].NumFrame > 0)
	{
		Skinning();
	}

	//	device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE );
	//device->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_DIFFUSE);
	device->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1);

	int start = 0;
	for (int m = 0; m < NumMesh; m++)
	{
		int material_no = MeshMaterial[m];
		device->SetMaterial(&material);
		if (Textures[material_no] != nullptr)
		{
			device->SetTexture(0, Textures[material_no]);
		}
		//	描画
		device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, NumVertices,
			MaterialFaces[m] / 3, &Indices[start], D3DFMT_INDEX32, Vertices, sizeof(PolygonVertex));
		start += MaterialFaces[m];
	}
	device->SetTexture(0, 0);
}


//****************************************************************

//***********
// テクスチャ多重読み込み禁止
char TextureName[1000][128];
int  TextureCount[1000]; // 参照カウンタ
IDirect3DTexture9* TextureSource[1000];
bool IsInitializeTexture = false;

void InitTexture()
{
	for (int i = 0; i < 1000; i++)
	{
		TextureName[i][0] = '\0';
		TextureSource[i] = nullptr;
		TextureCount[i] = 0;
	}
	IsInitializeTexture = true;
}

IDirect3DTexture9* LoadTexture(IDirect3DDevice9* device, char* filename)
{
	if (IsInitializeTexture == false)
	{
		InitTexture();
	}
	//	既に存在する場合
	for (int i = 0; i < 1000; i++)
	{
		if (TextureName[i][0] == '\0') continue;
		if (strcmp(TextureName[i], filename) == 0)
		{
			TextureCount[i]++;
			return TextureSource[i];
		}
	}

	//	空き場所に読み込み
	for (int i = 0; i < 1000; i++)
	{
		if (TextureName[i][0] != '\0') continue;

		IDirect3DTexture9* tex = nullptr;
		D3DXCreateTextureFromFile(device, filename, &tex);
		TextureSource[i] = tex;
		strcpy(TextureName[i], filename);
		TextureCount[i] = 1;
		return tex;
	}
	return nullptr;
}

void ReleaseTexture(IDirect3DTexture9* tex)
{
	//	検索
	for (int i = 0; i < 1000; i++)
	{
		if (TextureSource[i] != tex) continue;
		TextureCount[i]--;
		if (TextureCount[i] <= 0)
		{
			if (TextureSource[i] != nullptr)
			{
				TextureSource[i]->Release();
				TextureSource[i] = nullptr;
			}
			TextureName[i][0] = '\0';
		}
		return;
	}
	//	管理外のテクスチャ
	if (tex != nullptr)
	{
		tex->Release();
	}
}


//****************************************************************
//
//	初期化
//
//****************************************************************
FbxMeshAmg::FbxMeshAmg()
{
	//	材質設定
	//	アンビエント(環境)カラー
	material.Ambient.r = 1.0f;
	material.Ambient.g = 1.0f;
	material.Ambient.b = 1.0f;
	material.Ambient.a = 1.0f;
	//	ディフューズ(素材の)カラー
	material.Diffuse.r = 1.0f;
	material.Diffuse.g = 1.0f;
	material.Diffuse.b = 1.0f;
	material.Diffuse.a = 1.0f;
	//	スペキュラー（テカり）カラー
	material.Specular.r = 1.0f;
	material.Specular.g = 1.0f;
	material.Specular.b = 1.0f;
	material.Specular.a = 1.0f;
	material.Power = 15.0f;
	//	エミッシブ（発光）
	material.Emissive.r = 0.0f;
	material.Emissive.g = 0.0f;
	material.Emissive.b = 0.0f;
	material.Emissive.a = 0.0f;
}

//****************************************************************
//
//	解放
//
//****************************************************************
FbxMeshAmg::~FbxMeshAmg()
{
	//	頂点情報解放
	delete[] Vertices;
	delete[] Indices;
	delete[] Weights;
	delete[] VerticesSrc;
	//	材質関連解放
	delete[] MaterialFaces;
	delete[] MeshMaterial;
	//	テクスチャ解放
	for (int i = 0; i < NumMesh; i++)
	{
		if (Textures[i] != nullptr)
			//			Textures[i]->Release();
			ReleaseTexture(Textures[i]);

		if (NTextures[i] != nullptr)
			//			NTextures[i]->Release();
			ReleaseTexture(NTextures[i]);
	}
	delete[] Textures;
	delete[] NTextures;
	//	モーション関連解放
	for (auto i = motion.begin(); i != motion.end(); i++)
	{
		Motion* M = &i->second;
		for (int bone = 0; bone < NumBone; bone++)
		{
			delete[] M->key[bone];
		}
	}
	motion.clear();
}

//****************************************************************
//
//	ファイル読み込み
//
//****************************************************************

void FbxMeshAmg::Create(IDirect3DDevice9* device, const char* filename)
{
	//ファイル名を取り除く
	strcpy(FBXDir, filename);
	for (int n = strlen(FBXDir) - 1; n >= 0; n--)
	{
		if (FBXDir[n] == '/' || FBXDir[n] == '\\')
		{
			FBXDir[n + 1] = '\0';
			break;
		}
	}

	Load(device, filename);
	//	情報初期化
	Frame = 0;
	position = D3DXVECTOR3(0, 0, 0);
	rotation = D3DXVECTOR3(0, 0, 0);
	scale = D3DXVECTOR3(1, 1, 1);
	Update();
}


void FbxMeshAmg::Load(IDirect3DDevice9* device, const char* filename)
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

	//	メッシュ数
	NumMesh = scene->GetSrcObjectCount<FbxMesh>();

	//	頂点数計算
	int work = 0;
	for (int m = 0; m < NumMesh; m++)
	{
		FbxMesh* mesh = scene->GetSrcObject<FbxMesh>(m);
		int num = mesh->GetPolygonVertexCount();
		work += num; // 合計頂点数
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
			LoadMaterial(device, m, _material);
		}
		//	使用材質設定
		MeshMaterial[m] = m;

		//	頂点情報読み込み
		LoadPosition(mesh);		//	座標読み込み

								//	インデックス設定(三角形ごと)
		int i = 0;
		for (/*int */i = 0; i < num; i += 3)
		{
			Indices[i + 0 + NumVertices] = i + 2 + NumVertices;
			Indices[i + 1 + NumVertices] = i + 1 + NumVertices;
			Indices[i + 2 + NumVertices] = i + 0 + NumVertices;
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


//****************************************************************
//
//	頂点情報読み込み
//
//****************************************************************
//------------------------------------------------
//	座標読み込み
//------------------------------------------------
void FbxMeshAmg::LoadPosition(FbxMesh* mesh)
{
	int* index = mesh->GetPolygonVertices();
	FbxVector4* source = mesh->GetControlPoints();
	// メッシュのトランスフォーム
	FbxVector4 T = mesh->GetNode()->GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxVector4 R = mesh->GetNode()->GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 S = mesh->GetNode()->GetGeometricScaling(FbxNode::eSourcePivot);
	FbxAMatrix TRS = FbxAMatrix(T, R, S);
	//	全頂点変換
	for (int v = 0; v < mesh->GetControlPointsCount(); v++)
	{
		source[v] = TRS.MultT(source[v]);
	}

	// 頂点座標読み込み
	int num = mesh->GetPolygonVertexCount();
	for (int v = 0; v < num; v++)
	{
		int vindex = index[v];

		Vertices[v + NumVertices].x = (float)-source[vindex][0];
		Vertices[v + NumVertices].y = (float)source[vindex][1];
		Vertices[v + NumVertices].z = (float)source[vindex][2];

		Vertices[v + NumVertices].tu = 0;
		Vertices[v + NumVertices].tv = 0;
		//Vertices[v + NumVertices].color = 0xFFFFFFFF;
	}
}

//------------------------------------------------
//	法線読み込み
//------------------------------------------------
void FbxMeshAmg::LoadNormal(FbxMesh* mesh)
{
	FbxArray<FbxVector4> normal;
	mesh->GetPolygonVertexNormals(normal);
	for (int v = 0; v < normal.Size(); v++)
	{
		Vertices[v + NumVertices].nx = -(float)normal[v][0];
		Vertices[v + NumVertices].ny = (float)normal[v][1];
		Vertices[v + NumVertices].nz = (float)normal[v][2];
	}
}

//------------------------------------------------
//	ＵＶ読み込み
//------------------------------------------------
void FbxMeshAmg::LoadUV(FbxMesh* mesh)
{
	FbxStringList names;
	mesh->GetUVSetNames(names);
	FbxArray<FbxVector2> uv;
	mesh->GetPolygonVertexUVs(names.GetStringAt(0), uv);
	for (int v = 0; v < uv.Size(); v++)
	{
		Vertices[v + NumVertices].tu = (float)(uv[v][0]);
		Vertices[v + NumVertices].tv = (float)(1.0 - uv[v][1]);
	}
}

//------------------------------------------------
//	頂点カラー読み込み
//------------------------------------------------
void FbxMeshAmg::LoadVertexColor(FbxMesh* mesh)
{
	//参照されてないのでコメントアウトしときます
	//int vColorLayerCount = mesh->GetElementVertexColorCount();
	if (mesh->GetElementVertexColorCount() <= 0) return;
	//    頂点カラーレイヤー取得
	FbxGeometryElementVertexColor* element = mesh->GetElementVertexColor(0);

	//  保存形式の取得
	FbxGeometryElement::EMappingMode mapmode = element->GetMappingMode();
	FbxGeometryElement::EReferenceMode refmode = element->GetReferenceMode();

	//    ポリゴン頂点に対するインデックス参照形式のみ対応
	if (mapmode == FbxGeometryElement::eByPolygonVertex)
	{
		if (refmode == FbxGeometryElement::eIndexToDirect)
		{
			FbxLayerElementArrayTemplate<int>* index = &element->GetIndexArray();
			int indexCount = index->GetCount();
			for (int j = 0; j < indexCount; j++)
			{
				// FbxColor取得
				FbxColor c = element->GetDirectArray().GetAt(index->GetAt(j));
				// DWORD型のカラー作成        
				//DWORD color = ((DWORD)(c.mAlpha * 255) << 24) + ((DWORD)(c.mRed * 255) << 16) + ((DWORD)(c.mGreen * 255) << 8) + ((DWORD)(c.mBlue * 255));
				//Vertices[j + NumVertices].color = color;
			}
		}
	}
}

//****************************************************************
//	材質読み込み
//****************************************************************
void FbxMeshAmg::LoadMaterial(IDirect3DDevice9* device, int index, FbxSurfaceMaterial * _material)
{
	FbxProperty prop = _material->FindProperty(FbxSurfaceMaterial::sDiffuse);

	//	テクスチャ読み込み
	const char* path = nullptr;
	int fileTextureCount = prop.GetSrcObjectCount<FbxFileTexture>();
	if (fileTextureCount > 0)
	{
		FbxFileTexture* FileTex = prop.GetSrcObject<FbxFileTexture>(0);
		path = FileTex->GetFileName();
	}
	else
	{
		int numLayer = prop.GetSrcObjectCount<FbxLayeredTexture>();
		if (numLayer > 0)
		{
			FbxLayeredTexture* LayerTex = prop.GetSrcObject<FbxLayeredTexture>(0);
			FbxFileTexture* FileTex = LayerTex->GetSrcObject<FbxFileTexture>(0);
			path = FileTex->GetFileName();
		}
	}
	if (path == nullptr) return;

	//  C:\\AAA\\BBB\\a.fbx  C:/AAA/BBB/a.fbx
	const char* name = &path[strlen(path)];
	for (int i = 0; i < (int)strlen(path); i++)
	{
		name--;
		if (name[0] == '/') { name++; break; }
		if (name[0] == '\\') { name++; break; }
	}
	char work[128];
	strcpy(work, FBXDir);		//"AAA/BBB/";
	//strcat(work, "texture/");	//"AAA/BBB/texture/"
	strcat(work, name);			//"AAA/BBB/texture/a.png

	char filename[128];
	strcpy(filename, work);
	//D3DXCreateTextureFromFile(device, filename, &Textures[index] );
	Textures[index] = LoadTexture(device, filename);

	//	法線マップ読み込み
	strcpy(work, FBXDir);		//"AAA/BBB/";
	//strcat(work, "texture/N");	//"AAA/BBB/texture/N"
	strcat(work, name);			//"AAA/BBB/texture/Na.png
	strcpy(filename, work);
	//D3DXCreateTextureFromFile(device, filename, &NTextures[index]);
	NTextures[index] = LoadTexture(device, filename);

}

//****************************************************************
//	ボーン検索
//****************************************************************
int FbxMeshAmg::FindBone(const char* name)
{
	int bone = -1; // 見つからない
	for (int i = 0; i < NumBone; i++)
	{
		if (strcmp(name, Bone[i].Name) == 0)
		{
			bone = i;
			break;
		}
	}
	return bone;
}

void FbxMeshAmg::LoadBone(FbxMesh* mesh)
{
	//	メッシュ頂点数
	int num = mesh->GetPolygonVertexCount();

	//	スキン情報の有無
	int skinCount = mesh->GetDeformerCount(FbxDeformer::eSkin);
	if (skinCount <= 0)
	{
		LoadMeshAnim(mesh);
		return;
	}
	FbxSkin* skin = static_cast<FbxSkin*>(mesh->GetDeformer(0, FbxDeformer::eSkin));
	//	ボーン数
	int nBone = skin->GetClusterCount();
	//	全ボーン情報取得
	for (int bone = 0; bone < nBone; bone++)
	{
		//	ボーン情報取得
		FbxCluster* cluster = skin->GetCluster(bone);
		FbxAMatrix trans;
		cluster->GetTransformMatrix(trans);
		trans.mData[0][1] *= -1;
		trans.mData[0][2] *= -1;
		trans.mData[1][0] *= -1;
		trans.mData[2][0] *= -1;
		trans.mData[3][0] *= -1;

		//	ボーン名取得
		const char* name = cluster->GetLink()->GetName();

		//	ボーン検索
		bool isNewBone = false;
		int bone_no = FindBone(name);
		if (bone_no < 0)
		{
			bone_no = NumBone;
			NumBone++;
			isNewBone = true;
		}
		if (isNewBone)
		{
			strcpy(Bone[bone_no].Name, name);
			//	オフセット行列作成
			FbxAMatrix LinkMatrix;
			cluster->GetTransformLinkMatrix(LinkMatrix);
			LinkMatrix.mData[0][1] *= -1;
			LinkMatrix.mData[0][2] *= -1;
			LinkMatrix.mData[1][0] *= -1;
			LinkMatrix.mData[2][0] *= -1;
			LinkMatrix.mData[3][0] *= -1;

			FbxAMatrix Offset = LinkMatrix.Inverse() * trans;
			FbxDouble* OffsetM = (FbxDouble*)Offset;

			//	オフセット行列保存
			for (int i = 0; i < 16; i++)
			{
				Bone[bone_no].OffsetMatrix.m[i / 4][i % 4] = (float)OffsetM[i];
			}

			//	キーフレーム読み込み
			LoadKeyFrames("default", bone_no, cluster->GetLink());
		}

		//	ウェイト読み込み
		int wgtcount = cluster->GetControlPointIndicesCount();
		int* wgtindex = cluster->GetControlPointIndices();
		double* wgt = cluster->GetControlPointWeights();

		int* index = mesh->GetPolygonVertices();

		for (int i = 0; i < wgtcount; i++)
		{
			int wgtindex2 = wgtindex[i];
			//	全ポリゴンからwgtindex2番目の頂点検索
			for (int v = 0; v < num; v++)
			{
				if (index[v] != wgtindex2) continue;
				//	頂点にウェイト保存
				int w = Weights[v + NumVertices].count;
				if (w >= 4)
				{
					continue;
				}
				Weights[v + NumVertices].bone[w] = bone_no;
				Weights[v + NumVertices].weight[w] = (float)wgt[i];
				Weights[v + NumVertices].count++;
			}
		}
	}
}

//	ボーンのないメッシュのアニメーション
void FbxMeshAmg::LoadMeshAnim(FbxMesh* mesh)
{
	FbxNode* node = mesh->GetNode();

	int bone_no = NumBone;
	strcpy(Bone[bone_no].Name, node->GetName());

	//	オフセット行列（原点に移動させる行列）
	D3DXMatrixIdentity(
		&Bone[bone_no].OffsetMatrix
	);

	//	キーフレーム読み込み
	LoadKeyFrames("default", bone_no, node);

	//	ウェイト設定
	int num = mesh->GetPolygonVertexCount();
	for (int i = 0; i < num; i++)
	{
		Weights[i + NumVertices].bone[0] = bone_no;
		Weights[i + NumVertices].weight[0] = 1.0f;
		Weights[i + NumVertices].count = 1;
	}

	NumBone++;
}

//	キーフレーム読み込み
void FbxMeshAmg::LoadKeyFrames(std::string name, int bone, FbxNode* bone_node)
{
	//	メモリ確保
	Motion* M = &motion[name];
	M->key[bone] =
		new D3DXMATRIX[M->NumFrame + 1];

	double time = StartFrame * (1.0 / 60);
	FbxTime T;
	for (int f = 0; f < motion[name].NumFrame; f++)
	{
		T.SetSecondDouble(time);
		//	T秒の姿勢行列をGet
		FbxMatrix m = bone_node->EvaluateGlobalTransform(T);
		m.mData[0][1] *= -1;// _12
		m.mData[0][2] *= -1;// _13
		m.mData[1][0] *= -1;// _21
		m.mData[2][0] *= -1;// _31
		m.mData[3][0] *= -1;// _41

		FbxDouble* mat = (FbxDouble*)m;
		for (int i = 0; i < 16; i++)
		{
			motion[name].key[bone][f].m[i / 4][i % 4] = (float)mat[i];
		}

		time += 1.0 / 60.0;
	}
}


//	ボーン行列の補間
void MatrixInterporate(D3DXMATRIX& out, D3DXMATRIX& A, D3DXMATRIX B, float rate)
{
	out = A * (1.0f - rate) + B * rate;
}

void FbxMeshAmg::Skinning()
{
	//	int motion_no = MotionNo;
	Motion* M = &motion[MotionName];
	if (M == nullptr) return;

	//	配列用変数
	int f = (int)Frame;
	//	行列準備
	D3DXMATRIX KeyMatrix[256];
	for (int b = 0; b < NumBone; b++)
	{
		//	行列補間
		D3DXMATRIX m;
		MatrixInterporate(m, M->key[b][f], M->key[b][f + 1], Frame - (int)Frame);
		Bone[b].transform = m;
		//	キーフレーム
		KeyMatrix[b] = Bone[b].OffsetMatrix * m;
	}

	//	頂点変形
	for (int v = 0; v < NumVertices; v++)
	{
		//	頂点 * ボーン行列
		// b = v番目の頂点の影響ボーン[n]
		if (Weights[v].count <= 0) continue;

		Vertices[v].x = 0;
		Vertices[v].y = 0;
		Vertices[v].z = 0;

		//	影響個数分ループ
		for (int n = 0; n < Weights[v].count; n++)
		{
			int b = Weights[v].bone[n];

			float x = VerticesSrc[v].x;
			float y = VerticesSrc[v].y;
			float z = VerticesSrc[v].z;
			//	座標を影響力分移動
			Vertices[v].x += (x*KeyMatrix[b]._11 + y * KeyMatrix[b]._21 + z * KeyMatrix[b]._31 + 1 * KeyMatrix[b]._41)*Weights[v].weight[n];
			Vertices[v].y += (x*KeyMatrix[b]._12 + y * KeyMatrix[b]._22 + z * KeyMatrix[b]._32 + 1 * KeyMatrix[b]._42)*Weights[v].weight[n];
			Vertices[v].z += (x*KeyMatrix[b]._13 + y * KeyMatrix[b]._23 + z * KeyMatrix[b]._33 + 1 * KeyMatrix[b]._43)*Weights[v].weight[n];

			float nx = VerticesSrc[v].nx;
			float ny = VerticesSrc[v].ny;
			float nz = VerticesSrc[v].nz;
			//	法線を影響力分変換
			Vertices[v].nx += (nx*KeyMatrix[b]._11 + ny * KeyMatrix[b]._21 + nz * KeyMatrix[b]._31)*Weights[v].weight[n];
			Vertices[v].ny += (nx*KeyMatrix[b]._12 + ny * KeyMatrix[b]._22 + nz * KeyMatrix[b]._32)*Weights[v].weight[n];
			Vertices[v].nz += (nx*KeyMatrix[b]._13 + ny * KeyMatrix[b]._23 + nz * KeyMatrix[b]._33)*Weights[v].weight[n];

		}
	}
}

void FbxMeshAmg::AddMotion(std::string name, const char * filename)
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

	FbxTakeInfo* take = scene->GetTakeInfo(names[0]->Buffer());
	FbxLongLong start = take->mLocalTimeSpan.GetStart().Get();
	FbxLongLong stop = take->mLocalTimeSpan.GetStop().Get();
	FbxLongLong fps60 = FbxTime::GetOneFrameValue(FbxTime::eFrames60);

	StartFrame = (int)(start / fps60);
	motion[name].NumFrame = (int)((stop - start) / fps60);
	//	ルートノード取得
	FbxNode* root = scene->GetRootNode();

	//	全ボーン読み込み
	for (int b = 0; b < NumBone; b++)
	{
		//	ボーンノード検索
		FbxNode* bone = root->FindChild(Bone[b].Name);
		if (bone == nullptr) continue;
		//	キーフレーム読み込み
		LoadKeyFrames(name, b, bone);
	}
	//	解放
	scene->Destroy();
	manager->Destroy();
}


//****************************************************************
//	頂点最適化
//****************************************************************
void FbxMeshAmg::OptimizeVertices()
{
	int currentNum = 0;
	for (int v = 0; v < NumVertices; v++)
	{
		int sameIndex = -1;
		//	同一頂点検索
		for (int old = 0; old < currentNum; old++)
		{
			if (Vertices[v].x != Vertices[old].x) continue;
			if (Vertices[v].y != Vertices[old].y) continue;
			if (Vertices[v].z != Vertices[old].z) continue;
			if (Vertices[v].nx != Vertices[old].nx) continue;
			if (Vertices[v].ny != Vertices[old].ny) continue;
			if (Vertices[v].nz != Vertices[old].nz) continue;
			if (Vertices[v].tu != Vertices[old].tu) continue;
			if (Vertices[v].tv != Vertices[old].tv) continue;
			//if (Vertices[v].color != Vertices[old].color) continue;

			sameIndex = old;
			break;
		}

		int target = v;
		if (sameIndex == -1)
		{
			//	新規頂点
			CopyMemory(&Vertices[currentNum], &Vertices[v], sizeof(PolygonVertex));
			CopyMemory(&Weights[currentNum], &Weights[v], sizeof(WEIGHT));
			target = currentNum;
			currentNum++;
		}
		else
		{
			target = sameIndex;
		}
		//	インデックス更新
		for (int i = 0; i < NumVertices; i++)
		{
			if (Indices[i] == (DWORD)(v)) Indices[i] = target;
		}
	}

	//	新バッファ確保
	PolygonVertex* buf = new PolygonVertex[currentNum];
	CopyMemory(buf, Vertices, sizeof(PolygonVertex) * currentNum);
	NumVertices = currentNum;

	delete[] Vertices;
	Vertices = buf;
}


//	レイ判定
int	FbxMeshAmg::Raycast(const D3DXVECTOR3& pos, const D3DXVECTOR3& vec, float Dist, D3DXVECTOR3& out, D3DXVECTOR3& normal)
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

	float neart = Dist;

	//	全ポリゴンループ
	for (int face = 0; face < NumFace; face++)
	{
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

	return	ret;
}