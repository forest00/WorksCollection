
//�e�w�b�_�[
#include "..\public\game_debug.h"
//���̑��̃w�b�_�[
#include <stdio.h>
#include "..\..\input\public\input.h"
#include "..\..\font\public\font.h"
#include "..\..\player\public\player.h"
#include "..\..\enemy\public\enemy.h"


extern void __game_debug_infomation__()
{
	int font_pos_y = 0;
	int height = 16;
	static char str[0x100];
	{
		test_player_manager_t *manager = player_get_test_player_manager();
		test_player_object_t **player = test_player_manager_t_search_active(manager);
		const vector3_t *player_pos = test_player_manager_t_get_position(manager, *player);

		sprintf_s(str, 0x100, "�v���C���[���WX : %g", player_pos->x);
		font_draw_buffer(str, 0, font_pos_y += height, height);

		sprintf_s(str, 0x100, "�v���C���[���WY : %g", player_pos->y);
		font_draw_buffer(str, 0, font_pos_y += height, height);

		sprintf_s(str, 0x100, "�v���C���[���WZ : %g", player_pos->z);
		font_draw_buffer(str, 0, font_pos_y += height, height);
	}
	{
		double in = input_controller_left_x();
		sprintf_s(str, 0x100, "���X�e�BX : %g", in);
		font_draw_buffer(str, 0, font_pos_y += height, height);
	}
	{
		double in = input_controller_left_y();
		sprintf_s(str, 0x100, "���X�e�BY : %g", in);
		font_draw_buffer(str, 0, font_pos_y += height, height);
	}
	{
		double in = input_controller_right_x();
		sprintf_s(str, 0x100, "�E�X�e�BX : %g", in);
		font_draw_buffer(str, 0, font_pos_y += height, height);
	}
	{
		double in = input_controller_right_y();
		sprintf_s(str, 0x100, "�E�X�e�BY : %g", in);
		font_draw_buffer(str, 0, font_pos_y += height, height);
	}
	{
		int cont_cross_up = input_controller_cross_count(input_controller_cross_code_up);
		sprintf_s(str, 0x100, "�R���g���[���[�\�� �� : %d", cont_cross_up);
		font_draw_buffer(str, 0, font_pos_y += height, height);
	}
	{
		int cont_cross_down = input_controller_cross_count(input_controller_cross_code_down);
		sprintf_s(str, 0x100, "�R���g���[���[�\�� �� : %d", cont_cross_down);
		font_draw_buffer(str, 0, font_pos_y += height, height);
	}
	{
		int cont_cross_right = input_controller_cross_count(input_controller_cross_code_right);
		sprintf_s(str, 0x100, "�R���g���[���[�\�� �E : %d", cont_cross_right);
		font_draw_buffer(str, 0, font_pos_y += height, height);
	}
	{
		int cont_cross_left = input_controller_cross_count(input_controller_cross_code_left);
		sprintf_s(str, 0x100, "�R���g���[���[�\�� �� : %d", cont_cross_left);
		font_draw_buffer(str, 0, font_pos_y += height, height);
	}
	{
		int in = input_controller_ZL_ZR();
		sprintf_s(str, 0x100, "ZL�Ńv���X�AZR�Ń}�C�i�X : %d", in);
		font_draw_buffer(str, 0, font_pos_y += height, height);
	}
	{
		int mouse_move_x = input_mouse_move_x();
		sprintf_s(str, 0x100, "�}�E�X�� : %d", mouse_move_x);
		font_draw_buffer(str, 0, font_pos_y += height, height);
	}
	{
		int mouse_move_y = input_mouse_move_y();
		sprintf_s(str, 0x100, "�}�E�X�c : %d", mouse_move_y);
		font_draw_buffer(str, 0, font_pos_y += height, height);
	}
	{
		int mouse_wheel = input_mouse_wheel();
		sprintf_s(str, 0x100, "�}�E�X�z�C�[�� : %d", mouse_wheel);
		font_draw_buffer(str, 0, font_pos_y += height, height);
	}
	{
		test_enemy_manager_t *manager = enemy_get_test_enemy_manager();
		size_t num_of_test_enemy = test_enemy_manager_t_get_num_of_active(manager);
		size_t capacity_of_test_enemy = test_enemy_manager_t_get_capacity(manager);
		sprintf_s(str, 0x100, "�G�̐� : %4d / %4d", num_of_test_enemy, capacity_of_test_enemy);
		font_draw_buffer(str, 0, font_pos_y += height, height);
	}
}

