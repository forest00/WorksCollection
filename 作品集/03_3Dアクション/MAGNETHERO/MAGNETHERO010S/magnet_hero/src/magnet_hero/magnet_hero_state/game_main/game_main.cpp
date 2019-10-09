//	
//	�~������uMagnet Hero�v
//	
//	�쐬��	�X	�m�u
//	
//	����
//	2019/02/04...�v���[���[�����������Ƃ��ɃX�e�[�W���č\�z����悤�ɕύX
//	

//�e�w�b�_�[
#include "game_main.h"
//���̑��̃w�b�_�[
#include "sprite\sprite_integration.h"
#include "fbx_object\object_manager.h"
#include "glitter\glitter_manager.h"
#include "camera\camera_manager.h"
#include "connecter\info_holder.h"
#include "connecter\info_drawer.h"
#include "stage_event\stage_index_saver.h"
#include "stage_event\stage_setup.h"
#include "stage_event\stage_data.h"
#include "stage_event\stage_event.h"
#include "stage_event\stage_ui.h"
#include "..\..\..\mylib\mylib.h"
#include "..\..\..\calc\calc.h"
#include "..\..\..\input\input.h"
#include "..\..\..\sound\sound.h"
#include "..\..\..\texture_manager\texture_manager.h"

//�S�[����̑҂�����(�t���[��)
#define _goal_wait_frame		(120)

#define _coord_continue_x		(457.0f)
#define _coord_continue_y		(230.0f)
#define _coord_title_x			(475.0f)
#define _coord_title_y			(411.0f)
#define _coord_cursor_x1		(324.0f)
#define _coord_cursor_y1		(275.0f)
#define _coord_cursor_x2		(340.0f)
#define _coord_cursor_y2		(439.0f)

//�|�[�Y�̃��U���g
enum pause_result_t
{
	pr_pause,//�|�[�Y��ʂ̂܂ܑ�����ׂ�
	pr_continue,//�Q�[�����ĊJ����ׂ�
	pr_title,//�^�C�g���ɖ߂�ׂ�
};

//�|�[�Y���̃J�[�\��
struct cursor_t
{
	int _pos;//���݈ʒu�@0��A1��
	float _draw_x;//�`��ʒux
	float _draw_y;//�`��ʒuy
	bool _moving;//�ړ���
	float _change_sec;//�؂�ւ���ꂽ�u��
	float _move_duration_sec;//�ړ��ɂ�����S�̂̎���
};
//�|�[�Y���Ɏg���ϐ�
struct pause_t
{
	cursor_t _cursor;
	texture_id_t _continue_tex;
	texture_id_t _title_tex;
	float _cursor_draw_pos[2][2];//�ʒu���̏W�܂�
};
//�O���[�o���ϐ�
struct global_t
{
	pause_t _pause;
	game_main_fbx_object_array_t _object_array;
	bool _pause_initialized;
	bool _pause_mode;
};

static global_t g;

//�|�[�Y�̏���
#if 1
//�|�[�Y������
static void pause_init()
{
	pause_t *pause = &g._pause;
	cursor_t *cursor = &pause->_cursor;
	cursor->_pos = 0;
	cursor->_draw_x = _coord_cursor_x1;
	cursor->_draw_y = _coord_cursor_y1;
	cursor->_moving = false;
	cursor->_change_sec = 0.0f;
	cursor->_move_duration_sec = 0.3f;
	pause->_continue_tex = ti_pause_continue_on;
	pause->_title_tex = ti_pause_title_off;
	pause->_cursor_draw_pos[0][0] = _coord_cursor_x1;
	pause->_cursor_draw_pos[0][1] = _coord_cursor_y1;
	pause->_cursor_draw_pos[1][0] = _coord_cursor_x2;
	pause->_cursor_draw_pos[1][1] = _coord_cursor_y2;
}

//�|�[�Y�I��
static void pause_end()
{

}

//�|�[�Y���̂݌Ă΂��X�V
static pause_result_t pause_update(const virtual_input_t *trigger)
{
	pause_t *pause = &g._pause;
	cursor_t *cursor = &pause->_cursor;
	//�オ�����ꂽ
	if (trigger->_lkey_up || trigger->_rkey_up)
	{
		if (cursor->_pos != 0)
		{
			cursor->_moving = true;
			cursor->_change_sec = mylib_now_sec();
			cursor->_pos = 0;
			pause->_continue_tex = ti_pause_continue_on;
			pause->_title_tex = ti_pause_title_off;
		}
	}
	//���������ꂽ
	if (trigger->_lkey_down || trigger->_rkey_down)
	{
		if (cursor->_pos != 1)
		{
			cursor->_moving = true;
			cursor->_change_sec = mylib_now_sec();
			cursor->_pos = 1;
			pause->_continue_tex = ti_pause_continue_off;
			pause->_title_tex = ti_pause_title_on;
		}
	}
	//�J�[�\���ړ���
	if (cursor->_moving)
	{
		float elapsed = mylib_now_sec() - cursor->_change_sec;
		float t = elapsed / cursor->_move_duration_sec;
		if (t >= 1.0f)
		{
			t = 1.0f;
			cursor->_moving = false;
		}
		float start_x = pause->_cursor_draw_pos[1 - cursor->_pos][0];
		float start_y = pause->_cursor_draw_pos[1 - cursor->_pos][1];
		float end_x = pause->_cursor_draw_pos[cursor->_pos][0];
		float end_y = pause->_cursor_draw_pos[cursor->_pos][1];
		cursor->_draw_x = (float)call_easing(et_EaseOutCirc, t, start_x, end_x - start_x, 1.0f);
		cursor->_draw_y = (float)call_easing(et_EaseOutCirc, t, start_y, end_y - start_y, 1.0f);
	}
	//����L�[�������ꂽ
	if (trigger->_a)
	{
		switch (cursor->_pos)
		{
		case 0:
			//�Q�[�����ĊJ
			return pr_continue;
			break;
		case 1:
			//�^�C�g���ɖ߂�
			return pr_title;
			break;
		default:
			break;
		}
	}
	return pr_pause;
}

//�|�[�Y���̂݌Ă΂��`��
static void pause_draw()
{
	//���_��p��
	static D3DXVECTOR3 position[4];
	//UV���W��p��
	static D3DXVECTOR2 uv[4];
	uv[0] = D3DXVECTOR2(0.0f, 0.0f);
	uv[1] = D3DXVECTOR2(1.0f, 0.0f);
	uv[2] = D3DXVECTOR2(0.0f, 1.0f);
	uv[3] = D3DXVECTOR2(1.0f, 1.0f);
	//�w�i�`��
	{
		//���_��p��
		position[0] = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
		position[1] = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
		position[2] = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
		position[3] = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
		//�`��
		mylib_draw_board_2d(
			&position[0], &uv[0], 0xffffffff,
			&position[1], &uv[1], 0xffffffff,
			&position[2], &uv[2], 0xffffffff,
			&position[3], &uv[3], 0xffffffff,
			texture_manager_get(ti_pause_pause),
			mbm_none
		);
	}
	//CONTINUE�{�^���`��
	{
		//�T�C�Y�擾
		float w = (float)texture_manager_get_width(g._pause._continue_tex);
		float h = (float)texture_manager_get_height(g._pause._continue_tex);
		//����ƉE�������߂�
		float left = _coord_continue_x / 1280.0f;
		float top = _coord_continue_y / 720.0f;
		float right = left + w / 1280.0f;
		float bottom = top + h / 720.0f;
		//�␳
		left = left * 2.0f - 1.0f;
		top = -(top * 2.0f - 1.0f);
		right = right * 2.0f - 1.0f;
		bottom = -(bottom * 2.0f - 1.0f);
		//���_��p��
		position[0] = D3DXVECTOR3(left, top, 0.0f);
		position[1] = D3DXVECTOR3(right, top, 0.0f);
		position[2] = D3DXVECTOR3(left, bottom, 0.0f);
		position[3] = D3DXVECTOR3(right, bottom, 0.0f);
		//�`��
		mylib_draw_board_2d(
			&position[0], &uv[0], 0xffffffff,
			&position[1], &uv[1], 0xffffffff,
			&position[2], &uv[2], 0xffffffff,
			&position[3], &uv[3], 0xffffffff,
			texture_manager_get(g._pause._continue_tex),
			mbm_alpha
		);
	}
	//TITLE�{�^���`��
	{
		//�T�C�Y�擾
		float w = (float)texture_manager_get_width(g._pause._title_tex);
		float h = (float)texture_manager_get_height(g._pause._title_tex);
		//����ƉE�������߂�
		float left = _coord_title_x / 1280.0f;
		float top = _coord_title_y / 720.0f;
		float right = left + w / 1280.0f;
		float bottom = top + h / 720.0f;
		//�␳
		left = left * 2.0f - 1.0f;
		top = -(top * 2.0f - 1.0f);
		right = right * 2.0f - 1.0f;
		bottom = -(bottom * 2.0f - 1.0f);
		//���_��p��
		position[0] = D3DXVECTOR3(left, top, 0.0f);
		position[1] = D3DXVECTOR3(right, top, 0.0f);
		position[2] = D3DXVECTOR3(left, bottom, 0.0f);
		position[3] = D3DXVECTOR3(right, bottom, 0.0f);
		//�`��
		mylib_draw_board_2d(
			&position[0], &uv[0], 0xffffffff,
			&position[1], &uv[1], 0xffffffff,
			&position[2], &uv[2], 0xffffffff,
			&position[3], &uv[3], 0xffffffff,
			texture_manager_get(g._pause._title_tex),
			mbm_alpha
		);
	}
	//�J�[�\���`��
	{
		//�T�C�Y�擾
		float w = (float)texture_manager_get_width(ti_title_title_cursor);
		float h = (float)texture_manager_get_height(ti_title_title_cursor);
		//����ƉE�������߂�
		float left = g._pause._cursor._draw_x / 1280.0f;
		float top = g._pause._cursor._draw_y / 720.0f;
		float right = left + w / 1280.0f;
		float bottom = top + h / 720.0f;
		//�␳
		left = left * 2.0f - 1.0f;
		top = -(top * 2.0f - 1.0f);
		right = right * 2.0f - 1.0f;
		bottom = -(bottom * 2.0f - 1.0f);
		//���_��p��
		position[0] = D3DXVECTOR3(left, top, 0.0f);
		position[1] = D3DXVECTOR3(right, top, 0.0f);
		position[2] = D3DXVECTOR3(left, bottom, 0.0f);
		position[3] = D3DXVECTOR3(right, bottom, 0.0f);
		//�`��
		mylib_draw_board_2d(
			&position[0], &uv[0], 0xffffffff,
			&position[1], &uv[1], 0xffffffff,
			&position[2], &uv[2], 0xffffffff,
			&position[3], &uv[3], 0xffffffff,
			texture_manager_get(ti_title_title_cursor),
			mbm_alpha
		);
	}
}

//�|�[�Y��ʂɈڍs����ׂ��Ȃ�true��Ԃ�
static bool to_pause(const virtual_input_t *trigger)
{
	return trigger->_pause;
}

//�X�e�[�W���J�n���ĉ������炷
static void _stage_setup(stage_index_t index)
{
	stage_setup(index, &g._object_array);
	sound_play(si_se_voice_start);
}

#endif

///-----------------------���̏�Ԃ𐧌䂷���v�Ȋ֐�------------------------------------



extern void game_main_init()
{
	//FBX�I�u�W�F�N�g������
	gmfoa_t_init(&g._object_array, 8);
	//���炫��G�t�F�N�g������
	glitter_manager_init();
	//�J����������
	camera_manager_init();
	//�X�v���C�g������
	sprite_integration_init();
	//UI��������
	stage_ui_init();
	//����炷
	sound_play(si_bgm_bgm);
	//�X�e�[�W�J�n
	_stage_setup(stage_get_now());
	//�C�x���g�Ǘ�������
	stage_event_init();
	//�ŏ��̓|�[�Y���[�h�ɂ��Ȃ�
	g._pause_initialized = false;
	g._pause_mode = false;
}
extern void game_main_end()
{
	//�����~�߂�
	sound_stop(si_bgm_bgm);
	//UI�I��
	stage_ui_end();
	//�C�x���g�Ǘ��I��
	stage_event_end();
	//�J�����I��
	camera_manager_end();
	//���炫��G�t�F�N�g�I��
	glitter_manager_end();
	//FBX�I�u�W�F�N�g�I��
	gmfoa_t_end(&g._object_array);
	//�X�v���C�g�I��
	sprite_integration_end();
}
extern void game_main_update(magnet_hero_state_t *next_state)
{
	//���͂̎擾
	const virtual_input_t *trigger = mylib_get_virtual_input_trigger();
	//�|�[�Y����
	if (g._pause_mode)
	{
		//�|�[�Y��
		if (g._pause_initialized == false)
		{
			pause_init();
			g._pause_initialized = true;
		}
		pause_result_t result = pause_update(trigger);
		switch (result)
		{
		case pr_pause:
			//�������Ȃ�
			break;
		case pr_continue:
			//�|�[�Y�I��
			pause_end();
			g._pause_initialized = false;
			g._pause_mode = false;
			break;
		case pr_title:
			//�|�[�Y�I��
			pause_end();
			g._pause_initialized = false;
			g._pause_mode = false;
			//�Ƃ肠�����^�C�g���֖߂�(todo:���ԑ҂�����������)
			mylib_wait_sec(0.5f);
			*next_state = mhs_title;
			break;
		default:
			break;
		}
	}
	else
	{
		//�|�[�Y������Ȃ�
		g._pause_mode = to_pause(trigger);
		//FBX�I�u�W�F�N�g�X�V
		gmfoa_t_update(&g._object_array);
		//���炫��G�t�F�N�g�X�V
		glitter_manager_update();
		//�J�����X�V
		camera_manager_update(&g._object_array);
		//�r���[�s��ƃv���W�F�N�V�����s����v�Z&�Z�b�g
		camera_manager_set_view_and_projection(1.0f, 10000.0f);
		//�X�v���C�g�X�V
		sprite_integration_update();
		//UI�X�V
		stage_ui_update();
		//�C�x���g�Ǘ��X�V�E�v�Z
		stage_event_update();
		//�v���[���[������������X�e�[�W���Z�b�g
		if (holder_falled_player_get() == timeGetTime())
		{
			stage_reset(stage_get_now());
		}
		//�S�[���t���O�������Ă��邩
		if (stage_event_get_already_goal())
		{
			//�S�[�������u�Ԃ�
			if (stage_event_get_goal_elapsed() == 1)
			{
				//�S�[�������u�ԂȂ̂ŃS�[�����̉�����炷
				sound_play(si_se_voice_goal);
			}
			//���o���I����Ă��邩
			if (stage_event_get_goal_elapsed() >= _goal_wait_frame)
			{
				//���݂̃X�e�[�W��j��
				stage_destroy(stage_get_now(), &g._object_array);
				//�X�e�[�W��i�߂� & �S�N������
				if (stage_advance())
				{
					//�S�N���Ȃ̂ŃN���A����������(���ɗV�񂾎��͍ŏ�����ɂȂ��Ă�)
					stage_clear_data_init();
					//�G���f�B���O�ɍs��
					*next_state = mhs_ending;
				}
				else
				{
					//�X�e�[�W�J�n
					_stage_setup(stage_get_now());
				}
			}
		}
	}
}
extern void game_main_draw()
{
	if (g._pause_mode)
	{
		//�|�[�Y��
		pause_draw();
	}
	else
	{
		//�Ǘ����ɂ���FBX�I�u�W�F�N�g�`��
		gmfoa_t_draw(&g._object_array);
		//���炫��G�t�F�N�g�`��
		glitter_manager_draw();
		//�v���[���[�̔�΂���̃I�u�W�F�`��
		drawer_player_magnet_hand();
		//�v���[���[�̉e�`��
		drawer_player_shadow_draw();
		//�X�v���C�g�`��
		sprite_integration_draw();
		//UI�`��
		stage_ui_draw(stage_event_get_goal_elapsed(), _goal_wait_frame);
	}
}
