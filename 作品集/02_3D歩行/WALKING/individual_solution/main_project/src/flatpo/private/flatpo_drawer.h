#pragma once

//�|���S��(�ȍ~�A�|��)�������`�悷��t�@�C���ł�
//�|���\���̈�ɕt����̔|�����`��ł��܂�

#include <windows.h>
#include <d3dx9.h>
#include "..\public\flatpo_texture_loader.h"
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")


//�����K��
//
//�֐����͓���"flatpo_"��t���A�X�l�[�N�P�[�X�ŋL�q
//�֐��̈����̓X�l�[�N�P�[�X�ŋL�q
//�O���[�o���ϐ��͓���"g_"��t���A�X�l�[�N�P�[�X�ŋL�q
//�\���̂Ɨ񋓑̖̂��O�͖�����"_t"��t���A�X�l�[�N�P�[�X�ŋL�q
//�\���̃����o�[�͓���"_"��t���A�X�l�[�N�P�[�X�ŋL�q
//�񋓎q�͓��Ƀ^�O����"_"��t���A�X�l�[�N�P�[�X�ŋL�q
//���̑��̂��̂͑S�āA�X�l�[�N�P�[�X�ŋL�q
//


//���z�X�N���[���̕�
enum flatpo_screen_t
{
	flatpo_screen_width = 1350,
	flatpo_screen_height = 730,
};


//�|�����_(D3DFVF�Ƃ͖��֌W�ł�)
struct flatpo_vertex_t
{
	D3DXVECTOR3 _position;		//�ʒu
	float _u;					//�e�N�X�`�����WU
	float _v;					//�e�N�X�`�����WV
	int _alpha;					//�����x 0~255(�͈͊O�ł��N�����v����邩����v)
	int _red;					//�Ԑ��� 0~255(�͈͊O�ł��N�����v����邩����v)
	int _green;					//�ΐ��� 0~255(�͈͊O�ł��N�����v����邩����v)
	int _blue;					//���� 0~255(�͈͊O�ł��N�����v����邩����v)
};


//�|���\����
struct flatpo_t
{
	flatpo_texture_id_t _texture_id;			//�\��t����e�N�X�`����ID
	flatpo_vertex_t _vertices[4];			//���_4��(�����ɂȂ�悤�ɂ��Ă�������)
};


//�|���V�[���J�n
extern void flatpo_begin_scene(IDirect3DDevice9 *device, float aspect);

//�|���V�[���I��
extern void flatpo_end_scene();

//�|���̕`��
extern void flatpo_draw(flatpo_t *flatpo);

