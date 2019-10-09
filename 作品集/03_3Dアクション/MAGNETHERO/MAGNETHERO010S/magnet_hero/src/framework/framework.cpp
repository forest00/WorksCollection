//�e�w�b�_�[
#include "framework.h"
//���̑��̃w�b�_�[
#include "..\mylib\mylib.h"
#include "..\file_data\file_data.h"
#include "..\window\icon_file_name.h"
#include "..\window\startup_option.h"
#include "..\magnet_hero\magnet_hero.h"
#include "..\texture_manager\texture_manager.h"
#include "..\input\input.h"

struct global_t
{
	bool _secret_call;
};
static global_t g;

extern int frame_init(HINSTANCE instance)
{
	g._secret_call = false;
	//�e�L�X�g�t�@�C���̓ǂݍ���
	file_data_load_all();
	//���C�u�����̏�����
	switch (mylib_init(
		instance,
		startup_option_get_screen_mode(),
		startup_option_get_left(),
		startup_option_get_top(),
		startup_option_get_width(),
		startup_option_get_height(),
		startup_option_get_window_title(),
		icon_file_name_get()
	))
	{
	case -1:
		return -1;
	case -2:
		MessageBox(nullptr, "�������Ɏ��s���܂���", "�G���[", MB_OK);
		return -1;
	}
	//�}�E�X���B��
	if (startup_option_get_hide_mouse())
	{
		mylib_hide_mouse_cursor();
	}
	//�}�O�l�b�g�q�[���[�̏�����
	return magnet_hero_init();
}

extern void frame_end()
{
	if (mylib_get_d3d() && g._secret_call)
	{
		//�}�O�l�b�g�q�[���[�̏I������
		magnet_hero_end();
		//���C�u�����̏I��
		mylib_end();
	}
	//�e�L�X�g�t�@�C������ǂݍ��񂾃f�[�^�̔j��
	file_data_destroy_all();
}

extern bool frame_continue()
{
	//���b�Z�[�W����
	return mylib_process_message() && (!magnet_hero_ended());
}

extern void frame_update()
{
	//���͂̍X�V
	mylib_input_update();
	//�}�O�l�b�g�q�[���[�̍X�V
	magnet_hero_update();
	//
	if (
		input_keyboard_repeat(DIK_A) &&
		input_keyboard_repeat(DIK_M) &&
		input_keyboard_repeat(DIK_G)
		)
	{
		g._secret_call = true;
	}
	//
	if (
		input_controller_repeat(icc_a) &&
		input_controller_repeat(icc_b) &&
		input_controller_repeat(icc_x)
		)
	{
		g._secret_call = true;
	}
}


extern void frame_draw()
{
	//�}�O�l�b�g�q�[���[�̕`��
	magnet_hero_draw();
}
