//	
//	�~������uMagnet Hero�v
//	
//	�쐬��	�X	�m�u
//	


//�e�w�b�_�[
#include "texture_manager.h"
//���̑��̃w�b�_�[
#include <stdio.h>
#include "..\mylib\mylib.h"
#include "..\file_data\file_data.h"


//�O���[�o���ϐ�
struct global_t
{
	//�ǂݍ��񂾃e�N�X�`���̃n���h��
	int _texture_handles[(size_t)(ti_max)];
	//�ǂݍ��񂾃e�N�X�`���̃T�C�Y
	int _size_x[(size_t)(ti_max)];
	int _size_y[(size_t)(ti_max)];
};

static global_t g;

//�e�N�X�`����S���ǂݍ���
extern void texture_manager_init()
{
	//�e�N�X�`���t�@�C�������擾
	const datum_t *data = file_data_get(fdi_texture_file_databse, nullptr);
	//�ŏ��̃e�N�X�`���͑��݂��Ȃ��̂�-1
	g._texture_handles[0] = -1;
	g._size_x[0] = 0;
	g._size_y[0] = 0;
	//�e�N�X�`���̐�����for������
	for (size_t i = 1; i < (size_t)(ti_max); i++)
	{
		//�ǂݍ���
		const char * filename = data[i - 1]._String;
		int error_code;
		D3DXIMAGE_INFO image_info;
		mylib_load_texture(filename, &g._texture_handles[i], &image_info, &error_code);
		if (error_code != 0)
		{
			MessageBox(nullptr, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", "�ǂݍ��݃G���[", MB_OK);
		}
		g._size_x[i] = (int)(image_info.Width);
		g._size_y[i] = (int)(image_info.Height);
	}
}

extern IDirect3DTexture9 *texture_manager_get(texture_id_t texture_manager_id)
{
	return mylib_get_texture(g._texture_handles[(size_t)(texture_manager_id)], nullptr);
}

extern int texture_manager_get_width(texture_id_t texture_manager_id)
{
	return g._size_x[(size_t)(texture_manager_id)];
}

extern int texture_manager_get_height(texture_id_t texture_manager_id)
{
	return g._size_y[(size_t)(texture_manager_id)];
}