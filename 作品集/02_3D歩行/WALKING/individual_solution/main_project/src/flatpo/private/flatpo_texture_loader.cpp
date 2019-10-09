
//�e�w�b�_�[
#include "..\public\flatpo_texture_loader.h"

//�}�N��
#define safe_release(p)			{ if(p) { (p)->Release(); (p) = nullptr; } }


//�t�@�C�����̗���
static const char *g_file_names[flatpo_texture_id_max] = {
	"",
	"res/sprite_tex/amg_logo.png",
	//"res/sprite_tex/ICEPALADIN010E/Resource/png/�t�@�t�j�[��/�t�@�t�j�[������_�|��.png",
	//"res/sprite_tex/ICEPALADIN010E/Resource/png/�t�@�t�j�[��/�t�@�t�j�[���K��_�U��.png",
	//"res/sprite_tex/ICEPALADIN010E/Resource/png/",
	//"res/sprite_tex/",
	//"res/sprite_tex/",
	//"res/sprite_tex/",
	//"res/sprite_tex/",
};




static LPDIRECT3DTEXTURE9 g_all_textures[flatpo_texture_id_max] = {};



//�e�N�X�`����S���ǂݍ���
extern void flatpo_texture_loader_load_all(IDirect3DDevice9 *device)
{
	g_all_textures[0] = nullptr;
	for (size_t i = 1; i < (size_t)(flatpo_texture_id_max); i++)
	{
		//�ǂݍ���
		if (FAILED(D3DXCreateTextureFromFileEx(
			device,
			g_file_names[i],
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			1,
			0,
			D3DFMT_UNKNOWN,
			D3DPOOL_DEFAULT,
			D3DX_FILTER_NONE,
			D3DX_DEFAULT,
			0x00000000,
			NULL,
			NULL,
			&g_all_textures[i]
		)))
		{
			//�ǂݍ��ݎ��s���b�Z�[�W���o��
			static char message[0x400];
			message[0] = '\0';
			strcat_s(message, 0x400, "�|���S����������\n\n");
			strcat_s(message, 0x400, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���\n");
			strcat_s(message, 0x400, "�t�@�C���� : ");
			strcat_s(message, 0x400, g_file_names[i]);
			MessageBox(0, message, "", MB_OK);
		}
	}
}

//�e�N�X�`����S���j��
extern void flatpo_texture_loader_destroy_all()
{
	for (size_t i = 0; i < (size_t)(flatpo_texture_id_max); i++)
	{
		safe_release(g_all_textures[i]);
	}
}

//id�ɑΉ�����e�N�X�`���𓾂�
extern LPDIRECT3DTEXTURE9 flatpo_texture_loader_get(flatpo_texture_id_t texture_id)
{
	return g_all_textures[(size_t)(texture_id)];
}