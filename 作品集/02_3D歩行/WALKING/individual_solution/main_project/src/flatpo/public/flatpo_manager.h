//	
//	�S�Ă̔|�����Ǘ�����t�@�C���ł�
//	
//	�|���Ɋւ��鏈���͂��ׂĂ��̃t�@�C����ʂ��čs���܂�
//	

#pragma once

#include <d3dx9.h>

//�|���̏�����
extern void flatpo_manager_init(IDirect3DDevice9 *device);

//�|���̏I��
extern void flatpo_manager_end();

//�|���̍X�V
extern void flatpo_manager_update();

//�|���̕`��
extern void flatpo_manager_draw(IDirect3DDevice9 *device, double aspect);

