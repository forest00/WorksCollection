#pragma once


#include <d3dx9.h>
//#include <strsafe.h>
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

//D3D�C���^�[�t�F�[�X�ւ̃|�C���^���擾
extern LPDIRECT3D9 d3d();
//D3D�f�o�C�X���擾
extern IDirect3DDevice9 *d3d_device();



//D3D�̏�����
extern HRESULT d3d_init(HWND window_handle);

//D3D�̏I��
extern void d3d_end();
