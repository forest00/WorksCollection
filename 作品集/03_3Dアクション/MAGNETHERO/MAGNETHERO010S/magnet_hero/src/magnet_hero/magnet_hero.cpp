//	
//	�~������uMagnet Hero�v
//	
//	�쐬��	�X	�m�u
//	
//	�ŏI�X�V		2018/12/13
//	

//�e�w�b�_�[
#include "magnet_hero.h"
//���̑��̃w�b�_�[
#include "magnet_hero_state.h"
#include "..\mylib\mylib.h"
#include "..\option\option.h"
#include "..\texture_manager\texture_manager.h"
#include "..\fbx_table\fbx_table.h"
#include "..\file_data\file_data.h"
#include "..\magnet_hero\magnet_hero_state\game_main\stage_event\stage_index_saver.h"
#include "..\magnet_hero\magnet_hero_state\game_main\stage_event\stage_data.h"


//�O���[�o���ϐ�
struct global_t
{
	magnet_hero_state_t _now_state;
};
static global_t g;


//�Q�[���̏�����
extern int magnet_hero_init()
{
	////�}���`�����_�����O�ł��邩�`�F�b�N
	//{
	//	D3DCAPS9 caps;
	//	mylib_get_d3d_device()->GetDeviceCaps(&caps);
	//	DWORD RT = caps.NumSimultaneousRTs;
	//	if (RT <= 1)
	//	{
	//		MessageBox(nullptr, "�}���`�����_�����O�^�[�Q�b�g���T�|�[�g����Ă��܂���", "�T�|�[�g�O�G���[", MB_OK);
	//		return -1;
	//	}
	//}
	//�w�i����
	mylib_clear(0, 100, 200);
	//�w�i�F�݂̂�������
	mylib_present();
	//�e�N�X�`���̓ǂݍ���
	texture_manager_init();
	//FBX�t�@�C���̓ǂݍ��݃e�[�u��������
	fbx_table_init();
	//�ŏ��Ƀv���[���[�����ǂݍ���
	fbx_table_load(fri_player);
	//�X�e�[�g��none�ɂ���
	g._now_state = mhs_none;
	//�t�@�C������ŏ��̃X�e�[�g��ǂݍ���
	const datum_t *data = file_data_get(fdi_init_state, nullptr);
	//�ŏ��̃X�e�[�g���Z�b�g
	magnet_hero_state_set_first_state((magnet_hero_state_t)((data++)->_Int));
	//�X�e�[�W��񏉊���
	stage_data_init();
	//�X�e�[�W�N���A��񏉊���
	stage_clear_data_init();
	//�I�v�V�����̏�����
	option_init();
	return 0;
}

//�Q�[���̏I��
extern void magnet_hero_end()
{
	//�I�v�V�����̏I��
	option_end();
	//FBX�j��
	fbx_table_destroy();
	//FBX�ǂݍ��݃e�[�u���̏I��
	fbx_table_end();
}

//�Q�[���̍X�V
extern void magnet_hero_update()
{
	//�Q�[�������̍X�V
	magnet_hero_state_update(g._now_state, &g._now_state);
	//�I�v�V�����̍X�V
	option_update();
}

//�Q�[���̕`��
extern void magnet_hero_draw()
{
	//�����_�[�^�[�Q�b�g�̐؂�ւ�
	mylib_set_render_target_manage(1);
	//�w�i����
	mylib_clear(0, 100, 200);
	//�V�[���J�n
	if (mylib_begin_scene() == 0)
	{
		///�������ɕ`�揈��������
		//�Q�[���̕`��
		magnet_hero_state_draw(g._now_state);
		//�I�v�V�����̕`��
		option_draw();
		//�V�[���I��
		mylib_end_scene();
	}
	//�����_�[�^�[�Q�b�g�����ɖ߂�
	mylib_set_render_target_manage(0);
	//�w�i����
	mylib_clear(0, 100, 200);
	//�V�[���J�n
	if (mylib_begin_scene() == 0)
	{
		//�{�`��
		D3DXVECTOR3 position[4];
		D3DXVECTOR2 uv[4];
		position[0] = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
		position[1] = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
		position[2] = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
		position[3] = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
		uv[0] = D3DXVECTOR2(0.0f, 0.0f);
		uv[1] = D3DXVECTOR2(1.0f, 0.0f);
		uv[2] = D3DXVECTOR2(0.0f, 1.0f);
		uv[3] = D3DXVECTOR2(1.0f, 1.0f);
		mylib_draw_board_2d_wave(nullptr,
			&position[0], &uv[0], D3DCOLOR_ARGB(255, 255, 255, 255),
			&position[1], &uv[1], D3DCOLOR_ARGB(255, 255, 255, 255),
			&position[2], &uv[2], D3DCOLOR_ARGB(255, 255, 255, 255),
			&position[3], &uv[3], D3DCOLOR_ARGB(255, 255, 255, 255),
			mylib_get_rendered_texture()
		);
		//�f�o�b�O�p�����`��
		mylib_output_debug_string();
		//�V�[���I��
		mylib_end_scene();
	}
	//�o�b�N�o�b�t�@�Ɛ؂�ւ�
	mylib_present();
}

extern bool magnet_hero_ended()
{
	return g._now_state == mhs_ended;
}