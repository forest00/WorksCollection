//	
//	�~������uMagnet Hero�v
//	
//	�e�N�X�`����ǂݍ��ޏꏊ�ł�
//	
//	�����̃e�[�u���Ɋ�Â��ēǂݍ��݂��s���܂�
//	
//	�ǂݍ��񂾂��̂�j��������������˂Ă܂�
//	
//	���Direct3D�̏����������Ă����K�v������܂�
//	
//	�쐬��	�X	�m�u
//	
#pragma once
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <d3d9.h>
#include <d3dx9.h>
#include "texuture_id.h"


extern void texture_manager_init();

extern IDirect3DTexture9 *texture_manager_get(texture_id_t texture_manager_id);

extern int texture_manager_get_width(texture_id_t texture_manager_id);

extern int texture_manager_get_height(texture_id_t texture_manager_id);
