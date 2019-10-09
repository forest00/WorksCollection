#include "..\public\anim_model_base.h"
#include "..\..\common_def\public\common_def.h"

//フレームを作成する
HRESULT MY_HIERARCHY::CreateFrame(LPCTSTR Name, D3DXFRAME **ppNewFrame)
{
	HRESULT hr = S_OK;
	MYFRAME *pFrame;

	*ppNewFrame = NULL;

	pFrame = new MYFRAME;
	if (pFrame == NULL)
	{
		return E_OUTOFMEMORY;
	}
	pFrame->Name = new TCHAR[lstrlen(Name) + 1];
	if (!pFrame->Name)
	{
		return E_FAIL;
	}
	strcpy(pFrame->Name, Name);

	D3DXMatrixIdentity(&pFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);
	pFrame->pMeshContainer = NULL;
	pFrame->pFrameSibling = NULL;
	pFrame->pFrameFirstChild = NULL;
	*ppNewFrame = pFrame;

	return S_OK;
}
//
//HRESULT MY_HIERARCHY::CreateMeshContainer
//メッシュコンテナーを作成する
HRESULT MY_HIERARCHY::CreateMeshContainer(LPCSTR Name, CONST D3DXMESHDATA* pMeshData,
	CONST D3DXMATERIAL* pMaterials, CONST D3DXEFFECTINSTANCE* pEffectInstances,
	DWORD NumMaterials, CONST DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER *ppMeshContainer)
{

	HRESULT hr;
	MYMESHCONTAINER *pMeshContainer = NULL;
	int iFacesAmount;
	int iMaterial;
	LPDIRECT3DDEVICE9 pDevice = NULL;
	LPD3DXMESH pMesh = NULL;
	*ppMeshContainer = NULL;
	DWORD dwBoneNum = 0;

	pMesh = pMeshData->pMesh;
	pMeshContainer = new MYMESHCONTAINER;
	if (pMeshContainer == NULL)
	{
		return E_OUTOFMEMORY;
	}
	ZeroMemory(pMeshContainer, sizeof(MYMESHCONTAINER));

	pMeshContainer->Name = new TCHAR[lstrlen(Name) + 1];
	if (!pMeshContainer->Name)
	{
		return E_FAIL;
	}
	strcpy(pMeshContainer->Name, Name);
	pMesh->GetDevice(&pDevice);
	iFacesAmount = pMesh->GetNumFaces();

	pMeshContainer->MeshData.pMesh = pMesh;
	pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

	if (pSkinInfo == NULL)
	{
		pMesh->AddRef();//通常メッシュの場合はこれが必要。スキンの場合、これをするとなぜかメモリリークになる。
	}
	//メッシュのマテリアル設定
	pMeshContainer->NumMaterials = max(1, NumMaterials);
	pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
	pMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
	pMeshContainer->pAdjacency = new DWORD[iFacesAmount * 3];
	if ((pMeshContainer->pAdjacency == NULL) || (pMeshContainer->pMaterials == NULL))
	{
		return E_FAIL;
	}
	memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * iFacesAmount * 3);
	memset(pMeshContainer->ppTextures, 0, sizeof(LPDIRECT3DTEXTURE9) * pMeshContainer->NumMaterials);

	if (NumMaterials > 0)
	{
		memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * NumMaterials);

		for (iMaterial = 0; iMaterial < NumMaterials; iMaterial++)
		{
			if (pMeshContainer->pMaterials[iMaterial].pTextureFilename != NULL)
			{
				TCHAR strTexturePath[MAX_PATH];
				strcpy(strTexturePath, pMeshContainer->pMaterials[iMaterial].pTextureFilename);
				if (FAILED(D3DXCreateTextureFromFile(pDevice, strTexturePath,
					&pMeshContainer->ppTextures[iMaterial])))
					pMeshContainer->ppTextures[iMaterial] = NULL;
				pMeshContainer->pMaterials[iMaterial].pTextureFilename = NULL;
			}
		}
	}
	else
	{
		pMeshContainer->pMaterials[0].pTextureFilename = NULL;
		memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
	}
	//当該メッシュがスキン情報を持っている場合（スキンメッシュ固有の処理）
	if (pSkinInfo != NULL)
	{
		pMeshContainer->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();
		dwBoneNum = pSkinInfo->GetNumBones();
		pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[dwBoneNum];

		for (DWORD i = 0; i < dwBoneNum; i++)
		{
			memcpy(&pMeshContainer->pBoneOffsetMatrices[i],
				pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i), sizeof(D3DMATRIX));
		}
		if (FAILED(pMeshContainer->pSkinInfo->ConvertToBlendedMesh(
			pMesh,
			NULL, pMeshContainer->pAdjacency, NULL, NULL, NULL,
			&pMeshContainer->dwWeight,
			&pMeshContainer->dwBoneNum,
			&pMeshContainer->pBoneBuffer,
			&pMeshContainer->MeshData.pMesh)
		))
		{
			return E_FAIL;
		}
	}
	//ローカルに生成したメッシュコンテナーを呼び出し側にコピーする （コピーじゃないけど・・・）
	*ppMeshContainer = pMeshContainer;
	//参照カウンタを増やしたので減らす
	SAFE_RELEASE(pDevice);

	return S_OK;
}

//フレームを破棄する
HRESULT MY_HIERARCHY::DestroyFrame(D3DXFRAME *pFrameToFree)
{
	SAFE_DELETE_ARRAY(pFrameToFree->Name);

	if (pFrameToFree->pFrameFirstChild)
	{
		DestroyFrame(pFrameToFree->pFrameFirstChild);
	}
	if (pFrameToFree->pFrameSibling)
	{
		DestroyFrame(pFrameToFree->pFrameSibling);
	}

	SAFE_DELETE(pFrameToFree);

	return S_OK;
}

//メッシュコンテナーを破棄する
HRESULT MY_HIERARCHY::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	int iMaterial;
	MYMESHCONTAINER *pMeshContainer = (MYMESHCONTAINER*)pMeshContainerBase;

	SAFE_DELETE_ARRAY(pMeshContainer->Name);
	SAFE_RELEASE(pMeshContainer->pSkinInfo);
	SAFE_DELETE_ARRAY(pMeshContainer->pAdjacency);
	SAFE_DELETE_ARRAY(pMeshContainer->pMaterials);

	SAFE_DELETE_ARRAY(pMeshContainer->ppBoneMatrix);

	if (pMeshContainer->ppTextures != NULL)
	{
		for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
		{
			SAFE_RELEASE(pMeshContainer->ppTextures[iMaterial]);
		}
	}
	SAFE_DELETE_ARRAY(pMeshContainer->ppTextures);

	SAFE_RELEASE(pMeshContainer->MeshData.pMesh);

	if (pMeshContainer->pBoneBuffer != NULL)
	{
		SAFE_RELEASE(pMeshContainer->pBoneBuffer);
		SAFE_DELETE_ARRAY(pMeshContainer->pBoneOffsetMatrices);
	}

	SAFE_DELETE(pMeshContainer);

	return S_OK;
}


//え、何これは...?
HRESULT AllocateBoneMatrix(D3DXFRAME *pFrameRoot, LPD3DXMESHCONTAINER pMeshContainerBase)
{
	MYFRAME *pFrame = NULL;
	DWORD dwBoneNum = 0;

	MYMESHCONTAINER *pMeshContainer = (MYMESHCONTAINER*)pMeshContainerBase;
	if (pMeshContainer->pSkinInfo == NULL)
	{
		return S_OK;
	}
	dwBoneNum = pMeshContainer->pSkinInfo->GetNumBones();
	pMeshContainer->ppBoneMatrix = new D3DXMATRIX*[dwBoneNum];

	for (DWORD i = 0; i < dwBoneNum; i++)
	{
		pFrame = (MYFRAME*)D3DXFrameFind(pFrameRoot, pMeshContainer->pSkinInfo->GetBoneName(i));
		if (pFrame == NULL)
		{
			return E_FAIL;
		}
		pMeshContainer->ppBoneMatrix[i] = &pFrame->CombinedTransformationMatrix;

	}
	return S_OK;
}

//???
HRESULT AllocateAllBoneMatrices(D3DXFRAME *pFrameRoot, D3DXFRAME *pFrame)
{
	if (pFrame->pMeshContainer != NULL)
	{
		if (FAILED(AllocateBoneMatrix(pFrameRoot, pFrame->pMeshContainer)))
		{
			return E_FAIL;
		}
	}
	if (pFrame->pFrameSibling != NULL)
	{
		if (FAILED(AllocateAllBoneMatrices(pFrameRoot, pFrame->pFrameSibling)))
		{
			return E_FAIL;
		}
	}
	if (pFrame->pFrameFirstChild != NULL)
	{
		if (FAILED(AllocateAllBoneMatrices(pFrameRoot, pFrame->pFrameFirstChild)))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}

//フレーム内のそれぞれのメッシュをレンダリングする
VOID RenderMeshContainer(LPDIRECT3DDEVICE9 pDevice, MYMESHCONTAINER* pMeshContainer, MYFRAME* pFrame)
{

	DWORD i, k, m;
	DWORD dwBoneIndex, dwBlendMatrixNum;
	DWORD dwPrevBoneID;
	LPD3DXBONECOMBINATION pBoneCombination;
	UINT iMatrixIndex;
	D3DXMATRIX mStack;
	//スキンメッシュの場合
	if (pMeshContainer->pSkinInfo != NULL)
	{
		pBoneCombination = (LPD3DXBONECOMBINATION)pMeshContainer->pBoneBuffer->GetBufferPointer();

		dwPrevBoneID = UINT_MAX;
		for (i = 0; i < pMeshContainer->dwBoneNum; i++)
		{
			dwBlendMatrixNum = 0;
			for (k = 0; k < pMeshContainer->dwWeight; k++)
			{
				if (pBoneCombination[i].BoneId[k] != UINT_MAX)
				{
					dwBlendMatrixNum = k;
				}
			}
			pDevice->SetRenderState(D3DRS_VERTEXBLEND, dwBlendMatrixNum);
			for (k = 0; k < pMeshContainer->dwWeight; k++)
			{
				iMatrixIndex = pBoneCombination[i].BoneId[k];
				if (iMatrixIndex != UINT_MAX)
				{
					mStack = pMeshContainer->pBoneOffsetMatrices[iMatrixIndex] * (*pMeshContainer->ppBoneMatrix[iMatrixIndex]);
					pDevice->SetTransform(D3DTS_WORLDMATRIX(k), &mStack);
				}
			}
			//ちょっと明るくする
#if 0
			D3DMATERIAL9 m = pMeshContainer->pMaterials[pBoneCombination[i].AttribId].MatD3D;
			float g = 0.25f;
			m.Ambient.a += g;
			m.Ambient.r += g;
			m.Ambient.g += g;
			m.Ambient.b += g;
			m.Diffuse.a += g;
			m.Diffuse.r += g;
			m.Diffuse.g += g;
			m.Diffuse.b += g;
			m.Emissive.a += g;
			m.Emissive.r += g;
			m.Emissive.g += g;
			m.Emissive.b += g;
			m.Specular.a += g;
			m.Specular.r += g;
			m.Specular.g += g;
			m.Specular.b += g;
			pDevice->SetMaterial(&m);
#else
			pDevice->SetMaterial(&pMeshContainer->pMaterials[pBoneCombination[i].AttribId].MatD3D);
#endif
			pDevice->SetTexture(0, pMeshContainer->ppTextures[pBoneCombination[i].AttribId]);
			dwPrevBoneID = pBoneCombination[i].AttribId;
			pMeshContainer->MeshData.pMesh->DrawSubset(i);
		}
	}
	//通常メッシュの場合
	else
	{
		pDevice->SetRenderState(D3DRS_VERTEXBLEND, 0);
		pDevice->SetTransform(D3DTS_WORLD, &pFrame->CombinedTransformationMatrix);
		for (i = 0; i < pMeshContainer->NumMaterials; i++)
		{
			pDevice->SetMaterial(&pMeshContainer->pMaterials[i].MatD3D);
			pDevice->SetTexture(0, pMeshContainer->ppTextures[i]);
			pMeshContainer->MeshData.pMesh->DrawSubset(i);
		}
	}
}

//フレームをレンダリングする。
VOID RenderFrame(LPDIRECT3DDEVICE9 pDevice, D3DXFRAME *pFrameBase)
{
	MYFRAME* pFrame = (MYFRAME*)pFrameBase;
	MYMESHCONTAINER* pMeshContainer = (MYMESHCONTAINER*)pFrame->pMeshContainer;

	while (pMeshContainer != NULL)
	{
		RenderMeshContainer(pDevice, pMeshContainer, pFrame);

		pMeshContainer = (MYMESHCONTAINER*)pMeshContainer->pNextMeshContainer;
	}
	if (pFrame->pFrameSibling != NULL)
	{
		RenderFrame(pDevice, pFrame->pFrameSibling);
	}
	if (pFrame->pFrameFirstChild != NULL)
	{
		RenderFrame(pDevice, pFrame->pFrameFirstChild);
	}
}

//フレーム内のメッシュ毎にワールド変換行列を更新する
VOID UpdateFrameMatrices(D3DXFRAME *pFrameBase, const D3DXMATRIX *pParentMatrix)
{
	MYFRAME *pFrame = (MYFRAME*)pFrameBase;

	if (pParentMatrix != NULL)
	{
		D3DXMatrixMultiply(&pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix);
	}
	else
	{
		pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;
	}
	if (pFrame->pFrameSibling != NULL)
	{
		UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
	}
	if (pFrame->pFrameFirstChild != NULL)
	{
		UpdateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);
	}
}

extern ID3DXAllocateHierarchy *GetValidHierarchy()
{
	static MY_HIERARCHY cHierarchy;
	return &cHierarchy;
}

//階層内、全てのDirectXオブジェクトをリリースする
extern void FreeAnim(D3DXFRAME *pFrame, ID3DXAllocateHierarchy *cHierarchy)
{
	if (pFrame->pMeshContainer != NULL)
	{

		cHierarchy->DestroyMeshContainer(pFrame->pMeshContainer);
	}

	if (pFrame->pFrameSibling != NULL)
	{
		FreeAnim(pFrame->pFrameSibling, cHierarchy);
	}

	if (pFrame->pFrameFirstChild != NULL)
	{
		FreeAnim(pFrame->pFrameFirstChild, cHierarchy);
	}
}


///以上コピペ
///以下自作

//アニメーションの初期化
void anim_init(anim_parameter_t *anim)
{
	anim->_anim_kind = anim_model_kind_none;
	anim->_data = nullptr;
	anim->_controller = nullptr;
	anim->_playing = false;
	anim->_total_time = 0.0f;
	anim->_last_changed_time = 0.0f;
	anim->_playing_index = -1;
	anim->_repeat_flag = anim_repeat_on;
}

//アニメーションの準備
void anim_setup(anim_parameter_t *anim, anim_model_kind_t anim_kind)
{
	anim->_anim_kind = anim_kind;
	anim->_data = anim_data_get(anim_kind);
	anim_controller_clone(anim_kind, &anim->_controller);
	anim->_playing = false;
	anim->_total_time = 0.0f;
	anim->_last_changed_time = 0.0f;
	anim->_playing_index = -1;
	anim->_repeat_flag = anim_repeat_on;
}

//アニメーションをやめる
void anim_end(anim_parameter_t *anim)
{
	safe_release(anim->_controller);
}

//アニメーションを切り替える
/*
*@param anim						対象のパラメーター構造体
*@param change						何番のアニメーションにするか
*@param do_at_the_end				アニメーションが終わった時に繰り返し再生するか
*@return 0...成功 -1...アニメーションコントローラーが存在しない -2...changeが範囲外である
*/
int anim_play(anim_parameter_t *anim, int change, anim_repeat_flag_t repeat_flag)
{
	if (anim->_controller == nullptr)
	{
		return -1;
	}
	unsigned int animation_set_num = anim->_controller->GetMaxNumAnimationSets();
	if (change < 0 || change >= animation_set_num)
	{
		return -2;
	}
	D3DXTRACK_DESC track_desc = {};
	track_desc.Priority = D3DXPRIORITY_LOW;
	track_desc.Weight = 1.0f;
	track_desc.Speed = 1.0f;
	track_desc.Position = 0.0;
	track_desc.Enable = true;
	anim->_controller->SetTrackDesc(0, &track_desc);
	anim->_controller->SetTrackAnimationSet(0, anim->_data->pAnimSet[change]);
	anim->_controller->SetTrackEnable(0, true);
	anim->_controller->SetTrackEnable(1, false);
	anim->_last_changed_time = anim->_total_time;
	anim->_playing_index = change;
	anim->_repeat_flag = repeat_flag;
	return 0;
}


//アニメーションを進める
/*
*@param anim						対象のパラメーター構造体
*@param advance_time				進める時間
*@param *duration					アニメの一回分の再生時間を格納する
*@param *duration_nextafter0		アニメが再生され切らないぎりぎりの時間を格納する
*@param *anim_count					アニメがループされた回数を格納する
*@param *anim_position				処理後のアニメの位置を格納する
*@param *anim_prev_position			処理前のアニメの位置を格納する
*@param *anim_is_ended				アニメが再生され切らないぎりぎりの時間に達しているかを格納する(anim_position >= duration_nextafter0)
*@return 無し
*/
void anim_advance(
	anim_parameter_t *anim,
	float advance_time,
	float *duration,
	float *duration_nextafter0,
	size_t *anim_count,
	float *anim_position,
	float *anim_prev_position,
	bool *anim_is_ended
)
{
	float _duration = 0.0f;
	float _duration_nextafter0 = 0.0f;
	//このフレームでのアニメの位置(最終的に確定したものを入れる)
	float _anim_position = 0.0f;
	//アニメ時間をどれだけ進行させるか計算(アニメ停止中なら0、そうでなければ1フレームループの時間だけ進める)
	float anim_delta = 0.0f;
	if (anim->_playing)
	{
		anim_delta = advance_time;
	}
	//動く前のアニメの位置
	float _anim_prev_position = anim->_total_time - anim->_last_changed_time;
	//アニメ時間を進める
	anim->_total_time += anim_delta;
	//動いた後のアニメの位置
	float _anim_now_position = anim->_total_time - anim->_last_changed_time;
	//繰り返し再生をするか
	if (anim->_repeat_flag == anim_repeat_on)
	{
		///繰り返し再生しない
		//アニメの経過時間をそのまま使う
		_anim_position = _anim_now_position;
	}
	else
	{
		///繰り返し再生する
		//アニメーションの一回分の再生時間を得る
		{
			//アニメーションセットを得る
			ID3DXAnimationSet *anim_set;
			anim->_controller->GetTrackAnimationSet(0, &anim_set);
			//トラックのデスクを得る
			D3DXTRACK_DESC track_desc = {};
			anim->_controller->GetTrackDesc(0, &track_desc);
			//アニメーションの一回分の再生時間を得る
			_duration = anim_set->GetPeriod();
			_duration_nextafter0 = nextafterf(_duration, 0.0f);
			//不要になったものは片づける
			anim_set->Release();
		}
		//アニメ時間の最後の一瞬を超えたことを判定
		if (_anim_now_position >= _duration_nextafter0)
		{
			//アニメ時間を止める
			anim->_total_time = anim->_last_changed_time + _duration_nextafter0;
			//アニメの位置(?)を"最後の一瞬"に合わせる
			_anim_position = _duration_nextafter0;
		}
		else
		{
			_anim_position = _anim_now_position;
		}
	}





	//計算したアニメの位置(?)をセット
	anim->_controller->SetTrackPosition(0, _anim_position);

	//結果を教える
	if (duration)
	{
		*duration = _duration;
	}
	if (duration_nextafter0)
	{
		*duration_nextafter0 = _duration_nextafter0;
	}
	if (anim_count)
	{
		*anim_count = (size_t)(_anim_position / _duration);
	}
	if (anim_position)
	{
		*anim_position = _anim_position;
	}
	if (anim_prev_position)
	{
		*anim_prev_position = _anim_prev_position;
	}
	if (anim_is_ended)
	{
		*anim_is_ended = _anim_position >= _duration_nextafter0;
	}
}

//アニメーションする物体を描画
void anim_thing_draw(IDirect3DDevice9 *device, const D3DXMATRIX *world, const anim_parameter_t *anim)
{
	//ワールドトランスフォーム（絶対座標変換）
	device->SetTransform(D3DTS_WORLD, world);

	// レンダリング
	if (anim->_controller)
	{
		anim->_controller->AdvanceTime(0.0f, nullptr);
		UpdateFrameMatrices(anim->_data->pFrameRoot, world);
		RenderFrame(device, anim->_data->pFrameRoot);
	}
}