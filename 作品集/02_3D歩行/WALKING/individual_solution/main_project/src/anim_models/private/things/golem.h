#pragma once

//golem���o���t�@�C��
//golem�́A�A�j���[�V��������I�u�W�F�N�g�ł���



#include <d3dx9.h>


//golem�\����
struct golem_t;


//golem������ & �o��
extern void golem_init();

//golem�I��
extern void golem_end();

//golem�X�V
extern void golem_update();

//golem�`��
extern void golem_draw(IDirect3DDevice9 *device);


//���͂ɉ����ē����S�[�����̈ʒu�𓾂�
void golem_get_position_special(D3DXVECTOR3 *out);

