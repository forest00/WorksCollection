#pragma once

/*

�f�o�C�X�̏�������Ax�t�@�C���̓ǂݍ��݂Ȃǂ��T�|�[�g���郉�C�u�����ł�

��v�Ȍ��J�֐��́A"lib_" �Ƃ����L�[���[�h����n�܂�܂�

*/


#include <Windows.h>
#include <d3dx9.h>
#include <strsafe.h>
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib, "dxguid.lib")


//?
extern inline const char *window_class_name();


//��ʂ̃A�X�y�N�g����擾
extern double lib_get_aspect();
//�E�B���h�E�̐���&������
//�������ɐ��������E�B���h�E�̃n���h�����i�[���܂�
extern HRESULT lib_d3d_window_init(HWND *created_window,LPCSTR title, int width, int height);


//���C�����F�ł���悤�Ƀ��C�������_�����O����
void render_ray(IDirect3DDevice9 *device, D3DXVECTOR3 vStart, D3DXVECTOR3 vDir);
