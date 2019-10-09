//親ヘッダー
#include "..\public\anim_data_loader.h"
//その他のヘッダー
#include "..\public\anim_model_base.h"


#define safe_free(p)			{ if(p) { free(p);        (p) = nullptr; } }


static const char *g_file_names[anim_model_kind_max] = {
	"",
	"res//x_model//robot//RobotA_2motion_2truck.x",
	"res//x_model//ken//ken.x",
	"res//x_model//golem//golem.x",
	"res//x_model//josh//josh_2motion_2truck.x",
	"res//x_model//usagi//usagi.x",
	"res//x_model//block_man//player_multimotion.x",
};

static anim_data_t g_loaded_anim_data[anim_model_kind_max];



static void _anim_data_load(IDirect3DDevice9 *device, anim_data_t *anim_data, const char *file_name)
{
	// Xファイルからアニメーションメッシュを読み込み作成する
	if (FAILED(D3DXLoadMeshHierarchyFromX(
		file_name,
		D3DXMESH_MANAGED,
		device,
		GetValidHierarchy(),
		NULL,
		&anim_data->pFrameRoot,
		&anim_data->pAnimController
	)))
	{
		char message[0x400];
		message[0] = '\0';
		strcat_s(message, 0x400, "Xファイルの読み込みに失敗しました\n");
		strcat_s(message, 0x400, "ファイル名 : ");
		strcat_s(message, 0x400, file_name);
		MessageBox(NULL, message, file_name, MB_OK);
		return;
	}
	//
	AllocateAllBoneMatrices(anim_data->pFrameRoot, anim_data->pFrameRoot);
	//アニメーションするかチェック
	if(anim_data->pAnimController)
	{
		//アニメーショントラックを得る
		size_t i_max = anim_data->pAnimController->GetMaxNumAnimationSets();
		anim_data->pAnimSet = (ID3DXAnimationSet**)malloc(sizeof(ID3DXAnimationSet*) * i_max);
		for (DWORD i = 0;i < i_max;i++)
		{
			anim_data->pAnimController->GetAnimationSet(i, &anim_data->pAnimSet[i]);
		}
	}
	else
	{
		anim_data->pAnimSet = nullptr;
	}
}

static void _anim_data_destroy(anim_data_t *anim_data)
{
	FreeAnim(anim_data->pFrameRoot, GetValidHierarchy());
	GetValidHierarchy()->DestroyFrame(anim_data->pFrameRoot);
	safe_free(anim_data->pAnimSet);
}


//アニメデータを全部読み込む
extern void anim_data_load_all(IDirect3DDevice9 *device)
{
	g_loaded_anim_data[0].pFrameRoot = nullptr;
	g_loaded_anim_data[0].pAnimSet = nullptr;
	g_loaded_anim_data[0].pAnimController = nullptr;
	for (size_t i = 1; i < (size_t)(anim_model_kind_max); i++)
	{
		_anim_data_load(device, &g_loaded_anim_data[i], g_file_names[i]);
	}
}

//読み込んだアニメデータを全部破棄
extern void anim_data_destroy_all()
{
	for (size_t i = 1; i < (size_t)(anim_model_kind_max); i++)
	{
		_anim_data_destroy(&g_loaded_anim_data[i]);
	}
}

//読み込んだアニメデータを取得
extern const anim_data_t *anim_data_get(anim_model_kind_t anim_kind)
{
	return &g_loaded_anim_data[(size_t)(anim_kind)];
}

//アニメーションコントローラーのクローンを生成
extern void anim_controller_clone(anim_model_kind_t anim_kind, ID3DXAnimationController **destination)
{
	LPD3DXANIMATIONCONTROLLER source = g_loaded_anim_data[(size_t)(anim_kind)].pAnimController;
	if (source)
	{
		HRESULT result = source->CloneAnimationController(
			source->GetMaxNumAnimationOutputs(),
			source->GetMaxNumAnimationSets(),
			source->GetMaxNumTracks(),
			source->GetMaxNumEvents(),
			destination
		);
	}
	else
	{
		*destination = nullptr;
	}
}

