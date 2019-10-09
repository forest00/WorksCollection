//�e�w�b�_�[
#include "..\public\anim_data_loader.h"
//���̑��̃w�b�_�[
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
	// X�t�@�C������A�j���[�V�������b�V����ǂݍ��ݍ쐬����
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
		strcat_s(message, 0x400, "X�t�@�C���̓ǂݍ��݂Ɏ��s���܂���\n");
		strcat_s(message, 0x400, "�t�@�C���� : ");
		strcat_s(message, 0x400, file_name);
		MessageBox(NULL, message, file_name, MB_OK);
		return;
	}
	//
	AllocateAllBoneMatrices(anim_data->pFrameRoot, anim_data->pFrameRoot);
	//�A�j���[�V�������邩�`�F�b�N
	if(anim_data->pAnimController)
	{
		//�A�j���[�V�����g���b�N�𓾂�
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


//�A�j���f�[�^��S���ǂݍ���
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

//�ǂݍ��񂾃A�j���f�[�^��S���j��
extern void anim_data_destroy_all()
{
	for (size_t i = 1; i < (size_t)(anim_model_kind_max); i++)
	{
		_anim_data_destroy(&g_loaded_anim_data[i]);
	}
}

//�ǂݍ��񂾃A�j���f�[�^���擾
extern const anim_data_t *anim_data_get(anim_model_kind_t anim_kind)
{
	return &g_loaded_anim_data[(size_t)(anim_kind)];
}

//�A�j���[�V�����R���g���[���[�̃N���[���𐶐�
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

