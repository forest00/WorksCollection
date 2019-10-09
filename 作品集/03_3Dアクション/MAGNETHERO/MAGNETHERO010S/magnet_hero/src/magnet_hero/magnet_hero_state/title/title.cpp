//�e�w�b�_�[
#include "title.h"
//���̑��̃w�b�_�[
#include "..\..\..\mylib\mylib.h"
#include "..\..\..\input\input.h"
#include "..\..\..\sound\sound.h"
#include "..\..\..\movie\avi.h"
#include "..\..\..\texture_manager\texture_manager.h"
#include "..\..\..\easing\easing_type.h"
#include "..\..\..\fbx_table\fbx_table.h"

#if 1
//�Đ����铮��t�@�C��
#define _movie_filename			(L"res/movie/op.avi")
//���W
#define _coord_start_x			(318.0f)
#define _coord_start_y			(592.0f)
#define _coord_exit_x			(705.0f)
#define _coord_exit_y			(587.0f)
#define _coord_cursor_x1		(211.0f)
#define _coord_cursor_y1		(610.0f)
#define _coord_cursor_x2		(601.0f)
#define _coord_cursor_y2		(610.0f)
//�����̑҂�����
#define _voice_sec				(1.0f)
enum title_state_t
{
	ts_continue,
	ts_movie,
	ts_exit,
};
//�J�[�\��
struct cursor_t
{
	int _pos;//���݈ʒu�@0���A1�E
	float _draw_x;//�`��ʒux
	float _draw_y;//�`��ʒuy
	bool _moving;//�ړ���
	float _change_sec;//�؂�ւ���ꂽ�u��
	float _move_duration_sec;//�ړ��ɂ�����S�̂̎���
};
//���͑ҋ@���Ɏg���ϐ�
struct wait_t
{
	float _cursor_draw_pos[2][2];//�ʒu���̏W�܂�
	cursor_t _cursor;
	texture_id_t _start_tex;
	texture_id_t _exit_tex;
	bool _should_play_bgm;
	float _init_sec;
	bool _to_next;//���ɍs���ׂ�
};
//�O���[�o���ϐ�
struct global_t
{
	title_state_t _state;
	wait_t _wait;
};
static struct global_t g;
#endif

extern void title_init()
{
	g._state = ts_continue;
	g._wait._cursor._pos = 0;
	g._wait._cursor._change_sec = 0.0f;
	g._wait._cursor._moving = false;
	g._wait._cursor._move_duration_sec = 0.3f;
	g._wait._cursor._draw_x = _coord_cursor_x1;
	g._wait._cursor._draw_y = _coord_cursor_y1;
	g._wait._start_tex = ti_title_start_on;
	g._wait._exit_tex = ti_title_exit_off;
	g._wait._cursor_draw_pos[0][0] = _coord_cursor_x1;
	g._wait._cursor_draw_pos[0][1] = _coord_cursor_y1;
	g._wait._cursor_draw_pos[1][0] = _coord_cursor_x2;
	g._wait._cursor_draw_pos[1][1] = _coord_cursor_y2;
	sound_play(si_se_voice_title_call);
	g._wait._should_play_bgm = true;
	g._wait._init_sec = mylib_now_sec();
	g._wait._to_next = false;
}
extern void title_end()
{

}
#if 1
static void title_main_update(magnet_hero_state_t *next_state)
{
	if (g._wait._should_play_bgm)
	{
		if (mylib_now_sec() - g._wait._init_sec > _voice_sec)
		{
			g._wait._should_play_bgm = false;
			sound_play(si_bgm_title);
		}
	}
	cursor_t *cursor = &g._wait._cursor;
	//���͂̎擾
	const virtual_input_t *trigger = mylib_get_virtual_input_trigger();
	//���������ꂽ
	if (trigger->_lkey_left || trigger->_rkey_left)
	{
		if (cursor->_pos != 0)
		{
			cursor->_moving = true;
			cursor->_change_sec = mylib_now_sec();
			cursor->_pos = 0;
			g._wait._start_tex = ti_title_start_on;
			g._wait._exit_tex = ti_title_exit_off;
		}
	}
	//�E�������ꂽ
	if (trigger->_lkey_right || trigger->_rkey_right)
	{
		if (cursor->_pos != 1)
		{
			cursor->_moving = true;
			cursor->_change_sec = mylib_now_sec();
			cursor->_pos = 1;
			g._wait._start_tex = ti_title_start_off;
			g._wait._exit_tex = ti_title_exit_on;
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
		float start_x = g._wait._cursor_draw_pos[1 - cursor->_pos][0];
		float start_y = g._wait._cursor_draw_pos[1 - cursor->_pos][1];
		float end_x = g._wait._cursor_draw_pos[cursor->_pos][0];
		float end_y = g._wait._cursor_draw_pos[cursor->_pos][1];
		cursor->_draw_x = (float)call_easing(et_EaseOutCirc, t, start_x, end_x - start_x, 1.0f);
		cursor->_draw_y = (float)call_easing(et_EaseOutCirc, t, start_y, end_y - start_y, 1.0f);
	}
	//����L�[�������ꂽ
	if (trigger->_a)
	{
		//���ɍs���ׂ��t���O�𗧂Ă�
		g._wait._to_next = true;
	}
	if (g._wait._to_next)
	{
		//BGM���Đ�����Ă���ꍇ����
		if (g._wait._should_play_bgm == false)
		{
			switch (cursor->_pos)
			{
			case 0:
				//���[�r�[�ɍs��
				g._state = ts_movie;
				sound_stop(si_bgm_title);
				//���[�r�[�ɍs���O��FBX�ǂݍ���
				fbx_table_load(fri_sky_sphere);
				fbx_table_load(fri_wrench_and_bibibi);
				*next_state = mhs_title;
				break;
			case 1:
				//�Q�[������߂�ィ
				g._state = ts_exit;
				break;
			default:
				break;
			}
		}
	}
	*next_state = mhs_title;
}
static void title_movie_update(magnet_hero_state_t *next_state)
{
	play_avi(_movie_filename);
	*next_state = mhs_game_main;
}
#endif
extern void title_update(magnet_hero_state_t *next_state)
{
	switch (g._state)
	{
	case ts_continue:
		title_main_update(next_state);
		break;
	case ts_movie:
		title_movie_update(next_state);
		break;
	case ts_exit:
		//�Q�[������߂�ィ
		*next_state = mhs_ended;
	default:
		break;
	}
}
extern void title_draw()
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
			texture_manager_get(ti_title_title),
			mbm_none
		);
	}
	//START�{�^���`��
	{
		//�T�C�Y�擾
		float w = (float)texture_manager_get_width(g._wait._start_tex);
		float h = (float)texture_manager_get_height(g._wait._start_tex);
		//����ƉE�������߂�
		float left = _coord_start_x / 1280.0f;
		float top = _coord_start_y / 720.0f;
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
			texture_manager_get(g._wait._start_tex),
			mbm_alpha
		);
	}
	//EXIT�{�^���`��
	{
		//�T�C�Y�擾
		float w = (float)texture_manager_get_width(g._wait._exit_tex);
		float h = (float)texture_manager_get_height(g._wait._exit_tex);
		//����ƉE�������߂�
		float left = _coord_exit_x / 1280.0f;
		float top = _coord_exit_y / 720.0f;
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
			texture_manager_get(g._wait._exit_tex),
			mbm_alpha
		);
	}
	//�J�[�\���`��
	{
		//�T�C�Y�擾
		float w = (float)texture_manager_get_width(ti_title_title_cursor);
		float h = (float)texture_manager_get_height(ti_title_title_cursor);
		//����ƉE�������߂�
		float left = g._wait._cursor._draw_x / 1280.0f;
		float top = g._wait._cursor._draw_y / 720.0f;
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