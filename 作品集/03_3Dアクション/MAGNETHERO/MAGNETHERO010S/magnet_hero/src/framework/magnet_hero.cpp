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
#include "..\texture_manager\texture_manager.h"
#include "..\fbx_table\fbx_table.h"
#include "..\common_count\common_count_advance.h"
#include "..\draw_setup\light.h"
#include "..\draw_setup\view_and_projection_transformation.h"
#include "..\file_data\file_data.h"
#include "..\framework\magnet_hero_state\game_main\stage_event\stage_index_saver.h"
#include "..\framework\magnet_hero_state\game_main\stage_event\stage_data.h"
#include "..\window\icon_file_name.h"
#include "..\window\startup_option.h"


//�O���[�o���ϐ�
struct global_t
{
	magnet_hero_state_t _now_state;
};
static global_t g;


//�Q�[���̏�����
extern int magnet_hero_init()
{
	//�e�L�X�g�t�@�C���̓ǂݍ���
	file_data_load_all();
	//���C�u�����̏�����
	if (mylib_init(
		hInstance,
		startup_option_get_screen_mode(),
		startup_option_get_left(),
		startup_option_get_top(),
		startup_option_get_width(),
		startup_option_get_height(),
		startup_option_get_window_title(),
		icon_file_name_get()
	) != 0)
	{
		MessageBox(nullptr, "�������Ɏ��s���܂���", "�G���[", MB_OK);
		mylib_end();
		return E_FAIL;
	}
	//�}�E�X���B��
	if (startup_option_get_hide_mouse())
	{
		mylib_hide_mouse_cursor();
	}
	//�w�i����
	mylib_clear(0, 100, 200);
	//�w�i�F�݂̂�������
	mylib_present();
	//�e�N�X�`���̓ǂݍ���
	texture_manager_init();
	//FBX�t�@�C���̓ǂݍ���
	fbx_table_load();
	//�X�e�[�g��none�ɂ���
	g._now_state = mhs_none;
	//�t�@�C������ŏ��̃X�e�[�g��ǂݍ���
	const datum_t *data = file_data_get(fdi_init_state, nullptr);
	//�ŏ��̃X�e�[�g���Z�b�g
	magnet_hero_state_set_first_state((magnet_hero_state_t)(data[0]._Int));
	//�X�e�[�W��񏉊���
	stage_data_init();
	//�X�e�[�W�N���A��񏉊���
	stage_clear_data_init();
	return 0;
}

//�Q�[���̏I��
extern void magnet_hero_end()
{
	//FBX�j��
	fbx_table_destroy();
}

//�Q�[���̍X�V
extern void magnet_hero_update()
{
	//���͂̍X�V
	mylib_input_update();
	//�Q�[�������̍X�V
	magnet_hero_state_update(mylib_get_d3d_device(), g._now_state, &g._now_state);
	//�����̐ݒ�
	light_set_ambient(mylib_get_d3d_device(), D3DCOLOR_XRGB(100, 100, 100));
	//�f�B���N�V���i�����̐ݒ�
	light_set_directional(mylib_get_d3d_device());
	//�r���[�ϊ�&�v���W�F�N�V�����ϊ�
	view_and_projection_transformation(mylib_get_d3d_device());
	//�X�V�񐔂��L�^
	common_count_advance();
}

//�Q�[���̕`��
extern void magnet_hero_draw()
{
	//�����_�[�^�[�Q�b�g�̐؂�ւ�
	mylib_change_render_target(1);
	//�w�i����
	mylib_clear(0, 100, 200);
	//�V�[���J�n
	if (mylib_begin_scene() == 0)
	{
		//�������ɕ`�揈��������
		magnet_hero_state_draw(mylib_get_d3d_device(), g._now_state);
		//�V�[���I��
		mylib_end_scene();
	}
	//�����_�[�^�[�Q�b�g�����ɖ߂�
	mylib_change_render_target(0);
	//�w�i����
	mylib_clear(0, 100, 200);
	//�V�[���J�n
	if (mylib_begin_scene() == 0)
	{
		//�{�`��
		mylib_draw_board(mylib_get_d3d_device(),
			&D3DXVECTOR3(-1.0f, 1.0f, 0.0f), &D3DXVECTOR2(0.0f, 0.0f), D3DCOLOR_ARGB(255, 255, 255, 255),
			&D3DXVECTOR3(1.0f, 1.0f, 0.0f), &D3DXVECTOR2(1.0f, 0.0f), D3DCOLOR_ARGB(255, 255, 255, 255),
			&D3DXVECTOR3(-1.0f, -1.0f, 0.0f), &D3DXVECTOR2(0.0f, 1.0f), D3DCOLOR_ARGB(255, 255, 255, 255),
			&D3DXVECTOR3(1.0f, -1.0f, 0.0f), &D3DXVECTOR2(1.0f, 1.0f), D3DCOLOR_ARGB(255, 255, 255, 255),
			mylib_get_rendered_texture(), mbm_none);
		//�V�[���I��
		mylib_end_scene();
	}
	//�o�b�N�o�b�t�@�Ɛ؂�ւ�
	mylib_present();
}