/*-----------------------------------------------------------*/
//                                                           
//                                                           
// �Q�[�������̏������A�X�V�A�`��A�I���܂ł��T�|�[�g����ꏊ�ł�  
//                                                           
//                                                           
//    ���炩���߃V�X�e�������̑�������Ă����Ȃ��Ƃ����܂���      
//                                                           
//                                                           
//                                                           
//                                                           
//      �쐬��      �X  �m�u                                  
//                                                           
//                                                           
/*-----------------------------------------------------------*/


//�e�w�b�_�[
#include "game_base.h"
//���̑��̃w�b�_�[
#include "..\..\my_3d_lib\public\my_3d_lib.h"
#include "..\..\d3d_device\public\d3d_device.h"
#include "..\..\player\public\player.h"
#include "..\..\x_file\public\x_file.h"
#include "..\..\sound\public\sound.h"
#include "..\..\input\public\input.h"
#include "..\..\enemy\public\enemy.h"
#include "..\..\back\public\back.h"
#include "..\..\anim_models\public\anim_model_test.h"
#include "..\..\camera\public\camera.h"
#include "..\..\font\public\font.h"
#include "..\..\flatpo\public\flatpo_manager.h"
#include "..\..\makabe_mesh\public\makabe_mesh.h"
//�f�o�b�O�p�w�b�_�[
#include "..\..\game_debug\public\game_debug.h"



/*-------------------------------------------------------------------*/
///��������A���[�J���֐�

//�Q�[���̏���(���������I���������s)
static void game_base_setup()
{
	//�v���C���[���o��
	{
		test_player_manager_t *manager = player_get_test_player_manager();
		test_player_initializer_t in;
		in._position = { 0.0,0.0,0.0 };
		in._type = x_file_type_Hubon;
		test_player_manager_t_add(manager, &in);
	}
	//�ŏ��̊y�Ȃ��Đ�(�Q�[�������̏���������)
	sound_play(sound_index_bgm_1);
}


//���Z�b�g�{�^����������Ă��邩����(�Q�[�������̍X�V����)
static bool game_base_pushed_reset_button()
{
	//�������I�t
	//return false;
	bool input_reset = false;
	input_reset |= input_keyboard_trigger(DIK_RETURN);
	input_reset |= input_keyboard_trigger(DIK_SPACE);
	input_reset |= input_controller_trigger(input_controller_code_start);
	input_reset |= input_controller_trigger(input_controller_code_back);
	return input_reset;
}

//true���n���ꂽ��A�Q�[�������Z�b�g
static void game_base_reset_if(bool fire)
{
	if (fire)
	{
		game_base_end();

		game_base_init();
	}
}

///�����܂ŁA���[�J���֐�
/*-------------------------------------------------------------------*/


//�Q�[�������̏�����
extern void game_base_init()
{
	//�v���C���[�̏�����
	player_init();

	//�G�l�~�[�̏�����
	enemy_init();

	//�w�i�̏�����
	back_manager_init();

	//�A�j���[�V�������f��������
	anim_model_tester_all_init(d3d_device());

	//�J�����̏�����
	camera_manager_init();

	//�|��������
	flatpo_manager_init(d3d_device());

	//�܂��׃��b�V���̏�����
	makabe_mash_init(d3d_device());


	//���������I������̂ŃQ�[���̏���������
	game_base_setup();
}

//�Q�[�������̍X�V
extern void game_base_update()
{
	//�G�L��������e�X�g
	if (input_keyboard_trigger(DIK_1) || input_keyboard_trigger(DIK_2))
	{
		test_enemy_manager_t *manager = enemy_get_test_enemy_manager();
		test_enemy_initializer_t in;
		in._position = {
			(float)((double)(rand()) / (double)(RAND_MAX) * 20.0),
			(float)((double)(rand()) / (double)(RAND_MAX) + 1.0),
			(float)((double)(rand()) / (double)(RAND_MAX) * 20.0)
		};
		in._direction = 0.0;
		in._type = x_file_type_ball;
		test_enemy_manager_t_add(manager, &in);
	}
	if (input_keyboard_trigger(DIK_NUMPAD1) || input_keyboard_trigger(DIK_NUMPAD2))
	{
		test_enemy_manager_t *manager = enemy_get_test_enemy_manager();
		test_enemy_object_t **found = test_enemy_manager_t_search_active(manager);
		test_enemy_manager_t_delete(manager, found);
	}

	//�v���C���[�̍X�V
	player_update();

	//�G�l�~�[�̍X�V
	enemy_update();

	//�w�i�̍X�V
	back_manager_update();

	//�A�j���[�V�������f���X�V
	anim_model_tester_all_update();

	//�J�������X�V
	camera_manager_update();

	//�|���X�V
	flatpo_manager_update();

	//�܂��׃��b�V���̍X�V
	makabe_mash_update();

	{
		//�}�E�X�N���b�N������΋Ȃ��`�F���W
		{
			if (input_mouse_trigger(input_mouse_code_left))
			{
				sound_play(sound_index_bgm_1);
			}
			if (input_mouse_trigger(input_mouse_code_right))
			{
				sound_play(sound_index_bgm_2);
			}
			if (input_mouse_trigger(input_mouse_code_middle))
			{
				sound_play(sound_index_bgm_3);
			}
			if (input_mouse_wheel() > 0)
			{
				sound_play(sound_index_bgm_4);
			}
		}

		//���͂�����΁A�Q�[�������Z�b�g
		game_base_reset_if(game_base_pushed_reset_button());
	}


	//�f�o�b�O�p�����o��
	game_debug_infomation();

}

//�Q�[�������̕`��
extern void game_base_draw()
{
	//�w�i�̕`��
	back_manager_draw(d3d_device());


	//�v���C���[�̕`��
	player_draw(d3d_device());

	//�G�l�~�[�̕`��
	enemy_draw(d3d_device());

	//�A�j���[�V�������f���`��
	anim_model_tester_all_draw(d3d_device());

	//�|���`��
	flatpo_manager_draw(d3d_device(), lib_get_aspect());

	//�t�H���g��`��
	font_draw(d3d_device());

	//�܂��׃��b�V���̕`��
	makabe_mash_draw(d3d_device());

	//X�L�[�Ńv���C���[�̈ʒu�ɃX�t�B�A�`��(�e�X�g�p)
	if (input_keyboard_repeat(DIK_X))
	{
		test_player_manager_t *manager = player_get_test_player_manager();
		test_player_object_t **object = test_player_manager_t_search_active(manager);
		const vector3_t *player_pos = test_player_manager_t_get_position(manager, *object);
		sphere_t test;
		static double rr = 1.0;
		test.center = *player_pos;
		test.radius = rr;
		sphere_draw(d3d_device(), &test);
	}
}

//�Q�[�������̏I��
extern void game_base_end()
{
	//�v���C���[�̏I��
	player_end();

	//�G�l�~�[�̏I��
	enemy_end();

	//�w�i�̏I��
	back_manager_end();

	//�A�j���[�V�������f���j��
	anim_model_tester_all_end();

	//�J�����̏I��
	camera_manager_end();

	//�|���I��
	flatpo_manager_end();

	//�܂��׃��b�V���̏I��
	makabe_mash_end();
}


