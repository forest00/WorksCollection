
//�|���ɒ���t����e�N�X�`����ǂݍ��ޏꏊ�ł�
//���łɔj�������܂�
//�����œǂݍ��񂾃e�N�X�`���ȊO�́A�|���ɒ���t�����܂���


#pragma once
#include <d3dx9.h>
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")


//�e�N�X�`���̎��
enum flatpo_texture_id_t
{
	flatpo_texture_id_none,						//�e�N�X�`���Ȃ�
	flatpo_texture_id_test_amg_logo,			//�e�X�g�p�e�N�X�`��
	//flatpo_texture_id_fafnyls_head,			//�t�@�t�j�[���̓�
	//flatpo_texture_id_fafnyls_tail,			//�t�@�t�j�[���̂�����
	//flatpo_texture_id_,						//
	//flatpo_texture_id_,						//
	//flatpo_texture_id_,						//
	flatpo_texture_id_max,						//�S�e�N�X�`���̐�
};





//�e�N�X�`����S���ǂݍ���
extern void flatpo_texture_loader_load_all(IDirect3DDevice9 *device);

//�e�N�X�`����S���j��
extern void flatpo_texture_loader_destroy_all();

//id�ɑΉ�����e�N�X�`���𓾂�
extern LPDIRECT3DTEXTURE9 flatpo_texture_loader_get(flatpo_texture_id_t texture_id);
