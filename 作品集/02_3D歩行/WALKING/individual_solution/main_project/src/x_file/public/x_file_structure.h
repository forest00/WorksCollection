#pragma once

#include <d3dx9.h>

//x�t�@�C����ǂݍ���Ńf�[�^���i�[���邽�߂̍\����
struct x_file_t
{
	LPD3DXMESH _mesh;						//���b�V�����
	D3DMATERIAL9 *_materials;				//�}�e���A�����
	LPDIRECT3DTEXTURE9 *_textures;			//�e�N�X�`�����
	DWORD _materials_num;					//����
};
