
//�e�w�b�_�[
#include "flatpo_test.h"
//���̑��̃w�b�_�[
#include "flatpo_drawer.h"
#include "..\..\input\public\input.h"
//�f�o�b�O�p�w�b�_�[
#include "..\..\font\public\font.h"



static flatpo_t g_makabe_hiroshi_rotation;			//��]����^��
static flatpo_t g_makabe_hiroshi_dont_move;			//�s���̐^��

//static void makabe_


//�|���e�X�g�p�I�u�W�F�N�g��������
extern void flatpo_tester_all_init()
{
	;
}

//�|���e�X�g�p�I�u�W�F�N�g��j��
extern void flatpo_tester_all_end()
{
	;
}

//�|���e�X�g�p�I�u�W�F�N�g���X�V
extern void flatpo_tester_all_update()
{
	;
}

//�|���e�X�g�p�I�u�W�F�N�g��`��
extern void flatpo_tester_all_draw(IDirect3DDevice9 *device)
{
#if 0
	//1�ڂ̔|����`��
	{
		time_t millisec = (time_t)(timeGetTime());
		double sn1 = sin(3.14159265 * millisec / 1000.0);
		double cs1 = cos(3.14159265 * millisec / 1000.0);
		double sn2 = sin(3.14159265 * millisec / 1000.0 + 3.14159265);
		double cs2 = cos(3.14159265 * millisec / 1000.0 + 3.14159265);
		flatpo_t test;
		test._texture_id = flatpo_texture_id_fafnyls_tail;
		test._vertices[0]._position.x = cs1 * 1000;
		test._vertices[0]._position.y = 0.0;
		test._vertices[0]._position.z = sn1;
		test._vertices[0]._u = 0.0;
		test._vertices[0]._v = 0.0;
		test._vertices[0]._alpha = 255;
		test._vertices[0]._red = 255;
		test._vertices[0]._green = 255;
		test._vertices[0]._blue = 255;

		test._vertices[1]._position.x = cs2 * 1000;
		test._vertices[1]._position.y = 0.0;
		test._vertices[1]._position.z = sn2;
		test._vertices[1]._u = 1.0 / 15.0;
		test._vertices[1]._v = 0.0;
		test._vertices[1]._alpha = 255;
		test._vertices[1]._red = 255;
		test._vertices[1]._green = 255;
		test._vertices[1]._blue = 255;

		test._vertices[2]._position.x = cs2 * 1000;
		test._vertices[2]._position.y = 1000.0;
		test._vertices[2]._position.z = sn2;
		test._vertices[2]._u = 1.0 / 15.0;
		test._vertices[2]._v = 1.0;
		test._vertices[2]._alpha = 255;
		test._vertices[2]._red = 255;
		test._vertices[2]._green = 255;
		test._vertices[2]._blue = 255;

		test._vertices[3]._position.x = cs1 * 1000;
		test._vertices[3]._position.y = 1000.0;
		test._vertices[3]._position.z = sn1;
		test._vertices[3]._u = 0.0;
		test._vertices[3]._v = 1.0;
		test._vertices[3]._alpha = 255;
		test._vertices[3]._red = 255;
		test._vertices[3]._green = 255;
		test._vertices[3]._blue = 255;

		flatpo_draw(&test);
	}
#endif
	//2�ڂ̔|����`��
	{
		int alpha = (int)((double)(clock()) / (double)(CLOCKS_PER_SEC) * 0.5);
		flatpo_t test;
		static double y = 0.0;
		if (input_keyboard_repeat(DIK_K))
		{
			alpha++;
			alpha++;
			alpha++;
		}
		if (input_keyboard_repeat(DIK_M))
		{
			alpha--;
			alpha--;
			alpha--;
		}
#ifdef _DEBUG
		font_draw_format(100, 200, 50, "alpha : %d", alpha);
#endif
		y += input_controller_ZL_ZR() / 800.0;
		test._texture_id = flatpo_texture_id_test_amg_logo;
		test._vertices[0]._position.x = 0.0;
		test._vertices[0]._position.y = y;
		test._vertices[0]._position.z = 0.0;
		test._vertices[0]._u = 0.0;
		test._vertices[0]._v = 0.0;
		test._vertices[0]._alpha = alpha;
		test._vertices[0]._red = 255;
		test._vertices[0]._green = 255;
		test._vertices[0]._blue = 255;

		test._vertices[1]._position.x = flatpo_screen_width;
		test._vertices[1]._position.y = y;
		test._vertices[1]._position.z = 0.0;
		test._vertices[1]._u = 1.0;
		test._vertices[1]._v = 0.0;
		test._vertices[1]._alpha = alpha;
		test._vertices[1]._red = 255;
		test._vertices[1]._green = 255;
		test._vertices[1]._blue = 255;

		test._vertices[2]._position.x = flatpo_screen_width;
		test._vertices[2]._position.y = y + flatpo_screen_height;
		test._vertices[2]._position.z = 0.0;
		test._vertices[2]._u = 1.0;
		test._vertices[2]._v = 1.0;
		test._vertices[2]._alpha = alpha;
		test._vertices[2]._red = 255;
		test._vertices[2]._green = 255;
		test._vertices[2]._blue = 255;

		test._vertices[3]._position.x = 0.0;
		test._vertices[3]._position.y = y + flatpo_screen_height;
		test._vertices[3]._position.z = 0.0;
		test._vertices[3]._u = 0.0;
		test._vertices[3]._v = 1.0;
		test._vertices[3]._alpha = alpha;
		test._vertices[3]._red = 255;
		test._vertices[3]._green = 255;
		test._vertices[3]._blue = 255;
		flatpo_draw(&test);

	}
}

